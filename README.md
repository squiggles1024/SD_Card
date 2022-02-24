# SD_Card
A simple SD Card reading/writing program to be used on an STM32F4 microcontroller. This is largely based upon the FatFS uSD Example project provided by STMicro, but rewritten to be more readable and maintainable.

In order to port to another STM32F4 project , the user should only need to modify sd_hardware.h

The project can be imported and built using VSCode and PlatformIO with a J-Link Segger Debugger. This was originally done using a STM32F411CEUX Blackpill Board and 
