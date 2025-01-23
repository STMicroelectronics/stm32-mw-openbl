/**
  ******************************************************************************
  * @file    openbl_usb_cmd.h
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
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
#ifndef OPENBL_USB_CMD_H
#define OPENBL_USB_CMD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

uint16_t OPENBL_USB_EraseMemory(uint32_t Add);
uint32_t OPENBL_USB_Download(uint8_t *pSrc, uint32_t Alt, uint32_t Length, uint32_t BlockNumber);
uint8_t *OPENBL_USB_ReadMemory(uint32_t Alt, uint8_t *pDest, uint32_t Length, uint32_t BlockNumber);
void OPENBL_USB_Jump(uint32_t Address);
void OPENBL_USB_WriteProtect(uint8_t *pBuffer, uint32_t Length);
void OPENBL_USB_WriteUnprotect(void);
void OPENBL_USB_ReadProtect(void);
void OPENBL_USB_ReadUnprotect(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_USB_CMD_H */
