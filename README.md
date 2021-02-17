# st-link-dumper
This is based on the original @lujji st-link-dumper https://github.com/lujji/st-link-dumper modified a little bit to dump continuously the flash and adding flashing LED for start and stop dumps. I've changed the code to be compatible with latest libopencm3 library and adding definitions for 2 boards: STM32F103CxT6 Blue Pill and Nucleo boards.

Dumper firmware for stm32f103x. It will read flash contents from 0x08000000 to 0x08004000 and dump it over UART2 (TX=PA2). Default baud is 9600.

Firmware start is marked with bytes `AB CD` - these two bytes need to be removed from the dump.

Precompiled binaries are located in `bin` directory. There are two binaries: `dump_0x4000-F103Cx-BluePill.bin` and `dump_0x4000-F103Cx-Nucleo.bin` will dump the first 16k of flash (bootloader section). One is for the Blue Pill board and the other one is for Nucleo.
These binaries are _encrypted_ so that you can upload them directly to the programmer.

Change log:
  - update latest OpenCM3 library.
  - update the code to use latest functions from OpenCM3.
  - update the binaries.
  - the firmware before starts to dump the flash to the UASRT port LED will flash 8 times slow to announce the new transmision.
  - after finishing the dumping, the LED will flash fast 16 times to announce the ending of the transmision.
  - the firmware will wait a while and then will start from beginning.

PS: I'm using this tool to repair one of my Nucleo boards. Somehow I toasted the STLinkV2-1 MCU on board. I'm pretty sure the target MCU is defective, but at least it bootups but STLink MCU not. Is getting hot after few seconds. The problem is that I can't find a way to reflash the STLinkV2-1 MCU because there is no firmware/bootload available.
