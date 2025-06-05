/**
  ******************************************************************************
  * @file    usb_interface.h
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
#ifndef USB_INTERFACE_H
#define USB_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"
#include "stm32g0xx_hal.h"
#include "usbd_def.h"
#include "usbd_dfu.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void OPENBL_USB_Configuration(void);
uint8_t OPENBL_USB_ProtocolDetection(void);
uint16_t OPENBL_USB_SendAddressNack(USBD_HandleTypeDef *pDev);
uint16_t OPENBL_USB_DnloadRdpNack(USBD_HandleTypeDef *pDev);
void OPENBL_USB_UploadRdpNack(USBD_HandleTypeDef *pDev);

#ifdef __cplusplus
}
#endif
#endif /* USB_INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
