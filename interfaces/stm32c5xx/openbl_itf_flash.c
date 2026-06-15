/**
  ******************************************************************************
  * @file    openbl_itf_flash.c
  * @brief   Contains FLASH access functions
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

#include "openbl_itf_flash.h"
#include "openbl_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_FLASH_DATA_SIZE                  16U /* User flash quad-word size in bytes */
#define OPENBL_FLASH_WORD_SIZE_BYTE             4U  /* User flash word size in bytes */
#define OPENBL_FLASH_PAGES_NUMBER               (FLASH_SIZE_MAX / FLASH_PAGE_SIZE) /* User flash pages number */

#define OPENBL_FLASH_PROGRAM_TIMEOUT            3U     /* Timeout value for writing quad-word data size in user flash */
#define OPENBL_FLASH_ERASE_PAGE_TIMEOUT         10U    /* Timeout value for erasing one page */
#define OPENBL_FLASH_MASS_ERASE_TIMEOUT         1000U  /* Timeout value for erasing all user flash */
#define OPENBL_FLASH_ERASE_BANK_TIMEOUT         500U   /* Timeout value for erasing one bank from user flash */

/* Private macro -------------------------------------------------------------*/
/*! Macro to get the FLASH physical instance from the handle */
#define OPENBL_FLASH_GET_INSTANCE(hflash) ((FLASH_TypeDef *)((uint32_t)((hflash)->instance)))

/*! Macro to get the FLASH erase bank */
#define OPENBL_FLASH_GET_ERASE_BANK(bank) (((bank) == HAL_FLASH_BANK_1) ? LL_FLASH_ERASE_BANK_1 : \
                                           (((bank) == HAL_FLASH_BANK_2) ? LL_FLASH_ERASE_BANK_2 : \
                                            0U))

/*! Macro to get FLASH memory page index */
#define OPENBL_FLASH_GET_HW_PAGE_INDEX(page_index, max_page_index) \
  (((page_index) >= (max_page_index)) ? ((page_index) - (max_page_index)) : (page_index))

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_FLASH_Program(openbl_mem_t *p_mem, uint32_t flash_address, uint8_t *p_data, uint32_t size_byte);
static openbl_status_t OPENBL_FLASH_EnableWriteProtection(openbl_mem_t *p_mem, uint8_t *p_pages_list, uint32_t length);
static openbl_status_t OPENBL_FLASH_DisableWriteProtection(openbl_mem_t *p_mem);
static void OPENBL_FLASH_ProgramByWord(uint32_t address, uint8_t *p_data, uint32_t size_byte);

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the FLASH control register access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_FLASH_Unlock(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_FLASH_ITF_Unlock(HAL_FLASH);
}

/**
  * @brief  Unlock the FLASH Option Bytes Registers access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_FLASH_Unlock_OB(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_FLASH_ITF_Unlock(HAL_FLASH);

  (void)HAL_FLASH_ITF_OB_Unlock(HAL_FLASH);
}

/**
  * @brief  Lock the FLASH control register access.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_FLASH_Lock(openbl_mem_t *p_mem)
{
  STM32_UNUSED(p_mem);

  (void)HAL_FLASH_ITF_Lock(HAL_FLASH);
}

/**
  * @brief  Launch the option byte loading.
  * @param  p_mem Pointer to the memory object.
  * @retval None.
  */
void OPENBL_FLASH_OB_Launch(openbl_mem_t *p_mem)
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
uint8_t OPENBL_FLASH_Read(openbl_mem_t *p_mem, uint32_t address)
{
  STM32_UNUSED(p_mem);

  return (*(uint8_t *)(address));
}

