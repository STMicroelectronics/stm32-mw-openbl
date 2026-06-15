/**
  ******************************************************************************
  * @file    openbl_itf_usart.h
  * @brief   Header for openbl_itf_usart.c module
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
#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

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
openbl_status_t OPENBL_ITF_USART_Init(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_USART_DeInit(openbl_itf_obj_t *p_itf);
void openbl_user_usart_deinit(void);
uint8_t OPENBL_ITF_USART_ProtocolDetection(openbl_itf_obj_t *p_itf);
uint8_t OPENBL_ITF_USART_GetCommandOpcode(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_USART_SendAcknowledge(openbl_itf_obj_t *p_itf, uint8_t acknowledge);

uint8_t OPENBL_ITF_USART_ReadByte(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_USART_SendByte(openbl_itf_obj_t *p_itf, uint8_t byte);
void OPENBL_ITF_USART_SpecialCommandProcess(openbl_itf_obj_t *p_itf, openbl_special_cmd_t *p_special_cmd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* USART_INTERFACE_H */
