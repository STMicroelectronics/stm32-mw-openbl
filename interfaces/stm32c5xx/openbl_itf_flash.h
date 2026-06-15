/**
  ******************************************************************************
  * @file    openbl_itf_flash.h
  * @brief   Header for openbl_itf_flash.c module
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
#ifndef OPENBL_ITF_FLASH_H
#define OPENBL_ITF_FLASH_H

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
void OPENBL_FLASH_JumpToAddress(openbl_mem_t *p_mem, uint32_t address);
void OPENBL_FLASH_Lock(openbl_mem_t *p_mem);
void OPENBL_FLASH_Unlock(openbl_mem_t *p_mem);
void OPENBL_FLASH_Unlock_OB(openbl_mem_t *p_mem);
void OPENBL_FLASH_OB_Launch(openbl_mem_t *p_mem);
uint8_t OPENBL_FLASH_Read(openbl_mem_t *p_mem, uint32_t address);
void OPENBL_FLASH_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length);
openbl_status_t OPENBL_FLASH_MassOrBankErase(openbl_mem_t *p_mem, uint8_t *p_data, uint32_t data_length);
openbl_status_t OPENBL_FLASH_Erase(openbl_mem_t *p_mem, uint8_t *p_data, uint32_t data_length);
openbl_status_t OPENBL_FLASH_SetWriteProtection(openbl_mem_t *p_mem, openbl_state_t state, uint8_t *p_pages_list, \
                                                uint32_t length);

openbl_state_t OPENBL_FLASH_GetProductProtectionState(void);
void OPENBL_Enable_BusyState_Flag(void);
void OPENBL_Disable_BusyState_Flag(void);

OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_IsReadyForOperation(hal_flash_handle_t *hflash);
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_ErasePage(openbl_mem_t *p_mem,
                                                        hal_flash_bank_t bank,
                                                        uint32_t erase_page,
                                                        uint32_t erase_area,
                                                        uint32_t timeout_msec);
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_WaitForEndOfOperation(openbl_mem_t *p_mem, uint32_t timeout_msec);
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_MassErase(openbl_mem_t *p_mem, uint32_t timeout_msec);
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_EraseBank(openbl_mem_t *p_mem, hal_flash_bank_t bank, \
                                                        uint32_t timeout_msec);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_FLASH_H */
