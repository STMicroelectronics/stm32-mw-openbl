/**
  ******************************************************************************
  * @file    openbl_flash.h
  * @brief   Contains Open Bootloader Flash interface low level functions.
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
#ifndef OPENBL_FLASH_H
#define OPENBL_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Read the state of selected flash operation error flag.
  * @param  flashx FLASH instance.
  * @param  flags
  *         This parameter can be one or a combination of the following values:
  *         @arg @ref LL_FLASH_FLAG_BSY
  *         @arg @ref LL_FLASH_FLAG_WBNE
  *         @arg @ref LL_FLASH_FLAG_DBNE
  *         @arg @ref LL_FLASH_FLAG_STATUS_ALL
  *         @arg @ref LL_FLASH_FLAG_EOP
  *         @arg @ref LL_FLASH_FLAG_WRPERR
  *         @arg @ref LL_FLASH_FLAG_PGSERR
  *         @arg @ref LL_FLASH_FLAG_STRBERR
  *         @arg @ref LL_FLASH_FLAG_INCERR
  *         @arg @ref LL_FLASH_FLAG_OPTCHANGEERR
  *         @arg @ref LL_FLASH_FLAG_ERRORS_ALL
  * @retval State of selected flags (1 active / 0 not active).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_FLASH_ReadFlag(const FLASH_TypeDef *flashx, hal_flash_bank_t bank, uint32_t flags)
{
  const __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->AISR : &flashx->AISR;
  return (STM32_READ_BIT(*reg_addr, flags));
}

/**
  * @brief  Check the flash control access registers lock state.
  * @param  flashx FLASH instance.
  * @retval State of flash lock (1 locked / 0 unlocked).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_FLASH_IsLocked(const FLASH_TypeDef *flashx, hal_flash_bank_t bank)
{
  const __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->AICR : &flashx->AICR;
  return ((STM32_READ_BIT(*reg_addr, FLASH_ACR_LOCK) == (FLASH_ACR_LOCK)) ? 1UL : 0UL);
}

/**
  * @brief  Clear the flash operation error flag.
  * @param  flashx FLASH instance.
  * @param  flags
  *         This parameter can be one or a combination of the following values:
  *         @arg @ref LL_FLASH_FLAG_EOP
  *         @arg @ref LL_FLASH_FLAG_WRPERR
  *         @arg @ref LL_FLASH_FLAG_PGSERR
  *         @arg @ref LL_FLASH_FLAG_STRBERR
  *         @arg @ref LL_FLASH_FLAG_INCERR
  *         @arg @ref LL_FLASH_FLAG_OPTCHANGEERR
  *         @arg @ref LL_FLASH_FLAG_ERRORS_ALL
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_ClearFlag(FLASH_TypeDef *flashx, hal_flash_bank_t bank, uint32_t flags)
{
  __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->AICR : &flashx->BICR;
  STM32_SET_BIT(*reg_addr, flags);
}

/**
  * @brief  Check if any of the selected flag is active.
  * @param  flashx FLASH instance.
  * @param  flags
  *         This parameter can be one or a combination of the following values:
  *         @arg @ref LL_FLASH_FLAG_BSY
  *         @arg @ref LL_FLASH_FLAG_WBNE
  *         @arg @ref LL_FLASH_FLAG_DBNE
  *         @arg @ref LL_FLASH_FLAG_STATUS_ALL
  *         @arg @ref LL_FLASH_FLAG_EOP
  *         @arg @ref LL_FLASH_FLAG_WRPERR
  *         @arg @ref LL_FLASH_FLAG_PGSERR
  *         @arg @ref LL_FLASH_FLAG_STRBERR
  *         @arg @ref LL_FLASH_FLAG_INCERR
  *         @arg @ref LL_FLASH_FLAG_OPTCHANGEERR
  *         @arg @ref LL_FLASH_FLAG_ERRORS_ALL
  * @retval State of selected flag (1 if at least one is active / 0 if none of the selected flags are active).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_FLASH_IsActiveFlag(const FLASH_TypeDef *flashx, uint32_t bank, uint32_t flags)
{
  const __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->ASR : &flashx->BSR;
  return ((STM32_READ_BIT(*reg_addr, flags) != 0U) ? 1UL : 0UL);
}

/**
  * @brief  Clear the flash end of operation error flag.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_ClearFlag_EOP(FLASH_TypeDef *flashx, uint32_t bank)
{
  __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->AICR : &flashx->BICR;
  STM32_SET_BIT(*reg_addr, FLASH_AICR_EOPF);
}

/**
  * @brief  Start the flash erase page.
  * @param  flashx FLASH instance.
  * @param  bank
  *         This parameter can be one of the following values :
  *         @arg @ref LL_FLASH_ERASE_BANK_1
  *         @arg @ref LL_FLASH_ERASE_BANK_2
  * @param  area
  *         This parameter can be one of the following values :
  *         @arg @ref LL_FLASH_ERASE_USER_AREA
  *         @arg @ref LL_FLASH_ERASE_EDATA_AREA
  * @param  page This parameter can take any value in [0:31]
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_StartErasePage(FLASH_TypeDef *flashx, uint32_t bank, uint32_t page)
{
  __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->ACR : &flashx->BCR;
  STM32_MODIFY_REG(*reg_addr, (FLASH_ACR_SSN | FLASH_ACR_SER | FLASH_ACR_START),
                   ((page << FLASH_ACR_SSN_Pos) | FLASH_ACR_SER | FLASH_ACR_START));
}

/**
  * @brief  Disable the flash page erase.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_DisablePageErase(FLASH_TypeDef *flashx, uint32_t bank)
{
  __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->ACR : &flashx->BCR;
  STM32_CLEAR_BIT(*reg_addr, FLASH_ACR_SER);
}

/**
  * @brief  Start the flash erase bank.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_StartMassErase(FLASH_TypeDef *flashx)
{
  STM32_SET_BIT(flashx->ACR, (FLASH_ACR_BER | FLASH_ACR_START));
  STM32_SET_BIT(flashx->BCR, (FLASH_BCR_BER | FLASH_BCR_START));
}

/**
  * @brief  Disable the flash mass erase.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_DisableMassErase(FLASH_TypeDef *flashx)
{
  STM32_CLEAR_BIT(flashx->ACR, FLASH_ACR_BER);
  STM32_CLEAR_BIT(flashx->BCR, FLASH_BCR_BER);
}

/**
  * @brief  Start the flash erase bank.
  * @param  flashx FLASH instance.
  * @param  bank
  *         This parameter can be one of the following values :
  *         @arg @ref LL_FLASH_ERASE_BANK_1
  *         @arg @ref LL_FLASH_ERASE_BANK_2
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_StartEraseBank(FLASH_TypeDef *flashx, uint32_t bank)
{
  __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->ACR : &flashx->BCR;
  STM32_MODIFY_REG(*reg_addr, (FLASH_ACR_BER | FLASH_ACR_START), (FLASH_ACR_BER | FLASH_ACR_START));
}

/**
  * @brief  Disable the flash bank erase.
  * @param  flashx FLASH instance.
  * @param  bank
  *         This parameter can be one of the following values :
  *         @arg @ref LL_FLASH_ERASE_BANK_1
  *         @arg @ref LL_FLASH_ERASE_BANK_2
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_DisableBankErase(FLASH_TypeDef *flashx, uint32_t bank)
{
  __IO uint32_t *reg_addr = (bank == LL_FLASH_BANK_1) ? &flashx->ACR : &flashx->BCR;
  STM32_CLEAR_BIT(*reg_addr, (bank | FLASH_ACR_BER));
}

/**
  * @brief  Start the flash option bytes modification.
  * @param  flashx FLASH instance.
  * @retval None.
  */
__STATIC_INLINE void OPENBL_FLASH_OB_StartModification(FLASH_TypeDef *flashx)
{
  STM32_SET_BIT(flashx->OPTCR, FLASH_OPTCR_OPTSTART);
}

/**
  * @brief  Start the flash option bytes modification.
  * @param  flashx FLASH instance.
  * @retval None.
  */
__STATIC_INLINE hal_flash_bank_t OPENBL_FLASH_GetBank(uint32_t flash_address)
{
  if ((flash_address - FLASH_BASE) < FLASH_BANK_SIZE)
  {
    return HAL_FLASH_BANK_1;
  }
  else
  {
    return HAL_FLASH_BANK_2;
  }
}


#ifdef __cplusplus
}
#endif

#endif /* OPENBL_FLASH_H */
