/**
  ******************************************************************************
  * @file    openbl_i2c_cmd.c
  * @brief   Contains I2C protocol commands
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
#include "openbl_i2c_cmd.h"

#include "openbl_types.h"
#include "openbl_itf_i2c.h"
#include "openbl_itf_common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
/* Private function prototypes -----------------------------------------------*/
static uint8_t OPENBL_I2C_GetAddress(openbl_itf_obj_t *p_itf, openbl_mem_list_t *p_mem_list, uint32_t *p_address);
static uint8_t OPENBL_I2C_GetSpecialCmdOpCode(openbl_itf_obj_t *p_itf, openbl_special_cmd_type_t cmd_type, \
                                              uint16_t *p_opcode);

/* Exported variables --------------------------------------------------------*/
/* Exported functions---------------------------------------------------------*/

/**
  * @brief  This function is used to get a pointer to the structure that contains the available I2C commands.
  * @return Returns a pointer to the OPENBL_I2C_Commands struct.
  */
openbl_commands_t *OPENBL_I2C_GetCommandsList(void)
{
  static openbl_commands_t OPENBL_I2C_Commands =
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
    OPENBL_I2C_NonStretchReadoutUnprotect,
    NULL,
    OPENBL_I2C_SpecialCommand,
    OPENBL_I2C_ExtendedSpecialCommand
  };

  return (&OPENBL_I2C_Commands);
}

/**
  * @brief  This function is used to construct the command opcodes list.
  * @param  p_i2c_cmd Pointer to the structure containing the I2C supported commands.
  * @param  p_cmd_opcodes_list Pointer to the structure containing the list of supported commands by the I2C protocol.
  * @return None.
  */
