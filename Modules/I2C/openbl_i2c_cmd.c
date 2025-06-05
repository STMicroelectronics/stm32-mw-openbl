/**
  ******************************************************************************
  * @file    openbl_i2c_cmd.c
  * @author  MCD Application Team
  * @brief   Contains I2C protocol commands
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
#include "openbl_i2c_cmd.h"

#include "openbootloader_conf.h"
#include "i2c_interface.h"
#include "flash_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_I2C_COMMANDS_NB          18U       /* Number of supported commands */

#define I2C_RAM_BUFFER_SIZE             1156U     /* Size of I2C buffer used to store received data from the host */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
static uint8_t I2C_RAM_Buf[I2C_RAM_BUFFER_SIZE];
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_I2C_GetCommand(void);
static void OPENBL_I2C_GetVersion(void);
static void OPENBL_I2C_GetID(void);
static void OPENBL_I2C_ReadMemory(void);
static void OPENBL_I2C_WriteMemory(void);
static void OPENBL_I2C_Go(void);
static void OPENBL_I2C_ReadoutProtect(void);
static void OPENBL_I2C_ReadoutUnprotect(void);
static void OPENBL_I2C_EraseMemory(void);
static void OPENBL_I2C_WriteProtect(void);
static void OPENBL_I2C_WriteUnprotect(void);
static void OPENBL_I2C_NonStretchWriteMemory(void);
static void OPENBL_I2C_NonStretchEraseMemory(void);
static void OPENBL_I2C_NonStretchWriteProtect(void);
static void OPENBL_I2C_NonStretchWriteUnprotect(void);
static void OPENBL_I2C_NonStretchReadoutProtect(void);
static void OPENBL_I2C_NonStretchReadoutUnprotect(void);

static uint8_t OPENBL_I2C_GetAddress(uint32_t *pAddress);

/* Exported variables --------------------------------------------------------*/
OPENBL_CommandsTypeDef OPENBL_I2C_Commands =
{
  OPENBL_I2C_GetCommand,
  OPENBL_I2C_GetVersion,
  OPENBL_I2C_GetID,
  OPENBL_I2C_ReadMemory,
  OPENBL_I2C_WriteMemory,
  OPENBL_I2C_Go,
  OPENBL_I2C_ReadoutProtect,
  OPENBL_I2C_ReadoutUnprotect,
  OPENBL_I2C_EraseMemory,
  OPENBL_I2C_WriteProtect,
  OPENBL_I2C_WriteUnprotect,
  OPENBL_I2C_NonStretchWriteMemory,
  OPENBL_I2C_NonStretchEraseMemory,
  OPENBL_I2C_NonStretchWriteProtect,
  OPENBL_I2C_NonStretchWriteUnprotect,
  OPENBL_I2C_NonStretchReadoutProtect,
  OPENBL_I2C_NonStretchReadoutUnprotect
};

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to get a pointer to the structure that contains the available I2C commands.
  * @return Returns a pointer to the OPENBL_I2C_Commands struct.
  */
