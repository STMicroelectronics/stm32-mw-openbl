/**
  ******************************************************************************
  * @file    openbl_hw_defs.h
  * @brief   Contains Open Bootloader HW related definitions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_HW_DEFS_H
#define OPENBL_HW_DEFS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/


/* -------------------------------- Device ID ------------------------------- */
#if (defined (STM32C531xx) || defined (STM32C532xx) || defined (STM32C542xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID */
#define OPENBL_DEVICE_ID_LSB              0x4FU                       /* LSB byte of device ID */
#endif /* End of STM32C531xx || STM32C532xx || STM32C542xx */

#if (defined (STM32C551xx) || defined (STM32C552xx) || defined (STM32C562xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID */
#define OPENBL_DEVICE_ID_LSB              0x4EU                       /* LSB byte of device ID */
#endif /* End of STM32C551xx || STM32C552xx || STM32C562xx */

#if (defined (STM32C591xx) || defined (STM32C593xx) || defined (STM32C5A3xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID */
#define OPENBL_DEVICE_ID_LSB              0x5AU                       /* LSB byte of device ID */
#endif /* End of STM32C591xx || STM32C593xx || STM32C5A3xx */

/* -------------------------- Definitions for Memories ---------------------- */
#define OPENBL_FLASH_MEM_SIZE             FLASH_SIZE                  /* Size of user Flash in bytes */
#define OPENBL_FLASH_START_ADDRESS        FLASH_BASE
#define OPENBL_FLASH_END_ADDRESS          (FLASH_BASE + OPENBL_FLASH_MEM_SIZE)

#define OPENBL_OTP_SIZE                   FLASH_OTP_SIZE              /* Size of OTP area in bytes */
#define OPENBL_OTP_START_ADDRESS          FLASH_OTP_BASE
#define OPENBL_OTP_END_ADDRESS            (OPENBL_OTP_START_ADDRESS + OPENBL_OTP_SIZE)

#define OPENBL_ICP_SIZE                   FLASH_SYSTEM_SIZE           /* Size of ICP area in bytes */
#define OPENBL_ICP_START_ADDRESS          FLASH_SYSTEM_BASE
#define OPENBL_ICP_END_ADDRESS            (OPENBL_ICP_START_ADDRESS + OPENBL_ICP_SIZE)

#define OPENBL_DEFAULT_MEM                OPENBL_FLASH_START_ADDRESS  /* Used for Erase and Write protect CMDs */

#if (defined (STM32C531xx) || defined (STM32C532xx) || defined (STM32C542xx))
#define OPENBL_RAM_SIZE                   (SRAM1_SIZE + SRAM2_SIZE)   /* Size of SRAM in bytes */
#define OPENBL_RAM_START_ADDRESS          SRAM1_BASE
#define OPENBL_RAM_END_ADDRESS            (OPENBL_RAM_START_ADDRESS + OPENBL_RAM_SIZE)

#define OPENBL_OB_START_ADDRESS           0x40022050U                 /* Option bytes (OB) registers start address */

#define OPENBL_OB1_SIZE                   120U                        /* Size of OB1 area in bytes */
#define OPENBL_OB1_START_ADDRESS          OPENBL_OB_START_ADDRESS
#define OPENBL_OB1_END_ADDRESS            (OPENBL_OB1_START_ADDRESS + OPENBL_OB1_SIZE)

#define OPENBL_OB2_SIZE                   36U                         /* Size of OB2 area in bytes */
#define OPENBL_OB2_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x54U)
#define OPENBL_OB2_END_ADDRESS            (OPENBL_OB2_START_ADDRESS + OPENBL_OB2_SIZE)

#define OPENBL_OB3_SIZE                   24U                         /* Size of OB3 area in bytes */
#define OPENBL_OB3_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x98U)
#define OPENBL_OB3_END_ADDRESS            (OPENBL_OB3_START_ADDRESS + OPENBL_OB3_SIZE)

#define OPENBL_OB4_SIZE                   24U                         /* Size of OB4 area in bytes */
#define OPENBL_OB4_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x198U)
#define OPENBL_OB4_END_ADDRESS            (OPENBL_OB4_START_ADDRESS + OPENBL_OB4_SIZE)

#define OPENBL_RO_SIZE                    1536U                       /* Size of RO area in bytes */
#define OPENBL_RO_START_ADDRESS           UID_BASE
#define OPENBL_RO_END_ADDRESS             (OPENBL_RO_START_ADDRESS + OPENBL_RO_SIZE)

#define OPENBL_EB_SIZE                    (1U * 1024U)                /* Size of Engi bytes area in bytes */
#define OPENBL_EB_START_ADDRESS           0x40022400U
#define OPENBL_EB_END_ADDRESS             (OPENBL_EB_START_ADDRESS + OPENBL_EB_SIZE)

#define OPENBL_EEPROM_SIZE                (48U * 1024U)               /* Size of EEPROM area in bytes */
#define OPENBL_EEPROM_START_ADDRESS       0x09000000U
#define OPENBL_EEPROM_END_ADDRESS         (OPENBL_EEPROM_START_ADDRESS + OPENBL_EEPROM_SIZE)
#endif /* End of STM32C531xx || STM32C532xx || STM32C542xx */

