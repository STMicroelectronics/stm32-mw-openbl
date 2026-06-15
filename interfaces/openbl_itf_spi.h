/**
  ******************************************************************************
  * @file    spi_interface.h
  * @brief   Header for spi_interface.c module
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
#ifndef OPENBL_ITF_SPI_H
#define OPENBL_ITF_SPI_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_SPI_DUMMY_BYTE                    0xDBU  /* Dummy byte */
#define OPENBL_SPI_SYNC_BYTE                     0x5AU  /* Synchronization byte */
#define OPENBL_SPI_BUSY_BYTE                     0xA5U  /* Busy byte */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
openbl_status_t OPENBL_ITF_SPI_Init(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_SPI_DeInit(openbl_itf_obj_t *p_itf);
void openbl_user_spi_deinit(void);
uint8_t OPENBL_ITF_SPI_ProtocolDetection(openbl_itf_obj_t *p_itf);
uint8_t OPENBL_ITF_SPI_GetCommandOpcode(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_SPI_SendAcknowledge(openbl_itf_obj_t *p_itf, uint8_t byte);
void OPENBL_ITF_SPI_SpecialCommandProcess(openbl_itf_obj_t *p_itf, openbl_special_cmd_t *p_special_cmd);

void OPENBL_ITF_SPI_EnableBusyState(openbl_itf_obj_t *p_itf);
void OPENBL_ITF_SPI_DisableBusyState(openbl_itf_obj_t *p_itf);

OPENBL_RAM_FUNCTION uint8_t OPENBL_ITF_SPI_ReadByte(openbl_itf_obj_t *p_itf);
OPENBL_RAM_FUNCTION void OPENBL_ITF_SPI_SendByte(openbl_itf_obj_t *p_itf, uint8_t byte);
OPENBL_RAM_FUNCTION void OPENBL_ITF_SPI_SetRxNotEmptyState(uint8_t state);
OPENBL_RAM_FUNCTION uint8_t OPENBL_ITF_SPI_GetRxNotEmptyState(void);
OPENBL_RAM_FUNCTION void OPENBL_ITF_SPI_SendBusyByte(openbl_itf_obj_t *p_itf);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_SPI_H */