OPENBL_CommandsTypeDef *OPENBL_I2C_GetCommandsList(void)
{
  return (&OPENBL_I2C_Commands);
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  This function is used to get the list of the available I2C commands
  * @retval None.
  */
static void OPENBL_I2C_GetCommand(void)
{
  uint32_t counter;
  const uint8_t a_OPENBL_I2C_CommandsList[OPENBL_I2C_COMMANDS_NB] =
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
    CMD_READ_UNPROTECT,
    CMD_NS_WRITE_MEMORY,
    CMD_NS_ERASE_MEMORY,
    CMD_NS_WRITE_PROTECT,
    CMD_NS_WRITE_UNPROTECT,
    CMD_NS_READ_PROTECT,
    CMD_NS_READ_UNPROTECT,
    CMD_CHECKSUM
  };

  /* Send ACK */
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  /* wait for address to match */
  OPENBL_I2C_WaitAddress();

  /* Send the number of commands supported by the I2C protocol */
  OPENBL_I2C_SendByte(OPENBL_I2C_COMMANDS_NB);

  /* Send I2C protocol version */
  OPENBL_I2C_SendByte(OPENBL_I2C_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < OPENBL_I2C_COMMANDS_NB; counter++)
  {
    OPENBL_I2C_SendByte(a_OPENBL_I2C_CommandsList[counter]);
  }

  /* wait until NACK is detected */
  OPENBL_I2C_WaitNack();

  /* wait until STOP is detected */
  OPENBL_I2C_WaitStop();

  /* Send last Acknowledge synchronization byte */
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the I2C protocol version.
  * @retval None.
  */
static void OPENBL_I2C_GetVersion(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  /* wait for address to match */
  OPENBL_I2C_WaitAddress();

  /* Send I2C protocol version */
  OPENBL_I2C_SendByte(OPENBL_I2C_VERSION);

  /* wait until NACK is detected */
  OPENBL_I2C_WaitNack();

  /* wait until STOP is detected */
  OPENBL_I2C_WaitStop();

  /* Send last Acknowledge synchronization byte */
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @retval None.
  */
static void OPENBL_I2C_GetID(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  /* wait for address to match */
  OPENBL_I2C_WaitAddress();

  OPENBL_I2C_SendByte(0x01);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_I2C_SendByte(DEVICE_ID_MSB);
  OPENBL_I2C_SendByte(DEVICE_ID_LSB);

  /* wait until NACK is detected */
  OPENBL_I2C_WaitNack();

  /* wait until STOP is detected */
  OPENBL_I2C_WaitStop();

  /* Send last Acknowledge synchronization byte */
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);
}

/**
 * @brief  This function is used to read memory from the device.
 * @retval None.
 */
static void OPENBL_I2C_ReadMemory(void)
{
  uint32_t address;
  uint32_t counter;
  uint32_t memory_index;
  uint8_t data;
  uint8_t xor;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_I2C_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

      /* wait for address to match */
      OPENBL_I2C_WaitAddress();

      /* Get the number of bytes to be received */
      data = OPENBL_I2C_ReadByte();
      xor  = ~data;

      /* Check data integrity */
      if (OPENBL_I2C_ReadByte() != xor)
      {
        OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        /* Send ACK */
        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

        /* Get the memory index to know from which memory we will read */
        memory_index = OPENBL_MEM_GetMemoryIndex(address);

        /* wait for address to match */
        OPENBL_I2C_WaitAddress();

        /* Read the data (data + 1) from the memory and send them to the host */
        for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
        {
          OPENBL_I2C_SendByte(OPENBL_MEM_Read(address, memory_index));
          address++;
        }
        /* wait until NACK is detected */
        OPENBL_I2C_WaitNack();

        /* wait until STOP is detected */
        OPENBL_I2C_WaitStop();
      }
    }
  }
}

/**
 * @brief  This function is used to write in to device memory.
 * @retval None.
 */
static void OPENBL_I2C_WriteMemory(void)
{
  uint32_t address;
  uint32_t xor;
  uint32_t counter;
  uint32_t codesize;
  uint32_t mem_area;
  uint8_t *p_ramaddress;
  uint8_t data;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_I2C_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

      /* Read number of bytes to be written and data */
      p_ramaddress = (uint8_t *)I2C_RAM_Buf;

      OPENBL_I2C_WaitAddress();

      /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
      data = OPENBL_I2C_ReadByte();

      /* Number of data to be written = data + 1 */
      codesize = (uint32_t)data + 1U;

      /* Checksum Initialization */
      xor = data;

      /* I2C receive data and send to RAM Buffer */
      for (counter = codesize; counter != 0U ; counter--)
      {
        data    = OPENBL_I2C_ReadByte();
        xor ^= data;

        *(__IO uint8_t *)(p_ramaddress) = data;

        p_ramaddress++;
      }

      /* Send NACk if Checksum is incorrect */
      if (OPENBL_I2C_ReadByte() != xor)
      {
        OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        /* Write data to memory */
        OPENBL_MEM_Write(address, (uint8_t *)I2C_RAM_Buf, codesize);

        /* Send last Acknowledge synchronization byte */
        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

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
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
static void OPENBL_I2C_Go(void)
{
  uint32_t address;
  uint32_t mem_area;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Get memory address */
    if (OPENBL_I2C_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      /* Check if received address is valid or not */
      mem_area = OPENBL_MEM_GetAddressArea(address);

      if ((mem_area != FLASH_AREA) && (mem_area != RAM_AREA))
      {
        OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        /* If the jump address is valid then send ACK */
        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

        OPENBL_MEM_JumpToAddress(address);
      }
    }
  }
}

/**
 * @brief  This function is used to enable readout protection.
 * @retval None.
 */
static void OPENBL_I2C_ReadoutProtect(void)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(ENABLE);

    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Launch Option Bytes reload */
    OPENBL_MEM_OptionBytesLaunch();
  }
}

