/**
  ******************************************************************************
  * @file    openbl_itf_eeprom.c
  * @brief   Contains EEPROM access functions
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
#include "openbl_conf.h"

#include "openbl_itf_flash.h"
#include "openbl_itf_eeprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_EEPROM_DATA_SIZE                  0x02U  /* EEPROM half-word size in bytes */
#define OPENBL_EEPROM_PROGRAM_TIMEOUT            10U    /* Timeout value for writing half-word data size in EEPROM */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_EEPROM_Program(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t size_byte);

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to read data from a given address.
  * @param  p_mem Pointer to the memory object.
  * @param  address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_EEPROM_Read(openbl_mem_t *p_mem, uint32_t address)
{
  uint32_t tmp_address = address;
  uint32_t index       = 0U;
  uint16_t read_data;

  STM32_UNUSED(p_mem);

  if ((tmp_address % OPENBL_EEPROM_DATA_SIZE) != 0U)
  {
    index        = (tmp_address % OPENBL_EEPROM_DATA_SIZE);
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
void OPENBL_EEPROM_Write(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  __ALIGNED(4) uint8_t data[OPENBL_EEPROM_DATA_SIZE] = {0x0U};
  uint8_t remaining;

  if ((p_data != NULL) && (data_length != 0U))
  {
    /* Unlock the flash memory for write operation */
    OPENBL_FLASH_Unlock(p_mem);

    /* Clear all FLASH errors flags before starting write operation */
    LL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

    /* Program half-word by half-word (2 bytes) */
    while (data_length > OPENBL_EEPROM_DATA_SIZE)
    {
      for (index = 0U; index < OPENBL_EEPROM_DATA_SIZE; index++)
      {
        data[index] = *(p_data + index);
      }

      OPENBL_EEPROM_Program(p_mem, address, (uint8_t *)data, OPENBL_EEPROM_DATA_SIZE);

      address     += OPENBL_EEPROM_DATA_SIZE;
      p_data      += OPENBL_EEPROM_DATA_SIZE;
      data_length -= OPENBL_EEPROM_DATA_SIZE;
    }

    /* If remaining count, go back to fill the rest with 0xFF */
    if (data_length > 0U)
    {
      remaining = OPENBL_EEPROM_DATA_SIZE - data_length;

      /* Copy the remaining bytes */
      for (index = 0U; index < data_length; index++)
      {
        data[index] = *(p_data + index);
      }

      /* Fill the upper bytes with 0xFF */
      for (index = 0U; index < remaining; index++)
      {
        data[index + data_length] = 0xFFU;
      }

      /* FLASH word program */
      OPENBL_EEPROM_Program(p_mem, address, (uint8_t *)data, OPENBL_EEPROM_DATA_SIZE);
    }

    /* Lock the Flash to disable the flash control register access */
    OPENBL_FLASH_Lock(p_mem);
  }
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Program EEPROM memory at specified address.
  * @param  p_mem Pointer to the memory object.
  * @param  address Specifies the address where data will be programmed.
  * @param  p_data Pointer to the buffer of data to be programmed.
  * @param  size_byte Size in bytes of the data to be programmed.
  * @retval None.
  */
static void OPENBL_EEPROM_Program(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t size_byte)
{
  hal_flash_handle_t *hflash = (hal_flash_handle_t *)p_mem->hw_context.p_itf_hw_context;

  /* Clear all FLASH errors flags before starting write operation */
  LL_FLASH_ClearFlag((FLASH_TypeDef *)((uint32_t)HAL_FLASH), LL_FLASH_FLAG_ERRORS_ALL);

  /* Write data in EEPROM without checking for errors */
  (void) HAL_FLASH_EDATA_ProgramByAddr(hflash,
                                       address,
                                       (uint32_t *)p_data,
                                       size_byte,
                                       OPENBL_EEPROM_PROGRAM_TIMEOUT);
}
