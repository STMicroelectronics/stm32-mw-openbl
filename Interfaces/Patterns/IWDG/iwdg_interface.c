/**
  ******************************************************************************
  * @file    iwdg_interface.c
  * @author  MCD Application Team
  * @brief   Contains IWDG HW configuration
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"
#include "main.h"
#include "iwdg_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static IWDG_HandleTypeDef IWDGHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void OPENBL_IWDG_Configuration(void)
{
  IWDGHandle.Instance       = IWDG;
  IWDGHandle.Init.Prescaler = IWDG_PRESCALER_256;
  IWDGHandle.Init.Window    = IWDG_WINDOW_DISABLE;
  IWDGHandle.Init.Reload    = IWDG_KEY_RELOAD;

  /* In case the user has enabled the IWDG through HW before entering the Open Bootloader */
  IWDG->KR = IWDG_KEY_WRITE_ACCESS_ENABLE;
  IWDG->PR = IWDG_PRESCALER_256;
  IWDG->KR = IWDG_KEY_RELOAD;
}

void OPENBL_IWDG_Refresh(void)
{
  /* Refresh IWDG: reload counter */
  if (HAL_IWDG_Refresh(&IWDGHandle) != HAL_OK)
  {
    /* Refresh Error */
    Error_Handler();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
