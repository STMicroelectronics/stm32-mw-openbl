/**
  ******************************************************************************
  * @file    openbl_i2c_cmd.h
  * @brief   Header for openbl_i2c_cmd.c module
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
#ifndef OPENBL_I2C_CMD_H
#define OPENBL_I2C_CMD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_I2C_VERSION                 0x20U               /* Open Bootloader I2C protocol V1.2 */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
openbl_commands_t *OPENBL_I2C_GetCommandsList(void);
void OPENBL_I2C_FillCommandsOpcodesList(const openbl_commands_t *p_i2c_cmd, \
                                        openbl_cmd_opcodes_list_t *p_cmd_opcodes_list);
void OPENBL_I2C_GetCommand(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_GetVersion(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_GetID(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_ReadMemory(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_WriteMemory(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_Go(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_ReadoutProtect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_ReadoutUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_EraseMemory(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_WriteProtect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_WriteUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_NonStretchWriteMemory(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_NonStretchEraseMemory(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_NonStretchWriteProtect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_NonStretchWriteUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_NonStretchReadoutProtect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_NonStretchReadoutUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_SpecialCommand(openbl_itf_obj_t *p_itf);
void OPENBL_I2C_ExtendedSpecialCommand(openbl_itf_obj_t *p_itf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_I2C_CMD_H */
