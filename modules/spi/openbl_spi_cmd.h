/**
  ******************************************************************************
  * @file    openbl_spi_cmd.h
  * @brief   Header for openbl_spi_cmd.c module
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
#ifndef OPENBL_SPI_CMD_H
#define OPENBL_SPI_CMD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_SPI_VERSION                 0x20U  /* Open Bootloader SPI protocol V1.1 */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
openbl_commands_t *OPENBL_SPI_GetCommandsList(void);
void OPENBL_SPI_FillCommandsOpcodesList(const openbl_commands_t *p_spi_cmd, \
                                        openbl_cmd_opcodes_list_t *p_cmd_opcodes_list);
void OPENBL_SPI_GetCommand(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_GetVersion(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_GetID(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_ReadMemory(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_WriteMemory(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_Go(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_ReadoutProtect(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_ReadoutUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_EraseMemory(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_WriteProtect(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_WriteUnprotect(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_SpecialCommand(openbl_itf_obj_t *p_itf);
void OPENBL_SPI_ExtendedSpecialCommand(openbl_itf_obj_t *p_itf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_SPI_CMD_H */
