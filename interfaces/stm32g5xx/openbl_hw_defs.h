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
/* Device ID from DBGMCU_IDCODE (DEV_ID[11:0]) - RM0514 "Debug support (DBGMCU)". */

/* DEV_ID = 0x44A : STM32G55x / STM32G56x */
#if (defined (STM32G551xx) || defined (STM32G56xx) || defined (STM32G561xx) || defined (STM32G564xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID (0x44A >> 8) */
#define OPENBL_DEVICE_ID_LSB              0x4AU                       /* LSB byte of device ID (0x44A & 0xFF) */
#endif /* End of STM32G55x / STM32G56x */

/* DEV_ID = 0x44B : STM32G53x / STM32G54x */
#if (defined (STM32G53xx) || defined (STM32G54xx) || defined (STM32G531xx) || defined (STM32G541xx) \
     || defined (STM32G534xx) || defined (STM32G544xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID (0x44B >> 8) */
#define OPENBL_DEVICE_ID_LSB              0x4BU                       /* LSB byte of device ID (0x44B & 0xFF) */
#endif /* End of STM32G53x / STM32G54x */

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

#if (defined (STM32G551xx) || defined (STM32G56xx) || defined (STM32G561xx) || defined (STM32G564xx) \
     || defined (STM32G53xx) || defined (STM32G54xx) || defined (STM32G531xx) || defined (STM32G541xx) \
     || defined (STM32G534xx) || defined (STM32G544xx))
#define OPENBL_RAM_SIZE                   SRAM1_SIZE                  /* Size of user-accessible SRAM in bytes */
#define OPENBL_RAM_START_ADDRESS          SRAM1_BASE
#define OPENBL_RAM_END_ADDRESS            (OPENBL_RAM_START_ADDRESS + OPENBL_RAM_SIZE)

#define OPENBL_OB_START_ADDRESS           0x40022050U                 /* FLASH option bytes registers start address */

#define OPENBL_OB1_SIZE                   0xB0U                       /* OB area 1: from OPTSR_PRG up to HDP1R_PRG */
#define OPENBL_OB1_START_ADDRESS          OPENBL_OB_START_ADDRESS
#define OPENBL_OB1_END_ADDRESS            (OPENBL_OB1_START_ADDRESS + OPENBL_OB1_SIZE)

#define OPENBL_OB2_SIZE                   0x20U                       /* OB area 2: WRP2R_PRG, EDATA2R_PRG, HDP2R_PRG */
#define OPENBL_OB2_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x190U) /* 0x400221E0U */
#define OPENBL_OB2_END_ADDRESS            (OPENBL_OB2_START_ADDRESS + OPENBL_OB2_SIZE)

#define OPENBL_OB3_SIZE                   0x0U                       /* There is no OB3 option byte region */
#define OPENBL_OB3_START_ADDRESS          0x0U
#define OPENBL_OB3_END_ADDRESS            0x0U

#define OPENBL_OB4_SIZE                   0x0U                       /* There is no OB4 option byte region */
#define OPENBL_OB4_START_ADDRESS          0x0U
#define OPENBL_OB4_END_ADDRESS            0x0U

#define OPENBL_EB_SIZE                    (1024U)                     /* Size of Engi bytes area in bytes */
#define OPENBL_EB_START_ADDRESS           0x40022400U
#define OPENBL_EB_END_ADDRESS             (OPENBL_EB_START_ADDRESS + OPENBL_EB_SIZE)

#define OPENBL_RO_SIZE                    2048U                       /* Size of RO area in bytes */
#define OPENBL_RO_START_ADDRESS           UID_BASE
#define OPENBL_RO_END_ADDRESS             (OPENBL_RO_START_ADDRESS + OPENBL_RO_SIZE)

#if (defined (STM32G551xx) || defined (STM32G56xx) || defined (STM32G561xx) || defined (STM32G564xx))
#define OPENBL_EEPROM_SIZE                (96U * 1024U)               /* Size of high-cycle data area in bytes */
#elif (defined (STM32G53xx) || defined (STM32G54xx) || defined (STM32G531xx) || defined (STM32G541xx) \
       || defined (STM32G534xx) || defined (STM32G544xx))
#define OPENBL_EEPROM_SIZE                (48U * 1024U)               /* Size of high-cycle data area in bytes */
#endif /* End of STM32G55x / G56x / G53x / G54x */
#define OPENBL_EEPROM_START_ADDRESS       0x09000000U
#define OPENBL_EEPROM_END_ADDRESS         (OPENBL_EEPROM_START_ADDRESS + OPENBL_EEPROM_SIZE)
#endif /* End of STM32G55x / G56x / G53x / G54x */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_HW_DEFS_H */
