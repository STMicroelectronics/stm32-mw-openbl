/**
  ******************************************************************************
  * @file    openbl_itf_nvm.h
  * @brief   Header for openbl_itf_nvm.c module
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
#ifndef OPENBL_ITF_NVM_H
#define OPENBL_ITF_NVM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_types.h"
#include "openbl_mem.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_NVM_JumpToAddress(openbl_mem_t *p_mem, uint32_t address);
void OPENBL_NVM_Lock(openbl_mem_t *p_mem);
void OPENBL_NVM_Unlock(openbl_mem_t *p_mem);
void OPENBL_NVM_Unlock_OB(openbl_mem_t *p_mem);
void OPENBL_NVM_OB_Launch(openbl_mem_t *p_mem);
uint8_t OPENBL_NVM_Read(openbl_mem_t *p_mem, uint32_t address);
void OPENBL_NVM_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length);
openbl_status_t OPENBL_NVM_SetWriteProtection(openbl_mem_t *p_mem, openbl_state_t state, uint8_t *p_pages_list, \
                                              uint32_t length);

openbl_state_t OPENBL_NVM_GetProductProtectionState(void);
OPENBL_RAM_FUNCTION hal_status_t OPENBL_NVM_IsReadyForOperation(hal_nvm_handle_t *hnvm);
OPENBL_RAM_FUNCTION hal_status_t OPENBL_NVM_WaitForEndOfOperation(openbl_mem_t *p_mem, uint32_t timeout_msec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_NVM_H */