/**
  * @brief  This function is used to write data in FLASH memory.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_FLASH_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  __ALIGNED(4) uint8_t data[OPENBL_FLASH_DATA_SIZE] = {0x0U};
  uint32_t remaining;
  uint32_t count = 0U;
  uint32_t address_tmp = address;
  uint32_t data_length_tmp = data_length;

  if ((p_data != NULL) && (data_length_tmp != 0U))
  {
    /* Unlock the flash memory for write operation */
    OPENBL_FLASH_Unlock(p_mem);

    /* Clear error programming flags */
    OPENBL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

    /* Program user flash by quad word (16 bytes) */
    while ((data_length_tmp >> 4U) > 0U)
    {
      for (index = 0U; index < OPENBL_FLASH_DATA_SIZE; index++)
      {
        data[index] = p_data[count + index];
      }

      OPENBL_FLASH_Program(p_mem, address_tmp, data, OPENBL_FLASH_DATA_SIZE);

      address_tmp     += OPENBL_FLASH_DATA_SIZE;
      count       += OPENBL_FLASH_DATA_SIZE;
      data_length_tmp -= OPENBL_FLASH_DATA_SIZE;
    }

    /* If remaining count, go back to fill the rest with 0xFF */
    if (data_length_tmp > 0U)
    {
      remaining = (OPENBL_FLASH_DATA_SIZE - data_length_tmp);

      /* Copy the remaining bytes */
      for (index = 0U; index < data_length_tmp; index++)
      {
        data[index] = p_data[count + index];
      }

      /* Fill the upper bytes with 0xFF */
      for (index = 0U; index < remaining; index++)
      {
        data[index + data_length_tmp] = 0xFFU;
      }

      OPENBL_FLASH_Program(p_mem, address_tmp, data, OPENBL_FLASH_DATA_SIZE);
    }

    /* Lock the Flash to disable the flash control register access */
    OPENBL_FLASH_Lock(p_mem);
  }
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where the function will jump.
  * @retval None.
  */
void OPENBL_FLASH_JumpToAddress(openbl_mem_t *p_mem, uint32_t address)
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
openbl_state_t OPENBL_FLASH_GetProductProtectionState(void)
{
  openbl_state_t protection_state;
  hal_flash_itf_ob_rdp_level_t rdp_level;

  rdp_level = HAL_FLASH_ITF_OB_GetRDPLevel(HAL_FLASH);

  if (rdp_level == HAL_FLASH_ITF_OB_RDP_LEVEL_0)
  {
    protection_state = OPENBL_DISABLE;
  }
  else
  {
    protection_state = OPENBL_ENABLE;
  }

  return protection_state;
}

/**
  * @brief  This function is used to enable or disable write protection of the specified FLASH areas.
  * @param  p_mem Pointer to the memory object.
  * @param  state Can be one of these values:
  *         @arg OPENBL_DISABLE: Disable FLASH write protection
  *         @arg OPENBL_ENABLE: Enable FLASH write protection
  * @param  p_pages_list Contains the list of sectors to be protected.
  * @param  length The length of the list of sectors to be protected.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Enable or disable of the write protection is done
  *          - OPENBL_ERROR:   Enable or disable of the write protection is not done
  */
