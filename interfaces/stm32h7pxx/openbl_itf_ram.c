/**
  ******************************************************************************
  * @file    openbl_itf_ram.c
  * @brief   Contains RAM access functions
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

#include "openbl_core.h"

#include "openbl_itf_ram.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to read data from a given address.
  * @param  p_mem  Pointer to the memory object.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_RAM_Read(openbl_mem_t *p_mem, uint32_t address)
{
  STM32_UNUSED(p_mem);

  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in RAM memory.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_RAM_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  uint32_t aligned_length = data_length;
  uint32_t tmp_data;

  STM32_UNUSED(p_mem);

  if ((aligned_length & 0x3U) != 0U)
  {
    aligned_length = (aligned_length & 0xFFFFFFFCU) + 4U;
  }

  for (index = 0U; index < aligned_length; index += 4U)
  {
    tmp_data  = ((uint32_t)p_data[index + 0U]);
    tmp_data |= ((uint32_t)p_data[index + 1U] << 8U);
    tmp_data |= ((uint32_t)p_data[index + 2U] << 16U);
    tmp_data |= ((uint32_t)p_data[index + 3U] << 24U);

    *(__IO uint32_t *)(address + index) = tmp_data;
  }
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where the function will jump.
  * @retval None.
  */
void OPENBL_RAM_JumpToAddress(openbl_mem_t *p_mem, uint32_t address)
{
  STM32_UNUSED(p_mem);

  /* De-initialize all HW resources used by the Open Bootloader to their reset values */
  OPENBL_DeInit();

  /* Enable IRQ */
  OPENBL_Common_EnableIrq();

  /* Jump to the application address */
  OPENBL_Common_JumpToAddress(address);
}