#if (defined (STM32C551xx) || defined (STM32C552xx) || defined (STM32C562xx))
#define OPENBL_RAM_SIZE                   (SRAM1_SIZE + SRAM2_SIZE)   /* Size of SRAM in bytes */
#define OPENBL_RAM_START_ADDRESS          SRAM1_BASE
#define OPENBL_RAM_END_ADDRESS            (OPENBL_RAM_START_ADDRESS + OPENBL_RAM_SIZE)

#define OPENBL_OB_START_ADDRESS           0x40022050U                 /* Option bytes (OB) registers start address */

#define OPENBL_OB1_SIZE                   120U                        /* Size of OB1 area in bytes */
#define OPENBL_OB1_START_ADDRESS          OPENBL_OB_START_ADDRESS
#define OPENBL_OB1_END_ADDRESS            (OPENBL_OB1_START_ADDRESS + OPENBL_OB1_SIZE)

#define OPENBL_OB2_SIZE                   36U                         /* Size of OB2 area in bytes */
#define OPENBL_OB2_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x54U)
#define OPENBL_OB2_END_ADDRESS            (OPENBL_OB2_START_ADDRESS + OPENBL_OB2_SIZE)

#define OPENBL_OB3_SIZE                   24U                         /* Size of OB3 area in bytes */
#define OPENBL_OB3_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x98U)
#define OPENBL_OB3_END_ADDRESS            (OPENBL_OB3_START_ADDRESS + OPENBL_OB3_SIZE)

#define OPENBL_OB4_SIZE                   24U                         /* Size of OB4 area in bytes */
#define OPENBL_OB4_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x198U)
#define OPENBL_OB4_END_ADDRESS            (OPENBL_OB4_START_ADDRESS + OPENBL_OB4_SIZE)

#define OPENBL_RO_SIZE                    1536U                       /* Size of RO area in bytes */
#define OPENBL_RO_START_ADDRESS           UID_BASE
#define OPENBL_RO_END_ADDRESS             (OPENBL_RO_START_ADDRESS + OPENBL_RO_SIZE)

#define OPENBL_EB_SIZE                    (1U * 1024U)                /* Size of Engi bytes area in bytes */
#define OPENBL_EB_START_ADDRESS           0x40022400U
#define OPENBL_EB_END_ADDRESS             (OPENBL_EB_START_ADDRESS + OPENBL_EB_SIZE)

#define OPENBL_EEPROM_SIZE                (48U * 1024U)               /* Size of EEPROM area in bytes */
#define OPENBL_EEPROM_START_ADDRESS       0x09000000U
#define OPENBL_EEPROM_END_ADDRESS         (OPENBL_EEPROM_START_ADDRESS + OPENBL_EEPROM_SIZE)
#endif /* End of STM32C551xx || STM32C552xx || STM32C562xx */

#if (defined (STM32C591xx) || defined (STM32C593xx) || defined (STM32C5A3xx))
#define OPENBL_RAM_SIZE                   (SRAM1_SIZE + SRAM2_SIZE)   /* Size of SRAM in bytes */
#define OPENBL_RAM_START_ADDRESS          SRAM1_BASE
#define OPENBL_RAM_END_ADDRESS            (OPENBL_RAM_START_ADDRESS + OPENBL_RAM_SIZE)

#define OPENBL_OB_START_ADDRESS           0x40022050U                 /* Option bytes (OB) registers start address */

#define OPENBL_OB1_SIZE                   120U                        /* Size of OB1 area in bytes */
#define OPENBL_OB1_START_ADDRESS          OPENBL_OB_START_ADDRESS
#define OPENBL_OB1_END_ADDRESS            (OPENBL_OB1_START_ADDRESS + OPENBL_OB1_SIZE)

#define OPENBL_OB2_SIZE                   36U                         /* Size of OB2 area in bytes */
#define OPENBL_OB2_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x54U)
#define OPENBL_OB2_END_ADDRESS            (OPENBL_OB2_START_ADDRESS + OPENBL_OB2_SIZE)

#define OPENBL_OB3_SIZE                   24U                         /* Size of OB3 area in bytes */
#define OPENBL_OB3_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x98U)
#define OPENBL_OB3_END_ADDRESS            (OPENBL_OB3_START_ADDRESS + OPENBL_OB3_SIZE)

#define OPENBL_OB4_SIZE                   24U                         /* Size of OB4 area in bytes */
#define OPENBL_OB4_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x198U)
#define OPENBL_OB4_END_ADDRESS            (OPENBL_OB4_START_ADDRESS + OPENBL_OB4_SIZE)

#define OPENBL_RO_SIZE                    1536U                       /* Size of RO area in bytes */
#define OPENBL_RO_START_ADDRESS           UID_BASE
#define OPENBL_RO_END_ADDRESS             (OPENBL_RO_START_ADDRESS + OPENBL_RO_SIZE)

#define OPENBL_EB_SIZE                    (1U * 1024U)                /* Size of Engi bytes area in bytes */
#define OPENBL_EB_START_ADDRESS           0x40022400U
#define OPENBL_EB_END_ADDRESS             (OPENBL_EB_START_ADDRESS + OPENBL_EB_SIZE)

#define OPENBL_EEPROM_SIZE                (48U * 1024U)               /* Size of EEPROM area in bytes */
#define OPENBL_EEPROM_START_ADDRESS       0x09000000U
#define OPENBL_EEPROM_END_ADDRESS         (OPENBL_EEPROM_START_ADDRESS + OPENBL_EEPROM_SIZE)
#endif /* End of STM32C591xx || STM32C593xx || STM32C5A3xx */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_HW_DEFS_H */
