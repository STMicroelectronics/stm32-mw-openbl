/**
  ******************************************************************************
  * @file    usb_interface.h
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
void OPENBL_USB_DeInit(void);
uint8_t OPENBL_USB_ProtocolDetection(void);
uint16_t OPENBL_USB_SendAddressNack(USBD_HandleTypeDef *pDev);
uint16_t OPENBL_USB_DnloadRdpNack(USBD_HandleTypeDef *pDev);
void OPENBL_USB_UploadRdpNack(USBD_HandleTypeDef *pDev);
uint32_t OPENBL_USB_GetPage(uint32_t Address);

#ifdef __cplusplus
}
#endif
#endif /* USB_INTERFACE_H */
