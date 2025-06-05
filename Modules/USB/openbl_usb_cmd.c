/**
  ******************************************************************************
  * @file    openbl_usb_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USB protocol commands
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
#include "openbl_usb_cmd.h"
#include "openbl_mem.h"
#include "openbootloader_conf.h"
#include "usb_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USB_RAM_BUFFER_SIZE             1156U     /* Size of USB buffer used to store received data from the host */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint32_t GetPage(uint32_t Address);

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  Erase sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successful, MAL_FAIL else.
  */
uint16_t OPENBL_USB_EraseMemory(uint32_t Add)
{
  ErrorStatus error_value;
  uint8_t status;
  uint32_t numpage;
  uint32_t page;
  uint8_t usb_ram_buffer[USB_RAM_BUFFER_SIZE];
  uint8_t *ramaddress;

  ramaddress = (uint8_t *) usb_ram_buffer;
  numpage = 1;

  *ramaddress = (uint8_t)(numpage & 0x00FFU);
  ramaddress++;

  *ramaddress = (uint8_t)((numpage & 0xFF00U) >> 8);
  ramaddress++;

  page = GetPage(Add);

  *ramaddress = (uint8_t)(page & 0x00FFU);
  ramaddress++;

  *ramaddress = (uint8_t)((page & 0xFF00U) >> 8);
  ramaddress++;

  error_value = OPENBL_MEM_Erase(FLASH_START_ADDRESS, (uint8_t *) usb_ram_buffer, 20);

  if (error_value != SUCCESS)
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
  * @brief  Memory write routine.
  * @param  pSrc: Pointer to the source buffer. Address to be written to.
  * @param  pDest: Pointer to the destination buffer.
  * @param  Length: Number of data to be written (in bytes).
  * @retval USBD_OK if operation is successful, MAL_FAIL else.
  */
void OPENBL_USB_WriteMemory(uint8_t *pSrc, uint8_t *pDest, uint32_t Length)
{
  uint32_t mem_area;
  uint8_t *ptemp = ((uint8_t *)(&(pDest)));
  uint32_t addr = (uint32_t)ptemp[0] | ((uint32_t)ptemp[1] << 8) |
                  ((uint32_t)ptemp[2] << 16) | ((uint32_t)ptemp[3] << 24);

  OPENBL_MEM_Write(addr, pSrc, Length);

  /* Check if the received address is an option byte address */
  mem_area = OPENBL_MEM_GetAddressArea(addr);

  if (mem_area == OB_AREA)
  {
    /* Launch Option Bytes reload */
    OPENBL_MEM_OptionBytesLaunch();
  }
}

/**
  * @brief  Memory read routine.
  * @param  pSrc: Pointer to the source buffer. Address to be written to.
  * @param  dest: Pointer to the destination buffer.
  * @param  Length: Number of data to be read (in bytes).
  * @retval Pointer to the physical address where data should be read.
  */
uint8_t *OPENBL_USB_ReadMemory(uint8_t *pSrc, uint8_t *pDest, uint32_t Length)
{
  uint32_t memory_index;
  uint32_t i;
  uint8_t *ptemp = ((uint8_t *)(&(pSrc)));
  uint32_t address = (uint32_t)ptemp[0] | ((uint32_t)ptemp[1] << 8) |
                     ((uint32_t)ptemp[2] << 16) | ((uint32_t)ptemp[3] << 24);

  memory_index = OPENBL_MEM_GetMemoryIndex(address);

  for (i = 0; i < Length; i++)
  {
    pDest[i] = OPENBL_MEM_Read(address, memory_index);
    address++;
  }

  /* Return a valid address to avoid HardFault */
  return pDest;
}

/**
  * @brief  Gets the page of a given address
  * @param  Address: Address of the FLASH Memory
  * @retval The page of a given address
  */

/* Private functions ---------------------------------------------------------*/
static uint32_t GetPage(uint32_t Address)
{
  uint32_t page;

  if (Address < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Address - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Address - FLASH_BASE) / FLASH_PAGE_SIZE;
  }

  return page;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
