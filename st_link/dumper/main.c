#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#define __BOARD_BLUE_PILL__
//#define __BOARD_NUCLEO__

#if defined(__BOARD_BLUE_PILL__)
/* Definition of peripherials for Blue Pill boards */
#define LED_PIN         GPIO13
#define LED_GPIO        GPIOC
#define LED_RCC_GPIO    RCC_GPIOC
#define __USART_RCC     RCC_USART2
#define __USART         USART2
#define __USART_GPIO    GPIOA
#define __USART_TX      GPIO_USART2_TX
#define __USART_RX      GPIO_USART2_RX

#elif defined (__BOARD_NUCLEO__)

/* Definition of peripherials for Nucleo boards */
#define LED_PIN         GPIO9
#define LED_GPIO        GPIOA
#define LED_RCC_GPIO    RCC_GPIOA
#define __USART         USART2
#define __USART_RCC     RCC_USART2
#define __USART_GPIO    GPIOA
#define __USART_TX      GPIO_USART2_TX
#define __USART_RX      GPIO_USART2_RX

#else
#error "Please select a board!"
#endif

static void delay(int x) {
    for (int i = 0; i < x; i++)
        for (int j = 0; j < 4000; j++)
            __asm__("nop");
}

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(LED_RCC_GPIO);
    rcc_periph_clock_enable(__USART_RCC);
//    rcc_periph_clock_enable(__USART_GPIO);
}

static void usart_setup(void) {
    gpio_set_mode(__USART_GPIO, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, __USART_TX);

    usart_set_baudrate(__USART, 9600);
    usart_set_databits(__USART, 8);
    usart_set_stopbits(__USART, USART_STOPBITS_1);
    usart_set_mode(__USART, USART_MODE_TX);
    usart_set_parity(__USART, USART_PARITY_NONE);
    usart_set_flow_control(__USART, USART_FLOWCONTROL_NONE);

    usart_enable(__USART);
}

int main(void) {
    uint32_t i;
    clock_setup();
    usart_setup();
    gpio_set_mode(LED_GPIO, GPIO_MODE_OUTPUT_50_MHZ,
            GPIO_CNF_OUTPUT_PUSHPULL, LED_PIN);

    // go into infinit loop and output to the serial port
    while(1) 
    {
	/* Blink the LED 8 times slow to announce the starting of dumping */
	for (i=0; i<8; i++)
	{
	    gpio_set(LED_GPIO, LED_PIN);
	    delay(1000);
	    gpio_clear(LED_GPIO, LED_PIN);
	    delay(1000);
	}
	/* Turn on the LED */
#if defined(__BOARD_BLUE_PILL__)
/* The LED is output is inverted */
	gpio_clear(LED_GPIO, LED_PIN);
#else
	gpio_set(LED_GPIO, LED_PIN);
#endif

	/* Mark firmware start */
	usart_send_blocking(__USART, 0xAB);
	delay(10);
	usart_send_blocking(__USART, 0xCD);
	delay(10);

	for (uint32_t ptr = 0x08000000; ptr <= 0x08000000 + 0x4000; ptr++) {
	    usart_send_blocking(__USART, *((uint16_t *) ptr));
	    delay(10);
	}

	/* Indicate that we're done by flashing few times */
	/* Blink the LED 16 times fast to announce the ending of dumping */
	for (i=0; i<16; i++)
	{
	    gpio_set(LED_GPIO, LED_PIN);
	    delay(500);
	    gpio_clear(LED_GPIO, LED_PIN);
	    delay(500);
	}
	/* Turn off the LED */
#if defined(__BOARD_BLUE_PILL__)
/* The LED is output is inverted */
	gpio_set(LED_GPIO, LED_PIN);
#else
	gpio_clear(LED_GPIO, LED_PIN);
#endif
	/* Wait a while until we start again to dump */
	delay(10000);
    }
}
