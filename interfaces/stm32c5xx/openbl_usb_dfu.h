/**
  ******************************************************************************
  * @file    openbl_usb_dfu.h
  *
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
#ifndef OPENBL_USB_DFU_H
#define OPENBL_USB_DFU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_dfu.h"

#include "openbl_interfaces.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void OPENBL_USB_DFU_Activate(void *dfu_instance);
void OPENBL_USB_DFU_Deactivate(void *dfu_instance);
UINT OPENBL_USB_DFU_Upload(VOID *dfu_instance, ULONG block_number, UCHAR *p_data, ULONG length, ULONG *actual_length);
UINT OPENBL_USB_DFU_Download(VOID *dfu_instance, ULONG block_number, UCHAR *p_data, ULONG length, ULONG *media_status);
UINT OPENBL_USB_DFU_GetStatus(VOID *dfu_instance, ULONG *media_status);
UINT OPENBL_USB_DFU_ConnectionCallback(ULONG device_state);
UINT OPENBL_USB_DFU_Notify(VOID *dfu_instance, ULONG notification);
UINT OPENBL_USB_FDU_Leave(VOID *dfu_instance, UX_SLAVE_TRANSFER *transfer);

void OPENBL_USB_DFU_Process(void);

uint32_t OPENBL_USB_DFU_GetPageFromAddress(uint32_t address);
void OPENBL_USBD_DFU_SetInterfaceInstance(openbl_itf_obj_t *p_itf);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_USB_DFU_H */
