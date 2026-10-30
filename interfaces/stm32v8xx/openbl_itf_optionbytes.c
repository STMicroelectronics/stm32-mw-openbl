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

#include "openbl_nvm.h"
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

  (void)HAL_NVM_ITF_Unlock(HAL_NVM);

  (void)HAL_NVM_ITF_OB_Unlock(HAL_NVM);
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

  hal_nvm_handle_t *hnvm = (hal_nvm_handle_t *)p_mem->hw_context.p_itf_hw_context;

  OPENBL_NVM_OB_StartModification(((NVM_TypeDef *)((uint32_t)((hnvm)->instance))));
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
  LL_NVM_ClearFlag((NVM_TypeDef *)((uint32_t)HAL_NVM), LL_NVM_FLAG_ERRORS_ALL);

  if (address == OPENBL_OB1_START_ADDRESS)
  {
    /* Write the OPTSR1 register */
    if (data_length > 0x04U)
    {
      NVM->OPTSR1_PRG = *(uint32_t *)(uint32_t)&p_data[0x04U];
    }

    /* Write the OPTSR2 register */
    if (data_length > 0x24U)
    {
      NVM->OPTSR2_PRG = *(uint32_t *)(uint32_t)&p_data[0x24U];
    }

    /* Write the Non secure VTOR address */
    if (data_length > 0x34U)
    {
      NVM->VTORNSR_PRG = *(uint32_t *)(uint32_t)&p_data[0x34U];
    }

    /* Write the Secure Boot address */
    if (data_length > 0x3CU)
    {
      NVM->BOOTR_PRG = *(uint32_t *)(uint32_t)&p_data[0x3CU];
    }

    /* Write the Non secure OTP block lock */
    if (data_length > 0x44U)
    {
      NVM->OTPBLR1_PRG = *(uint32_t *)(uint32_t)&p_data[0x44U];
    }

    /* Write the Non secure OTP block lock */
    if (data_length > 0x4CU)
    {
      NVM->OTPBLR2_PRG = *(uint32_t *)(uint32_t)&p_data[0x4CU];
    }

    /* Write the OPTSR3 register */
    if (data_length > 0x54U)
    {
      NVM->OPTSR3_PRG = *(uint32_t *)(uint32_t)&p_data[0x54U];
    }

    /* Write the CNTR register */
    if (data_length > 0xD4U)
    {
      NVM->CNTR_PRG = *(uint32_t *)(uint32_t)&p_data[0xD4U];
    }

    /* Write the Secure CNTR register */
    if (data_length > 0xDCU)
    {
      NVM->SCNTR_PRG = *(uint32_t *)(uint32_t)&p_data[0xDCU];
    }
  }
  else if (address == OPENBL_OB2_START_ADDRESS)
  {
    /* Write the Secure Water Mark register 1 */
    if (data_length > 0x04U)
    {
      NVM->SECWM1R_PRG = *(uint32_t *)(uint32_t)&p_data[0x04U];
    }

    /* Write the Secure HDP1 register */
    if (data_length > 0x0CU)
    {
      NVM->HDP1R_PRG = *(uint32_t *)(uint32_t)&p_data[0x0CU];
    }

    /* Write the WRP1R1 register */
    if (data_length > 0x14U)
    {
      NVM->WRP1R1_PRG = *(uint32_t *)(uint32_t)&p_data[0x14U];
    }

    /* Write the WRP1R2 register */
    if (data_length > 0x1CU)
    {
      NVM->WRP1R2_PRG = *(uint32_t *)(uint32_t)&p_data[0x1CU];
    }

    /* Write the EEPROM1 register */
    if (data_length > 0x54U)
    {
      NVM->EDATA1R_PRG = *(uint32_t *)(uint32_t)&p_data[0x54U];
    }

    /* Write the secure WRPLOCK1 register */
    if (data_length > 0x7CU)
    {
      NVM->WRPLOCK1R_PRG = *(uint32_t *)(uint32_t)&p_data[0x7CU];
    }
  }
  else if (address == OPENBL_OB3_START_ADDRESS)
  {
    /* Write the Secure Water Mark register 2 */
    if (data_length > 0x04U)
    {
      NVM->SECWM2R_PRG = *(uint32_t *)(uint32_t)&p_data[0x04U];
    }

    /* Write the secure HDP2 register */
    if (data_length > 0x0CU)
    {
      NVM->HDP2R_PRG = *(uint32_t *)(uint32_t)&p_data[0x0CU];
    }

    /* Write the WRP2R1 register */
    if (data_length > 0x14U)
    {
      NVM->WRP2R1_PRG = *(uint32_t *)(uint32_t)&p_data[0x14U];
    }

    /* Write the WRP2R2 register */
    if (data_length > 0x1CU)
    {
      NVM->WRP2R2_PRG = *(uint32_t *)(uint32_t)&p_data[0x1CU];
    }

    /* Write the EEPROM2 register */
    if (data_length > 0x54U)
    {
      NVM->EDATA2R_PRG = *(uint32_t *)(uint32_t)&p_data[0x54U];
    }

    /* Write the secure WRPLOCK2 register */
    if (data_length > 0x7CU)
    {
      NVM->WRPLOCK2R_PRG = *(uint32_t *)(uint32_t)&p_data[0x7CU];
    }
  }
  else
  {
    /* Nothing to do */
  }

  /* Register option bytes launch callback */
  p_mem->mem_ops.PostProcessing = OPENBL_OB_Launch;
}
