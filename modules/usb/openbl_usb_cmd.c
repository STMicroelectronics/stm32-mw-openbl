/**
  ******************************************************************************
  * @file    openbl_usb_cmd.c
  * @brief   Contains USB protocol commands
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "openbl_types.h"
#include "openbl_mem.h"
#include "openbl_usb_cmd.h"
#include "openbl_itf_usb.h"
#include "openbl_itf_common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to erase a sector.
  * @param  p_itf Pointer to the interface object.
  * @param  address Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t OPENBL_USB_EraseMemory(openbl_itf_obj_t *p_itf, uint32_t address)
{
  openbl_status_t error_value;
  uint8_t status;
  uint32_t numpage;
  uint32_t page;
  uint8_t usb_ram_buffer[OPENBL_USB_RAM_BUFFER_SIZE];
  uint8_t *ramaddress;

  ramaddress = (uint8_t *) usb_ram_buffer;
  numpage = 1U;

  *ramaddress = (uint8_t)(numpage & 0x00FFU);
  ramaddress++;

  *ramaddress = (uint8_t)((numpage & 0xFF00U) >> 8);
  ramaddress++;

  page = OPENBL_ITF_USB_GetPage(address);

  *ramaddress = (uint8_t)(page & 0x00FFU);
  ramaddress++;

  *ramaddress = (uint8_t)((page & 0xFF00U) >> 8);
  ramaddress++;

  STM32_UNUSED(ramaddress);

  error_value = OPENBL_MEM_Erase(p_itf->p_mem_list, address, (uint8_t *) usb_ram_buffer, OPENBL_USB_RAM_BUFFER_SIZE);

  if (error_value != OPENBL_SUCCESS)
  {
    status = 1U;
  }
  else
  {
    status = 0U;
  }

  return status;
}

/**
  * @brief  This function is used to read from memory.
  * @param  p_itf Pointer to the interface object.
  * @param  address Address to read from.
  * @param  p_data Pointer to the buffer to store read data.
  * @param  length Number of bytes to be read.
  * @retval None.
  */
void OPENBL_USB_ReadMemory(openbl_itf_obj_t *p_itf, uint32_t address, uint8_t *p_data, uint32_t length)
{
  openbl_mem_t *p_mem;
  uint32_t i;
  uint32_t address_tmp = address;

  p_mem = OPENBL_MEM_GetMemoryFromAddress(p_itf->p_mem_list, address_tmp);

  for (i = 0U; i < length; i++)
  {
    p_data[i] = OPENBL_MEM_Read(p_mem, address_tmp);

    address_tmp++;
  }
}

/**
  * @brief  This function is used to write to memory.
  * @param  p_itf Pointer to the interface object.
  * @param  address Address to write to.
  * @param  p_data Pointer to the buffer containing data to be written.
  * @param  length Number of bytes to be written.
  * @retval None.
  */
void OPENBL_USB_WriteMemory(openbl_itf_obj_t *p_itf, uint32_t address, uint8_t *p_data, uint32_t length)
{
  OPENBL_MEM_Write(p_itf->p_mem_list, address, p_data, length);

  /* Start post processing task if needed */
  OPENBL_MEM_PostProcessing(p_itf->p_mem_list, address);
}

/**
  * @brief  This function is used to jump to the user application.
  * @param  p_itf Pointer to the interface object.
  * @param  address Address to jump to.
  * @retval None.
  */
void OPENBL_USB_Jump(openbl_itf_obj_t *p_itf, uint32_t address)
{
  uint8_t status;

  /* Check if received address is valid or not */
  status = OPENBL_MEM_CheckJumpAddress(p_itf->p_mem_list, address);

  if (status == 1U)
  {
    OPENBL_MEM_JumpToAddress(p_itf->p_mem_list, address);
  }
}
