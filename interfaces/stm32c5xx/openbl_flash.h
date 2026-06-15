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
__STATIC_FORCEINLINE uint32_t OPENBL_FLASH_ReadFlag(const FLASH_TypeDef *flashx, uint32_t flags)
{
  return (STM32_READ_BIT(flashx->SR, flags));
}

/**
  * @brief  Check the flash control access registers lock state.
  * @param  flashx FLASH instance.
  * @retval State of flash lock (1 locked / 0 unlocked).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_FLASH_IsLocked(const FLASH_TypeDef *flashx)
{
  return ((STM32_READ_BIT(flashx->CR, FLASH_CR_LOCK) == (FLASH_CR_LOCK)) ? 1UL : 0UL);
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
__STATIC_FORCEINLINE void OPENBL_FLASH_ClearFlag(FLASH_TypeDef *flashx, uint32_t flags)
{
  STM32_SET_BIT(flashx->CCR, flags);
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
__STATIC_FORCEINLINE uint32_t OPENBL_FLASH_IsActiveFlag(const FLASH_TypeDef *flashx, uint32_t flags)
{
  return ((STM32_READ_BIT(flashx->SR, flags) != 0U) ? 1UL : 0UL);
}

/**
  * @brief  Clear the flash end of operation error flag.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_ClearFlag_EOP(FLASH_TypeDef *flashx)
{
  STM32_SET_BIT(flashx->CCR, FLASH_CCR_CLR_EOP);
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
__STATIC_FORCEINLINE void OPENBL_FLASH_StartErasePage(FLASH_TypeDef *flashx, uint32_t bank, uint32_t area, \
                                                      uint32_t page)
{
  STM32_MODIFY_REG(flashx->CR, (FLASH_CR_BKSEL | FLASH_CR_EDATASEL | FLASH_CR_PNB | FLASH_CR_STRT | FLASH_CR_PER), \
                   (bank | area | (page << FLASH_CR_PNB_Pos) | FLASH_CR_STRT | FLASH_CR_PER));
}

/**
  * @brief  Disable the flash page erase.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_DisablePageErase(FLASH_TypeDef *flashx)
{
  STM32_CLEAR_BIT(flashx->CR, FLASH_CR_PER);
}

/**
  * @brief  Start the flash erase bank.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_StartMassErase(FLASH_TypeDef *flashx)
{
  STM32_SET_BIT(flashx->CR, (FLASH_CR_MER | FLASH_CR_STRT));
}

/**
  * @brief  Disable the flash mass erase.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_FLASH_DisableMassErase(FLASH_TypeDef *flashx)
{
  STM32_CLEAR_BIT(flashx->CR, FLASH_CR_MER);
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
  STM32_MODIFY_REG(flashx->CR, (FLASH_CR_BKSEL | FLASH_CR_BER | FLASH_CR_STRT), (bank | FLASH_CR_BER | FLASH_CR_STRT));
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
  STM32_CLEAR_BIT(flashx->CR, (bank | FLASH_CR_BER));
}

/**
  * @brief  Start the flash option bytes modification.
  * @param  flashx FLASH instance.
  * @retval None.
  */
__STATIC_INLINE void OPENBL_FLASH_OB_StartModification(FLASH_TypeDef *flashx)
{
  STM32_SET_BIT(flashx->OPTCR, FLASH_OPTCR_OPTSTRT);
}

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_FLASH_H */