void OPENBL_I2C_FillCommandsOpcodesList(const openbl_commands_t *p_i2c_cmd, \
                                        openbl_cmd_opcodes_list_t *p_cmd_opcodes_list)
{
  uint8_t i = 0U;

  if ((p_i2c_cmd->GetCommand != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GET_COMMAND;
    i++;
  }

  if ((p_i2c_cmd->GetVersion != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GET_VERSION;
    i++;
  }

  if ((p_i2c_cmd->GetID != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GET_ID;
    i++;
  }

  if ((p_i2c_cmd->ReadMemory != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_READ_MEMORY;
    i++;
  }

  if ((p_i2c_cmd->Go != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_GO;
    i++;
  }

  if ((p_i2c_cmd->WriteMemory != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_WRITE_MEMORY;
    i++;
  }

  if ((p_i2c_cmd->EraseMemory != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_EXT_ERASE_MEMORY;
    i++;
  }

  if ((p_i2c_cmd->WriteProtect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_WRITE_PROTECT;
    i++;
  }

  if ((p_i2c_cmd->WriteUnprotect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_WRITE_UNPROTECT;
    i++;
  }

  if ((p_i2c_cmd->ReadoutProtect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_READ_PROTECT;
    i++;
  }

  if ((p_i2c_cmd->ReadoutUnprotect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_READ_UNPROTECT;
    i++;
  }

  if ((p_i2c_cmd->NsWriteMemory != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_NS_WRITE_MEMORY;
    i++;
  }

  if ((p_i2c_cmd->NsEraseMemory != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_NS_ERASE_MEMORY;
    i++;
  }

  if ((p_i2c_cmd->NsWriteProtect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_NS_WRITE_PROTECT;
    i++;
  }

  if ((p_i2c_cmd->NsWriteUnprotect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_NS_WRITE_UNPROTECT;
    i++;
  }

  if ((p_i2c_cmd->NsReadoutProtect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_NS_READ_PROTECT;
    i++;
  }

  if ((p_i2c_cmd->NsReadoutUnprotect != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_NS_READ_UNPROTECT;
    i++;
  }

  if ((p_i2c_cmd->SpecialCommand != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_SPECIAL_COMMAND;
    i++;
  }

  if ((p_i2c_cmd->ExtendedSpecialCommand != NULL) && (i < (uint8_t)OPENBL_I2C_COMMANDS_MAX_NBR))
  {
    p_cmd_opcodes_list->p_opcodes[i] = OPENBL_CMD_EXT_SPECIAL_COMMAND;
    i++;
  }

  p_cmd_opcodes_list->number = i;
}

/**
  * @brief  This function is used to get the list of the available I2C commands.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_GetCommand(openbl_itf_obj_t *p_itf)
{
  uint32_t counter;

  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Wait for address to match */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  /* Send the number of commands supported by the I2C protocol */
  OPENBL_ITF_I2C_SendByte(p_itf, (uint8_t)p_itf->cmd_opcodes_list.number);

  /* Send I2C protocol version */
  OPENBL_ITF_I2C_SendByte(p_itf, OPENBL_I2C_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < p_itf->cmd_opcodes_list.number; counter++)
  {
    OPENBL_ITF_I2C_SendByte(p_itf, p_itf->cmd_opcodes_list.p_opcodes[counter]);
  }

  /* Wait until NACK is detected */
  OPENBL_ITF_I2C_WaitNack(p_itf);

  /* Wait until STOP is detected */
  OPENBL_ITF_I2C_WaitStop(p_itf);

  /* Send last Acknowledge synchronization byte */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
}

/**
  * @brief  This function is used to get the I2C protocol version.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_GetVersion(openbl_itf_obj_t *p_itf)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Wait for address to match */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  /* Send I2C protocol version */
  OPENBL_ITF_I2C_SendByte(p_itf, OPENBL_I2C_VERSION);

  /* Wait until NACK is detected */
  OPENBL_ITF_I2C_WaitNack(p_itf);

  /* Wait until STOP is detected */
  OPENBL_ITF_I2C_WaitStop(p_itf);

  /* Send last Acknowledge synchronization byte */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_GetID(openbl_itf_obj_t *p_itf)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Wait for address to match */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  OPENBL_ITF_I2C_SendByte(p_itf, 0x01U);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_ITF_I2C_SendByte(p_itf, OPENBL_DEVICE_ID_MSB);
  OPENBL_ITF_I2C_SendByte(p_itf, OPENBL_DEVICE_ID_LSB);

  /* Wait until NACK is detected */
  OPENBL_ITF_I2C_WaitNack(p_itf);

  /* Wait until STOP is detected */
  OPENBL_ITF_I2C_WaitStop(p_itf);

  /* Send last Acknowledge synchronization byte */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
}

/**
  * @brief  This function is used to read memory from the device.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_ReadMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint32_t counter;
  openbl_mem_t *p_mem;
  uint8_t data;
  uint8_t xor;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_I2C_GetAddress(p_itf, p_itf->p_mem_list, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      /* Wait for address to match */
      OPENBL_ITF_I2C_WaitAddress(p_itf);

      /* Get the number of bytes to be received */
      data = OPENBL_ITF_I2C_ReadByte(p_itf);
      xor  = ~data;

      /* Check data integrity */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
      {
        /* Wait until STOP is detected */
        OPENBL_ITF_I2C_WaitStop(p_itf);

        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Wait until STOP is detected */
        OPENBL_ITF_I2C_WaitStop(p_itf);

        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Get the memory index to know from which memory we will read */
        p_mem = OPENBL_MEM_GetMemoryFromAddress(p_itf->p_mem_list, address);

        /* Wait for address to match */
        OPENBL_ITF_I2C_WaitAddress(p_itf);

        /* Read the data (data + 1) from the memory and send them to the host */
        for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
        {
          OPENBL_ITF_I2C_SendByte(p_itf, OPENBL_MEM_Read(p_mem, address));
          address++;
        }

        /* Wait until NACK is detected */
        OPENBL_ITF_I2C_WaitNack(p_itf);

        /* Wait until STOP is detected */
        OPENBL_ITF_I2C_WaitStop(p_itf);
      }
    }
  }
}

/**
  * @brief  This function is used to write in to device memory.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_WriteMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint32_t xor;
  uint32_t counter;
  uint32_t codesize;
  uint8_t *p_tmp_buffer;
  uint8_t data;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_I2C_GetAddress(p_itf, p_itf->p_mem_list, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      /* Read number of bytes to be written and data */
      p_tmp_buffer = p_itf->buffer.p_data;

      OPENBL_ITF_I2C_WaitAddress(p_itf);

      /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
      data = OPENBL_ITF_I2C_ReadByte(p_itf);

      /* Number of data to be written = data + 1 */
      codesize = (uint32_t)data + 1U;

      /* Checksum Initialization */
      xor = data;

      /* I2C receive data and send to RAM Buffer */
      for (counter = codesize; counter != 0U ; counter--)
      {
        data    = OPENBL_ITF_I2C_ReadByte(p_itf);
        xor ^= data;

        *(__IO uint8_t *)(p_tmp_buffer) = data;

        p_tmp_buffer++;
      }

      /* Send NACK if Checksum is incorrect */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Write data to memory */
        OPENBL_MEM_Write(p_itf->p_mem_list, address, p_itf->buffer.p_data, codesize);

        /* Send last Acknowledge synchronization byte */
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

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
void OPENBL_I2C_Go(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint8_t status;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get memory address */
    if (OPENBL_I2C_GetAddress(p_itf, p_itf->p_mem_list, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      /* Check if received address is valid or not */
      status = OPENBL_MEM_CheckJumpAddress(p_itf->p_mem_list, address);

      if (status == 0U)
      {
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* If the jump address is valid then send ACK */
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

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
void OPENBL_I2C_ReadoutProtect(openbl_itf_obj_t *p_itf)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, OPENBL_ENABLE);

    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Start post processing task if needed */
    OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
  }
}

/**
  * @brief  This function is used to disable readout protection.
  * @param  p_itf Pointer to the interface object.
  * @note   Going from RDP level 1 to RDP level 0 erase all the flash,
  *         so the send of second acknowledge after Disabling the read protection
  *         is not possible what make the communication with the host get lost.
  * @retval None.
  */
void OPENBL_I2C_ReadoutUnprotect(openbl_itf_obj_t *p_itf)
{
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

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
void OPENBL_I2C_EraseMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t numpage;
  uint16_t data;
  openbl_status_t error_value;
  uint8_t status = OPENBL_ACK_BYTE;
  uint8_t *p_tmp_buffer;

  p_tmp_buffer = p_itf->buffer.p_data;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Wait for address to match */
    OPENBL_ITF_I2C_WaitAddress(p_itf);

    /* Read number of pages to be erased */
    data = OPENBL_ITF_I2C_ReadByte(p_itf);
    data = (uint16_t)(data << 8) | OPENBL_ITF_I2C_ReadByte(p_itf);

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);

        status = OPENBL_NACK_BYTE;
      }
      else
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);

        if ((data == OPENBL_FLASH_MASS_ERASE)
            || (data == OPENBL_FLASH_BANK1_ERASE)
            || (data == OPENBL_FLASH_BANK2_ERASE))
        {
          p_tmp_buffer[0] = (uint8_t)(data & 0x00FFU);
          p_tmp_buffer[1] = (uint8_t)((data & 0xFF00U) >> 8);

          error_value = OPENBL_MEM_MassErase(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, p_itf->buffer.p_data, \
                                             OPENBL_I2C_RAM_BUFFER_SIZE);

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
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);

        status = OPENBL_NACK_BYTE;
      }
      else
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);

        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        p_tmp_buffer = p_itf->buffer.p_data;

        /* Number of pages to be erased (data + 1) */
        numpage = (uint32_t)data + 1U;

        *p_tmp_buffer = (uint8_t)(numpage & 0x00FFU);
        p_tmp_buffer++;

        *p_tmp_buffer = (uint8_t)((numpage & 0xFF00U) >> 8);
        p_tmp_buffer++;

        OPENBL_ITF_I2C_WaitAddress(p_itf);

        xor = 0;

        /* Get the pages to be erased */
        for (counter = numpage; counter != 0U ; counter--)
        {
          /* Receive the MSB byte */
          data  = OPENBL_ITF_I2C_ReadByte(p_itf);
          xor  ^= data;
          data  = (uint16_t)((data & 0x00FFU) << 8);

          /* Receive the LSB byte */
          data |= (uint8_t)(OPENBL_ITF_I2C_ReadByte(p_itf) & 0x00FFU);
          xor  ^= ((uint32_t)data & 0x00FFU);

          /* Only store data that fit in the buffer length */
          if (counter < (OPENBL_I2C_RAM_BUFFER_SIZE / 2U))
          {
            *p_tmp_buffer = (uint8_t)(data & 0x00FFU);
            p_tmp_buffer++;

            *p_tmp_buffer = (uint8_t)((data & 0xFF00U) >> 8);
            p_tmp_buffer++;
          }
        }

        /* Check data integrity */
        if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
        {
          OPENBL_ITF_I2C_WaitStop(p_itf);

          status = OPENBL_NACK_BYTE;
        }
        else
        {
          OPENBL_ITF_I2C_WaitStop(p_itf);

          error_value = OPENBL_MEM_Erase(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, p_itf->buffer.p_data, \
                                         OPENBL_I2C_RAM_BUFFER_SIZE);

          /* Errors from memory erase are not managed, always return ACK */
          if (error_value == OPENBL_SUCCESS)
          {
            status = OPENBL_ACK_BYTE;
          }
        }
      }
    }

    OPENBL_ITF_I2C_SendAcknowledge(p_itf, status);
  }
}

/**
  * @brief  This function is used to enable write protect.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_WriteProtect(openbl_itf_obj_t *p_itf)
{
  uint16_t counter;
  uint16_t length;
  uint8_t data;
  uint8_t xor;
  openbl_status_t error_value;
  uint8_t *p_tmp_buffer;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Wait for address to match */
    OPENBL_ITF_I2C_WaitAddress(p_itf);

    /* Get the data length */
    data = OPENBL_ITF_I2C_ReadByte(p_itf);
    xor  = ~data;

    /* Send NACk if Checksum is incorrect */
    if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
    {
      OPENBL_ITF_I2C_WaitStop(p_itf);
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_I2C_WaitStop(p_itf);
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      p_tmp_buffer = p_itf->buffer.p_data;
      length       = (uint16_t) data + 1U;

      /* Checksum Initialization */
      xor = 0U;

      /* Wait for address to match */
      OPENBL_ITF_I2C_WaitAddress(p_itf);

      /* Receive data and write to RAM Buffer */
      for (counter = (length); counter != 0U ; counter--)
      {
        data  = OPENBL_ITF_I2C_ReadByte(p_itf);
        xor  ^= data;

        *(__IO uint8_t *)(p_tmp_buffer) = (uint8_t) data;

        p_tmp_buffer++;
      }

      /* Check data integrity and send NACK if Checksum is incorrect */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        p_tmp_buffer = p_itf->buffer.p_data;

        /* Enable the write protection */
        error_value = OPENBL_MEM_SetWriteProtection(p_itf->p_mem_list, OPENBL_ENABLE, OPENBL_DEFAULT_MEM, \
                                                    p_tmp_buffer, length);

        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

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
void OPENBL_I2C_WriteUnprotect(openbl_itf_obj_t *p_itf)
{
  openbl_status_t error_value;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(p_itf->p_mem_list, OPENBL_DISABLE, OPENBL_DEFAULT_MEM, NULL, 0U);

    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    if (error_value == OPENBL_SUCCESS)
    {
      /* Start post processing task if needed */
      OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
    }
  }
}

/**
  * @brief  This function is used to write in to device memory in non stretch mode.
  * @param  p_itf Pointer to the interface object.
  * @note   In this mode, when the write memory operation is executed the device
  *         send busy bytes to the host.
  * @retval None.
  */
void OPENBL_I2C_NonStretchWriteMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t address;
  uint32_t xor;
  uint32_t counter;
  uint32_t codesize;
  uint8_t *p_tmp_buffer;
  uint8_t data;

  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_I2C_GetAddress(p_itf, p_itf->p_mem_list, &address) == OPENBL_NACK_BYTE)
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      /* Read number of bytes to be written and data */
      p_tmp_buffer = p_itf->buffer.p_data;

      OPENBL_ITF_I2C_WaitAddress(p_itf);

      /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
      data = OPENBL_ITF_I2C_ReadByte(p_itf);

      /* Number of data to be written = data + 1 */
      codesize = (uint32_t)data + 1U;

      /* Checksum Initialization */
      xor = data;

      /* I2C receive data and send to RAM Buffer */
      for (counter = codesize; counter != 0U ; counter--)
      {
        data  = OPENBL_ITF_I2C_ReadByte(p_itf);
        xor  ^= data;

        *(__IO uint8_t *)(p_tmp_buffer) = data;

        p_tmp_buffer++;
      }

      /* Send NACk if Checksum is incorrect */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Send Busy Byte */
        OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

        /* Write data to memory */
        OPENBL_MEM_Write(p_itf->p_mem_list, address, p_itf->buffer.p_data, codesize);

        /* Send Busy Byte */
        OPENBL_ITF_I2C_DisableBusyStateSending(p_itf);

        /* Send last Acknowledge synchronization byte */
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Start post processing task if needed */
        OPENBL_MEM_PostProcessing(p_itf->p_mem_list, address);
      }
    }
  }
}

/**
  * @brief  This function is used to erase a memory in non stretch mode.
  * @param  p_itf Pointer to the interface object.
  * @note   In this mode, when the erase memory operation is executed the device
  *         send busy bytes to the host.
  * @retval None.
  */
void OPENBL_I2C_NonStretchEraseMemory(openbl_itf_obj_t *p_itf)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t numpage;
  uint16_t data;
  openbl_status_t error_value;
  uint8_t status = OPENBL_ACK_BYTE;
  uint8_t *p_tmp_buffer;

  p_tmp_buffer = p_itf->buffer.p_data;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Wait for address to match */
    OPENBL_ITF_I2C_WaitAddress(p_itf);

    /* Read number of pages to be erased */
    data = OPENBL_ITF_I2C_ReadByte(p_itf);
    data = (uint16_t)(data << 8) | OPENBL_ITF_I2C_ReadByte(p_itf);

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);
        status = OPENBL_NACK_BYTE;
      }
      else
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);

        /* Send Busy Byte */
        OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

        if ((data == OPENBL_FLASH_MASS_ERASE)
            || (data == OPENBL_FLASH_BANK1_ERASE)
            || (data == OPENBL_FLASH_BANK2_ERASE))
        {
          p_tmp_buffer[0] = (uint8_t)(data & 0x00FFU);
          p_tmp_buffer[1] = (uint8_t)((data & 0xFF00U) >> 8);

          error_value = OPENBL_MEM_MassErase(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, p_itf->buffer.p_data, \
                                             OPENBL_I2C_RAM_BUFFER_SIZE);

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
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);
        status = OPENBL_NACK_BYTE;
      }
      else
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        p_tmp_buffer = p_itf->buffer.p_data;

        /* Number of pages to be erased (data + 1) */
        numpage = (uint32_t)data + 1U;

        *p_tmp_buffer = (uint8_t)(numpage & 0x00FFU);
        p_tmp_buffer++;

        *p_tmp_buffer = (uint8_t)((numpage & 0xFF00U) >> 8);
        p_tmp_buffer++;

        /* Wait for address to match */
        OPENBL_ITF_I2C_WaitAddress(p_itf);

        /* Checksum Initialization */
        xor = 0U;

        /* Get the pages to be erased */
        for (counter = numpage; counter != 0U ; counter--)
        {
          /* Receive the MSB byte */
          data  = OPENBL_ITF_I2C_ReadByte(p_itf);
          xor  ^= data;
          data  = (uint16_t)((data & 0x00FFU) << 8);

          /* Receive the LSB byte */
          data |= (uint8_t)(OPENBL_ITF_I2C_ReadByte(p_itf) & 0x00FFU);
          xor  ^= ((uint32_t)data & 0x00FFU);

          /* Only store data that fit in the buffer length */
          if (counter < (OPENBL_I2C_RAM_BUFFER_SIZE / 2U))
          {
            *p_tmp_buffer = (uint8_t)(data & 0x00FFU);
            p_tmp_buffer++;

            *p_tmp_buffer = (uint8_t)((data & 0xFF00U) >> 8);
            p_tmp_buffer++;
          }
        }

        /* Check data integrity */
        if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
        {
          OPENBL_ITF_I2C_WaitStop(p_itf);
          status = OPENBL_NACK_BYTE;
        }
        else
        {
          OPENBL_ITF_I2C_WaitStop(p_itf);

          /* Send Busy Byte */
          OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

          error_value = OPENBL_MEM_Erase(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, p_itf->buffer.p_data, \
                                         OPENBL_I2C_RAM_BUFFER_SIZE);

          /* Disable Busy Byte */
          OPENBL_ITF_I2C_DisableBusyStateSending(p_itf);

          /* Errors from memory erase are not managed, always return ACK */
          if (error_value == OPENBL_SUCCESS)
          {
            status = OPENBL_ACK_BYTE;
          }
        }
      }
    }

    OPENBL_ITF_I2C_SendAcknowledge(p_itf, status);
  }
}

