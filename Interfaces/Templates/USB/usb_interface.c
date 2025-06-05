/**
  ******************************************************************************
  * @file    usb_interface.c
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

/* Includes ------------------------------------------------------------------*/
#include "usb_interface.h"
#include "usbd_core.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal.h"
#include "usbd_def.h"
#include "usbd_dfu.h"
#include "usbd_ioreq.h"
#include "usbd_dfu_if.h"
#include "usbd_desc.h"
#include "usb_device.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

uint8_t USB_Detection = 0;

/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure USB pins and then initialize the used USB instance.
 * @retval None.
 */
void OPENBL_USB_Configuration(void)
{
}

/**
 * @brief  This function is used to detect if there is any activity on USB protocol.
 * @retval None.
 */
uint8_t OPENBL_USB_ProtocolDetection(void)
{
  uint8_t detected;
  return detected;
}

/**
  * @brief  This function is used to send a nack when the address is not valide by changing
  * the state of the usb to dfu_error.
  * @retval Returns USBD_FAIL.
  */
uint16_t OPENBL_USB_SendAddressNack(USBD_HandleTypeDef *pDev)
{
  return (uint16_t)USBD_FAIL;
}

/**
  * @brief  This function is used to send a nack when the RDP level of download command is egale
  * to 1 by changing the state of the usb to dfu_error.
  * @retval Returns USBD_FAIL.
  */
uint16_t OPENBL_USB_DnloadRdpNack(USBD_HandleTypeDef *pDev)
{
  return (uint16_t)USBD_FAIL;
}

/**
  * @brief  This function is used to send a nack when the RDP level of upload command is egale
  * to 1 by changing the state of the usb to dfu_error.
  * @retval Returns USBD_FAIL.
  */
void OPENBL_USB_UploadRdpNack(USBD_HandleTypeDef *pDev)
{
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
