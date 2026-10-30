/**
  ******************************************************************************
  * @file    openbl_itf_nvm.c
  * @brief   Contains NVM access functions
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

#include "openbl_conf.h"
#include "openbl_itf_common.h"

#include "openbl_core.h"
#include "openbl_mem.h"
#include "openbl_commands.h"

#include "openbl_itf_nvm.h"
#include "openbl_nvm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_NVM_DATA_SIZE                  16U /* User nvm quad-word size in bytes */
#define OPENBL_NVM_WORD_SIZE_BYTE             4U  /* User nvm word size in bytes */
#define OPENBL_NVM_PAGES_NUMBER               (NVM_SIZE_DEFAULT / NVM_PAGE_SIZE) /* User nvm pages number */

#define OPENBL_NVM_PROGRAM_TIMEOUT            3U  /* Timeout value for writing quad-word data size in user nvm */

/* Private macro -------------------------------------------------------------*/
/*! Macro to get the NVM physical instance from the handle */
#define OPENBL_NVM_GET_INSTANCE(hnvm) ((NVM_TypeDef *)((uint32_t)((hnvm)->instance)))

/*! Macro to get NVM memory page index */
#define OPENBL_NVM_GET_HW_PAGE_INDEX(page_index, max_page_index) \
  (((page_index) >= (max_page_index)) ? ((page_index) - (max_page_index)) : (page_index))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_NVM_Program(openbl_mem_t *p_mem, uint32_t nvm_address, uint8_t *p_data, uint32_t size_byte);
static openbl_status_t OPENBL_NVM_EnableWriteProtection(openbl_mem_t *p_mem, uint8_t *p_pages_list, uint32_t length);
static openbl_status_t OPENBL_NVM_DisableWriteProtection(openbl_mem_t *p_mem);
static void OPENBL_NVM_ProgramByWord(uint32_t address, uint8_t *p_data, uint32_t size_byte);
static void OPENBL_NVM_ProgramByByte(uint32_t address, uint8_t *p_data, uint32_t size_byte);

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the NVM control register access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_NVM_Unlock(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_NVM_ITF_Unlock(HAL_NVM);
}

/**
  * @brief  Unlock the NVM Option Bytes Registers access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_NVM_Unlock_OB(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_NVM_ITF_Unlock(HAL_NVM);

  (void)HAL_NVM_ITF_OB_Unlock(HAL_NVM);
}

/**
  * @brief  Lock the NVM control register access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_NVM_Lock(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_NVM_ITF_Lock(HAL_NVM);
}

/**
  * @brief  Launch the option byte loading.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_NVM_OB_Launch(openbl_mem_t *p_mem)
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
uint8_t OPENBL_NVM_Read(openbl_mem_t *p_mem, uint32_t address)
{
  STM32_UNUSED(p_mem);

  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in NVM memory.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_NVM_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  __ALIGNED(4) uint8_t data[OPENBL_NVM_DATA_SIZE] = {0x0U};
  uint32_t count                                  = 0U;
  uint32_t address_tmp                            = address;
  uint32_t data_length_tmp                        = data_length;

  if ((p_data != NULL) && (data_length_tmp != 0U))
  {
    /* Unlock the nvm memory for write operation */
    OPENBL_NVM_Unlock(p_mem);

    /* Clear error programming flags */
    OPENBL_NVM_ClearFlag((NVM_TypeDef *)((uint32_t)HAL_NVM), LL_NVM_FLAG_ALL);

    /* Program user nvm by quad word (16 bytes) */
    while ((data_length_tmp >> OPENBL_NVM_WORD_SIZE_BYTE) > 0U)
    {
      for (index = 0U; index < OPENBL_NVM_DATA_SIZE; index++)
      {
        data[index] = p_data[count + index];
      }

      OPENBL_NVM_Program(p_mem, address_tmp, data, OPENBL_NVM_DATA_SIZE);

      address_tmp     += OPENBL_NVM_DATA_SIZE;
      count           += OPENBL_NVM_DATA_SIZE;
      data_length_tmp -= OPENBL_NVM_DATA_SIZE;
    }

    /* If remaining count, go back to fill the rest with 0xFF */
    if (data_length_tmp > 0U)
    {
      /* Copy the remaining bytes */
      for (index = 0U; index < data_length_tmp; index++)
      {
        data[index] = p_data[count + index];

        OPENBL_NVM_Program(p_mem, address_tmp, &data[index], 1U);

        address_tmp++;
      }
    }

    /* Lock the nvm to disable the nvm control register access */
    OPENBL_NVM_Lock(p_mem);
  }
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where the function will jump.
  * @retval None.
  */
