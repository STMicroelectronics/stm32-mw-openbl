/**
  ******************************************************************************
  * @file    openbl_common.c
  * @brief   Contains common functions used by different interfaces
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
#include "stm32_hal.h"
#include "openbl_itf_common.h"

#include "openbl_itf_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Assigns the given value to the Main Stack Pointer (MSP).
  * @param  top_of_main_stack  Main Stack Pointer value to set.
  * @retval None.
  */
void OPENBL_Common_SetMsp(uint32_t top_of_main_stack)
{
  __set_MSP(top_of_main_stack);
}

/**
  * @brief  Enable IRQ Interrupts.
  * @retval None.
  */
void OPENBL_Common_EnableIrq(void)
{
  __enable_irq();
}

/**
  * @brief  Disable IRQ Interrupts.
  * @retval None.
  */
void OPENBL_Common_DisableIrq(void)
{
  __disable_irq();
}

/**
  * @brief  Checks whether the target Protection Status is set or not.
  * @retval Returns SET if protection is enabled else return RESET.
  */
openbl_state_t OPENBL_Common_GetProtectionStatus(void)
{
  openbl_state_t state;

  state = OPENBL_FLASH_GetProductProtectionState();

  return state;
}
