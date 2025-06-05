/**
  ******************************************************************************
  * @file    openbl_fdcan_cmd.c
  * @author  MCD Application Team
  * @brief   Contains FDCAN protocol commands
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
#include "openbl_mem.h"
#include "openbl_core.h"
#include "openbl_fdcan_cmd.h"

#include "openbootloader_conf.h"
#include "fdcan_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_FDCAN_COMMANDS_NB             11U    /* Number of supported commands */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_FDCAN_GetCommand(void);
static void OPENBL_FDCAN_GetVersion(void);
static void OPENBL_FDCAN_GetID(void);
static void OPENBL_FDCAN_ReadMemory(void);
static void OPENBL_FDCAN_WriteMemory(void);
static void OPENBL_FDCAN_Go(void);
static void OPENBL_FDCAN_ReadoutProtect(void);
static void OPENBL_FDCAN_ReadoutUnprotect(void);
static void OPENBL_FDCAN_EraseMemory(void);
static void OPENBL_FDCAN_WriteProtect(void);
static void OPENBL_FDCAN_WriteUnprotect(void);


static uint8_t OPENBL_FDCAN_GetAddress(uint32_t *Address);

/* Exported variables --------------------------------------------------------*/

OPENBL_CommandsTypeDef OPENBL_FDCAN_Commands =
{
  OPENBL_FDCAN_GetCommand,
  OPENBL_FDCAN_GetVersion,
  OPENBL_FDCAN_GetID,
  OPENBL_FDCAN_ReadMemory,
  OPENBL_FDCAN_WriteMemory,
  OPENBL_FDCAN_Go,
  OPENBL_FDCAN_ReadoutProtect,
  OPENBL_FDCAN_ReadoutUnprotect,
  OPENBL_FDCAN_EraseMemory,
  OPENBL_FDCAN_WriteProtect,
  OPENBL_FDCAN_WriteUnprotect,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to get a pointer to the structure that contains the available FDCAN commands.
  * @return Returns a pointer to the OPENBL_CommandsTypeDef struct.
  */
OPENBL_CommandsTypeDef *OPENBL_FDCAN_GetCommandsList(void)
{
  return (&OPENBL_FDCAN_Commands);
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to get the list of the available FDCAN commands
  * @retval None.
  */
static void OPENBL_FDCAN_GetCommand(void)
{
  uint32_t counter;
  const uint8_t a_OPENBL_FDCAN_CommandsList[OPENBL_FDCAN_COMMANDS_NB] =
  {
    CMD_GET_COMMAND,
    CMD_GET_VERSION,
    CMD_GET_ID,
    CMD_READ_MEMORY,
    CMD_GO,
    CMD_WRITE_MEMORY,
    CMD_EXT_ERASE_MEMORY,
    CMD_WRITE_PROTECT,
    CMD_WRITE_UNPROTECT,
    CMD_READ_PROTECT,
    CMD_READ_UNPROTECT
  };

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_FDCAN_SendByte(ACK_BYTE);

  /* Send the number of commands supported by FDCAN protocol */
  OPENBL_FDCAN_SendByte(OPENBL_FDCAN_COMMANDS_NB);

  /* Send FDCAN protocol version */
  OPENBL_FDCAN_SendByte(OPENBL_FDCAN_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < OPENBL_FDCAN_COMMANDS_NB; counter++)
  {
    OPENBL_FDCAN_SendByte(a_OPENBL_FDCAN_CommandsList[counter]);
  }

  /* Send last Acknowledge synchronization byte */
  OPENBL_FDCAN_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the FDCAN protocol version.
  * @retval None.
  */
static void OPENBL_FDCAN_GetVersion(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_FDCAN_SendByte(ACK_BYTE);

  /* Send FDCAN protocol version */
  OPENBL_FDCAN_SendByte(OPENBL_FDCAN_VERSION);

  /* Send dummy bytes */
  TxData[0] = 0x0;
  TxData[1] = 0x0;
  OPENBL_FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_2);

  /* Send last Acknowledge synchronization byte */
  OPENBL_FDCAN_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @retval None.
  */
static void OPENBL_FDCAN_GetID(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_FDCAN_SendByte(ACK_BYTE);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  TxData[0] = DEVICE_ID_MSB;
  TxData[1] = DEVICE_ID_LSB;
  OPENBL_FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_2);

  /* Send last Acknowledge synchronization byte */
  OPENBL_FDCAN_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to read memory from the device.
  * @retval None.
  */
static void OPENBL_FDCAN_ReadMemory(void)
{
  uint32_t address;
  uint32_t counter;
  uint32_t number_of_bytes;
  uint32_t count;
  uint32_t single;
  uint32_t memory_index;
  uint8_t  data_length;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    if (OPENBL_FDCAN_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_FDCAN_SendByte(NACK_BYTE);
    }
    else
    {
      OPENBL_FDCAN_SendByte(ACK_BYTE);

      /* Get the number of bytes to be read from memory (Max: data + 1 = 256) */
      number_of_bytes = (uint32_t)RxData[4] + 1U;

      count  = number_of_bytes / 64U;
      single = (uint32_t)(number_of_bytes % 64U);

      /* Get the memory index to know from which memory we will read */
      memory_index = OPENBL_MEM_GetMemoryIndex(address);

      while (count != 0U)
      {
        data_length = 0;

        for (counter = 64U ; counter > 0U; counter--)
        {
          TxData[data_length] = OPENBL_MEM_Read(address, memory_index);

          data_length++;
          address++;
        }

        OPENBL_FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_64);

        count--;
      }

      if (single != 0U)
      {
        data_length = 0U;

        for (counter = single ; counter > 0U; counter--)
        {
          TxData[data_length] = OPENBL_MEM_Read(address, memory_index);

          data_length++;
          address++;
        }

        /* Fill the rest of the buffer with 0xFF */
        for (counter = (64U - single) ; counter > 0U; counter--)
        {
          TxData[data_length] = 0xFF;

          data_length++;
        }

        OPENBL_FDCAN_SendBytes(TxData, FDCAN_DLC_BYTES_64);
      }

      OPENBL_FDCAN_SendByte(ACK_BYTE);
    }
  }
}

/**
  * @brief  This function is used to write in to device memory.
  * @retval None.
  */
static void OPENBL_FDCAN_WriteMemory(void)
{
  uint32_t address;
  uint32_t CodeSize;
  uint32_t count;
  uint32_t single;
  uint32_t mem_area;
  uint8_t data_length;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    if (OPENBL_FDCAN_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_FDCAN_SendByte(NACK_BYTE);
    }
    else
    {
      OPENBL_FDCAN_SendByte(ACK_BYTE);

      /* Get the number of bytes to be written to memory (Max: data + 1 = 256) */
      CodeSize = (uint32_t)RxData[4] + 1U;

      count = CodeSize / 64U;
      single = (uint32_t)(CodeSize % 64U);

      data_length = 0;

      if (count != 0U)
      {
        while (data_length != count)
        {
          OPENBL_FDCAN_ReadBytes(&RxData[data_length * 64U], 64U);

          data_length++;
        }
      }

      if (single != 0U)
      {
        OPENBL_FDCAN_ReadBytes(&RxData[(CodeSize - single)], 64U);
      }

      /* Write data to memory */
      OPENBL_MEM_Write(address, (uint8_t *)RxData, CodeSize);

      /* Send last Acknowledge synchronization byte */
      OPENBL_FDCAN_SendByte(ACK_BYTE);

      /* Check if the received address is an option byte address */
      mem_area = OPENBL_MEM_GetAddressArea(address);

      if (mem_area == OB_AREA)
      {
        /* Launch Option Bytes reload */
        OPENBL_MEM_OptionBytesLaunch();
      }
    }
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
static void OPENBL_FDCAN_Go(void)
{
  uint32_t address;
  uint32_t mem_area;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_FDCAN_SendByte(ACK_BYTE);

    if (OPENBL_FDCAN_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_FDCAN_SendByte(NACK_BYTE);
    }
    else
    {
      /* Check if received address is valid or not */
      mem_area = OPENBL_MEM_GetAddressArea(address);

      if ((mem_area != FLASH_AREA) && (mem_area != RAM_AREA))
      {
        OPENBL_FDCAN_SendByte(NACK_BYTE);
      }
      else
      {
        /* If the jump address is valid then send ACK */
        OPENBL_FDCAN_SendByte(ACK_BYTE);

        OPENBL_MEM_JumpToAddress(address);
      }
    }
  }
}

/**
 * @brief  This function is used to enable readout protection.
 * @retval None.
 */
static void OPENBL_FDCAN_ReadoutProtect(void)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_FDCAN_SendByte(ACK_BYTE);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(ENABLE);

    OPENBL_FDCAN_SendByte(ACK_BYTE);

    /* Launch Option Bytes reload */
    OPENBL_MEM_OptionBytesLaunch();
  }
}

