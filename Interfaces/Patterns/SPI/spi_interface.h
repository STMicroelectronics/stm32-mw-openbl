/**
  ******************************************************************************
  * @file    spi_interface.h
  * @author  MCD Application Team
  * @brief   Header for spi_interface.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SPI_INTERFACE_H
#define SPI_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_SPI_Configuration(void);
void OPENBL_SPI_DeInit(void);
uint8_t OPENBL_SPI_ProtocolDetection(void);
uint8_t OPENBL_SPI_GetCommandOpcode(void);
uint8_t OPENBL_SPI_ReadByte(void);
void OPENBL_SPI_SendByte(uint8_t Byte);
void OPENBL_SPI_SendAcknowledgeByte(uint8_t Byte);
void OPENBL_SPI_SpecialCommandProcess(OPENBL_SpecialCmdTypeDef *Frame);

void OPENBL_SPI_EnableBusyState(void);
void OPENBL_SPI_DisableBusyState(void);
__ramfunc void OPENBL_SPI_IRQHandler(void);
__ramfunc void OPENBL_SPI_SendBusyByte(void);

#endif /* SPI_INTERFACE_H */