/**
  * @brief  This function is used to enable write protect in non stretch mode.
  * @param  p_itf Pointer to the interface object.
  * @note   In this mode, when enabling the write protection the device
  *         send busy bytes to the host.
  * @retval None.
  */
void OPENBL_I2C_NonStretchWriteProtect(openbl_itf_obj_t *p_itf)
{
  uint16_t counter;
  uint16_t length;
  uint8_t data;
  uint8_t xor;
  openbl_status_t error_value;
  uint8_t *p_tmp_buffer;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Wait for address to match */
    OPENBL_ITF_I2C_WaitAddress(p_itf);

    /* Get the data length */
    data = OPENBL_ITF_I2C_ReadByte(p_itf);
    xor  = ~data;

    /* Send NACk if Checksum is incorrect */
    if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
    {
      OPENBL_ITF_I2C_WaitStop(p_itf);
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      OPENBL_ITF_I2C_WaitStop(p_itf);
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

      p_tmp_buffer = p_itf->buffer.p_data;
      length       = (uint16_t) data + 1U;

      /* Checksum Initialization */
      xor = 0U;

      /* Wait for address to match */
      OPENBL_ITF_I2C_WaitAddress(p_itf);

      /* Receive data and write to RAM Buffer */
      for (counter = (length); counter != 0U ; counter--)
      {
        data  = OPENBL_ITF_I2C_ReadByte(p_itf);
        xor  ^= data;

        *(__IO uint8_t *)(p_tmp_buffer) = (uint8_t) data;

        p_tmp_buffer++;
      }

      /* Check data integrity and send NACK if Checksum is incorrect */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != (uint8_t) xor)
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        OPENBL_ITF_I2C_WaitStop(p_itf);
        p_tmp_buffer = p_itf->buffer.p_data;

        /* Send Busy Byte */
        OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

        /* Enable the write protection */
        error_value = OPENBL_MEM_SetWriteProtection(p_itf->p_mem_list, OPENBL_ENABLE, OPENBL_DEFAULT_MEM, \
                                                    p_tmp_buffer, length);

        /* Disable Busy Byte */
        OPENBL_ITF_I2C_DisableBusyStateSending(p_itf);

        if (error_value == OPENBL_SUCCESS)
        {
          OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

          /* Start post processing task if needed */
          OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
        }
      }
    }
  }
}