/**
 * @brief  This function is used to disable readout protection.
 * @retval None.
 */
static void OPENBL_FDCAN_ReadoutUnprotect(void)
{
  OPENBL_FDCAN_SendByte(ACK_BYTE);

  /* Once the option bytes modification start bit is set in FLASH CR register,
     all the RAM is erased, this causes the erase of the Open Bootloader RAM.
     This is why the last ACK is sent before the call of OPENBL_MEM_SetReadOutProtection */
  OPENBL_FDCAN_SendByte(ACK_BYTE);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(DISABLE);

  /* Launch Option Bytes reload and reset system */
  OPENBL_MEM_OptionBytesLaunch();
}

/**
 * @brief  This function is used to erase a memory.
 * @retval None.
 */
static void OPENBL_FDCAN_EraseMemory(void)
{
  uint16_t data;
  uint16_t counter;
  uint16_t i;
  uint8_t tempdata;
  uint8_t status = ACK_BYTE;
  ErrorStatus error_value;

  /* Check if the memory is protected or not */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_FDCAN_SendByte(ACK_BYTE);

    /* Read number of pages to be erased:
     * RxData[0] contains the MSB byte
     * RxData[1] contains the LSB byte
     */
    data = (uint16_t)RxData[0] << 8;
    data = data | RxData[1];

    /* Rewrite the data in right order in the RxData buffer
     * RxData[0] = LSB
     * RxData[1] = MSB
     */
    RxData[0] = (uint8_t)(data & 0x00FFU);
    RxData[1] = (uint8_t)((data & 0xFF00U) >> 8);

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      if ((data == 0xFFFFU) || (data == 0xFFFEU) || (data == 0xFFFDU))
      {
        error_value = OPENBL_MEM_MassErase(FLASH_START_ADDRESS, RxData, FDCAN_RAM_BUFFER_SIZE);

        if (error_value == SUCCESS)
        {
          status = ACK_BYTE;
        }
        else
        {
          status = NACK_BYTE;
        }
      }
      else
      {
        /* This sub-command is not supported */
        status = NACK_BYTE;
      }
    }
    else
    {
      OPENBL_FDCAN_SendByte(ACK_BYTE);

      /* Receive the list of pages to be erased (each page num is on two bytes)
       * The order of data received is LSB first
       */
      OPENBL_FDCAN_ReadBytes(&RxData[2], 64U);

      i = 2;

      for (counter = data; counter != (uint16_t)0; counter--)
      {
        /* Receive the MSB byte */
        tempdata = RxData[i];
        i++;
        RxData[i - (uint16_t)1] = RxData[i];
        RxData[i] = tempdata;
        i++;
      }

      error_value = OPENBL_MEM_Erase(FLASH_START_ADDRESS, RxData, FDCAN_RAM_BUFFER_SIZE);

      /* Errors from memory erase are not managed, always return ACK */
      if (error_value == SUCCESS)
      {
        status = ACK_BYTE;
      }
    }

    OPENBL_FDCAN_SendByte(status);
  }
}

