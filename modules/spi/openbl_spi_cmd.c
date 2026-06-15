/**
  ******************************************************************************
  * @file    openbl_spi_cmd.c
  * @brief   Contains SPI protocol commands
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
#include "openbl_mem.h"
#include "openbl_spi_cmd.h"

#include "openbl_types.h"
#include "openbl_itf_spi.h"
#include "openbl_itf_common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
/* Private function prototypes -----------------------------------------------*/
static uint8_t OPENBL_SPI_GetAddress(openbl_itf_obj_t *p_itf, uint32_t *p_address);
static uint8_t OPENBL_SPI_GetSpecialCmdOpCode(openbl_itf_obj_t *p_itf, openbl_special_cmd_type_t cmd_type, \
                                              uint16_t *p_opcode);

/* Exported variables --------------------------------------------------------*/
/* Exported functions---------------------------------------------------------*/

/**
  * @brief  This function is used to get a pointer to the structure that contains the available SPI commands.
  * @return Returns a pointer to the OPENBL_SPI_Commands struct.
  */
openbl_commands_t *OPENBL_SPI_GetCommandsList(void)
{
  static openbl_commands_t OPENBL_SPI_Commands =
  {
    OPENBL_SPI_GetCommand,
    OPENBL_SPI_GetVersion,
    OPENBL_SPI_GetID,
    OPENBL_SPI_ReadMemory,
    OPENBL_SPI_WriteMemory,
    OPENBL_SPI_Go,
    OPENBL_SPI_ReadoutProtect,
    OPENBL_SPI_ReadoutUnprotect,
    OPENBL_SPI_EraseMemory,
    OPENBL_SPI_WriteProtect,
    OPENBL_SPI_WriteUnprotect,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    OPENBL_SPI_SpecialCommand,
    OPENBL_SPI_ExtendedSpecialCommand
  };

  return (&OPENBL_SPI_Commands);
}

/**
  * @brief  This function is used to construct the command opcodes list.
  * @param  p_spi_cmd Pointer to the structure containing the SPI supported commands.
  * @param  p_cmd_opcodes_list Pointer to the structure containing the list of supported commands by the SPI protocol.
  * @return None.
  */
