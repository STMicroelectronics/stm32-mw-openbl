/**
  ******************************************************************************
  * @file    openbootloader_conf.h
  * @author  MCD Application Team
  * @brief   Contains Open Bootloader configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBOOTLOADER_CONF_H
#define OPENBOOTLOADER_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "platform.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* -------------------------------- Device ID ------------------------------- */
#define DEVICE_ID_MSB                     0x04U  /* MSB byte of device ID */
#define DEVICE_ID_LSB                     0x82U  /* LSB byte of device ID */

/* -------------------------- Definitions for Memories ---------------------- */
#define FLASH_BL_SIZE                     (2048U * 1024U)  /* Size of FLASH 2 MByte */
#define FLASH_START_ADDRESS               FLASH_BASE  /* start of Flash  */
#define FLASH_END_ADDRESS                 (FLASH_BASE + FLASH_BL_SIZE)  /* end of Flash  */

#define RAM_SIZE                          (768U * 1024U)  /* Size of RAM 768 kByte */
#define RAM_START_ADDRESS                 0x20000000U  /* start of SRAM  */
#define RAM_END_ADDRESS                   (RAM_START_ADDRESS + RAM_SIZE)  /* end of SRAM  */

#define OB_SIZE                           64U  /* Size of OB 64 Byte */
#define OB_START_ADDRESS                  0x40022040U  /* Option bytes registers address */
#define OB_END_ADDRESS                    (OB_START_ADDRESS + OB_SIZE)  /* Option bytes end address*/

#define OTP_BL_SIZE                       512U  /* Size of OTP 512 Byte */
#define OTP_START_ADDRESS                 0x0BFA0000U  /* OTP registers address */
#define OTP_END_ADDRESS                   (OTP_START_ADDRESS + OTP_BL_SIZE)  /* OTP end address */

#define ICP_SIZE                          (32U * 1024U)  /* Size of ICP 32 kByte */
#define ICP_START_ADDRESS                 0x0BF90000U  /* System memory registers address */
#define ICP_END_ADDRESS                   (ICP_START_ADDRESS + ICP_SIZE)  /* System memory registers end address */

#define EB_SIZE                           1120U  /* Size of Engi bytes 1120 Byte */
#define EB_START_ADDRESS                  0x0BFA0500U  /* Engi bytes start address */
#define EB_END_ADDRESS                    (EB_START_ADDRESS + EB_SIZE)  /* Engi bytes end address  */

#define OPENBL_RAM_SIZE                   0x11800U  /* RAM used by the Open Bootloader 71680 Bytes */

#define OPENBL_DEFAULT_MEM                FLASH_START_ADDRESS  /* Default address used for erase and write/read protect commands */

#define RDP_LEVEL_0                       OB_RDP_LEVEL_0
#define RDP_LEVEL_1                       OB_RDP_LEVEL_1
#define RDP_LEVEL_2                       OB_RDP_LEVEL_2

#define AREA_ERROR                        0x0U  /* Error Address Area */
#define FLASH_AREA                        0x1U  /* Flash Address Area */
#define RAM_AREA                          0x2U  /* RAM Address area */
#define OB_AREA                           0x3U  /* Option bytes Address area */
#define OTP_AREA                          0x4U  /* OTP Address area */
#define ICP_AREA                          0x5U  /* System memory area */
#define EB_AREA                           0x6U  /* Engi bytes Address area */

#define FLASH_MASS_ERASE                  0xFFFF
#define FLASH_BANK1_ERASE                 0xFFFE
#define FLASH_BANK2_ERASE                 0xFFFD

#define INTERFACES_SUPPORTED              6U

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* OPENBOOTLOADER_CONF_H */
