/**
  ******************************************************************************
  * @file    openbl_itf_read_only.c
  * @brief   Contains read-only area access functions
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

#include "openbl_itf_read_only.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_RO_DATA_SIZE                  0x02U  /* Read-only half-word size in bytes */

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
uint8_t OPENBL_RO_Read(openbl_mem_t *p_mem, uint32_t address)
{
  uint32_t tmp_address = address;
  uint32_t index       = 0U;
  uint16_t read_data;

  STM32_UNUSED(p_mem);

  if ((tmp_address % OPENBL_RO_DATA_SIZE) != 0U)
  {
    index        = (tmp_address % OPENBL_RO_DATA_SIZE);
    tmp_address -= index;
  }

  /* Read a half word to be able to extract from it the needed byte */
  read_data = (*(uint16_t *)tmp_address);

  /* Return only the needed byte */
  return (*(((uint8_t *)&read_data) + index));
}

/* Private functions ---------------------------------------------------------*/
