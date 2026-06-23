

# Release Notes for Open Bootloader Middleware
Copyright &copy; 2020 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com)

# Purpose

The **Open Bootloader** package provides an example of an IAP Bootloader that supports all ST Standard Bootloader commands.

The Open Bootloader protocols commands were developed in a manner to be compatible with the Standard Bootloader protocols commands that is programmed by default in the system memory of STM32 devices.

The Open Bootloader is also compatible with STM32CubeProgrammer tool.

In the current release, the Open Bootloader supports USART, FDCAN, I2C, I3C, USB and SPI protocols.

Here is the list of references to the user documents:

- [AN3155](https://www.st.com/resource/en/application_note/CD00264342.pdf) : USART protocol used in the STM32 bootloader
- [AN5405](https://www.st.com/resource/en/application_note/dm00660346.pdf) : FDCAN protocol used in the STM32 bootloader
- [AN4221](https://www.st.com/resource/en/application_note/DM00072315.pdf) : I2C protocol used in the STM32 bootloader
- [AN3156](https://www.st.com/resource/en/application_note/cd00264379.pdf) : USB DFU protocol used in the STM32 bootloader
- [AN4286](https://www.st.com/resource/en/application_note/DM00081379.pdf) : SPI protocol used in the STM32 bootloader
- [AN5927](https://www.st.com/resource/en/application_note/an5927-i3c-protocol-used-in-the-stm32-bootloader-stmicroelectronics.pdf) : I3C protocol used in the STM32 bootloader

# Update history

<label for="collapse-section10" aria-hidden="true">__V6.1.3 / 08-May-2026__</label>
<div>

## Main changes

- Add SW security level file.

## Known limitations

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section10" aria-hidden="true">__V6.1.2 / 16-October-2025__</label>
<div>

## Main changes

- Fix comment about FDCAN nominal baud rate and data baud rate in FDCAN interface pattern file.
- SLA0044 Legal terms updated to latest revision (Rev 6 / October 2025).

## Known limitations

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section9" aria-hidden="true">__V6.1.1 / 26-January-2023__</label>
<div>

## Main changes

This is a patch release that aims to improve FDCAN memory management.

## Known limitations

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section8" aria-hidden="true">__V6.1.0 / 14-October-2022__</label>
<div>

## Main changes

This new release of Open Bootloader Middleware adds support for the I3C protocol.

## Known limitations

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section7" aria-hidden="true">__V6.0.1 / 26-January-2022__</label>
<div>

## Main changes

This is a patch release of Open Bootloader Middleware that fixes a regression in USB protocol introduced with the official release v6.0.0.

Nothing new in this release, only fixes to USB regression were made:

  - Fix compilation issue due to use of wrong include file
  - Restore removed functions:
    - Jump function
    - Write Unprotect function
    - Write Protect function
    - Read Protect function
    - Read Unprotect function

## Known limitations

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section6" aria-hidden="true">__V6.0.0 / 11-October-2021__</label>
<div>

## Main changes

### Sixth release

This is the sixth official release of Open Bootloader Middleware that adds some improvements to the code and adds
support for special commands and CAN interface.

New in this release:

  - Add support for Special Commands
  - Add support for CAN interface

__There is a compatibility break with previous release due to the below changes:__

  - Removed dependencies with FLASH interface
    - No more direct include of "flash_interface.h"
    - Read out get/set protection functions are now generic
    - Option bytes management is now generic and managed in application side
  - The user can now redefine the list of supported commands by interface in application side
  - Changed the I2C busy state management API by removing dependencies with FLASH interface

## Known limitations

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Not Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section5" aria-hidden="true">__V5.0.0 / 03-June-2021__</label>
<div>

## Main changes

### Fifth release

This is the fifth official release of Open Bootloader Middleware that adds support for USBX stack

- List of implemented commands on USB protocol:
  - Get version
  - Get ID
  - Read memory
  - Write memory
  - Go command
  - Erase FLASH
  - Read protect
  - Read unprotect
  - Write protect
  - Write unprotect

## Known limitations

- List of missing commands on USART, FDCAN, I2C and SPI protocols:
  - Special read command
  - Special write command

- List of missing commands on USB protocol:
  - Get command
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section4" aria-hidden="true">__V4.0.0 / 20-April-2021__</label>
<div>

## Main changes

### Forth release

This is the forth official release of Open Bootloader Middleware that adds support for SPI protocol

The Open Bootloader SPI commands are compatible with the Standard Bootloader SPI commands.

- List of implemented commands on SPI protocol:
  - Get commands
  - Get version
  - Get ID
  - Read memory
  - Write memory
  - Go command
  - Erase FLASH
  - Read protect
  - Read unprotect
  - Write protect
  - Write unprotect

## Known limitations

- List of missing commands on USART, FDCAN, I2C and SPI protocols:
  - Special read command
  - Special write command

- List of missing commands on USB protocol:
  - Get command
  - Read unprotect
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section3" aria-hidden="true">__V3.0.0 / 23-October-2020__</label>
<div>			

## Main changes

### Third release

This is the third official release of Open Bootloader Middleware that adds support for USB DFU protocol

The Open Bootloader USB commands are compatible with the Standard Bootloader USB commands except get command, read unprotect, mass erase, go commands

- List of implemented commands on USB protocol:
  - Read memory
  - Write memory
  - Set Address Pointer
  - Erase FLASH

## Known limitations

- List of missing commands on USART, FDCAN and I2C protocols:
  - Special read command
  - Special write command

- List of missing commands on USB protocol:
  - Get command
  - Read unprotect
  - Mass erase

## Backward compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section2" aria-hidden="true">__V2.0.0 / 12-May-2020__</label>
<div>

## Main Changes

### Second release

This is the second official release of Open Bootloader Middleware that adds support for I2C protocol.

The Open Bootloader I2C commands are compatible with the Standard Bootloader I2C commands

- List of implemented commands on I2C protocol:
  - Get commands
  - Get version
  - Get ID
  - Read memory
  - Write memory
  - Non stretch write memory
  - Go command
  - Erase FLASH
  - Non stretch erase FLASH
  - Read protect
  - Non stretch read protect
  - Read unprotect
  - Non stretch read unprotect
  - Write protect
  - Non stretch write protect
  - Write unprotect
  - Non stretch write unprotect

## Known Limitations

- List of missing commands on USART, FDCAN and I2C protocols:
  - Special read command
  - Special write command

## Backward Compatibility

- Compatible with previous release.

## Dependencies

- None.

</div>

<label for="collapse-section1" aria-hidden="true">__V1.0.0 / 01-August-2019__</label>
<div>

## Main Changes

### First release

This is the first official release of Open Bootloader Middleware that supports USART and FDCAN protocols.

The Open Bootloader USART and FDCAN commands are compatible with the Standard Bootloader commands.

- List of implemented commands on USART and FDCAN protocols:
  - Get commands
  - Get version
  - Get ID
  - Read memory
  - Write memory
  - Go command
  - Erase FLASH
  - Read protect
  - Read unprotect
  - Write protect
  - Write unprotect

## Known Limitations

- List of missing commands on USART and FDCAN protocols:
  - Special read command
  - Special write command

## Backward Compatibility

- None.

## Dependencies

- None.

</div>



For complete documentation on **STM32 Microcontrollers** ,
visit: [www.st.com/STM32](http://www.st.com/STM32)
<abbr title="Based on template cx566953 version 3.0">Info</abbr>