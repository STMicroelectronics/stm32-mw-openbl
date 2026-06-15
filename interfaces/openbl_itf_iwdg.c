/**
  ******************************************************************************
  * @file    iwdg_interface.c
  * @brief   Contains IWDG HW configuration
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "openbl_itf_iwdg.h"
#include "openbl_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the watchdog.
  * @retval None.
  */
__WEAK openbl_status_t OPENBL_ITF_IWDG_Init(void *p_hw_context)
{
  IWDG_TypeDef *p_iwdg = (IWDG_TypeDef *)p_hw_context;

  /* In case the user has enabled the IWDG through HW before entering the Open Bootloader */
  OPENBL_IWDG_EnableWriteAccess(p_iwdg);
  OPENBL_IWDG_SetPrescaler(p_iwdg);
  OPENBL_IWDG_ReloadCounter(p_iwdg);

  return OPENBL_SUCCESS;
}

/**
  * @brief  This function is used to refresh the watchdog.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_IWDG_Refresh(void *p_hw_context)
{
  IWDG_TypeDef *p_iwdg = (IWDG_TypeDef *)p_hw_context;

  /* Refresh IWDG counter */
  OPENBL_IWDG_ReloadCounter(p_iwdg);
}
