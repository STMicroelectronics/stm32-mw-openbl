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
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu.h"
#include "openbl_core.h"

uint16_t OPENBL_USB_EraseMemory(uint32_t Add);
void OPENBL_USB_WriteMemory(uint8_t *pSrc, uint8_t *pDest, uint32_t Length);
uint8_t *OPENBL_USB_ReadMemory(uint8_t *pSrc, uint8_t *pDest, uint32_t Length);

/* Exported variables --------------------------------------------------------*/
extern USBD_HandleTypeDef hUsbDeviceFS;

#ifdef __cplusplus
}
#endif
#endif /* OPENBL_USB_CMD_H */
