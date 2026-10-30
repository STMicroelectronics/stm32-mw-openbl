/**
  ******************************************************************************
  * @file    openbl_itf_ram.h
  * @brief   Header for openbl_itf_ram.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_ITF_RAM_H
#define OPENBL_ITF_RAM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_mem.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_RAM_JumpToAddress(openbl_mem_t *p_mem, uint32_t address);
uint8_t OPENBL_RAM_Read(openbl_mem_t *p_mem, uint32_t address);
void OPENBL_RAM_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_RAM_H */
