/**
  ******************************************************************************
  * @file    systemmemory_interface.c
  * @author  MCD Application Team
  * @brief   Contains System Memory access functions
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
#include "openbl_mem.h"
#include "app_openbootloader.h"
#include "common_interface.h"
#include "systemmemory_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef ICP1_Descriptor =
{
  ICP1_START_ADDRESS,
  ICP1_END_ADDRESS,
  (28U * 1024U),
  ICP1_AREA,
  OPENBL_ICP_Read,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

OPENBL_MemoryTypeDef ICP2_Descriptor =
{
  ICP2_START_ADDRESS,
  ICP2_END_ADDRESS,
  (28U * 1024U),
  ICP2_AREA,
  OPENBL_ICP_Read,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_ICP_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
