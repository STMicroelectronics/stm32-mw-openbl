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
#if (defined (STM32H7P4xx) || defined (STM32H7P5xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID */
#define OPENBL_DEVICE_ID_LSB              0x7BU                       /* LSB byte of device ID */
#endif /* End of STM32H7P4xx || STM32H7P5xx */

/* -------------------------- Definitions for Memories ---------------------- */
#define OPENBL_FLASH_MEM_SIZE             FLASH_SIZE                  /* Size of user Flash in bytes */
#define OPENBL_FLASH_START_ADDRESS        FLASH_BASE
#define OPENBL_FLASH_END_ADDRESS          (FLASH_BASE + OPENBL_FLASH_MEM_SIZE)

#define OPENBL_OTP_SIZE                   FLASH_OTP_SIZE              /* Size of OTP area in bytes */
#define OPENBL_OTP_START_ADDRESS          OTP_AREA_BASE
#define OPENBL_OTP_END_ADDRESS            (OPENBL_OTP_START_ADDRESS + OPENBL_OTP_SIZE)

#define OPENBL_ICP_SIZE                   0x20000           /* Size of ICP area in bytes */
#define OPENBL_ICP_START_ADDRESS          SYSTEM_FLASH_BASE
#define OPENBL_ICP_END_ADDRESS            (OPENBL_ICP_START_ADDRESS + OPENBL_ICP_SIZE)

#define OPENBL_DEFAULT_MEM                OPENBL_FLASH_START_ADDRESS  /* Used for Erase and Write protect CMDs */

#if (defined (STM32H7P4xx) || defined (STM32H7P5xx))
/* Size of SRAM in bytes */
#define OPENBL_RAM_SIZE                   (SRAM1_AXI_SIZE + SRAM2_AXI_SIZE + SRAM3_AXI_SIZE + SRAM4_AXI_SIZE)
#define OPENBL_RAM_START_ADDRESS          SRAM1_AXI_BASE
#define OPENBL_RAM_END_ADDRESS            (OPENBL_RAM_START_ADDRESS + OPENBL_RAM_SIZE)

#define OPENBL_OB_START_ADDRESS           0x52002050U                 /* Option bytes (OB) registers start address */

#define OPENBL_OB1_SIZE                   16U                        /* Size of OB1 area in bytes */
#define OPENBL_OB1_START_ADDRESS          OPENBL_OB_START_ADDRESS
#define OPENBL_OB1_END_ADDRESS            (OPENBL_OB1_START_ADDRESS + OPENBL_OB1_SIZE)

#define OPENBL_OB2_SIZE                   16U                         /* Size of OB2 area in bytes */
#define OPENBL_OB2_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x100U)
#define OPENBL_OB2_END_ADDRESS            (OPENBL_OB2_START_ADDRESS + OPENBL_OB2_SIZE)

#define OPENBL_OB3_SIZE                   88U                         /* Size of OB3 area in bytes */
#define OPENBL_OB3_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x1C0U)
#define OPENBL_OB3_END_ADDRESS            (OPENBL_OB3_START_ADDRESS + OPENBL_OB3_SIZE)

#define OPENBL_OB4_SIZE                   0x0U                        /* There is no OB4 option byte region */
#define OPENBL_OB4_START_ADDRESS          0x0U
#define OPENBL_OB4_END_ADDRESS            0x0U

#define OPENBL_RO_SIZE                    2048U                       /* Size of RO area in bytes */
#define OPENBL_RO_START_ADDRESS           UID_BASE
#define OPENBL_RO_END_ADDRESS             (OPENBL_RO_START_ADDRESS + OPENBL_RO_SIZE)

#define OPENBL_EB_SIZE                    1168U                /* Size of Engi bytes area in bytes */
#define OPENBL_EB_START_ADDRESS           0x52002400U
#define OPENBL_EB_END_ADDRESS             (OPENBL_EB_START_ADDRESS + OPENBL_EB_SIZE)

#define OPENBL_EEPROM_SIZE                0U               /* Size of EEPROM area in bytes */
#define OPENBL_EEPROM_START_ADDRESS       NULL
#define OPENBL_EEPROM_END_ADDRESS         NULL
#endif /* End of STM32H7P4xx || STM32H7P5xx */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_HW_DEFS_H */