void OPENBL_SPI_FillCommandsOpcodesList(const openbl_commands_t *p_spi_cmd, \
                                        openbl_cmd_opcodes_list_t *p_cmd_opcodes_list)
{
  uint8_t i = 0U;

  if ((p_spi_cmd->GetCommand != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GET_COMMAND;
    i++;
  }

  if ((p_spi_cmd->GetVersion != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GET_VERSION;
    i++;
  }

  if ((p_spi_cmd->GetID != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GET_ID;
    i++;
  }

  if ((p_spi_cmd->ReadMemory != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_READ_MEMORY;
    i++;
  }

  if ((p_spi_cmd->Go != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GO;
    i++;
  }

  if ((p_spi_cmd->WriteMemory != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_WRITE_MEMORY;
    i++;
  }

  if ((p_spi_cmd->EraseMemory != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_EXT_ERASE_MEMORY;
    i++;
  }

  if ((p_spi_cmd->WriteProtect != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_WRITE_PROTECT;
    i++;
  }

  if ((p_spi_cmd->WriteUnprotect != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_WRITE_UNPROTECT;
    i++;
  }

  if ((p_spi_cmd->ReadoutProtect != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_READ_PROTECT;
    i++;
  }

  if ((p_spi_cmd->ReadoutUnprotect != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_READ_UNPROTECT;
    i++;
  }

  if ((p_spi_cmd->SpecialCommand != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_SPECIAL_COMMAND;
    i++;
  }

  if ((p_spi_cmd->ExtendedSpecialCommand != NULL) && (i < (uint8_t)OPENBL_SPI_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_EXT_SPECIAL_COMMAND;
    i++;
  }

  p_cmd_opcodes_list->number = i;
}

/**
  * @brief  This function is used to get the list of the available SPI commands.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_GetCommand(openbl_itf_obj_t *p_itf)
{
  uint32_t counter;

  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Send the number of commands supported by the SPI protocol */
  OPENBL_ITF_SPI_SendByte(p_itf, (uint8_t)(p_itf->cmd_opcodes_list.number));

  /* Send SPI protocol version */
  OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_SPI_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < p_itf->cmd_opcodes_list.number; counter++)
  {
    OPENBL_ITF_SPI_SendByte(p_itf, p_itf->cmd_opcodes_list.p_opcodes[counter]);
  }

  /* Send last Acknowledge synchronization byte */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
}

/**
  * @brief  This function is used to get the SPI protocol version.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_GetVersion(openbl_itf_obj_t *p_itf)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Send SPI protocol version */
  OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_SPI_VERSION);

  /* Send last Acknowledge synchronization byte */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_GetID(openbl_itf_obj_t *p_itf)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  OPENBL_ITF_SPI_SendByte(p_itf, 0x01U);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_DEVICE_ID_MSB);
  OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_DEVICE_ID_LSB);

  /* Send last Acknowledge synchronization byte */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
}

/**
  * @brief  This function is used to read memory from the device.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_ReadMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint32_t counter;
  openbl_mem_t *p_mem;
  uint8_t data;
  uint8_t xor;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_SPI_GetAddress(p_itf, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      /* Get the number of bytes to be received */
      data = OPENBL_ITF_SPI_ReadByte(p_itf);
      xor  = ~data;

      /* Check data integrity */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Get the memory index to know from which memory we will read */
        p_mem = OPENBL_MEM_GetMemoryFromAddress(p_itf->p_mem_list, address);

        /* Read the data (data + 1) from the memory and send them to the host */
        for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
        {
          OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_MEM_Read(p_mem, address));
          address++;
        }
      }
    }
  }
}

/**
  * @brief  This function is used to write in to device memory.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_WriteMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint32_t xor;
  uint32_t counter;
  uint32_t data_size;
  uint8_t *p_tmp_buffer;
  uint8_t data;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_SPI_GetAddress(p_itf, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      /* Read number of bytes to be written and data */
      p_tmp_buffer = p_itf->buffer.p_data;

      /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
      data = OPENBL_ITF_SPI_ReadByte(p_itf);

      /* Number of data to be written = data + 1 */
      data_size = (uint32_t)data + 1U;

      /* Checksum Initialization */
      xor = data;

      /* SPI receive data and send to RAM Buffer */
      for (counter = data_size; counter != 0U ; counter--)
      {
        data  = OPENBL_ITF_SPI_ReadByte(p_itf);
        xor  ^= data;

        *(__IO uint8_t *)(p_tmp_buffer) = data;

        p_tmp_buffer++;
      }

      /* Send NACk if Checksum is incorrect */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Write data to memory */
        OPENBL_MEM_Write(p_itf->p_mem_list, address, p_itf->buffer.p_data, data_size);

        /* Send last Acknowledge synchronization byte */
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Start post processing task if needed */
        OPENBL_MEM_PostProcessing(p_itf->p_mem_list, address);
      }
    }
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_Go(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint8_t status;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get memory address */
    if (OPENBL_SPI_GetAddress(p_itf, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      /* Check if received address is valid or not */
      status = OPENBL_MEM_CheckJumpAddress(p_itf->p_mem_list, address);

      if (status == 0U)
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* If the jump address is valid then send ACK */
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        OPENBL_MEM_JumpToAddress(p_itf->p_mem_list, address);
      }
    }
  }
}

/**
  * @brief  This function is used to enable readout protection.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_ReadoutProtect(openbl_itf_obj_t *p_itf)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, OPENBL_ENABLE);

    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Start post processing task if needed */
    OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
  }
}

/**
  * @brief  This function is used to disable readout protection.
  * @param  p_itf Pointer to the interface object.
  * @note   Going from RDP level 1 to RDP level 0 erases all the flash,
  *         so the send of the second acknowledge after disabling the read protection
  *         is not possible, this is why the two ACKs are sent successively.
  * @retval None.
  */