/**
  * @brief  This function is used to disable write protect in non stretch mode.
  * @param  p_itf Pointer to the interface object.
  * @note   In this mode, when disabling the write protection the device
  *         send busy bytes to the host.
  * @retval None.
  */
void OPENBL_I2C_NonStretchWriteUnprotect(openbl_itf_obj_t *p_itf)
{
  openbl_status_t error_value;

  /* Check if the memory is not protected */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Send Busy Byte */
    OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(p_itf->p_mem_list, OPENBL_DISABLE, OPENBL_DEFAULT_MEM, NULL, 0U);

    /* Disable Busy Byte */
    OPENBL_ITF_I2C_DisableBusyStateSending(p_itf);

    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    if (error_value == OPENBL_SUCCESS)
    {
      /* Start post processing task if needed */
      OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
    }
  }
}

/**
  * @brief  This function is used to enable readout protection in non stretch mode.
  * @param  p_itf Pointer to the interface object.
  * @note   In this mode, when enabling the readout protection the device
  *         send busy bytes to the host.
  * @retval None.
  */
void OPENBL_I2C_NonStretchReadoutProtect(openbl_itf_obj_t *p_itf)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_Common_GetProtectionStatus() != OPENBL_DISABLE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Send Busy Byte */
    OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, OPENBL_ENABLE);

    /* Disable Busy Byte */
    OPENBL_ITF_I2C_DisableBusyStateSending(p_itf);

    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Start post processing task if needed */
    OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
  }
}

