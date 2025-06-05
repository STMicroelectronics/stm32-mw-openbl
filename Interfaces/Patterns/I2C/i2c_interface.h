/**
  ******************************************************************************
  * @file    i2c_interface.h
  * @author  MCD Application Team
  * @brief   Header for i2c_interface.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "common_interface.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_I2C_Configuration(void);
uint8_t OPENBL_I2C_ProtocolDetection(void);

uint8_t OPENBL_I2C_GetCommandOpcode(void);
uint8_t OPENBL_I2C_ReadByte(void);
void OPENBL_I2C_SendByte(uint8_t Byte);
void OPENBL_I2C_WaitAddress(void);
void OPENBL_I2C_SendAcknowledgeByte(uint8_t Byte);

#if defined (__CC_ARM)
void OPENBL_I2C_WaitNack(void);
void OPENBL_I2C_WaitStop(void);
void OPENBL_I2C_SendBusyByte(void);
#else
__ramfunc void OPENBL_I2C_WaitNack(void);
__ramfunc void OPENBL_I2C_WaitStop(void);
__ramfunc void OPENBL_I2C_SendBusyByte(void);
#endif /* (__CC_ARM) */

#endif /* I2C_INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