void OPENBL_SPI_ReadoutUnprotect(openbl_itf_obj_t *p_itf)
{
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, OPENBL_DISABLE);

  /* Start post processing task if needed */
  OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
}

/**
  * @brief  This function is used to erase a memory.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_EraseMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t pages_number;
  uint16_t data;
  openbl_status_t error_value;
  uint8_t status = OPENBL_ACK_BYTE;
  uint8_t *p_tmp_buffer;

  p_tmp_buffer = p_itf->buffer.p_data;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Read number of pages to be erased */
    data = OPENBL_ITF_SPI_ReadByte(p_itf);
    data = (uint16_t)(data << 8) | OPENBL_ITF_SPI_ReadByte(p_itf);

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != (uint8_t) xor)
      {
        status = OPENBL_NACK_BYTE;
      }
      else
      {
        if ((data == OPENBL_FLASH_MASS_ERASE)
            || (data == OPENBL_FLASH_BANK1_ERASE)
            || (data == OPENBL_FLASH_BANK2_ERASE))
        {
          p_tmp_buffer[0] = (uint8_t)(data & 0x00FFU);
          p_tmp_buffer[1] = (uint8_t)((data & 0xFF00U) >> 8);

          /* Enable busy state */
          OPENBL_ITF_SPI_EnableBusyState(p_itf);

          error_value = OPENBL_MEM_MassErase(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, p_itf->buffer.p_data, \
                                             OPENBL_SPI_RAM_BUFFER_SIZE);

          /* Disable busy state */
          OPENBL_ITF_SPI_DisableBusyState(p_itf);

          if (error_value == OPENBL_SUCCESS)
          {
            status = OPENBL_ACK_BYTE;
          }
          else
          {
            status = OPENBL_NACK_BYTE;
          }
        }
        else
        {
          /* This sub-command is not supported */
          status = OPENBL_NACK_BYTE;
        }
      }
    }
    else
    {
      /* Check data integrity */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != (uint8_t) xor)
      {
        status = OPENBL_NACK_BYTE;
      }
      else
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        p_tmp_buffer = p_itf->buffer.p_data;

        /* Number of pages to be erased (data + 1) */
        pages_number = (uint32_t)data + 1U;

        *p_tmp_buffer = (uint8_t)(pages_number & 0x00FFU);
        p_tmp_buffer++;

        *p_tmp_buffer = (uint8_t)((pages_number & 0xFF00U) >> 8);
        p_tmp_buffer++;

        /* Checksum Initialization */
        xor = 0U;

        /* Get the pages to be erased */
        for (counter = pages_number; counter != 0U ; counter--)
        {
          /* Receive the MSB byte */
          data  = OPENBL_ITF_SPI_ReadByte(p_itf);
          xor  ^= data;
          data  = (uint16_t)((data & 0x00FFU) << 8);

          /* Receive the LSB byte */
          data |= (uint8_t)(OPENBL_ITF_SPI_ReadByte(p_itf) & 0x00FFU);
          xor  ^= ((uint32_t)data & 0x00FFU);

          /* Only store data that fit in the buffer length */
          if (counter < (OPENBL_SPI_RAM_BUFFER_SIZE / 2U))
          {
            *p_tmp_buffer = (uint8_t)(data & 0x00FFU);
            p_tmp_buffer++;

            *p_tmp_buffer = (uint8_t)((data & 0xFF00U) >> 8);
            p_tmp_buffer++;
          }
        }

        /* Check data integrity */
        if (OPENBL_ITF_SPI_ReadByte(p_itf) != (uint8_t) xor)
        {
          status = OPENBL_NACK_BYTE;
        }
        else
        {
          /* Enable busy state */
          OPENBL_ITF_SPI_EnableBusyState(p_itf);

          error_value = OPENBL_MEM_Erase(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, p_itf->buffer.p_data, \
                                         OPENBL_SPI_RAM_BUFFER_SIZE);

          /* Disable busy state */
          OPENBL_ITF_SPI_DisableBusyState(p_itf);

          /* Errors from memory erase are not managed, always return ACK */
          if (error_value == OPENBL_SUCCESS)
          {
            status = OPENBL_ACK_BYTE;
          }
        }
      }
    }

    OPENBL_ITF_SPI_SendAcknowledge(p_itf, status);
  }
}

