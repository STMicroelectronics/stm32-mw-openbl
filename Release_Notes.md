

# Release Notes for
# <mark>Open Bootloader SW Pack</mark>


# Purpose

STM32Cube enables developers to achieve design success. With a comprehensive suite of professional development tools and embedded software components, STM32Cube allows developers to differentiate products, streamline design cycles, and reduce costs. STM32Cube ecosystem supports all design steps, including selection, configuration, development, debugging, programming, and monitoring.

The STM32Cube embedded software offer provides ready-to-use software components that can be added to a project. It includes STM32 peripheral driver APIs with two levels of abstraction, middleware, board drivers, and examples. There are several distribution channels, including the STM32CubeMX2 tool, the ST website, and GitHub. All embedded software comes with enhanced online documentation, with flowcharts and user sequences.

**OpenBootloader** is an In-Application Programming (IAP) solution. It is fully compatible with the STM32 System Bootloader, meaning it supports the same interfaces and commands. It also uses the same tools, such as STM32CubeProgrammer.

## Documentation

More documentation is available at [**Open Bootloader SW Pack online documentation**](https://dev.st.com/stm32cube-docs/mw-open-bootloader/latest/en/index.html).

Since Open Bootloader supports exactly the same protocol interfaces as the STM32 System Bootloader, the following documents provide details on how to use each protocol:

- [**AN3155**](https://www.st.com/resource/en/application_note/CD00264342.pdf): USART protocol used in the STM32 Bootloader
- [**AN4221**](https://www.st.com/resource/en/application_note/DM00072315.pdf): I2C protocol used in the STM32 Bootloader
- [**AN3156**](https://www.st.com/resource/en/application_note/CD00264379.pdf): USB DFU protocol used in the STM32 Bootloader
- [**AN4286**](https://www.st.com/resource/en/application_note/DM00081379.pdf): SPI protocol used in the STM32 Bootloader


# Update history

<label for="collapse-section-7.1.0" aria-hidden="true">__7.1.0 / 21-September-2026__</label>
<div>

## Main changes

Release of Open Bootloader SW Pack update:

- Add STM32G5, STM32H7P and STM32V8 series support.


## Contents

**OpenBootloader** supports most STM32 System Bootloader commands, including:

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

## Known limitations

- None

## Development toolchains and compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V9.60.3 + ST-LINK
- MDK-ARM Keil uVision V5.42
- STM32CubeIDE for Visual Studio Code (GCC13 compiler)
- STM32CubeMX2 V1.1.1

## Supported devices and boards

- STM32C5 series
- STM32G5 series
- STM32H7P series
- STM32V8 series

## Backward compatibility

- None

## Dependencies

- STM32C5xx HAL Drivers V2.0.0
- STM32G5xx HAL Drivers V2.0.0
- STM32V8xx HAL Drivers V2.0.0
- STM32H7Pxx HAL Drivers V2.0.0
- USBX SW Pack V2.2.0

</div>

<label for="collapse-section-7.0.0" aria-hidden="true">__7.0.0 / 12-June-2026__</label>
<div>

## Main changes

First official release of **Open Bootloader SW Pack**.


## Contents

**OpenBootloader** supports most STM32 System Bootloader commands, including:

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

## Known limitations

- None

## Development toolchains and compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V9.60.3 + ST-LINK
- MDK-ARM Keil uVision V5.42
- STM32CubeIDE for Visual Studio Code (GCC13 compiler)
- STM32CubeMX2 V1.0.1

## Supported devices and boards

- STM32C5 series

## Backward compatibility

- None

## Dependencies

- STM32C5xx HAL Drivers V2.0.0
- USBX SW Pack V2.1.0

</div>


For complete documentation on STM32 microcontrollers,
visit: [www.st.com/stm32](http://www.st.com/stm32)
<abbr title="Based on template cx566953 version 2.1">Info</abbr>

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.