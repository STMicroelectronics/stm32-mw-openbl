/**
  ******************************************************************************
  * @file    openbl_can_cmd.h
  * @author  MCD Application Team
  * @brief   Header for openbl_can_cmd.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 20201 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_CAN_CMD_H
#define OPENBL_CAN_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_CAN_VERSION                0x10U    /* Open Bootloader CAN protocol V1.0 */
#define CAN_RAM_BUFFER_SIZE               256U    /* Size of CAN buffer used to store received data from the host */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t tCanRxData[CAN_RAM_BUFFER_SIZE];

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_CAN_GetCommandsList(void);
void OPENBL_CAN_SetCommandsList(OPENBL_CommandsTypeDef *pCanCmd);
void OPENBL_CAN_GetCommand(void);
void OPENBL_CAN_GetVersion(void);
void OPENBL_CAN_GetID(void);
void OPENBL_CAN_Speed(void);
void OPENBL_CAN_ReadMemory(void);
void OPENBL_CAN_WriteMemory(void);
void OPENBL_CAN_Go(void);
void OPENBL_CAN_ReadoutProtect(void);
void OPENBL_CAN_ReadoutUnprotect(void);
void OPENBL_CAN_LegacyEraseMemory(void);
void OPENBL_CAN_WriteProtect(void);
void OPENBL_CAN_WriteUnprotect(void);

#endif /* OPENBL_CAN_CMD_H */