/**
  * @brief  This function is used to enable write protect.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_WriteProtect(openbl_itf_obj_t *p_itf)
{
  uint8_t counter;
  uint8_t length;
  uint8_t data;
  uint8_t xor;
  openbl_status_t error_value;
  uint8_t *p_tmp_buffer;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get the data length */
    data = OPENBL_ITF_SPI_ReadByte(p_itf);
    xor  = ~data;

    /* Send NACk if Checksum is incorrect */
    if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      p_tmp_buffer = p_itf->buffer.p_data;
      length       = data + 1U;

      /* Checksum Initialization */
      xor = 0U;

      /* Receive data and write to RAM Buffer */
      for (counter = (length); counter != 0U ; counter--)
      {
        data  = OPENBL_ITF_SPI_ReadByte(p_itf);
        xor  ^= data;

        *(__IO uint8_t *)(p_tmp_buffer) = (uint8_t) data;

        p_tmp_buffer++;
      }

      /* Check data integrity and send NACK if Checksum is incorrect */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        p_tmp_buffer = p_itf->buffer.p_data;

        /* Enable the write protection */
        error_value = OPENBL_MEM_SetWriteProtection(p_itf->p_mem_list, OPENBL_ENABLE, OPENBL_DEFAULT_MEM, \
                                                    p_tmp_buffer, length);

        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        if (error_value == OPENBL_SUCCESS)
        {
          /* Start post processing task if needed */
          OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
        }
      }
    }
  }
}

/**
  * @brief  This function is used to disable write protect.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_WriteUnprotect(openbl_itf_obj_t *p_itf)
{
  openbl_status_t error_value;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(p_itf->p_mem_list, OPENBL_DISABLE, OPENBL_DEFAULT_MEM, NULL, 0U);

    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    if (error_value == OPENBL_SUCCESS)
    {
      /* Start post processing task if needed */
      OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
    }
  }
}

/**
  * @brief  This function is used to parse and execute special commands.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_SPI_SpecialCommand(openbl_itf_obj_t *p_itf)
{
  openbl_special_cmd_t *special_cmd;
  uint16_t op_code;
  uint8_t index;
  uint8_t data;
  uint8_t xor;

  /* Point to the RAM SPI buffer to gain size and reliability */
  special_cmd = (openbl_special_cmd_t *)(uint32_t) p_itf->buffer.p_data;

  /* Send acknowledgment */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Get the command operation code */
  if (OPENBL_SPI_GetSpecialCmdOpCode(p_itf, OPENBL_SPECIAL_CMD, &op_code) == OPENBL_NACK_BYTE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    /* Send Operation code acknowledgment */
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Initialize the special command frame */
    special_cmd->type   = OPENBL_SPECIAL_CMD;
    special_cmd->opcode = op_code;

    /* Initialize the xor variable */
    xor = 0U;

    /* Get the number of bytes to be received */
    /* Read the MSB of the size byte */
    data                        = OPENBL_ITF_SPI_ReadByte(p_itf);
    special_cmd->size_buffer_1  = ((uint16_t)data) << 8;
    xor                        ^= data;

    /* Read the LSB of the size byte */
    data                        = OPENBL_ITF_SPI_ReadByte(p_itf);
    special_cmd->size_buffer_1 |= (uint16_t)data;
    xor                        ^= data;

    if (special_cmd->size_buffer_1 > OPENBL_SPECIAL_CMD_SIZE_BUFFER1)
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      if (special_cmd->size_buffer_1 != 0U)
      {
        /* Read received bytes */
        for (index = 0U; index < special_cmd->size_buffer_1; index++)
        {
          data                          = OPENBL_ITF_SPI_ReadByte(p_itf);
          special_cmd->buffer_1[index]  = data;
          xor                          ^= data;
        }
      }

      /* Check data integrity */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Send received size acknowledgment */
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Process the special command */
        OPENBL_ITF_SPI_SpecialCommandProcess(p_itf, special_cmd);

        /* NOTE: In case of any operation inside "SpecialCommandProcess" function that prevents the code
         * from returning to here (reset operation...), to be compatible with the OpenBL protocol,
         * the user must ensure sending the last ACK in the application side.
         */

        /* Send last acknowledgment */
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
      }
    }
  }
}

