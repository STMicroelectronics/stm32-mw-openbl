/**
  ******************************************************************************
  * @file    usart_interface.h
  * @author  MCD Application Team
  * @brief   Header for usart_interface.c module
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
#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_USART_Configuration(void);
uint8_t OPENBL_USART_ProtocolDetection(void);

uint8_t OPENBL_USART_GetCommandOpcode(void);
uint8_t OPENBL_USART_ReadByte(void);
void OPENBL_USART_SendByte(uint8_t Byte);

#endif /* USART_INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
