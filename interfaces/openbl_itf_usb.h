/**
  ******************************************************************************
  * @file    openbl_itf_usb.h
  * @brief   Header for openbl_itf_usb.c module
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
#ifndef OPENBL_ITF_USB_H
#define OPENBL_ITF_USB_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
openbl_status_t OPENBL_ITF_USB_Init(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_USB_DeInit(openbl_itf_obj_t *p_itf);
uint8_t OPENBL_ITF_USB_ProtocolDetection(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_USB_SetSofDetect(void);
void OPENBL_ITF_USB_SetJumpRequest(void);
void OPENBL_ITF_USB_Process(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_USB_SetJumpAddress(uint32_t address);
uint32_t OPENBL_ITF_USB_GetPage(uint32_t address);

void openbl_user_usb_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_USB_H */