/**
  * @brief  This function is used to disable readout protection in non stretch mode.
  * @param  p_itf Pointer to the interface object.
  * @note   In this mode, when disabling the readout protection the device
  *         send busy bytes to the host.
  *         going from RDP level 1 to RDP level 0 erase all the flash,
  *         so the send of second acknowledge after Disabling the read protection
  *         is not possible what make the communication with the host get lost.
  * @retval None.
  */
void OPENBL_I2C_NonStretchReadoutUnprotect(openbl_itf_obj_t *p_itf)
{
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Send Busy Byte */
  OPENBL_ITF_I2C_EnableBusyStateSending(p_itf);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(p_itf->p_mem_list, OPENBL_DEFAULT_MEM, OPENBL_DISABLE);

  /* Disable Busy Byte */
  OPENBL_ITF_I2C_DisableBusyStateSending(p_itf);

  /* Start post processing task if needed */
  OPENBL_MEM_PostProcessing(p_itf->p_mem_list, OPENBL_DEFAULT_MEM);
}

/**
  * @brief  This function is used to execute special command commands.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_SpecialCommand(openbl_itf_obj_t *p_itf)
{
  openbl_special_cmd_t *special_cmd;
  uint16_t op_code;
  uint8_t xor;
  uint8_t index;
  uint8_t data;

  /* Point to the RAM I2C buffer to gain size and reliability */
  special_cmd = (openbl_special_cmd_t *)(uint32_t) p_itf->buffer.p_data;

  /* Send Operation code acknowledgment */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Wait for address to match */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  /* Get the command operation code */
  if (OPENBL_I2C_GetSpecialCmdOpCode(p_itf, OPENBL_SPECIAL_CMD, &op_code) == OPENBL_NACK_BYTE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    /* Send Operation code acknowledgment */
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Wait for address to match */
    OPENBL_ITF_I2C_WaitAddress(p_itf);

    /* Initialize the special command frame */
    special_cmd->type   = OPENBL_SPECIAL_CMD;
    special_cmd->opcode = op_code;

    /* Initialize the xor variable */
    xor = 0U;

    /* Get the number of bytes to be received */
    /* Read the MSB of the size byte */
    data                        = OPENBL_ITF_I2C_ReadByte(p_itf);
    special_cmd->size_buffer_1  = ((uint16_t)data) << 8;
    xor                        ^= data;

    /* Read the LSB of the size byte */
    data                        = OPENBL_ITF_I2C_ReadByte(p_itf);
    special_cmd->size_buffer_1 |= (uint16_t)data;
    xor                        ^= data;

    if (special_cmd->size_buffer_1 > OPENBL_SPECIAL_CMD_SIZE_BUFFER1)
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      if (special_cmd->size_buffer_1 != 0U)
      {
        /* Read received bytes */
        for (index = 0U; index < special_cmd->size_buffer_1; index++)
        {
          data                          = OPENBL_ITF_I2C_ReadByte(p_itf);
          special_cmd->buffer_1[index]  = data;
          xor                          ^= data;
        }
      }

      /* Check data integrity */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Send received size acknowledgment */
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Wait for address to match */
        OPENBL_ITF_I2C_WaitAddress(p_itf);

        /* Process the special command */
        OPENBL_ITF_I2C_SpecialCommandProcess(p_itf, special_cmd);

        /* NOTE: In case of any operation inside "SpecialCommandProcess" function that prevents the code
         * from returning to here (reset operation...), to be compatible with the OpenBL protocol,
         * the user must ensure sending the last ACK in the application side.
         */

        /* Send acknowledgment */
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Wait until NACK is detected */
        OPENBL_ITF_I2C_WaitNack(p_itf);

        /* Wait until STOP is detected */
        OPENBL_ITF_I2C_WaitStop(p_itf);
      }
    }
  }
}