void OPENBL_NVM_JumpToAddress(openbl_mem_t *p_mem, uint32_t address)
{
  STM32_UNUSED(p_mem);

  /* De-initialize all HW resources used by the Open Bootloader to their reset values */
  OPENBL_DeInit();

  /* Enable IRQ */
  OPENBL_Common_EnableIrq();

  /* Jump to the application address */
  OPENBL_Common_JumpToAddress(address);
}

/**
  * @brief  Get the product protection state.
  * @retval Returns the product protection state.
  *         This returned value can be one of @ref openbl_state_t
  */
openbl_state_t OPENBL_NVM_GetProductProtectionState(void)
{
  openbl_state_t protection_state;

  /* No Specific protection needed on V8 */
  protection_state = OPENBL_DISABLE;

  return protection_state;
}

/**
  * @brief  This function is used to enable or disable write protection of the specified NVM areas.
  * @param  p_mem Pointer to the memory object.
  * @param  state Can be one of these values:
  *         @arg OPENBL_DISABLE: Disable NVM write protection
  *         @arg OPENBL_ENABLE: Enable NVM write protection
  * @param  p_pages_list Contains the list of sectors to be protected.
  * @param  length The length of the list of sectors to be protected.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Enable or disable of the write protection is done
  *          - OPENBL_ERROR:   Enable or disable of the write protection is not done
  */
