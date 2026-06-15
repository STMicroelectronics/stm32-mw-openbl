/**
  ******************************************************************************
  * @file    openbl_usb_dfu_descriptors.h
  * @brief   Open Bootloader USB DFU Device descriptor header file
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
#ifndef OPENBL_USB_DFU_DESCRIPTORS_H
#define OPENBL_USB_DFU_DESCRIPTORS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
uint8_t *OPENBL_USB_Desc_GetStringFramework(ULONG *Length);

/* Private defines -----------------------------------------------------------*/

#define USBD_IDX_MFC_STR                               0x01U
#define USBD_IDX_PRODUCT_STR                           0x02U
#define USBD_IDX_SERIAL_STR                            0x03U

#define OPENBL_USBD_STRING_FRAMEWORK_MAX_LENGTH        320U

#define OPENBL_USBD_DEVICE_ID1                        (UID_BASE)
#define OPENBL_USBD_DEVICE_ID2                        (UID_BASE + 0x4U)
#define OPENBL_USBD_DEVICE_ID3                        (UID_BASE + 0x8U)
#define OPENBL_USBD_STRING_SERIAL_SIZE                0x1AU

#define OPENBL_USBD_DFU_STRING_FLASH_DESC_INDEX       0x06U
#define OPENBL_USBD_DFU_STRING_FLASH_DESC             "@Internal Flash /0x08000000/64*08Kg"

#define OPENBL_USBD_DFU_STRING_OB_DESC_INDEX          0x07U
#define OPENBL_USBD_DFU_STRING_OB_DESC                "@Option Bytes /0x40022050/01*432e"

#define OPENBL_USBD_DFU_STRING_RO_DESC_INDEX          0x08U
#define OPENBL_USBD_DFU_STRING_RO_DESC                "@Read only Memory /0x08FFF800/01.5*01Kg"

#define OPENBL_USBD_DFU_STRING_OTP_DESC_INDEX         0x09U
#define OPENBL_USBD_DFU_STRING_OTP_DESC               "@OTP Memory /0x08FFE000/04.5*01Kg"

#define OPENBL_USBD_DFU_STRING_ENGI_DESC_INDEX        0x0AU
#define OPENBL_USBD_DFU_STRING_ENGI_DESC              "@ENGI Memory /0x40022400/01*01Kg"

#define OPENBL_USBD_DFU_STRING_DEVICE_ID_DESC_INDEX   0x14U
#define OPENBL_USBD_DFU_STRING_DEVICE_ID_DESC         "@Device ID/0x44E,@Revision ID/0x0001"

/* Private macro -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* OPENBL_USB_DFU_DESCRIPTORS_H */