/**
  * @brief  This function is used to execute special write commands.
  * @retval None.
  */
void OPENBL_SPI_ExtendedSpecialCommand(openbl_itf_obj_t *p_itf)
{
  openbl_special_cmd_t *special_cmd;
  uint16_t op_code;
  uint16_t index;
  uint8_t xor;
  uint8_t data;

  /* Point to the RAM SPI buffer to gain size and reliability */
  special_cmd = (openbl_special_cmd_t *)(uint32_t) p_itf->buffer.p_data;

  /* Send special write code acknowledgment */
  OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Get the command operation code */
  if (OPENBL_SPI_GetSpecialCmdOpCode(p_itf, OPENBL_EXTENDED_SPECIAL_CMD, &op_code) == OPENBL_NACK_BYTE)
  {
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    /* Send Operation code acknowledgment */
    OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Initialize the special command frame */
    special_cmd->type   = OPENBL_EXTENDED_SPECIAL_CMD;
    special_cmd->opcode = op_code;

    /* Initialize the xor variable */
    xor = 0U;

    /* Get the number of bytes to be received */
    /* Read the MSB of the size byte */
    data                        = OPENBL_ITF_SPI_ReadByte(p_itf);
    special_cmd->size_buffer_1  = ((uint16_t)data) << 8;
    xor                        ^= data;

    /* Read the LSB of the size byte */
    data                        = OPENBL_ITF_SPI_ReadByte(p_itf);
    special_cmd->size_buffer_1 |= (uint16_t)data;
    xor                        ^= data;

    if (special_cmd->size_buffer_1 > OPENBL_SPECIAL_CMD_SIZE_BUFFER1)
    {
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      if (special_cmd->size_buffer_1 != 0U)
      {
        /* Read received bytes */
        for (index = 0U; index < special_cmd->size_buffer_1; index++)
        {
          data                          = OPENBL_ITF_SPI_ReadByte(p_itf);
          special_cmd->buffer_1[index]  = data;
          xor                          ^= data;
        }
      }

      /* Check data integrity */
      if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Send receive size acknowledgment */
        OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Get the number of bytes to be written */
        /* Read the MSB of the size byte */
        xor                         = 0U;
        data                        = OPENBL_ITF_SPI_ReadByte(p_itf);
        special_cmd->size_buffer_2  = ((uint16_t)data) << 8;
        xor                        ^= data;

        /* Read the LSB of the size byte */
        data                        = OPENBL_ITF_SPI_ReadByte(p_itf);
        special_cmd->size_buffer_2 |= (uint16_t)data;
        xor                        ^= data;

        if (special_cmd->size_buffer_2 > OPENBL_SPECIAL_CMD_SIZE_BUFFER2)
        {
          OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
        }
        else
        {
          if (special_cmd->size_buffer_2 != 0U)
          {
            /* Read received bytes */
            for (index = 0U; index < special_cmd->size_buffer_2; index++)
            {
              data                          = OPENBL_ITF_SPI_ReadByte(p_itf);
              special_cmd->buffer_2[index]  = data;
              xor                          ^= data;
            }
          }

          /* Check data integrity */
          if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
          {
            OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
          else
          {
            /* Send receive write size acknowledgment */
            OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

            /* Process the special command */
            OPENBL_ITF_SPI_SpecialCommandProcess(p_itf, special_cmd);

            /* NOTE: In case of any operation inside "SpecialCommandProcess" function that prevents the code
             * from returning to here (reset operation...), to be compatible with the OpenBL protocol,
             * the user must ensure sending the last ACK in the application side.
             */

            /* Send last acknowledgment */
            OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
          }
        }
      }
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to get a valid address.
  * @param  p_itf Pointer to the interface object.
  * @param  p_address Pointer to the variable that will store the received address.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_SPI_GetAddress(openbl_itf_obj_t *p_itf, uint32_t *p_address)
{
  uint8_t data[4] = {0U, 0U, 0U, 0U};
  uint8_t status;
  uint8_t xor;

  data[3] = OPENBL_ITF_SPI_ReadByte(p_itf);
  data[2] = OPENBL_ITF_SPI_ReadByte(p_itf);
  data[1] = OPENBL_ITF_SPI_ReadByte(p_itf);
  data[0] = OPENBL_ITF_SPI_ReadByte(p_itf);

  xor = data[3] ^ data[2] ^ data[1] ^ data[0];

  /* Check the integrity of received data */
  if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
  {
    status = OPENBL_NACK_BYTE;
  }
  else
  {
    *p_address = ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    /* Check if received address is valid or not */
    if (OPENBL_MEM_GetAreaFromAddress(p_itf->p_mem_list, *p_address) == OPENBL_MEM_AREA_ERROR)
    {
      status = OPENBL_NACK_BYTE;
    }
    else
    {
      status = OPENBL_ACK_BYTE;
    }
  }

  return status;
}


/**
  * @brief  This function is used to get the operation code.
  * @param  p_opcode Pointer to the operation code to be returned.
  * @param  cmd_type Type of the command, Special read or special write.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_SPI_GetSpecialCmdOpCode(openbl_itf_obj_t *p_itf, openbl_special_cmd_type_t cmd_type,
                                              uint16_t *p_opcode)
{
  uint8_t tmp_opcode[2];
  uint8_t xor;
  uint8_t status;
  uint16_t index;

  /* Initialize the status variable */
  status = OPENBL_NACK_BYTE;

  /* Get the command OpCode (2 bytes) */
  tmp_opcode[0] = OPENBL_ITF_SPI_ReadByte(p_itf); /* Read the MSB byte */
  tmp_opcode[1] = OPENBL_ITF_SPI_ReadByte(p_itf); /* Read the LSB byte */

  /* Get the checksum */
  xor  = tmp_opcode[0];
  xor ^= tmp_opcode[1];

  if (OPENBL_ITF_SPI_ReadByte(p_itf) != xor)
  {
    status = OPENBL_NACK_BYTE;
  }
  else
  {
    /* Get the operation code */
    *p_opcode = ((uint16_t)tmp_opcode[0] << 8) | (uint16_t)tmp_opcode[1];

    switch (cmd_type)
    {
      case OPENBL_SPECIAL_CMD:
        for (index = 0U; index < p_itf->special_cmd_opcodes_list.number; index += 2U)
        {
          if ((p_itf->special_cmd_opcodes_list.p_opcodes[index] == tmp_opcode[1])          /* Check LSB */
              && (p_itf->special_cmd_opcodes_list.p_opcodes[index + 1U] == tmp_opcode[0])) /* Check MSB */
          {
            status = OPENBL_ACK_BYTE;
            break;
          }
        }
        break;

      case OPENBL_EXTENDED_SPECIAL_CMD:
        for (index = 0U; index < p_itf->ext_special_cmd_opcodes_list.number; index += 2U)
        {
          if ((p_itf->ext_special_cmd_opcodes_list.p_opcodes[index] == tmp_opcode[1])          /* Check LSB */
              && (p_itf->ext_special_cmd_opcodes_list.p_opcodes[index + 1U] == tmp_opcode[0])) /* Check MSB */
          {
            status = OPENBL_ACK_BYTE;
            break;
          }
        }
        break;

      default:
        status = OPENBL_NACK_BYTE;
        break;
    }
  }

  return status;
}
