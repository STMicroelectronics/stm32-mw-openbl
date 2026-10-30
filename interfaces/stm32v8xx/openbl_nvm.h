/**
  ******************************************************************************
  * @file    openbl_nvm.h
  * @brief   Contains Open Bootloader NVM interface low level functions.
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
#ifndef OPENBL_NVM_H
#define OPENBL_NVM_H

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
__STATIC_FORCEINLINE uint32_t OPENBL_NVM_ReadFlag(const NVM_TypeDef *nvmx, uint32_t flags)
{
  return (STM32_READ_BIT(nvmx->SSR, flags));
}

/**
  * @brief  Check the flash control access registers lock state.
  * @param  flashx FLASH instance.
  * @retval State of flash lock (1 locked / 0 unlocked).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_NVM_IsLocked(const NVM_TypeDef *nvmx)
{
  return ((STM32_READ_BIT(nvmx->SCR, NVM_CR_LOCK) == (NVM_CR_LOCK)) ? 1UL : 0UL);
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
__STATIC_FORCEINLINE void OPENBL_NVM_ClearFlag(NVM_TypeDef *nvmx, uint32_t flags)
{
  STM32_SET_BIT(nvmx->SCCR, flags);
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
__STATIC_FORCEINLINE uint32_t OPENBL_NVM_IsActiveFlag(const NVM_TypeDef *nvmx, uint32_t flags)
{
  return ((STM32_READ_BIT(nvmx->SSR, flags) != 0U) ? 1UL : 0UL);
}

/**
  * @brief  Clear the flash end of operation error flag.
  * @param  flashx FLASH instance.
  */
__STATIC_FORCEINLINE void OPENBL_NVM_ClearFlag_EOP(NVM_TypeDef *nvmx)
{
  STM32_SET_BIT(nvmx->SCCR, NVM_CCR_EOP);
}

/**
  * @brief  Start the flash option bytes modification.
  * @param  flashx FLASH instance.
  * @retval None.
  */
__STATIC_INLINE void OPENBL_NVM_OB_StartModification(NVM_TypeDef *nvmx)
{
  STM32_SET_BIT(nvmx->OPTCR, NVM_OPTCR_OPTSTRT);
}

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_NVM_H */