/**
 * @brief  This function is used to disable readout protection.
 * @note  going from RDP level 1 to RDP level 0 erase all the flash,
 *        so the send of second acknowledge after Disabling the read protection
 *        is not possible what make the communication with the host get lost
 * @retval None.
 */
static void OPENBL_I2C_ReadoutUnprotect(void)
{
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(DISABLE);

  /* Launch Option Bytes reload and reset system */
  OPENBL_MEM_OptionBytesLaunch();
}

/**
 * @brief  This function is used to erase a memory.
 * @retval None.
 */
static void OPENBL_I2C_EraseMemory(void)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t numpage;
  uint16_t data;
  ErrorStatus error_value;
  uint8_t status = ACK_BYTE;
  uint8_t *p_ramaddress;

  p_ramaddress = (uint8_t *) I2C_RAM_Buf;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Wait for address to match */
    OPENBL_I2C_WaitAddress();

    /* Read number of pages to be erased */
    data = OPENBL_I2C_ReadByte();
    data = (uint16_t)(data << 8) | OPENBL_I2C_ReadByte();

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
      {
        OPENBL_I2C_WaitStop();

        status = NACK_BYTE;
      }
      else
      {
        OPENBL_I2C_WaitStop();

        if ((data == 0xFFFFU) || (data == 0xFFFEU) || (data == 0xFFFDU))
        {
          p_ramaddress[0] = (uint8_t)(data & 0x00FFU);
          p_ramaddress[1] = (uint8_t)((data & 0xFF00U) >> 8);

          error_value = OPENBL_MEM_MassErase(FLASH_START_ADDRESS, (uint8_t *) I2C_RAM_Buf, I2C_RAM_BUFFER_SIZE);

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
    }
    else
    {
      /* Check data integrity */
      if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
      {
        OPENBL_I2C_WaitStop();

        status = NACK_BYTE;
      }
      else
      {
        OPENBL_I2C_WaitStop();

        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

        p_ramaddress = (uint8_t *) I2C_RAM_Buf;

        /* Number of pages to be erased (data + 1) */
        numpage = (uint32_t)data + 1U;

        *p_ramaddress = (uint8_t)(numpage & 0x00FFU);
        p_ramaddress++;

        *p_ramaddress = (uint8_t)((numpage & 0xFF00U) >> 8);
        p_ramaddress++;

        OPENBL_I2C_WaitAddress();

        xor = 0;

        /* Get the pages to be erased */
        for (counter = numpage; counter != 0U ; counter--)
        {
          /* Receive the MSB byte */
          data  = OPENBL_I2C_ReadByte();
          xor  ^= data;
          data  = (uint16_t)((data & 0x00FFU) << 8);

          /* Receive the LSB byte */
          data |= (uint8_t)(OPENBL_I2C_ReadByte() & 0x00FFU);
          xor  ^= ((uint32_t)data & 0x00FFU);

          /* Only store data that fit in the buffer length */
          if (counter < (I2C_RAM_BUFFER_SIZE / 2U))
          {
            *p_ramaddress = (uint8_t)(data & 0x00FFU);
            p_ramaddress++;

            *p_ramaddress = (uint8_t)((data & 0xFF00U) >> 8);
            p_ramaddress++;
          }
        }

        /* Check data integrity */
        if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
        {
          OPENBL_I2C_WaitStop();

          status = NACK_BYTE;
        }
        else
        {
          OPENBL_I2C_WaitStop();

          error_value = OPENBL_MEM_Erase(FLASH_START_ADDRESS, (uint8_t *) I2C_RAM_Buf, I2C_RAM_BUFFER_SIZE);

          /* Errors from memory erase are not managed, always return ACK */
          if (error_value == SUCCESS)
          {
            status = ACK_BYTE;
          }
        }
      }
    }

    OPENBL_I2C_SendAcknowledgeByte(status);
  }
}


