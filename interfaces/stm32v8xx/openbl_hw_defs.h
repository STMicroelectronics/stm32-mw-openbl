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
#if (defined (STM32V863xx) || defined (STM32V873xx))
#define OPENBL_DEVICE_ID_MSB              0x04U                       /* MSB byte of device ID */
#define OPENBL_DEVICE_ID_LSB              0x99U                       /* LSB byte of device ID */
#endif /* End of STM32V863xx || STM32V863xx */

/* -------------------------- Definitions for Memories ---------------------- */
#define OPENBL_NVM_MEM_SIZE               NVM_SIZE                      /* Size of user Flash in bytes */
#define OPENBL_NVM_START_ADDRESS          (NVM_BASE - 0x10000000U)
#define OPENBL_NVM_END_ADDRESS            (OPENBL_NVM_START_ADDRESS + OPENBL_NVM_MEM_SIZE)

#define OPENBL_OTP_SIZE                   NVM_OTP_SIZE                /* Size of OTP area in bytes */
#define OPENBL_OTP_START_ADDRESS          (NVM_OTP_BASE - 0x10000000U)
#define OPENBL_OTP_END_ADDRESS            (OPENBL_OTP_START_ADDRESS + OPENBL_OTP_SIZE)

#define OPENBL_ICP_SIZE                   0x1C800U                    /* Size of ICP area in bytes */
#define OPENBL_ICP_START_ADDRESS          0x0C000000U
#define OPENBL_ICP_END_ADDRESS            (OPENBL_ICP_START_ADDRESS + OPENBL_ICP_SIZE)

#define OPENBL_DEFAULT_MEM                OPENBL_NVM_START_ADDRESS    /* Used for Erase and Write protect CMDs */

#if (defined (STM32V863xx) || defined (STM32V873xx))
#define OPENBL_RAM_SIZE                   (AXISRAM1_SIZE + AXISRAM2_SIZE + AXISRAM3_SIZE + AXISRAM4_SIZE + \
                                           AXISRAM5_SIZE + AXISRAM6FIX_SIZE)   /* Size of AXISRAM in bytes */
#define OPENBL_RAM_START_ADDRESS          (AXISRAM1_BASE - 0x10000000U)
#define OPENBL_RAM_END_ADDRESS            (OPENBL_RAM_START_ADDRESS + OPENBL_RAM_SIZE)

#define OPENBL_OB_START_ADDRESS           0x48022050U                 /* Option bytes (OB) registers start address */

#define OPENBL_OB1_SIZE                   224U                        /* Size of OB1 area in bytes */
#define OPENBL_OB1_START_ADDRESS          OPENBL_OB_START_ADDRESS
#define OPENBL_OB1_END_ADDRESS            (OPENBL_OB1_START_ADDRESS + OPENBL_OB1_SIZE)

#define OPENBL_OB2_SIZE                   640U                        /* Size of OB2 area in bytes */
#define OPENBL_OB2_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x230U)
#define OPENBL_OB2_END_ADDRESS            (OPENBL_OB2_START_ADDRESS + OPENBL_OB2_SIZE)

#define OPENBL_OB3_SIZE                   128U                        /* Size of OB3 area in bytes */
#define OPENBL_OB3_START_ADDRESS          (OPENBL_OB_START_ADDRESS + 0x430U)
#define OPENBL_OB3_END_ADDRESS            (OPENBL_OB3_START_ADDRESS + OPENBL_OB3_SIZE)

#define OPENBL_OB4_SIZE                   0x0U                        /* There is no OB4 option byte region */
#define OPENBL_OB4_START_ADDRESS          0x0U
#define OPENBL_OB4_END_ADDRESS            0x0U

#define OPENBL_RO_SIZE                    2048U                       /* Size of RO area in bytes */
#define OPENBL_RO_START_ADDRESS           UID_BASE
#define OPENBL_RO_END_ADDRESS             (OPENBL_RO_START_ADDRESS + OPENBL_RO_SIZE)

#define OPENBL_EB_SIZE                    (164U)                      /* Size of Engi bytes area in bytes */
#define OPENBL_EB_START_ADDRESS           0x48022800U
#define OPENBL_EB_END_ADDRESS             (OPENBL_EB_START_ADDRESS + OPENBL_EB_SIZE)

#define OPENBL_EEPROM_SIZE                NVM_EDATA_SIZE              /* Size of EEPROM area in bytes */
#define OPENBL_EEPROM_START_ADDRESS       0x09000000U
#define OPENBL_EEPROM_END_ADDRESS         (OPENBL_EEPROM_START_ADDRESS + OPENBL_EEPROM_SIZE)
#endif /* End of STM32V863xx || STM32V873xx */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_HW_DEFS_H */