/**
  * @brief  This function is used to execute extended special command commands.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_I2C_ExtendedSpecialCommand(openbl_itf_obj_t *p_itf)
{
  openbl_special_cmd_t *special_cmd;
  uint16_t op_code;
  uint16_t index;
  uint8_t xor;
  uint8_t data;

  /* Point to the RAM I2C buffer to gain size and reliability */
  special_cmd = (openbl_special_cmd_t *)(uint32_t) p_itf->buffer.p_data;

  /* Send Operation code acknowledgment */
  OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

  /* Wait for address to match */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  /* Get the command operation code */
  if (OPENBL_I2C_GetSpecialCmdOpCode(p_itf, OPENBL_EXTENDED_SPECIAL_CMD, &op_code) == OPENBL_NACK_BYTE)
  {
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
  }
  else
  {
    /* Send Operation code acknowledgment */
    OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

    /* Wait for address to match */
    OPENBL_ITF_I2C_WaitAddress(p_itf);

    /* Initialize the special command frame */
    special_cmd->type   = OPENBL_EXTENDED_SPECIAL_CMD;
    special_cmd->opcode = op_code;

    /* Initialize the xor variable */
    xor = 0U;

    /* Get the number of bytes to be received */
    /* Read the MSB of the size byte */
    data                        = OPENBL_ITF_I2C_ReadByte(p_itf);
    special_cmd->size_buffer_1  = ((uint16_t)data) << 8;
    xor                        ^= data;

    /* Read the LSB of the size byte */
    data                        = OPENBL_ITF_I2C_ReadByte(p_itf);
    special_cmd->size_buffer_1 |= (uint16_t)data;
    xor                        ^= data;

    if (special_cmd->size_buffer_1 > OPENBL_SPECIAL_CMD_SIZE_BUFFER1)
    {
      OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
    }
    else
    {
      if (special_cmd->size_buffer_1 != 0U)
      {
        /* Read received bytes */
        for (index = 0U; index < special_cmd->size_buffer_1; index++)
        {
          data                          = OPENBL_ITF_I2C_ReadByte(p_itf);
          special_cmd->buffer_1[index]  = data;
          xor                          ^= data;
        }
      }

      /* Check data integrity */
      if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
      {
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
      }
      else
      {
        /* Send receive size acknowledgment */
        OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

        /* Wait for address to match */
        OPENBL_ITF_I2C_WaitAddress(p_itf);

        /* Get the number of bytes to be written */
        /* Read the MSB of the size byte */
        xor                         = 0U;
        data                        = OPENBL_ITF_I2C_ReadByte(p_itf);
        special_cmd->size_buffer_2  = ((uint16_t)data) << 8;
        xor                        ^= data;

        /* Read the LSB of the size byte */
        data                        = OPENBL_ITF_I2C_ReadByte(p_itf);
        special_cmd->size_buffer_2 |= (uint16_t)data;
        xor                        ^= data;

        if (special_cmd->size_buffer_2 > OPENBL_SPECIAL_CMD_SIZE_BUFFER2)
        {
          OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
        }
        else
        {
          if (special_cmd->size_buffer_2 != 0U)
          {
            /* Read received bytes */
            for (index = 0U; index < special_cmd->size_buffer_2; index++)
            {
              data                         = OPENBL_ITF_I2C_ReadByte(p_itf);
              special_cmd->buffer_2[index] = data;
              xor                         ^= data;
            }
          }

          /* Check data integrity */
          if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
          {
            OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
          else
          {
            /* Send receive write size acknowledgment */
            OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

            /* Wait for address to match */
            OPENBL_ITF_I2C_WaitAddress(p_itf);

            /* Process the special command */
            OPENBL_ITF_I2C_SpecialCommandProcess(p_itf, special_cmd);

            /* NOTE: In case of any operation inside "SpecialCommandProcess" function that prevents the code
             * from returning to here (reset operation...), to be compatible with the OpenBL protocol,
             * the user must ensure sending the last ACK in the application side.
             */

            /* Send acknowledgment */
            OPENBL_ITF_I2C_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);

            /* Wait until NACK is detected */
            OPENBL_ITF_I2C_WaitNack(p_itf);

            /* Wait until STOP is detected */
            OPENBL_ITF_I2C_WaitStop(p_itf);
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
  * @param  p_mem_list Pointer to the memory list.
  * @param  p_address Pointer to the variable that will contain the valid address.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_I2C_GetAddress(openbl_itf_obj_t *p_itf, openbl_mem_list_t *p_mem_list, uint32_t *p_address)
{
  uint8_t data[4] = {0U, 0U, 0U, 0U};
  uint8_t status;
  uint8_t xor;

  /* Wait for address to match */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  data[3] = OPENBL_ITF_I2C_ReadByte(p_itf);
  data[2] = OPENBL_ITF_I2C_ReadByte(p_itf);
  data[1] = OPENBL_ITF_I2C_ReadByte(p_itf);
  data[0] = OPENBL_ITF_I2C_ReadByte(p_itf);

  xor = data[3] ^ data[2] ^ data[1] ^ data[0];

  /* Check the integrity of received data */
  if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
  {
    status = OPENBL_NACK_BYTE;
  }
  else
  {
    /* Wait until STOP is detected */
    OPENBL_ITF_I2C_WaitStop(p_itf);

    *p_address = ((uint32_t)data[3] << 24) | ((uint32_t)data[2] << 16) | ((uint32_t)data[1] << 8) | (uint32_t)data[0];

    /* Check if received address is valid or not */
    if (OPENBL_MEM_GetAreaFromAddress(p_mem_list, *p_address) == OPENBL_MEM_AREA_ERROR)
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
  * @brief  This function is used to get the special command operation opcode.
  * @param  p_itf Pointer to the interface object.
  * @param  cmd_type Type of the command, special command or extended special command.
  * @param  p_opcode Pointer to the operation code to be returned.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_I2C_GetSpecialCmdOpCode(openbl_itf_obj_t *p_itf, openbl_special_cmd_type_t cmd_type, \
                                              uint16_t *p_opcode)
{
  uint8_t tmp_opcode[2];
  uint8_t xor;
  uint8_t status;
  uint16_t index;

  /* Initialize the status variable */
  status = OPENBL_NACK_BYTE;

  /* Get the command OpCode (2 bytes) */
  tmp_opcode[0] = OPENBL_ITF_I2C_ReadByte(p_itf); /* Read the MSB byte */
  tmp_opcode[1] = OPENBL_ITF_I2C_ReadByte(p_itf); /* Read the LSB byte */

  /* Get the checksum */
  xor  = tmp_opcode[0];
  xor ^= tmp_opcode[1];

  if (OPENBL_ITF_I2C_ReadByte(p_itf) != xor)
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
