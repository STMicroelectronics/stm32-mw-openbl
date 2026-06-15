/**
  ******************************************************************************
  * @file    openbl_itf_optionbytes.c
  * @brief   Contains Option Bytes access functions
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

#include "openbl_conf.h"

#include "openbl_flash.h"
#include "openbl_itf_optionbytes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the FLASH Option Bytes Registers access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_OB_Unlock(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_FLASH_ITF_Unlock(HAL_FLASH);

  (void)HAL_FLASH_ITF_OB_Unlock(HAL_FLASH);
}

/**
  * @brief  Launch the option byte loading.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_OB_Launch(openbl_mem_t *p_mem)
{
  /* No reset needed after option bytes modification in STM32C5xx product */
  /* ==================================================================== */

  hal_flash_handle_t *hflash = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;

  OPENBL_FLASH_OB_StartModification(((FLASH_TypeDef *)((uint32_t)((hflash)->instance))));
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_OB_Read(openbl_mem_t *p_mem, uint32_t address)
{
  STM32_UNUSED(p_mem);

  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in Option bytes.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_OB_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  /* Unlock the FLASH & Option Bytes Registers access */
  OPENBL_OB_Unlock(p_mem);

  /* Clear all FLASH errors flags before starting write operation */
  LL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

  if (address == OPENBL_OB1_START_ADDRESS)
  {
    if (data_length > 0x04U)
    {
      FLASH->OPTSR_PRG = *(uint32_t *)(uint32_t)&p_data[0x04U];
    }

    if (data_length > 0x24U)
    {
      FLASH->OPTSR2_PRG = *(uint32_t *)(uint32_t)&p_data[0x24U];
    }

    if (data_length > 0x34U)
    {
      FLASH->BOOTR_PRG = *(uint32_t *)(uint32_t)&p_data[0x34U];
    }

    if (data_length > 0x44U)
    {
      FLASH->OTPBLR_PRG = *(uint32_t *)(uint32_t)&p_data[0x44U];
    }

    if (data_length > 0x4CU)
    {
      FLASH->BL_COM_CFG_PRG = *(uint32_t *)(uint32_t)&p_data[0x4CU];
    }

    if (data_length > 0x54U)
    {
      FLASH->OEMKEYR1_PRG = *(uint32_t *)(uint32_t)&p_data[0x54U];
    }

    if (data_length > 0x5CU)
    {
      FLASH->OEMKEYR2_PRG = *(uint32_t *)(uint32_t)&p_data[0x5CU];
    }

    if (data_length > 0x64U)
    {
      FLASH->OEMKEYR3_PRG = *(uint32_t *)(uint32_t)&p_data[0x64U];
    }

    if (data_length > 0x6CU)
    {
      FLASH->OEMKEYR4_PRG = *(uint32_t *)(uint32_t)&p_data[0x6CU];
    }

    if (data_length > 0x74U)
    {
      FLASH->BSKEYR_PRG = *(uint32_t *)(uint32_t)&p_data[0x74U];
    }
  }
  else if (address == OPENBL_OB2_START_ADDRESS)
  {
    if (data_length > 0x0U)
    {
      FLASH->OEMKEYR1_PRG = *(uint32_t *)(uint32_t)&p_data[0x0U];
    }

    if (data_length > 0x8U)
    {
      FLASH->OEMKEYR2_PRG = *(uint32_t *)(uint32_t)&p_data[0x8U];
    }

    if (data_length > 0x10U)
    {
      FLASH->OEMKEYR3_PRG = *(uint32_t *)(uint32_t)&p_data[0x10U];
    }

    if (data_length > 0x18U)
    {
      FLASH->OEMKEYR4_PRG = *(uint32_t *)(uint32_t)&p_data[0x18U];
    }

    if (data_length > 0x20U)
    {
      FLASH->BSKEYR_PRG = *(uint32_t *)(uint32_t)&p_data[0x20U];
    }
  }
  else if (address == OPENBL_OB3_START_ADDRESS)
  {
    if (data_length > 0x04U)
    {
      FLASH->WRP1R_PRG = *(uint32_t *)(uint32_t)&p_data[0x04U];
    }

    if (data_length > 0x14U)
    {
      FLASH->HDP1R_PRG = *(uint32_t *)(uint32_t)&p_data[0x14U];
    }
  }
  else if (address == OPENBL_OB4_START_ADDRESS)
  {
    if (data_length > 0x04U)
    {
      FLASH->WRP2R_PRG = *(uint32_t *)(uint32_t)&p_data[0x04U];
    }

    if (data_length > 0x14U)
    {
      FLASH->HDP2R_PRG = *(uint32_t *)(uint32_t)&p_data[0x14U];
    }
  }
  else
  {
    /* Nothing to do */
  }

  /* Register option bytes launch callback */
  p_mem->mem_ops.PostProcessing = OPENBL_OB_Launch;
}