/**
 * @brief  This function is used to enable write protect.
 * @retval None.
 */
static void OPENBL_I2C_WriteProtect(void)
{
  uint16_t counter;
  uint16_t length;
  uint8_t data;
  uint8_t xor;
  ErrorStatus error_value;
  uint8_t *p_ramaddress;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* wait for address to match */
    OPENBL_I2C_WaitAddress();

    /* Get the data length */
    data = OPENBL_I2C_ReadByte();
    xor = ~data;

    /* Send NACk if Checksum is incorrect */
    if (OPENBL_I2C_ReadByte() != xor)
    {
      OPENBL_I2C_WaitStop();
      OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_I2C_WaitStop();
      OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

      p_ramaddress = (uint8_t *) I2C_RAM_Buf;
      length       = (uint16_t) data + 1U;

      /* Checksum Initialization */
      xor = 0U;

      /* wait for address to match */
      OPENBL_I2C_WaitAddress();

      /* Receive data and write to RAM Buffer */
      for (counter = (length); counter != 0U ; counter--)
      {
        data  = OPENBL_I2C_ReadByte();
        xor  ^= data;

        *(__IO uint8_t *)(p_ramaddress) = (uint8_t) data;

        p_ramaddress++;
      }

      /* Check data integrity and send NACK if Checksum is incorrect */
      if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
      {
        OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        p_ramaddress = (uint8_t *) I2C_RAM_Buf;

        /* Enable the write protection */
        error_value = OPENBL_MEM_SetWriteProtection(ENABLE, FLASH_START_ADDRESS, p_ramaddress, length);

        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

        if (error_value == SUCCESS)
        {
          OPENBL_MEM_OptionBytesLaunch();
        }
      }
    }
  }
}

/**
 * @brief  This function is used to disable write protect.
 * @retval None.
 */
static void OPENBL_I2C_WriteUnprotect(void)
{
  ErrorStatus error_value;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(DISABLE, FLASH_START_ADDRESS, NULL, 0);

    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    if (error_value == SUCCESS)
    {
      OPENBL_MEM_OptionBytesLaunch();
    }
  }
}

/**
 * @brief  This function is used to write in to device memory in non stretch mode.
 * @note   In this mode, when the write memory operation is executed the device
 *         send busy bytes to the host
 * @retval None.
 */