/**
 * @brief  This function is used to enable write protect.
 * @retval None.
 */
static void OPENBL_FDCAN_WriteProtect(void)
{
  uint32_t length;
  ErrorStatus error_value;

  /* Check if the memory is protected or not */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_FDCAN_SendByte(ACK_BYTE);

    length = RxData[0];

    /* Enable the write protection */
    error_value = OPENBL_MEM_SetWriteProtection(ENABLE, FLASH_START_ADDRESS, (uint8_t *) &RxData[1], length);

    OPENBL_FDCAN_SendByte(ACK_BYTE);

    if (error_value == SUCCESS)
    {
      OPENBL_MEM_OptionBytesLaunch();
    }
  }
}

/**
 * @brief  This function is used to disable write protect.
 * @retval None.
 */
static void OPENBL_FDCAN_WriteUnprotect(void)
{
  ErrorStatus error_value;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FDCAN_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_FDCAN_SendByte(ACK_BYTE);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(DISABLE, FLASH_START_ADDRESS, NULL, 0);

    OPENBL_FDCAN_SendByte(ACK_BYTE);

    if (error_value == SUCCESS)
    {
      OPENBL_MEM_OptionBytesLaunch();
    }
  }
}

/**
  * @brief  This function is used to get a valid address.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_FDCAN_GetAddress(uint32_t *Address)
{
  uint8_t status;

  *Address = (((((uint32_t) RxData[0]) << 24)  |
               (((uint32_t) RxData[1]) << 16)  |
               (((uint32_t) RxData[2]) << 8)   |
               (((uint32_t) RxData[3]))));

  /* Check if received address is valid or not */
  if (OPENBL_MEM_GetAddressArea(*Address) == AREA_ERROR)
  {
    status = NACK_BYTE;
  }
  else
  {
    status = ACK_BYTE;
  }

  return status;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
