/**
  ******************************************************************************
  * @file    openbl_iwdg.h
  * @brief   Contains Open Bootloader IWDG low level functions.
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
#ifndef OPENBL_IWDG_H
#define OPENBL_IWDG_H

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
  * @brief  Enable write access to IWDG_PR, IWDG_RLR and IWDG_WINR registers.
  * @param  p_iwdgx IWDG Instance
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_IWDG_EnableWriteAccess(IWDG_TypeDef *p_iwdgx)
{
  STM32_WRITE_REG(p_iwdgx->KR, LL_IWDG_KEY_WR_ACCESS_ENABLE);
}

/**
  * @brief  Select the prescaler of the IWDG.
  * @param  p_iwdgx IWDG Instance
  * @param  prescaler This parameter can be one of the following values:
  *         @arg @ref LL_IWDG_PRESCALER_4
  *         @arg @ref LL_IWDG_PRESCALER_8
  *         @arg @ref LL_IWDG_PRESCALER_16
  *         @arg @ref LL_IWDG_PRESCALER_32
  *         @arg @ref LL_IWDG_PRESCALER_64
  *         @arg @ref LL_IWDG_PRESCALER_128
  *         @arg @ref LL_IWDG_PRESCALER_256
  *         @arg @ref LL_IWDG_PRESCALER_512
  *         @arg @ref LL_IWDG_PRESCALER_1024
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_IWDG_SetPrescaler(IWDG_TypeDef *p_iwdgx)
{
  STM32_WRITE_REG(p_iwdgx->PR, IWDG_PR_PR & LL_IWDG_PRESCALER_1024);
}

/**
  * @brief  Reload IWDG counter with value defined in the reload register.
  * @param  p_iwdgx IWDG Instance
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_IWDG_ReloadCounter(IWDG_TypeDef *p_iwdgx)
{
  STM32_WRITE_REG(p_iwdgx->KR, LL_IWDG_KEY_RELOAD);
}

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_IWDG_H */
