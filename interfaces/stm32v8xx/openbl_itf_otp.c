/**
  ******************************************************************************
  * @file    openbl_itf_otp.c
  * @brief   Contains One-time programmable access functions
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

#include "openbl_itf_nvm.h"
#include "openbl_nvm.h"
#include "openbl_itf_otp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_OTP_DATA_SIZE                  16U    /* OTP quad-word size in bytes */
#define OPENBL_OTP_WORD_SIZE_BYTE             4U     /* OTP word size in bytes */
#define OPENBL_OTP_PROGRAM_TIMEOUT            10U    /* Timeout value for writing quad-word data size in OTP */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_OTP_Program(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t size_byte);
static void OPENBL_OTP_ProgramByWord(uint32_t address, uint8_t *p_data, uint32_t size_byte);
static void OPENBL_OTP_ProgramByByte(uint32_t address, uint8_t *p_data, uint32_t size_byte);

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to read data from a given address.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_OTP_Read(openbl_mem_t *p_mem, uint32_t address)
{
  uint32_t tmp_address = address;
  uint32_t index       = 0U;
  uint16_t read_data;

  if ((tmp_address % OPENBL_OTP_DATA_SIZE) != 0U)
  {
    index        = (tmp_address % OPENBL_OTP_DATA_SIZE);
    tmp_address -= index;
  }

  /* Read a half word to be able to extract from it the needed byte */
  read_data = (*(uint16_t *)tmp_address);

  /* Return only the needed byte */
  return (*(((uint8_t *)&read_data) + index));
}

/**
  * @brief  This function is used to write data in OTP.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_OTP_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  __ALIGNED(4) uint8_t data[OPENBL_OTP_DATA_SIZE] = {0x0U};
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
    while ((data_length_tmp >> OPENBL_OTP_WORD_SIZE_BYTE) > 0U)
    {
      for (index = 0U; index < OPENBL_OTP_DATA_SIZE; index++)
      {
        data[index] = p_data[count + index];
      }

      OPENBL_OTP_Program(p_mem, address_tmp, data, OPENBL_OTP_DATA_SIZE);

      address_tmp     += OPENBL_OTP_DATA_SIZE;
      count           += OPENBL_OTP_DATA_SIZE;
      data_length_tmp -= OPENBL_OTP_DATA_SIZE;
    }

    /* If remaining count, go back to fill the rest with 0xFF */
    if (data_length_tmp > 0U)
    {
      /* Copy the remaining bytes */
      for (index = 0U; index < data_length_tmp; index++)
      {
        data[index] = p_data[count + index];

        OPENBL_OTP_Program(p_mem, address_tmp, &data[index], 1U);

        address_tmp++;
      }
    }

    /* Lock the nvm to disable the nvm control register access */
    OPENBL_NVM_Lock(p_mem);
  }
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
static void OPENBL_OTP_Program(openbl_mem_t *p_mem, uint32_t nvm_address, uint8_t *p_data, uint32_t size_byte)
{
  hal_status_t status;
  hal_nvm_handle_t *hnvm = (hal_nvm_handle_t *)p_mem->hw_context.p_itf_hw_context;

  /* Clear all NVM errors flags before starting write operation */
  OPENBL_NVM_ClearFlag((NVM_TypeDef *)((uint32_t)((hnvm)->instance)), LL_NVM_FLAG_ALL);

  status = OPENBL_NVM_IsReadyForOperation(hnvm);

  if (status == HAL_OK)
  {
    LL_NVM_EnableProgramming((NVM_TypeDef *)((uint32_t)((hnvm)->instance)));

    if (size_byte >= OPENBL_OTP_DATA_SIZE)
    {
      OPENBL_OTP_ProgramByWord(nvm_address, p_data, size_byte);
    }
    else
    {
      /* Enable read-modify-write for lower programming units */
      LL_NVM_EnableReadModifyWrite((NVM_TypeDef *)((uint32_t)((hnvm)->instance)));

      OPENBL_OTP_ProgramByByte(nvm_address, p_data, size_byte);
    }

    /* Only wait for operation end without checking errors */
    (void) OPENBL_NVM_WaitForEndOfOperation(p_mem, OPENBL_OTP_PROGRAM_TIMEOUT);

    LL_NVM_DisableAllOperation((NVM_TypeDef *)((uint32_t)((hnvm)->instance)));
  }
}

/**
  * @brief  Program up to a quad-word (128-bit) at a specified address by successive word write operations.
  * @param  address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed. Must be a multiple of the word size (4 bytes).
  * @retval None.
  */
static void OPENBL_OTP_ProgramByWord(uint32_t address, uint8_t *p_data, uint32_t size_byte)
{
  uint32_t increment_index = size_byte / OPENBL_OTP_DATA_SIZE;
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
static void OPENBL_OTP_ProgramByByte(uint32_t address, uint8_t *p_data, uint32_t size_byte)
{
  uint8_t *dest_addr  = (uint8_t *)(address);
  uint8_t *src_addr   = p_data;
  uint32_t primask_bit;

  /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Program */
  *dest_addr = *src_addr;

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}