openbl_status_t OPENBL_NVM_SetWriteProtection(openbl_mem_t *p_mem,
                                              openbl_state_t state,
                                              uint8_t *p_pages_list,
                                              uint32_t length)
{
  openbl_status_t status = OPENBL_SUCCESS;

  if (state == OPENBL_ENABLE)
  {
    (void) OPENBL_NVM_EnableWriteProtection(p_mem, p_pages_list, length);
  }
  else if (state == OPENBL_DISABLE)
  {
    (void) OPENBL_NVM_DisableWriteProtection(p_mem);
  }
  else
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Program usar NVM memory at specified address.
  * @param  p_mem Pointer to the memory object.
  * @param  nvm_address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed.
  * @retval None.
  */
static void OPENBL_NVM_Program(openbl_mem_t *p_mem, uint32_t nvm_address, uint8_t *p_data, uint32_t size_byte)
{
  hal_status_t status;
  hal_nvm_handle_t *hnvm = (hal_nvm_handle_t *)p_mem->hw_context.p_itf_hw_context;

  /* Clear all NVM errors flags before starting write operation */
  OPENBL_NVM_ClearFlag(OPENBL_NVM_GET_INSTANCE(hnvm), LL_NVM_FLAG_ALL);

  status = OPENBL_NVM_IsReadyForOperation(hnvm);

  if (status == HAL_OK)
  {
    LL_NVM_EnableProgramming(OPENBL_NVM_GET_INSTANCE(hnvm));

    if (size_byte >= OPENBL_NVM_DATA_SIZE)
    {
      OPENBL_NVM_ProgramByWord(nvm_address, p_data, size_byte);
    }
    else
    {
      /* Enable read-modify-write for lower programming units */
      LL_NVM_EnableReadModifyWrite(OPENBL_NVM_GET_INSTANCE(hnvm));

      OPENBL_NVM_ProgramByByte(nvm_address, p_data, size_byte);
    }

    /* Only wait for operation end without checking errors */
    (void) OPENBL_NVM_WaitForEndOfOperation(p_mem, OPENBL_NVM_PROGRAM_TIMEOUT);

    LL_NVM_DisableAllOperation(OPENBL_NVM_GET_INSTANCE(hnvm));
  }
}

/**
  * @brief  Program up to a quad-word (128-bit) at a specified address by successive word write operations.
  * @param  address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed. Must be a multiple of the word size (4 bytes).
  * @retval None.
  */
static void OPENBL_NVM_ProgramByWord(uint32_t address, uint8_t *p_data, uint32_t size_byte)
{
  uint32_t increment_index = size_byte / OPENBL_NVM_WORD_SIZE_BYTE;
  uint32_t *dest_addr      = (uint32_t *)(address);
  uint32_t *src_addr       = (uint32_t *)p_data;
  uint32_t primask_bit;

  /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Program */
  do
  {
    *dest_addr = *src_addr;
    dest_addr++;
    src_addr++;
    increment_index--;
  } while (increment_index != 0U);

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}

/**
  * @brief  Program by byte (8-bit) at a specified address by successive write operations.
  * @param  address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed.
  * @retval None.
  */
static void OPENBL_NVM_ProgramByByte(uint32_t address, uint8_t *p_data, uint32_t size_byte)
{
  uint8_t *dest_addr       = (uint8_t *)(address);
  uint8_t *src_addr        = p_data;
  uint32_t primask_bit;

  /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Program */
  *dest_addr = *src_addr;

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}

/**
  * @brief  This function is used to enable write protection of the specified NVM areas.
  * @param  p_mem Pointer to the memory object.
  * @param  p_pages_list Contains the list of pages to be protected.
  * @param  length The length of the list of pages to be protected.
  * @retval Returns an openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Enable or disable of the write protection is done
  *          - OPENBL_ERROR:   Enable or disable of the write protection is not done
  */
static openbl_status_t OPENBL_NVM_EnableWriteProtection(openbl_mem_t *p_mem, uint8_t *p_pages_list, uint32_t length)
{
  uint32_t index;
  uint32_t page;
  openbl_status_t status = OPENBL_SUCCESS;

  /* Clear error programming flags */
  OPENBL_NVM_ClearFlag((NVM_TypeDef *)((uint32_t)HAL_NVM), LL_NVM_FLAG_ERRORS_ALL);

  /* Unlock the NVM registers & Option Bytes registers access */
  OPENBL_NVM_Unlock_OB(p_mem);

  for (index = 0U; index < length; index++)
  {
    page = (uint32_t)p_pages_list[index];

    /* Pages to protect from 0 to 127 for WRP1R */
    if (page <= ((OPENBL_NVM_PAGES_NUMBER / 8U) - 1U))
    {
      if (HAL_NVM_ITF_OB_EnablePageWRP(HAL_NVM, HAL_NVM_BANK_1, (4U * page), 1U) != HAL_OK)
      {
        status = OPENBL_ERROR;
      }
    }
    /* Pages to protect from 128 to 256 for WRP2R */
    else if (page <= (OPENBL_NVM_PAGES_NUMBER - 1U))
    {
      page = page - (OPENBL_NVM_PAGES_NUMBER / 8U);

      if (HAL_NVM_ITF_OB_EnablePageWRP(HAL_NVM, HAL_NVM_BANK_2, (4U * page), 1U) != HAL_OK)
      {
        status = OPENBL_ERROR;
      }
    }
    else
    {
      /* Nothing to do */
    }
  }

  if (HAL_NVM_ITF_OB_Program(HAL_NVM) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to disable write protection.
  * @param  p_mem Pointer to the memory object.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Enable or disable of the write protection is done
  *          - OPENBL_ERROR:   Enable or disable of the write protection is not done
  */
static openbl_status_t OPENBL_NVM_DisableWriteProtection(openbl_mem_t *p_mem)
{
  openbl_status_t status = OPENBL_SUCCESS;

  /* Unlock the nvm registers & Option Bytes registers access */
  OPENBL_NVM_Unlock_OB(p_mem);

  /* Clear all NVM errors flags before starting write operation */
  OPENBL_NVM_ClearFlag((NVM_TypeDef *)((uint32_t)HAL_NVM), LL_NVM_FLAG_ERRORS_ALL);

  if (HAL_NVM_ITF_OB_DisablePageWRP(HAL_NVM, HAL_NVM_BANK_1, 0U, (OPENBL_NVM_PAGES_NUMBER / 2U)) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  if (HAL_NVM_ITF_OB_DisablePageWRP(HAL_NVM, HAL_NVM_BANK_2, 0U, (OPENBL_NVM_PAGES_NUMBER / 2U)) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  if (HAL_NVM_ITF_OB_Program(HAL_NVM) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  Check if the NVM operation for a new operation.
  * @param  hnvm       Pointer to a \ref hal_nvm_handle_t structure.
  * @retval HAL_BUSY     NVM is busy.
  * @retval HAL_ERROR    NVM is locked or an error is pending.
  * @retval HAL_OK       NVM is ready.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_NVM_IsReadyForOperation(hal_nvm_handle_t *hnvm)
{
  uint32_t flags;

  flags = OPENBL_NVM_ReadFlag(OPENBL_NVM_GET_INSTANCE(hnvm), LL_NVM_FLAG_ALL | LL_NVM_FLAG_ERRORS_ALL);

  if ((OPENBL_NVM_IsLocked(OPENBL_NVM_GET_INSTANCE(hnvm)) != 0U) || ((flags & LL_NVM_FLAG_ERRORS_ALL) != 0U))
  {
    return HAL_ERROR;
  }
  else if ((flags & LL_NVM_FLAG_ALL) != 0U)
  {
    return HAL_BUSY;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  Wait for a NVM operation to complete.
  * @param  p_mem        Pointer to the memory object.
  * @param  timeout_msec Maximum nvm operation timeout..
  * @retval HAL_TIMEOUT  User timeout.
  * @retval HAL_ERROR    Error during operation.
  * @retval HAL_OK       No operation to be waiting.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_NVM_WaitForEndOfOperation(openbl_mem_t *p_mem, uint32_t timeout_msec)
{
  uint32_t tickstart;
  uint32_t error_code;
  hal_nvm_handle_t *hnvm  = (hal_nvm_handle_t *)p_mem->hw_context.p_itf_hw_context;
  openbl_itf_obj_t *p_itf_obj = (openbl_itf_obj_t *)p_mem->p_parent;

  tickstart = HAL_GetTick();

  while (OPENBL_NVM_IsActiveFlag(OPENBL_NVM_GET_INSTANCE(hnvm),
                                 (LL_NVM_FLAG_BSY | LL_NVM_FLAG_WBNE | LL_NVM_FLAG_BOG)) != 0U)
  {
    /* Check if we need to send a busy byte */
    if (p_itf_obj != NULL)
    {
      if ((p_itf_obj->send_busy_byte == OPENBL_ENABLE) && (p_itf_obj->p_ops->SendBusyByte != NULL))
      {
        p_itf_obj->p_ops->SendBusyByte(p_itf_obj);
      }
    }

    if ((HAL_GetTick() - tickstart) > timeout_msec)
    {
      if (OPENBL_NVM_IsActiveFlag(OPENBL_NVM_GET_INSTANCE(hnvm),
                                  (LL_NVM_FLAG_BSY | LL_NVM_FLAG_WBNE | LL_NVM_FLAG_BOG)) != 0U)
      {
        return HAL_TIMEOUT;
      }
    }
  }

  error_code = OPENBL_NVM_ReadFlag(OPENBL_NVM_GET_INSTANCE(hnvm), LL_NVM_FLAG_ERRORS_ALL);

  if (error_code != 0U)
  {
#if defined (USE_HAL_NVM_GET_LAST_ERRORS) && (USE_HAL_NVM_GET_LAST_ERRORS == 1)
    /* Fill the NVM handle with the detected error codes */
    NVM_FillErrorCode(hnvm, error_code);
#endif /* USE_HAL_NVM_GET_LAST_ERRORS */

    /* Check error flags */
    OPENBL_NVM_ClearFlag(OPENBL_NVM_GET_INSTANCE(hnvm), LL_NVM_FLAG_ERRORS_ALL);

    return HAL_ERROR;
  }

  /* Clear NVM End of Operation pending bit */
  OPENBL_NVM_ClearFlag_EOP(OPENBL_NVM_GET_INSTANCE(hnvm));

  /* If no error flag is set */
  return HAL_OK;
}

/**
  * @brief Provides a tick value in milliseconds.
  * @retval uint32_t HAL tick current value (unit: milliseconds)
  */
OPENBL_RAM_FUNCTION uint32_t HAL_GetTick(void)
{
  return uwTick;
}