static void OPENBL_I2C_NonStretchWriteMemory(void)
{
  uint32_t address;
  uint32_t xor;
  uint32_t counter;
  uint32_t codesize;
  uint32_t mem_area;
  uint8_t *p_ramaddress;
  uint8_t data;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_I2C_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
    }
    else
    {
      OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

      /* Read number of bytes to be written and data */
      p_ramaddress = (uint8_t *)I2C_RAM_Buf;

      OPENBL_I2C_WaitAddress();

      /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
      data = OPENBL_I2C_ReadByte();

      /* Number of data to be written = data + 1 */
      codesize = (uint32_t)data + 1U;

      /* Checksum Initialization */
      xor = data;

      /* I2C receive data and send to RAM Buffer */
      for (counter = codesize; counter != 0U ; counter--)
      {
        data    = OPENBL_I2C_ReadByte();
        xor     ^= data;

        *(__IO uint8_t *)(p_ramaddress) = data;

        p_ramaddress++;
      }

      /* Send NACk if Checksum is incorrect */
      if (OPENBL_I2C_ReadByte() != xor)
      {
        OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
      }
      else
      {
        /* Send Busy Byte */
        OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

        /* Write data to memory */
        OPENBL_MEM_Write(address, (uint8_t *)I2C_RAM_Buf, codesize);

        /* Send last Acknowledge synchronization byte */
        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

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
}

/**
 * @brief  This function is used to erase a memory in non stretch mode.
 * @note   In this mode, when the erase memory operation is executed the device
 *         send busy bytes to the host
 * @retval None.
 */
static void OPENBL_I2C_NonStretchEraseMemory(void)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t numpage;
  uint16_t data;
  ErrorStatus error_value;
  uint8_t status = ACK_BYTE;
  uint8_t *p_ramaddress;

  p_ramaddress = (uint8_t *) I2C_RAM_Buf;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Wait for address to match */
    OPENBL_I2C_WaitAddress();

    /* Read number of pages to be erased */
    data = OPENBL_I2C_ReadByte();
    data = (uint16_t)(data << 8) | OPENBL_I2C_ReadByte();

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
      {
        OPENBL_I2C_WaitStop();
        status = NACK_BYTE;
      }
      else
      {
        OPENBL_I2C_WaitStop();

        /* Send Busy Byte */
        OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

        if ((data == 0xFFFFU) || (data == 0xFFFEU) || (data == 0xFFFDU))
        {
          p_ramaddress[0] = (uint8_t)(data & 0x00FFU);
          p_ramaddress[1] = (uint8_t)((data & 0xFF00U) >> 8);

          error_value = OPENBL_MEM_MassErase(FLASH_START_ADDRESS, (uint8_t *) I2C_RAM_Buf, I2C_RAM_BUFFER_SIZE);

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
    }
    else
    {
      /* Check data integrity */
      if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
      {
        OPENBL_I2C_WaitStop();
        status = NACK_BYTE;
      }
      else
      {
        OPENBL_I2C_WaitStop();
        OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

        p_ramaddress = (uint8_t *) I2C_RAM_Buf;

        /* Number of pages to be erased (data + 1) */
        numpage = (uint32_t)data + 1U;

        *p_ramaddress = (uint8_t)(numpage & 0x00FFU);
        p_ramaddress++;

        *p_ramaddress = (uint8_t)((numpage & 0xFF00U) >> 8);
        p_ramaddress++;

        /* Wait for address to match */
        OPENBL_I2C_WaitAddress();

        /* Checksum Initialization */
        xor = 0U;

        /* Get the pages to be erased */
        for (counter = numpage; counter != 0U ; counter--)
        {
          /* Receive the MSB byte */
          data  = OPENBL_I2C_ReadByte();
          xor  ^= data;
          data  = (uint16_t)((data & 0x00FFU) << 8);

          /* Receive the LSB byte */
          data |= (uint8_t)(OPENBL_I2C_ReadByte() & 0x00FFU);
          xor  ^= ((uint32_t)data & 0x00FFU);

          /* Only store data that fit in the buffer length */
          if (counter < (I2C_RAM_BUFFER_SIZE / 2U))
          {
            *p_ramaddress = (uint8_t)(data & 0x00FFU);
            p_ramaddress++;

            *p_ramaddress = (uint8_t)((data & 0xFF00U) >> 8);
            p_ramaddress++;
          }
        }

        /* Check data integrity */
        if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
        {
          OPENBL_I2C_WaitStop();
          status = NACK_BYTE;
        }
        else
        {
          OPENBL_I2C_WaitStop();

          /* Send Busy Byte */
          OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

          error_value = OPENBL_MEM_Erase(FLASH_START_ADDRESS, (uint8_t *) I2C_RAM_Buf, I2C_RAM_BUFFER_SIZE);

          /* Errors from memory erase are not managed, always return ACK */
          if (error_value == SUCCESS)
          {
            status = ACK_BYTE;
          }
        }
      }
    }

    OPENBL_I2C_SendAcknowledgeByte(status);
  }
}

/**
 * @brief  This function is used to enable write protect in non stretch mode.
 * @note   In this mode, when enabling the write protection the device
 *         send busy bytes to the host
 * @retval None.
 */
