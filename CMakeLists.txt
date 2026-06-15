# origin-pack: STMicroelectronics::open_bootloader@7.0.0
# file-format: 1.0.0
project(STMicroelectronics_open_bootloader_7_0_0)
cmake_minimum_required(VERSION 3.30)
add_library(STMicroelectronics_open_bootloader_7_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dname)
  set(CMSIS_Dname "CMSIS_Dname not set")
  message(DEBUG "CMSIS_Dname is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dvendor)
  set(CMSIS_Dvendor "CMSIS_Dvendor not set")
  message(DEBUG "CMSIS_Dvendor is not set to any value")
endif()


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_open_bootloader_7_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Core#Csub:Common#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Interface#Csub:Interface#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader USART Module#Csub:USART_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader I2C Module#Csub:I2C_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader SPI Module#Csub:SPI_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader USB Module#Csub:USB_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Flash Module#Csub:Flash_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Ram Module#Csub:Ram_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader OB Module#Csub:OB_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader OTP Module#Csub:OTP_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader EEPROM Module#Csub:EEPROM_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader ENGI Bytes Module#Csub:ENGI_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader RO Bytes Module#Csub:RO_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader ICP Module#Csub:ICP_Module#Cversion:7.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader IWDG Module#Csub:IWDG_Module#Cversion:7.0.0")
endif()

# All conditions used by this package

# condition: STMicroelectronics.open_bootloader.7.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(STMicroelectronics.open_bootloader.7.0.0 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0 enabled")
else()
  set(STMicroelectronics.open_bootloader.7.0.0 0)
endif()


# condition: STMicroelectronics.open_bootloader.7.0.0:OpenBL_C5
# description: Open Bootloader ready to use files on STM2C5
if((CMSIS_Dname MATCHES "STM32C5.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5 enabled")
else()
  set(STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5 0)
endif()


# condition: STMicroelectronics.open_bootloader.7.0.0:Use EEPROM
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_EEPROM "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_EEPROM enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use ENGI
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_ENGI "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_ENGI enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use EXTMEM
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_EXTMEM "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_EXTMEM enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use FLASH
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_FLASH "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_FLASH enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use I2C
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_I2C "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_I2C enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use ICP
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_ICP "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_ICP enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use IWDG
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_IWDG "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_IWDG enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use OB
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_OB "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_OB enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use OTP
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_OTP "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_OTP enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use RAM
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_RAM "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_RAM enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use RO
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_RO "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_RO enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use SPI
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_SPI "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_SPI enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use USART
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_USART "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_USART enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use USB
# description: 
set(STMicroelectronics.open_bootloader.7.0.0_Use_USB "1")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_USB enabled")


# condition: STMicroelectronics.open_bootloader.7.0.0:Use full features
# description: OpenBootloader global configuration :dependency to different files
set(STMicroelectronics.open_bootloader.7.0.0_Use_full_features "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Core#.*Csub:Common(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:BOOTLOADER#.*Cgroup:OpenBootloader Interface#.*Csub:Interface(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.open_bootloader.7.0.0_Use_full_features enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Core#Csub:Common#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader Core
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_Core_Common_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "-DCMSIS_USE_BOOTLOADER_OpenBootloader_Core_Common_7_0_0=1")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/core")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/modules/")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/modules/mem/")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/core/openbl_core.c")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/modules/mem/openbl_mem.c")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/modules/mem/openbl_mem.h")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Interface#Csub:Interface#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader Interface
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_Interface_Interface_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "-DCMSIS_USE_BOOTLOADER_OpenBootloader_Interface_Interface_7_0_0=1")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/interfaces/")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_common.c")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_common.h")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_common.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_hw_defs.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader USART Module#Csub:USART_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader USART Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_USART_Module_USART_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USART}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_USART_Module_USART_Module_7_0_0=1>")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USART}:${CMAKE_CURRENT_LIST_DIR}/modules/usart/>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USART}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_usart.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USART}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_usart.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USART}:${CMAKE_CURRENT_LIST_DIR}/modules/usart/openbl_usart_cmd.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USART}:${CMAKE_CURRENT_LIST_DIR}/modules/usart/openbl_usart_cmd.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_USART},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_usart.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader I2C Module#Csub:I2C_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader I2C Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_I2C_Module_I2C_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_I2C}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_I2C_Module_I2C_Module_7_0_0=1>")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_I2C}:${CMAKE_CURRENT_LIST_DIR}/modules/i2c/>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_I2C}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_i2c.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_I2C}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_i2c.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_I2C}:${CMAKE_CURRENT_LIST_DIR}/modules/i2c/openbl_i2c_cmd.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_I2C}:${CMAKE_CURRENT_LIST_DIR}/modules/i2c/openbl_i2c_cmd.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_I2C},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_i2c.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader SPI Module#Csub:SPI_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader SPI Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_SPI_Module_SPI_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_SPI}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_SPI_Module_SPI_Module_7_0_0=1>")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_SPI}:${CMAKE_CURRENT_LIST_DIR}/modules/spi/>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_SPI}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_spi.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_SPI}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_spi.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_SPI}:${CMAKE_CURRENT_LIST_DIR}/modules/spi/openbl_spi_cmd.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_SPI}:${CMAKE_CURRENT_LIST_DIR}/modules/spi/openbl_spi_cmd.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_SPI},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_spi.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader USB Module#Csub:USB_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader USB Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_USB_Module_USB_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USB}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_USB_Module_USB_Module_7_0_0=1>")
  target_include_directories(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USB}:${CMAKE_CURRENT_LIST_DIR}/modules/usb/>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USB}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_usb.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USB}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_usb.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USB}:${CMAKE_CURRENT_LIST_DIR}/modules/usb/openbl_usb_cmd.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_USB}:${CMAKE_CURRENT_LIST_DIR}/modules/usb/openbl_usb_cmd.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_USB},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_usb_dfu.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_USB},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_usb_dfu.h>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_USB},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_usb_dfu_descriptors.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_USB},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_usb_dfu_descriptors.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Flash Module#Csub:Flash_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader Flash Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_Flash_Module_Flash_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_FLASH}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_Flash_Module_Flash_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_FLASH},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_flash.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_FLASH},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_flash.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader Ram Module#Csub:Ram_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader RAM Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_Ram_Module_Ram_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_RAM}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_Ram_Module_Ram_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_RAM},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_ram.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_RAM},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_ram.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader OB Module#Csub:OB_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader Option Bytes Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_OB_Module_OB_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_OB}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_OB_Module_OB_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_OB},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_optionbytes.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_OB},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_optionbytes.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader OTP Module#Csub:OTP_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader OTP Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_OTP_Module_OTP_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_OTP}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_OTP_Module_OTP_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_OTP},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_otp.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_OTP},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_otp.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader EEPROM Module#Csub:EEPROM_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader EEPROM Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_EEPROM_Module_EEPROM_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_EEPROM}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_EEPROM_Module_EEPROM_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_EEPROM},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_eeprom.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_EEPROM},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_eeprom.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader ENGI Bytes Module#Csub:ENGI_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader ENGI Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_ENGI_Bytes_Module_ENGI_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_ENGI}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_ENGI_Bytes_Module_ENGI_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_ENGI},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_engibytes.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_ENGI},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_engibytes.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader RO Bytes Module#Csub:RO_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader ReadOnly bytes Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_RO_Bytes_Module_RO_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_RO}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_RO_Bytes_Module_RO_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_RO},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_read_only.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_RO},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_read_only.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader ICP Module#Csub:ICP_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader System Memory Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_ICP_Module_ICP_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_ICP}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_ICP_Module_ICP_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_ICP},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_systemmemory.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<$<AND:${STMicroelectronics.open_bootloader.7.0.0_Use_ICP},${STMicroelectronics.open_bootloader.7.0.0_OpenBL_C5}>:${CMAKE_CURRENT_LIST_DIR}/interfaces/stm32c5xx/openbl_itf_systemmemory.h>")
endif()

if("Cvendor:STMicroelectronics#Cclass:BOOTLOADER#Cgroup:OpenBootloader IWDG Module#Csub:IWDG_Module#Cversion:7.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # OpenBootloader IWDG Module
  message(DEBUG "Using component BOOTLOADER_OpenBootloader_IWDG_Module_IWDG_Module_7_0_0")
  target_compile_definitions(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_IWDG}:-DCMSIS_USE_BOOTLOADER_OpenBootloader_IWDG_Module_IWDG_Module_7_0_0=1>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_IWDG}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_iwdg.c>")
  target_sources(STMicroelectronics_open_bootloader_7_0_0 INTERFACE "$<${STMicroelectronics.open_bootloader.7.0.0_Use_IWDG}:${CMAKE_CURRENT_LIST_DIR}/interfaces/openbl_itf_iwdg.h>")
endif()

