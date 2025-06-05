/**
  ******************************************************************************
  * @file    ram_interface.c
  * @author  MCD Application Team
  * @brief   Contains RAM access functions
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
#include "ram_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef RAM_Descriptor =
{
  RAM_START_ADDRESS + OPENBL_RAM_SIZE, /* OPENBL_RAM_SIZE is added to the RAM start address to protect OpenBootloader RAM area */
  RAM_END_ADDRESS,
  (128 * 1024),
  RAM_AREA,
  OPENBL_RAM_Read,
  OPENBL_RAM_Write,
  NULL,
  NULL,
  OPENBL_RAM_JumpToAddress,
  NULL,
  NULL
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_RAM_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/**
  * @brief  This function is used to write data in RAM memory.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_RAM_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
  uint32_t index          = 0U;
  uint32_t aligned_length = DataLength;

  if (aligned_length & 0x3)
  {
    aligned_length = (aligned_length & 0xFCU) + 4U;
  }

  for (index = 0U; index < aligned_length; index += 4U)
  {
    *(__IO uint32_t *)(Address + index) = *(__IO uint32_t *)(Data + index);
  }
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  Address The address where the function will jump.
  * @retval None.
  */
void OPENBL_RAM_JumpToAddress(uint32_t Address)
{
  Function_Pointer jump_to_address;

  /* Deinitialize all HW resources used by the Bootloader to their reset values */
  OpenBootloader_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
