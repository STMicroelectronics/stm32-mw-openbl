# Middleware Open Bootloader MCU Component

![tag](https://img.shields.io/badge/tag-STM32N6_v1.0.2-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/stm32-mw-openbl/blob/stm32n6/Release_Notes.html)
[![offer](https://img.shields.io/badge/stm32cube_mcu_offer-browse-orange.svg)](https://github.com/STMicroelectronics/STM32Cube_MCU_Overall_Offer)

## Overview

**Open Bootloader** is an In-Application programming (IAP) provided in the STM32Cube MCU Packages and GitHub. It is fully compatible with STM32 System Bootloader so that it have the same supported interfaces and commands. It's also using the same Tools such as STM32CubeProgrammer.

**Open Bootloader** is provided as an example that can be used by any customer who wants to build and customize his own Bootloader starting from a good basis. It allows all possible bootloader operations (Read, write, erase, jump...) into internal (Flash, SRAM, OTP...) or external memory using one of the available communication interfaces (USART, I2C, SPI, USB-DFU, FDCAN...).

**Open Bootloader** supplies services to the Host (can be STM32CubeProgrammer or another user made host) in order to perform all possible Bootloader operations.

**Open Bootloader** relies on STM32Cube HAL/LL drivers for hardware system initialization such as the clocks and the communication interfaces configuration.

**Open Bootloader** code can be loaded at any address of user Flash memory with taking necessary precautions to avoid erasing or corrupting it by error (for example use write protection mechanism).

**Open Bootloader** is executed by Cortex-M processor on the non-secure domain and uses the following resources:
 - Non secure internal flash memory/SRAM1
 - Interrupts
 - Clocks and power
 - Communication interfaces
 - GPIOs
 - Systick
 - IWDG

**Open Bootloader** can be customized by changing its location (ie. load it in last user Flash sector or other sectors), its supported protocols, its supported interfaces, and its supported operations.

**Open Bootloader** applications can be found in STM32Cube MCU Packages under the directory `/Projects/<STM32xxBoardyy>/Applications/OpenBootloader/`, where `<STM32xxBoardyy>` is the reference of the used board **e.g.**, `B-U585I-IOT02A`.

## Documentation

Since Open Bootloader supports exactly same protocol interfaces as STM32 System Bootloader, following list of documents provide details of how to use each protocol:
- [AN3155](https://www.st.com/resource/en/application_note/CD00264342.pdf): USART protocol used in the STM32 Bootloader
- [AN5405](https://www.st.com/resource/en/application_note/dm00660346.pdf): FDCAN protocol used in the STM32 Bootloader
- [AN4221](https://www.st.com/resource/en/application_note/DM00072315.pdf): I2C protocol used in the STM32 Bootloader
- [AN3156](https://www.st.com/resource/en/application_note/cd00264379.pdf): USB DFU protocol used in the STM32 Bootloader
- [AN4286](https://www.st.com/resource/en/application_note/DM00081379.pdf): SPI protocol used in the STM32 Bootloader

A useful introductory video series, in six parts, explaining how to use Open Bootloader step by step, can be found here: 
 - [Part 1](https://www.youtube.com/watch?v=_gejWsAn5kg): Introduction
 - [Part 2](https://www.youtube.com/watch?v=kYr7UMieRTo): Using a NUCLEO-G474RE
 - [Part 3](https://www.youtube.com/watch?v=JUBac27tOis): Loading an application
 - [Part 4](https://www.youtube.com/watch?v=7sMDBSlZ7bU): Adding support for the I2C interface
 - [Part 5](https://www.youtube.com/watch?v=rr1W5h94qLU): STLINK-V3SET I2C setup
 - [Part 6](https://www.youtube.com/watch?v=IZ6BpDIm6O0): Loading an application over I2C

## List of Supported Commands

All STM32 System Bootloader commands can be supported by Open Bootloader, which includes:
 - Get Version
 - Get Device ID
 - Get Available Command List
 - Write Memory
 - Read Memory
 - Write Protection setting
 - Read Protection setting
 - Jump to Application
 - Flash Erase
 - Special Command
 - Extended Special Command

## How to use

**Open Bootloader** examples showing how to use this library are available in dedicated repositories, the list of which can be found via the _STM32Cube MCU Offer_ **badge** above.

## Feedback and contributions

Please refer to the [CONTRIBUTING.md](CONTRIBUTING.md) guide.