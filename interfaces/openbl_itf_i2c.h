/**
  ******************************************************************************
  * @file    openbl_itf_i2c.h
  * @brief   Header for openbl_itf_i2c.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_ITF_I2C_H
#define OPENBL_ITF_I2C_H

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
openbl_status_t OPENBL_ITF_I2C_Init(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_I2C_DeInit(openbl_itf_obj_t *p_itf);
void openbl_user_i2c_deinit(void);
uint8_t OPENBL_ITF_I2C_ProtocolDetection(openbl_itf_obj_t *p_itf);

uint8_t OPENBL_ITF_I2C_GetCommandOpcode(openbl_itf_obj_t *p_itf);
uint8_t OPENBL_ITF_I2C_ReadByte(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_I2C_SendByte(openbl_itf_obj_t *p_itf, uint8_t byte);
void OPENBL_ITF_I2C_WaitAddress(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_I2C_SendAcknowledge(openbl_itf_obj_t *p_itf, uint8_t acknowledge);
void OPENBL_ITF_I2C_SpecialCommandProcess(openbl_itf_obj_t *p_itf, openbl_special_cmd_t *p_special_cmd);
void OPENBL_ITF_I2C_EnableBusyStateSending(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_I2C_DisableBusyStateSending(openbl_itf_obj_t *p_itf);

OPENBL_RAM_FUNCTION void OPENBL_I2C_CommunicationError(void);
OPENBL_RAM_FUNCTION void OPENBL_ITF_I2C_WaitNack(openbl_itf_obj_t *p_itf);
OPENBL_RAM_FUNCTION void OPENBL_ITF_I2C_WaitStop(openbl_itf_obj_t *p_itf);
OPENBL_RAM_FUNCTION void OPENBL_ITF_I2C_SendBusyByte(openbl_itf_obj_t *p_itf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_I2C_H */
