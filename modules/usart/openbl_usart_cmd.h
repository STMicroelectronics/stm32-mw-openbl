/**
  ******************************************************************************
  * @file    openbl_usart_cmd.h
  * @brief   Header for openbl_usart_cmd.c module
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
#ifndef OPENBL_USART_CMD_H
#define OPENBL_USART_CMD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_USART_VERSION                 0x40U               /* Open Bootloader USART protocol V3.1 */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
openbl_commands_t *OPENBL_USART_GetCommandsList(void);
void OPENBL_USART_FillCommandsOpcodesList(const openbl_commands_t *p_usart_cmd, \
                                          openbl_cmd_opcodes_list_t *p_cmd_opcodes_list);
void OPENBL_USART_GetCommand(openbl_itf_obj_t *p_itf);
void OPENBL_USART_GetVersion(openbl_itf_obj_t *p_itf);
void OPENBL_USART_GetID(openbl_itf_obj_t *p_itf);
void OPENBL_USART_ReadMemory(openbl_itf_obj_t *p_itf);
void OPENBL_USART_WriteMemory(openbl_itf_obj_t *p_itf);
void OPENBL_USART_Go(openbl_itf_obj_t *p_itf);
void OPENBL_USART_ReadoutProtect(openbl_itf_obj_t *p_itf);
void OPENBL_USART_ReadoutUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_USART_EraseMemory(openbl_itf_obj_t *p_itf);
void OPENBL_USART_WriteProtect(openbl_itf_obj_t *p_itf);
void OPENBL_USART_WriteUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_USART_SpecialCommand(openbl_itf_obj_t *p_itf);
void OPENBL_USART_ExtendedSpecialCommand(openbl_itf_obj_t *p_itf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_USART_CMD_H */
