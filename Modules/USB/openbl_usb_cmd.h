/**
  ******************************************************************************
  * @file    openbl_usb_cmd.h
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
