/**
  ******************************************************************************
  * @file    openbl_usb_cmd.h
  * @brief   Contains USB protocol commands
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2026 STMicroelectronics.
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

uint16_t OPENBL_USB_EraseMemory(openbl_itf_obj_t *p_itf, uint32_t address);
void OPENBL_USB_WriteMemory(openbl_itf_obj_t *p_itf, uint32_t address, uint8_t *p_data, uint32_t length);
void OPENBL_USB_ReadMemory(openbl_itf_obj_t *p_itf, uint32_t address, uint8_t *p_data, uint32_t length);
void OPENBL_USB_Jump(openbl_itf_obj_t *p_itf, uint32_t address);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_USB_CMD_H */