openbl_status_t OPENBL_FLASH_SetWriteProtection(openbl_mem_t *p_mem,
                                                openbl_state_t state,
                                                uint8_t *p_pages_list,
                                                uint32_t length)
{
  openbl_status_t status = OPENBL_SUCCESS;

  if (state == OPENBL_ENABLE)
  {
    (void) OPENBL_FLASH_EnableWriteProtection(p_mem, p_pages_list, length);

    /* Register option bytes launch callback */
    p_mem->mem_ops.PostProcessing = OPENBL_FLASH_OB_Launch;
  }
  else if (state == OPENBL_DISABLE)
  {
    (void) OPENBL_FLASH_DisableWriteProtection(p_mem);

    /* Register option bytes launch callback */
    p_mem->mem_ops.PostProcessing = OPENBL_FLASH_OB_Launch;
  }
  else
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to start FLASH mass erase operation.
  * @param  p_mem Pointer to the memory object.
  * @param  p_data Pointer to the buffer that contains mass erase operation options.
  * @param  data_length Size of the Data buffer.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Mass erase operation done
  *          - OPENBL_ERROR:   Mass erase operation failed or the value of one parameter is not OK
  */
openbl_status_t OPENBL_FLASH_MassOrBankErase(openbl_mem_t *p_mem, uint8_t *p_data, uint32_t data_length)
{
  uint16_t bank_option;
  openbl_status_t status = OPENBL_SUCCESS;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock(p_mem);

  /* Clear all FLASH errors flags before starting write operation */
  OPENBL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

  if (data_length >= 2U)
  {
    bank_option = (uint16_t)p_data[0U] | ((uint16_t)p_data[1U] << 8U);

    if (bank_option == OPENBL_FLASH_MASS_ERASE)
    {
      (void)OPENBL_FLASH_MassErase(p_mem, OPENBL_FLASH_MASS_ERASE_TIMEOUT);
    }
    else if (bank_option == OPENBL_FLASH_BANK1_ERASE)
    {
      (void)OPENBL_FLASH_EraseBank(p_mem, HAL_FLASH_BANK_1, OPENBL_FLASH_ERASE_BANK_TIMEOUT);
    }
    else if (bank_option == OPENBL_FLASH_BANK2_ERASE)
    {
      (void)OPENBL_FLASH_EraseBank(p_mem, HAL_FLASH_BANK_2, OPENBL_FLASH_ERASE_BANK_TIMEOUT);
    }
    else
    {
      status = OPENBL_ERROR;
    }
  }
  else
  {
    status = OPENBL_ERROR;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock(p_mem);

  return status;
}

/**
  * @brief  This function is used to erase the specified FLASH sectors.
  * @param  p_mem Pointer to the memory object.
  * @param  p_data Pointer to the buffer that contains erase operation options.
  * @param  data_length Size of the Data buffer.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Erase operation done
  *          - OPENBL_ERROR:   Erase operation failed or the value of one parameter is not OK
  */
openbl_status_t OPENBL_FLASH_Erase(openbl_mem_t *p_mem, uint8_t *p_data, uint32_t data_length)
{
  uint32_t counter;
  uint32_t page;
  uint32_t pages_number;
  hal_status_t erase_status = HAL_ERROR;
  openbl_status_t status = OPENBL_SUCCESS;
  uint32_t count = 0U;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock(p_mem);

  /* Clear error programming flags */
  OPENBL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

  pages_number = (uint32_t)p_data[count] | ((uint32_t)p_data[count + 1U] << 8U);

  /* The sector number size is 2 bytes */
  count += 2U;

  if (pages_number < (data_length / 2U))
  {
    for (counter = 0U; (counter < pages_number); counter++)
    {
      page = (uint32_t)p_data[count] | ((uint32_t)p_data[count + 1U] << 8U);

      if (page <= ((OPENBL_FLASH_PAGES_NUMBER / 2U) - 1U))
      {
        erase_status = OPENBL_FLASH_ErasePage(p_mem,
                                              HAL_FLASH_BANK_1,
                                              page,
                                              LL_FLASH_ERASE_USER_AREA,
                                              OPENBL_FLASH_ERASE_PAGE_TIMEOUT);
      }
      else if (page <= (OPENBL_FLASH_PAGES_NUMBER - 1U))
      {
        erase_status = OPENBL_FLASH_ErasePage(p_mem,
                                              HAL_FLASH_BANK_2,
                                              page,
                                              LL_FLASH_ERASE_USER_AREA,
                                              OPENBL_FLASH_ERASE_PAGE_TIMEOUT);
      }
      else
      {
        /* Nothing to do */
      }

      /* The sector number size is 2 bytes */
      count += 2U;

      /* If the erase of one page fails then put the overall status to fail */
      if (erase_status != HAL_OK)
      {
        status = OPENBL_ERROR;
      }
    }
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock(p_mem);

  return status;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Program usar FLASH memory at specified address.
  * @param  p_mem Pointer to the memory object.
  * @param  flash_address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed.
  * @retval None.
  */
static void OPENBL_FLASH_Program(openbl_mem_t *p_mem, uint32_t flash_address, uint8_t *p_data, uint32_t size_byte)
{
  hal_status_t status;
  hal_flash_handle_t *hflash = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;

  /* Clear all FLASH errors flags before starting write operation */
  OPENBL_FLASH_ClearFlag(((FLASH_TypeDef *)((uint32_t)((hflash)->instance))), LL_FLASH_FLAG_ERRORS_ALL);

  status = OPENBL_FLASH_IsReadyForOperation(hflash);

  if (status == HAL_OK)
  {
    LL_FLASH_EnableProgramming(OPENBL_FLASH_GET_INSTANCE(hflash));

    OPENBL_FLASH_ProgramByWord(flash_address, p_data, size_byte);

    /* Only wait for operation end without checking errors */
    (void) OPENBL_FLASH_WaitForEndOfOperation(p_mem, OPENBL_FLASH_PROGRAM_TIMEOUT);

    LL_FLASH_DisableProgramming(OPENBL_FLASH_GET_INSTANCE(hflash));
  }
}

/**
  * @brief  Program up to a quad-word (128-bit) at a specified address by successive word write operations.
  * @param  address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed. Must be a multiple of the word size (4 bytes).
  * @retval None.
  */
static void OPENBL_FLASH_ProgramByWord(uint32_t address, uint8_t *p_data, uint32_t size_byte)
{
  uint32_t increment_index = size_byte / OPENBL_FLASH_WORD_SIZE_BYTE;
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
  * @brief  This function is used to enable write protection of the specified FLASH areas.
  * @param  p_mem Pointer to the memory object.
  * @param  p_pages_list Contains the list of pages to be protected.
  * @param  length The length of the list of pages to be protected.
  * @retval Returns an openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Enable or disable of the write protection is done
  *          - OPENBL_ERROR:   Enable or disable of the write protection is not done
  */
static openbl_status_t OPENBL_FLASH_EnableWriteProtection(openbl_mem_t *p_mem, uint8_t *p_pages_list, uint32_t length)
{
  uint32_t index;
  uint32_t page;
  openbl_status_t status = OPENBL_SUCCESS;

  /* Clear error programming flags */
  OPENBL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_Unlock_OB(p_mem);

  for (index = 0U; index < length; index++)
  {
    page = (uint32_t)p_pages_list[index];

    /* Pgaes to protect from 0 to 31 for WRP1R */
    if (page <= ((OPENBL_FLASH_PAGES_NUMBER / 2U) - 1U))
    {
      if (HAL_FLASH_ITF_OB_EnablePageWRP(HAL_FLASH, HAL_FLASH_BANK_1, page, 1U) != HAL_OK)
      {
        status = OPENBL_ERROR;
      }
    }
    /* Pages to protect from 0 to 31 for WRP2R */
    else if (page <= (OPENBL_FLASH_PAGES_NUMBER - 1U))
    {
      page = page - (OPENBL_FLASH_PAGES_NUMBER / 2U);

      if (HAL_FLASH_ITF_OB_EnablePageWRP(HAL_FLASH, HAL_FLASH_BANK_2, page, 1U) != HAL_OK)
      {
        status = OPENBL_ERROR;
      }
    }
    else
    {
      /* Nothing to do */
    }
  }

  if (HAL_FLASH_ITF_OB_Program(HAL_FLASH) != HAL_OK)
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
static openbl_status_t OPENBL_FLASH_DisableWriteProtection(openbl_mem_t *p_mem)
{
  openbl_status_t status = OPENBL_SUCCESS;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_Unlock_OB(p_mem);

  /* Clear all FLASH errors flags before starting write operation */
  OPENBL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

  if (HAL_FLASH_ITF_OB_DisablePageWRP(HAL_FLASH, HAL_FLASH_BANK_1, 0U, (OPENBL_FLASH_PAGES_NUMBER / 2U)) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  if (HAL_FLASH_ITF_OB_DisablePageWRP(HAL_FLASH, HAL_FLASH_BANK_2, 0U, (OPENBL_FLASH_PAGES_NUMBER / 2U)) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  if (HAL_FLASH_ITF_OB_Program(HAL_FLASH) != HAL_OK)
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  Check if the FLASH operation for a new operation.
  * @param  hflash       Pointer to a \ref hal_flash_handle_t structure.
  * @retval HAL_BUSY     FLASH is busy.
  * @retval HAL_ERROR    FLASH is locked or an error is pending.
  * @retval HAL_OK       FLASH is ready.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_IsReadyForOperation(hal_flash_handle_t *hflash)
{
  uint32_t flags;

  flags = OPENBL_FLASH_ReadFlag(OPENBL_FLASH_GET_INSTANCE(hflash), LL_FLASH_FLAG_STATUS_ALL | LL_FLASH_FLAG_ERRORS_ALL);

  if ((OPENBL_FLASH_IsLocked(OPENBL_FLASH_GET_INSTANCE(hflash)) != 0U) || ((flags & LL_FLASH_FLAG_ERRORS_ALL) != 0U))
  {
    return HAL_ERROR;
  }
  else if ((flags & LL_FLASH_FLAG_STATUS_ALL) != 0U)
  {
    return HAL_BUSY;
  }
  else
  {
    return HAL_OK;
  }
}

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  p_mem        Pointer to the memory object.
  * @param  timeout_msec Maximum flash operation timeout..
  * @retval HAL_TIMEOUT  User timeout.
  * @retval HAL_ERROR    Error during operation.
  * @retval HAL_OK       No operation to be waiting.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_WaitForEndOfOperation(openbl_mem_t *p_mem, uint32_t timeout_msec)
{
  uint32_t tickstart;
  uint32_t error_code;
  hal_flash_handle_t *hflash  = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;
  openbl_itf_obj_t *p_itf_obj = (openbl_itf_obj_t *)p_mem->p_parent;

  tickstart = HAL_GetTick();

  while (OPENBL_FLASH_IsActiveFlag(OPENBL_FLASH_GET_INSTANCE(hflash), LL_FLASH_FLAG_STATUS_ALL) != 0U)
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
      if (OPENBL_FLASH_IsActiveFlag(OPENBL_FLASH_GET_INSTANCE(hflash), LL_FLASH_FLAG_STATUS_ALL) != 0U)
      {
        return HAL_TIMEOUT;
      }
    }
  }

  error_code = OPENBL_FLASH_ReadFlag(OPENBL_FLASH_GET_INSTANCE(hflash), LL_FLASH_FLAG_ERRORS_ALL);

  if (error_code != 0U)
  {
#if defined (USE_HAL_FLASH_GET_LAST_ERRORS) && (USE_HAL_FLASH_GET_LAST_ERRORS == 1)
    /* Fill the FLASH handle with the detected error codes */
    FLASH_FillErrorCode(hflash, error_code);
#endif /* USE_HAL_FLASH_GET_LAST_ERRORS */

    /* Check error flags */
    OPENBL_FLASH_ClearFlag(OPENBL_FLASH_GET_INSTANCE(hflash), LL_FLASH_FLAG_ERRORS_ALL);

    return HAL_ERROR;
  }

  /* Clear FLASH End of Operation pending bit */
  OPENBL_FLASH_ClearFlag_EOP(OPENBL_FLASH_GET_INSTANCE(hflash));

  /* If no error flag is set */
  return HAL_OK;
}

/**
  * @brief  Erase pages of FLASH memory by address in polling mode.
  * @param  p_mem        Pointer to the memory object.
  * @param  bank         Bank to erase. This parameter can be one element of the \ref hal_flash_bank_t enumeration.
  * @param  erase_page   Index of the page to erase.
  * @param  erase_area   Area to erase.
  * @param  timeout_msec Maximum user timeout value for erase operation in milliseconds.
  * @retval HAL_ERROR    Error during erasing.
  * @retval HAL_TIMEOUT  User timeout.
  * @retval HAL_OK       No operation to be waiting.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_ErasePage(openbl_mem_t *p_mem,
                                                        hal_flash_bank_t bank,
                                                        uint32_t erase_page,
                                                        uint32_t erase_area,
                                                        uint32_t timeout_msec)
{
  hal_status_t status;
  hal_flash_handle_t *hflash = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;

  status = OPENBL_FLASH_IsReadyForOperation(hflash);

  if (status == HAL_OK)
  {
    HAL_CHECK_UPDATE_STATE(hflash, global_state, HAL_FLASH_STATE_IDLE, HAL_FLASH_STATE_ACTIVE);

    OPENBL_FLASH_StartErasePage(OPENBL_FLASH_GET_INSTANCE(hflash), OPENBL_FLASH_GET_ERASE_BANK(bank), erase_area, \
                                OPENBL_FLASH_GET_HW_PAGE_INDEX(erase_page, FLASH_PAGE_NB));

    status = OPENBL_FLASH_WaitForEndOfOperation(p_mem, timeout_msec);

    OPENBL_FLASH_DisablePageErase(OPENBL_FLASH_GET_INSTANCE(hflash));

    hflash->global_state = HAL_FLASH_STATE_IDLE;
  }

  return status;
}

/**
  * @brief  Mass erase the FLASH memory in polling mode.
  * @param  p_mem        Pointer to the memory object.
  * @param  timeout_msec Timeout value for mass erasing.
  * @retval HAL_INVALID_PARAM Returned when an invalid input parameter selected.
  * @retval HAL_ERROR         Returned when the FLASH access registers locked.
  * @retval HAL_TIMEOUT       Internal processing exceeded the timeout.
  * @retval HAL_BUSY          Returned when the FLASH is busy and cannot start a new erase operation.
  * @retval HAL_OK            Returned when the erasing operation completes successfully.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_MassErase(openbl_mem_t *p_mem, uint32_t timeout_msec)
{
  hal_status_t status;
  hal_flash_handle_t *hflash = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;

  ASSERT_DBG_PARAM(hflash != NULL);
  ASSERT_DBG_STATE(hflash->global_state, (uint32_t)HAL_FLASH_STATE_IDLE);

  status = OPENBL_FLASH_IsReadyForOperation(hflash);

  if (status == HAL_OK)
  {
    HAL_CHECK_UPDATE_STATE(hflash, global_state, HAL_FLASH_STATE_IDLE, HAL_FLASH_STATE_ACTIVE);

    OPENBL_FLASH_StartMassErase(OPENBL_FLASH_GET_INSTANCE(hflash));

    status = OPENBL_FLASH_WaitForEndOfOperation(p_mem, timeout_msec);

    OPENBL_FLASH_DisableMassErase(OPENBL_FLASH_GET_INSTANCE(hflash));

    hflash->global_state = HAL_FLASH_STATE_IDLE;
  }

  return status;
}

/**
  * @brief   Erase the FLASH memory by bank in polling mode.
  * @param   p_mem            Pointer to the memory object.
  * @param   bank              Bank to be erased. This parameter is one element of \ref hal_flash_bank_t enumeration.
  * @param   timeout_msec      Timeout value for bank erasing.
  * @warning This function will erase a specific Flash bank, thus it must not be mapped in this bank.
  * @retval  HAL_INVALID_PARAM Returned when an invalid input parameter selected.
  * @retval  HAL_ERROR         Returned when the FLASH access registers locked.
  * @retval  HAL_TIMEOUT       Internal processing exceeded the timeout.
  * @retval  HAL_BUSY          Returned when the FLASH is busy and cannot start a new erase operation.
  * @retval  HAL_OK            Returned when the erasing operation completes successfully.
  */
OPENBL_RAM_FUNCTION hal_status_t OPENBL_FLASH_EraseBank(openbl_mem_t *p_mem,
                                                        hal_flash_bank_t bank,
                                                        uint32_t timeout_msec)
{
  hal_status_t status;
  hal_flash_handle_t *hflash = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;

  ASSERT_DBG_PARAM(hflash != NULL);
  ASSERT_DBG_PARAM(bank != HAL_FLASH_BANK_ALL);
  ASSERT_DBG_PARAM(IS_FLASH_BANK(bank));
  ASSERT_DBG_STATE(hflash->global_state, (uint32_t)HAL_FLASH_STATE_IDLE);

  status = OPENBL_FLASH_IsReadyForOperation(hflash);

  if (status == HAL_OK)
  {
    HAL_CHECK_UPDATE_STATE(hflash, global_state, HAL_FLASH_STATE_IDLE, HAL_FLASH_STATE_ACTIVE);

    OPENBL_FLASH_StartEraseBank(OPENBL_FLASH_GET_INSTANCE(hflash), OPENBL_FLASH_GET_ERASE_BANK(bank));

    status = OPENBL_FLASH_WaitForEndOfOperation(p_mem, timeout_msec);

    OPENBL_FLASH_DisableBankErase(OPENBL_FLASH_GET_INSTANCE(hflash), OPENBL_FLASH_GET_ERASE_BANK(bank));

    hflash->global_state = HAL_FLASH_STATE_IDLE;
  }

  return status;
}

/**
  * @brief Provides a tick value in milliseconds.
  * @retval uint32_t HAL tick current value (unit: milliseconds)
  */
OPENBL_RAM_FUNCTION uint32_t HAL_GetTick(void)
{
  return uwTick;
}