static void OPENBL_I2C_NonStretchWriteProtect(void)
{
  uint16_t counter;
  uint16_t length;
  uint8_t data;
  uint8_t xor;
  ErrorStatus error_value;
  uint8_t status = ACK_BYTE;
  uint8_t *p_ramaddress;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* wait for address to match */
    OPENBL_I2C_WaitAddress();

    /* Get the data length */
    data = OPENBL_I2C_ReadByte();
    xor  = ~data;

    /* Send NACk if Checksum is incorrect */
    if (OPENBL_I2C_ReadByte() != xor)
    {
      OPENBL_I2C_WaitStop();
      status = NACK_BYTE;
    }
    else
    {
      OPENBL_I2C_WaitStop();
      OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

      p_ramaddress = (uint8_t *) I2C_RAM_Buf;
      length       = (uint16_t) data + 1U;

      /* Checksum Initialization */
      xor = 0U;

      /* wait for address to match */
      OPENBL_I2C_WaitAddress();

      /* Receive data and write to RAM Buffer */
      for (counter = (length); counter != 0U ; counter--)
      {
        data  = OPENBL_I2C_ReadByte();
        xor  ^= data;

        *(__IO uint8_t *)(p_ramaddress) = (uint8_t) data;

        p_ramaddress++;
      }

      /* Check data integrity and send NACK if Checksum is incorrect */
      if (OPENBL_I2C_ReadByte() != (uint8_t) xor)
      {
        OPENBL_I2C_WaitStop();
        status = NACK_BYTE;
      }
      else
      {
        OPENBL_I2C_WaitStop();
        p_ramaddress = (uint8_t *) I2C_RAM_Buf;

        /* Send Busy Byte */
        OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

        /* Enable the write protection */
        error_value = OPENBL_MEM_SetWriteProtection(ENABLE, FLASH_START_ADDRESS, p_ramaddress, length);

        if (error_value == SUCCESS)
        {
          OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);
          OPENBL_MEM_OptionBytesLaunch();
        }
      }
    }

    OPENBL_I2C_SendAcknowledgeByte(status);
  }
}

/**
 * @brief  This function is used to disable write protect in non stretch mode.
 * @note   In this mode, when disabling the write protection the device
 *         send busy bytes to the host
 * @retval None.
 */
static void OPENBL_I2C_NonStretchWriteUnprotect(void)
{
  ErrorStatus error_value;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Send Busy Byte */
    OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(DISABLE, FLASH_START_ADDRESS, NULL, 0);

    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    if (error_value == SUCCESS)
    {
      OPENBL_MEM_OptionBytesLaunch();
    }
  }
}

/**
 * @brief  This function is used to enable readout protection in non stretch mode.
 * @note   In this mode, when enabling the readout protection the device
 *         send busy bytes to the host
 * @retval None.
 */
static void OPENBL_I2C_NonStretchReadoutProtect(void)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_I2C_SendAcknowledgeByte(NACK_BYTE);
  }
  else
  {
    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Send Busy Byte */
    OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(ENABLE);

    OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

    /* Launch Option Bytes reload */
    OPENBL_MEM_OptionBytesLaunch();
  }
}

/**
 * @brief  This function is used to disable readout protection in non stretch mode.
 * @note   In this mode, when disabling the readout protection the device
 *         send busy bytes to the host.
 *         going from RDP level 1 to RDP level 0 erase all the flash,
 *         so the send of second acknowledge after Disabling the read protection
 *         is not possible what make the communication with the host get lost
 * @retval None.
 */
static void OPENBL_I2C_NonStretchReadoutUnprotect(void)
{
  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  OPENBL_I2C_SendAcknowledgeByte(ACK_BYTE);

  /* Send Busy Byte */
  OPENBL_Enable_BusyState_Sending(OPENBL_I2C_SendBusyByte);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(DISABLE);

  /* Launch Option Bytes reload and reset system */
  OPENBL_MEM_OptionBytesLaunch();
}

/**
 * @brief  This function is used to get a valid address.
 * @retval Returns NACK status in case of error else returns ACK status.
 */
static uint8_t OPENBL_I2C_GetAddress(uint32_t *pAddress)
{
  uint8_t data[4] = {0, 0, 0, 0};
  uint8_t status;
  uint8_t xor;

  /* wait for address to match */
  OPENBL_I2C_WaitAddress();

  data[3] = OPENBL_I2C_ReadByte();
  data[2] = OPENBL_I2C_ReadByte();
  data[1] = OPENBL_I2C_ReadByte();
  data[0] = OPENBL_I2C_ReadByte();

  xor = data[3] ^ data[2] ^ data[1] ^ data[0];

  /* Check the integrity of received data */
  if (OPENBL_I2C_ReadByte() != xor)
  {
    status = NACK_BYTE;
  }
  else
  {
    /* wait until STOP is detected */
    OPENBL_I2C_WaitStop();

    *pAddress = ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    /* Check if received address is valid or not */
    if (OPENBL_MEM_GetAddressArea(*pAddress) == AREA_ERROR)
    {
      status = NACK_BYTE;
    }
    else
    {
      status = ACK_BYTE;
    }
  }

  return status;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
