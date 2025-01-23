/**
  ******************************************************************************
  * @file    openbl_usart_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USART protocol commands
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
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
#include "openbl_usart_cmd.h"

#include "openbootloader_conf.h"
#include "app_openbootloader.h"
#include "usart_interface.h"
#include "common_interface.h"
#include "external_memory_interface.h"

#include "openbl_util.h"
#include "otp_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPENBL_USART_COMMANDS_NB_MAX      15U       /* The maximum number of supported commands */

#define USART_RAM_BUFFER_SIZE             1164U     /* Size of USART buffer used to store received data from the host */

#define OPENBL_USART_PACKET_SIZE          256U      /* Size of USART Packet send by the host */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t USART_RAM_Buf[USART_RAM_BUFFER_SIZE];    /* Buffer used to store received data from the host */
static uint8_t a_OPENBL_USART_CommandsList[OPENBL_USART_COMMANDS_NB_MAX] = {0U};
static uint8_t UsartCommandsNumber = 0U;
static uint8_t OperationType       = PHASE_FLASHLAYOUT;
static uint32_t DestinationAddress = RAM_WRITE_ADDRESS;
static uint32_t PacketNum          = 0U;
static uint32_t OtpReadIndex       = 0U;
static Otp_Partition_t Otp;

/* Private function prototypes -----------------------------------------------*/
static uint8_t OPENBL_USART_GetAddress(uint32_t *Address);
static uint8_t OPENBL_USART_GetSpecialCmdOpCode(uint16_t *OpCode, OPENBL_SpecialCmdTypeTypeDef CmdType);
static uint8_t OPENBL_USART_ConstructCommandsTable(const OPENBL_CommandsTypeDef *pUsartCmd);

/* External variables --------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions---------------------------------------------------------*/

/**
  * @brief  This function is used to get a pointer to the structure that contains the available USART commands.
  * @return Returns a pointer to the OPENBL_USART_Commands struct.
  */
OPENBL_CommandsTypeDef *OPENBL_USART_GetCommandsList(void)
{
  static OPENBL_CommandsTypeDef OPENBL_USART_Commands =
  {
    OPENBL_USART_GetCommand,
    OPENBL_USART_GetVersion,
    OPENBL_USART_GetID,
    OPENBL_USART_GetPhase,
    OPENBL_USART_ReadMemory,
    OPENBL_USART_ReadPartition,
    OPENBL_USART_Download,
    OPENBL_USART_Start,
    OPENBL_USART_ReadoutProtect,
    OPENBL_USART_ReadoutUnprotect,
    OPENBL_USART_EraseMemory,
    OPENBL_USART_WriteProtect,
    OPENBL_USART_WriteUnprotect,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    OPENBL_USART_SpecialCommand,
    OPENBL_USART_ExtendedSpecialCommand
  };

  OPENBL_USART_SetCommandsList(&OPENBL_USART_Commands);

  return (&OPENBL_USART_Commands);
}

/**
  * @brief  This function is used to set the list of USART supported commands.
  * @return None.
  */
void OPENBL_USART_SetCommandsList(OPENBL_CommandsTypeDef *pUsartCmd)
{
  UsartCommandsNumber = OPENBL_USART_ConstructCommandsTable(pUsartCmd);
}

/**
  * @brief  This function is used to get the list of the available USART commands
  * @retval None.
  */
void OPENBL_USART_GetCommand(void)
{
  uint32_t counter;

  /* Reset OTP read index counter */
  OtpReadIndex = 0U;

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send the number of commands supported by the USART protocol */
  OPENBL_USART_SendByte(UsartCommandsNumber);

  /* Send USART protocol version */
  OPENBL_USART_SendByte(OPENBL_USART_VERSION);

  /* Send the list of supported commands */
  for (counter = 0U; counter < UsartCommandsNumber; counter++)
  {
    OPENBL_USART_SendByte(a_OPENBL_USART_CommandsList[counter]);
  }

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the USART protocol version.
  * @retval None.
  */
void OPENBL_USART_GetVersion(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Send USART protocol version */
  OPENBL_USART_SendByte(OPENBL_USART_VERSION);

  OPENBL_USART_SendByte(0x00U);
  OPENBL_USART_SendByte(0x00U);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the device ID.
  * @retval None.
  */
void OPENBL_USART_GetID(void)
{
  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  OPENBL_USART_SendByte(0x01U);

  /* Send the device ID starting by the MSB byte then the LSB byte */
  OPENBL_USART_SendByte(DEVICE_ID_MSB);
  OPENBL_USART_SendByte(DEVICE_ID_LSB);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to get the phase ID.
  * @retval None.
  */
void OPENBL_USART_GetPhase(void)
{
  static uint32_t partition_num = 1U;
  static uint8_t phase_id       = PHASE_FLASHLAYOUT;
  uint8_t i;
  uint8_t temp_phase_id;

  /* Save the current phase ID value */
  temp_phase_id = phase_id;

  /* First phase is reserved for flash layout phase */
  if (phase_id == PHASE_FLASHLAYOUT)
  {
    phase_id            = PHASE_3;
    DestinationAddress = FLASHLAYOUT_ADDRESS;
  }
  else if (phase_id == PHASE_3)
  {
    phase_id            = PHASE_4;
    DestinationAddress = FLASH_LOADER_WRITE_ADDRESS;
  }
  /* Flash in External memory phase */
  else if ((phase_id == PHASE_4) || (phase_id == PHASE_5))
  {
    /* Check if the current Phase ID is present in the flash layout structure */
    for (i = 0U; i < FlashlayoutStruct.partsize; i++)
    {
      if (FlashlayoutStruct.id[i] == phase_id)
      {
        break;
      }
    }

    if (i == FlashlayoutStruct.partsize)
    {
      /* Phase ID was not found in the flash layout structure */
      DestinationAddress = UNDEF_ADDRESS;
    }
    else
    {
      /* Compute the destination address by using the offset provided by the user in the flash layout */
      DestinationAddress = EXT_MEMORY_START_ADDRESS + FlashlayoutStruct.offset[i];
    }

    if (phase_id == PHASE_4)
    {
      /* Init external memory */
      if (OPENBL_MEM_Init(DestinationAddress) != 1U)
      {
        /* In case of error during external memory initialization send a NACK to the host */
        OPENBL_USART_SendByte(NACK_BYTE);

        return;
      }

      i++;

      /* Check if the current phase the the last one in the flash layout structure or not */
      if (i == FlashlayoutStruct.partsize)
      {
        /* Last phase */
        phase_id = PHASE_END;
      }
      else
      {
        /* The is still the phase 5 to be executed */
        phase_id = PHASE_5;
      }
    }
    else
    {
      phase_id = PHASE_END;
    }
  }
  else if (phase_id == PHASE_END)
  {
    /* Nothing to do */
  }
  else /* Other phase and after flash layout parsing */
  {
    /* Check if there is available partition */
    if (partition_num < FlashlayoutStruct.partsize)
    {
      /* Get the current partition phase ID */
      phase_id = (uint8_t)FlashlayoutStruct.id[partition_num];

      /* Get the destination address based on current partition */
      if (strcmp(FlashlayoutStruct.ip[partition_num], "none") == 0)
      {
        /* If partition is "none" destination is RAM address */
        DestinationAddress = RAM_WRITE_ADDRESS;
      }
      else if ((strcmp(FlashlayoutStruct.ip[partition_num], "nor") == 0)
               || (strcmp(FlashlayoutStruct.ip[partition_num], "mmc") == 0))
      {
        /* If partition ID is "nor" destination is external memory address (QSPI NOR) */
        DestinationAddress = FlashlayoutStruct.offset[partition_num];
      }
      else /* Only none, nor are supported */
      {
        OPENBL_USART_SendByte(NACK_BYTE);
      }

      /* Go to the next partition */
      partition_num++;

      /* Init the external memories */
      if (OPENBL_MEM_Init(DestinationAddress) != 1U)
      {
        OPENBL_USART_SendByte(NACK_BYTE);

        return;
      }
    }
    else
    {
      /* End operation after this phase */
      phase_id = PHASE_END;
    }
  }

  /* Send Acknowledge byte to notify the host that the command is recognized */
  OPENBL_USART_SendByte(ACK_BYTE);

  OPENBL_USART_SendByte(6U);                          /* Data length */

  OPENBL_USART_SendByte(temp_phase_id & 0xFFU);       /* Partition ID */

  OPENBL_USART_SendByte((uint8_t)DestinationAddress);            /* Byte 1 of address */
  OPENBL_USART_SendByte((uint8_t)(DestinationAddress >> 8U));    /* Byte 2 of address */
  OPENBL_USART_SendByte((uint8_t)(DestinationAddress >> 16U));   /* Byte 3 of address */
  OPENBL_USART_SendByte((uint8_t)(DestinationAddress >> 24U));   /* Byte 4 of address */

  OPENBL_USART_SendByte(1U);                          /* Data length (always 1 in current implementation) */

  OPENBL_USART_SendByte(0U);

  /* Send last Acknowledge synchronization byte */
  OPENBL_USART_SendByte(ACK_BYTE);
}

/**
  * @brief  This function is used to read memory from the device.
  * @retval None.
  */
void OPENBL_USART_ReadMemory(void)
{
  uint32_t address;
  uint32_t counter;
  uint32_t memory_index;
  uint8_t data;
  uint8_t xor;

  /* Check memory protection then send adequate response */
  if (Common_GetProtectionStatus() != RESET)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Get the memory address */
    if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      OPENBL_USART_SendByte(ACK_BYTE);

      /* Get the number of bytes to be received */
      data = OPENBL_USART_ReadByte();
      xor  = ~data;

      /* Check data integrity */
      if (OPENBL_USART_ReadByte() != xor)
      {
        OPENBL_USART_SendByte(NACK_BYTE);
      }
      else
      {
        OPENBL_USART_SendByte(ACK_BYTE);

        /* Get the memory index to know from which memory we will read */
        memory_index = OPENBL_MEM_GetMemoryIndex(address);

        /* Read the data (data + 1) from the memory and send them to the host */
        for (counter = ((uint32_t)data + 1U); counter != 0U; counter--)
        {
          OPENBL_USART_SendByte(OPENBL_MEM_Read(address, memory_index));

          address++;
        }
      }
    }
  }
}

/**
  * @brief  This function is used to write in to device memory.
  * @retval None.
  */
void OPENBL_USART_Download(void)
{
  static bool is_flash_layout    = true;
  static uint32_t current_sector = 0U;
  static uint32_t last_sector    = 0U;
  static uint32_t otp_write_index  = 0U;
  uint32_t offset                = 0U;
  uint32_t address;
  uint32_t temp_xor;
  uint32_t counter;
  uint32_t codesize;
  uint32_t error;
  uint8_t *ramaddress;
  uint8_t data;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Read number of bytes to be written and data */
    ramaddress = (uint8_t *)USART_RAM_Buf;

    /* Read the number of bytes to be written: Max number of data = data + 1 = 256 */
    data = OPENBL_USART_ReadByte();

    /* Number of data to be written = data + 1 */
    codesize = (uint32_t)data + 1U;

    /* Checksum Initialization */
    temp_xor = data;

    /* UART receive data and send to RAM Buffer */
    for (counter = codesize; counter != 0U ; counter--)
    {
      data      = OPENBL_USART_ReadByte();
      temp_xor ^= data;

      *(__IO uint8_t *)(ramaddress) = data;

      ramaddress++;
    }

    /* Send NACK if Checksum is incorrect */
    if (OPENBL_USART_ReadByte() != temp_xor)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      /* Check if write operation is in OTP */
      if (OperationType == PHASE_OTP)
      {
        /* If first OTP packet */
        if (PacketNum == 0U)
        {
          /* Check OTP version */
          Otp.Version = ((uint32_t)USART_RAM_Buf[0] << 0U)
                        | ((uint32_t)USART_RAM_Buf[1] << 8U)
                        | ((uint32_t)USART_RAM_Buf[2] << 16U)
                        | ((uint32_t)USART_RAM_Buf[3] << 24U);

          /* NACK if not expected version */
          if (Otp.Version != OPENBL_OTP_VERSION)
          {
            OPENBL_USART_SendByte(NACK_BYTE);
          }

          /* Get global state */
          Otp.GlobalState = ((uint32_t)USART_RAM_Buf[4] << 0U)
                            | ((uint32_t)USART_RAM_Buf[5] << 8U)
                            | ((uint32_t)USART_RAM_Buf[6] << 16U)
                            | ((uint32_t)USART_RAM_Buf[7] << 24U);

          /* Set offset to 8, bytes number for version and global state */
          offset = 8U;

          /* Reset OTP write index */
          otp_write_index = 0U;
        }

        /* Get OTP word and status */
        for (counter = 0U + offset; (counter < codesize) && (otp_write_index < OTP_PART_SIZE); counter += 4U)
        {
          Otp.OtpPart[otp_write_index] = ((uint32_t)USART_RAM_Buf[counter] << 0U)
                                         | ((uint32_t)USART_RAM_Buf[counter + 1U] << 8U)
                                         | ((uint32_t)USART_RAM_Buf[counter + 2U] << 16U)
                                         | ((uint32_t)USART_RAM_Buf[counter + 3U] << 24U);

          otp_write_index++;
        }

        /* Write OTP since OTP structure is full */
        if (otp_write_index == OTP_PART_SIZE)
        {
          (void) OPENBL_OTP_Write(Otp);

          /* Reset OTP index */
          otp_write_index = 0U;
          OtpReadIndex    = 0U;
        }
      }
      else /* If normal download operation */
      {
        /* Check if write address is in external memory */
        if ((address >= EXT_MEMORY_START_ADDRESS) && (address <= EXT_MEMORY_END_ADDRESS))
        {
          current_sector = ((address - EXT_MEMORY_START_ADDRESS) / EXT_MEMORY_SECTOR_SIZE) + 1U;

          if (current_sector > last_sector)
          {
            /* Erase sector before writing on it */
            OPENBL_MEM_SectorErase(address, address, (address + codesize));

            last_sector = current_sector;
          }
        }

        /* Write data to memory */
        OPENBL_MEM_Write(address, (uint8_t *)USART_RAM_Buf, codesize);

        /* First write memory operation is reserved for the flash layout */
        if (is_flash_layout)
        {
          /* First packet number of flash layout download is reserved for ST binary signature */
          if (PacketNum == 0U)
          {
            /* Skip ST binary signature */
          }
          else
          {
            /* Parse the flash layout */
            if (OPENBL_FlashLayout_Parse_Layout(address, codesize) == PARSE_ERROR)
            {
              OPENBL_USART_SendByte(NACK_BYTE);
            }

            /* Leave the flash layout parsing */
            is_flash_layout = false;
          }
        }

        /* If External memory download, verify data write to memory */
        if ((address >= EXT_MEMORY_START_ADDRESS) && (address <= EXT_MEMORY_END_ADDRESS))
        {
          /* Verify data write to memory */
          error = (uint32_t) OPENBL_MEM_Verify(address, (uint32_t)USART_RAM_Buf, codesize, 0U);

          if ((error != 0U) && (error < (address + codesize)))
          {
            /* Return a NACK if a verification error is detected */
            OPENBL_USART_SendByte(NACK_BYTE);
          }
        }
      }

      /* Send last Acknowledge synchronization byte */
      OPENBL_USART_SendByte(ACK_BYTE);
    }
  }
}

/**
  * @brief  This function is used to read a partition.
  * @retval None.
  */
void OPENBL_USART_ReadPartition(void)
{
  uint32_t offset;
  uint32_t i;
  uint32_t temp_xor;
  uint32_t codesize;
  uint8_t data;
  uint8_t tmpOffset[4] = {0U, 0U, 0U, 0U};
  uint8_t partition_id;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get partition ID byte */
  partition_id = OPENBL_USART_ReadByte();

  /* Get offset address bytes */
  tmpOffset[3] = OPENBL_USART_ReadByte();
  tmpOffset[2] = OPENBL_USART_ReadByte();
  tmpOffset[1] = OPENBL_USART_ReadByte();
  tmpOffset[0] = OPENBL_USART_ReadByte();

  temp_xor = (uint32_t)tmpOffset[3]   \
             ^ (uint32_t)tmpOffset[2] \
             ^ (uint32_t)tmpOffset[1] \
             ^ (uint32_t)tmpOffset[0] \
             ^ (uint32_t)partition_id;

  /* Check the integrity of received data */
  if (OPENBL_USART_ReadByte() != temp_xor)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    /* Get offset value */
    offset = (((uint32_t)tmpOffset[3] << 24U)
              | ((uint32_t)tmpOffset[2] << 16U)
              | ((uint32_t)tmpOffset[1] << 8U)
              | (uint32_t)tmpOffset[0]);

    /* Check if the partition ID is supported */
    switch (partition_id)
    {
      case PHASE_OTP:
        break;

      default:
        OPENBL_USART_SendByte(NACK_BYTE);
        break;
    }

    /* Save the operation type */
    OperationType = partition_id;

    /* Save the packet number */
    PacketNum = offset / OPENBL_USART_PACKET_SIZE;

    OPENBL_USART_SendByte(ACK_BYTE);

    /* Number of data to read = data + 1 */
    data = OPENBL_USART_ReadByte();

    /* Number of data to be written = data + 1 */
    codesize = (uint32_t)data + 1U;

    /* Size for word writing */
    codesize /= 4U;

    /* Checksum initialization */
    temp_xor = data;

    /* Get the data */
    data = OPENBL_USART_ReadByte();

    /* Check the integrity of received data */
    if ((data ^ temp_xor) != 0xFFU)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }

    OPENBL_USART_SendByte(ACK_BYTE);

    /* Read OTP */
    Otp = OPENBL_OTP_Read();

    /* Check if first OTP packet */
    if (offset == 0U)
    {
      /* Reset OTP read index counter */
      OtpReadIndex = 0U;

      /* Send the OTP version */
      OPENBL_USART_SendWord(Otp.Version);

      /* Send the global state */
      OPENBL_USART_SendWord(Otp.GlobalState);

      /* Update codesize */
      codesize -= 2U;
    }

    /* Send OTP words */
    for (i = 0U; i < codesize; i++)
    {
      /* Send OTP words until its end and send 0 after to fill */
      if (OtpReadIndex < OTP_PART_SIZE)
      {
        OPENBL_USART_SendWord(Otp.OtpPart[OtpReadIndex]);

        OtpReadIndex++;
      }
      else
      {
        OPENBL_USART_SendWord(0U);
      }
    }
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
void OPENBL_USART_Start(void)
{
  uint32_t address;

  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get memory address */
  if (OPENBL_USART_GetAddress(&address) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    /* If the jump address is valid then send ACK */
    OPENBL_USART_SendByte(ACK_BYTE);
  }
}

/**
  * @brief  This function is used to enable readout protection.
  * @retval None.
  */
void OPENBL_USART_ReadoutProtect(void)
{
  /* Check memory protection then send adequate response */
  if (Common_GetProtectionStatus() != RESET)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Enable the read protection */
    OPENBL_MEM_SetReadOutProtection(OPENBL_DEFAULT_MEM, ENABLE);

    OPENBL_USART_SendByte(ACK_BYTE);

    /* Start post processing task if needed */
    Common_StartPostProcessing();
  }
}

/**
  * @brief  This function is used to disable readout protection.
  * @retval None.
  */
void OPENBL_USART_ReadoutUnprotect(void)
{
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Once the option bytes modification start bit is set in FLASH CR register,
     all the RAM is erased, this causes the erase of the Open Bootloader RAM.
     This is why the last ACK is sent before the call of OPENBL_MEM_SetReadOutProtection */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Disable the read protection */
  OPENBL_MEM_SetReadOutProtection(OPENBL_DEFAULT_MEM, DISABLE);

  /* Start post processing task if needed */
  Common_StartPostProcessing();
}

/**
  * @brief  This function is used to erase a memory.
  * @retval None.
  */
void OPENBL_USART_EraseMemory(void)
{
  uint32_t xor;
  uint32_t counter;
  uint32_t numpage;
  uint16_t data;
  uint8_t status = ACK_BYTE;
  uint8_t *ramaddress;

  ramaddress = (uint8_t *) USART_RAM_Buf;

  /* Check if the memory is not protected */
  if (Common_GetProtectionStatus() != RESET)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Read number of pages to be erased */
    data = OPENBL_USART_ReadByte();
    data = (uint16_t)(data << 8) | OPENBL_USART_ReadByte();

    /* Checksum initialization */
    xor  = ((uint32_t)data & 0xFF00U) >> 8;
    xor ^= (uint32_t)data & 0x00FFU;

    /* All commands in range 0xFFFZ are reserved for special erase features */
    if ((data & 0xFFF0U) == 0xFFF0U)
    {
      /* Check data integrity */
      if (OPENBL_USART_ReadByte() != (uint8_t) xor)
      {
        status = NACK_BYTE;
      }
      else
      {
        if ((data == 0xFFFFU) || (data == 0xFFFEU) || (data == 0xFFFDU))
        {
          ramaddress[0] = (uint8_t)(data & 0x00FFU);
          ramaddress[1] = (uint8_t)((data & 0xFF00U) >> 8);

          OPENBL_MEM_MassErase(OPENBL_DEFAULT_MEM);
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
      ramaddress = (uint8_t *) USART_RAM_Buf;

      /* Number of pages to be erased (data + 1) */
      numpage = (uint32_t)data + 1U;

      *ramaddress = (uint8_t)(numpage & 0x00FFU);
      ramaddress++;

      *ramaddress = (uint8_t)((numpage & 0xFF00U) >> 8);
      ramaddress++;

      /* Get the pages to be erased */
      for (counter = numpage; counter != 0U ; counter--)
      {
        /* Receive the MSB byte */
        data  = OPENBL_USART_ReadByte();
        xor  ^= data;
        data  = (uint16_t)((data & 0x00FFU) << 8);

        /* Receive the LSB byte */
        data |= (uint8_t)(OPENBL_USART_ReadByte() & 0x00FFU);
        xor  ^= ((uint32_t)data & 0x00FFU);

        /* Only store data that fit in the buffer length */
        if (counter < (USART_RAM_BUFFER_SIZE / 2U))
        {
          *ramaddress = (uint8_t)(data & 0x00FFU);
          ramaddress++;

          *ramaddress = (uint8_t)((data & 0xFF00U) >> 8);
          ramaddress++;
        }
        else
        {
          /* Continue to read data from USART */
        }
      }

      /* Check data integrity */
      if (OPENBL_USART_ReadByte() != (uint8_t) xor)
      {
        status = NACK_BYTE;
      }
      else
      {
        (void) OPENBL_MEM_Erase(OPENBL_DEFAULT_MEM, (uint8_t *) USART_RAM_Buf, USART_RAM_BUFFER_SIZE);

        status = ACK_BYTE;
      }
    }

    OPENBL_USART_SendByte(status);
  }
}

/**
  * @brief  This function is used to enable write protect.
  * @retval None.
  */
void OPENBL_USART_WriteProtect(void)
{
  uint32_t counter;
  uint32_t length;
  uint32_t data;
  uint32_t xor;
  ErrorStatus error_value;
  uint8_t *ramaddress;

  /* Check if the memory is not protected */
  if (Common_GetProtectionStatus() != RESET)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Get the data length */
    data = OPENBL_USART_ReadByte();

    ramaddress = (uint8_t *) USART_RAM_Buf;
    length     = data + 1U;

    /* Checksum Initialization */
    xor = data;

    /* Receive data and write to RAM Buffer */
    for (counter = length; counter != 0U ; counter--)
    {
      data  = OPENBL_USART_ReadByte();
      xor  ^= data;

      *(__IO uint8_t *)(ramaddress) = (uint8_t) data;

      ramaddress++;
    }

    /* Check data integrity and send NACK if Checksum is incorrect */
    if (OPENBL_USART_ReadByte() != (uint8_t) xor)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      ramaddress = (uint8_t *) USART_RAM_Buf;

      /* Enable the write protection */
      error_value = OPENBL_MEM_SetWriteProtection(ENABLE, OPENBL_DEFAULT_MEM, ramaddress, length);

      OPENBL_USART_SendByte(ACK_BYTE);

      if (error_value == SUCCESS)
      {
        Common_StartPostProcessing();
      }
    }
  }
}

/**
  * @brief  This function is used to disable write protect.
  * @retval None.
  */
void OPENBL_USART_WriteUnprotect(void)
{
  ErrorStatus error_value;

  /* Check if the memory is not protected */
  if (Common_GetProtectionStatus() != RESET)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Disable write protection */
    error_value = OPENBL_MEM_SetWriteProtection(DISABLE, OPENBL_DEFAULT_MEM, NULL, 0U);

    OPENBL_USART_SendByte(ACK_BYTE);

    if (error_value == SUCCESS)
    {
      Common_StartPostProcessing();
    }
  }
}

/**
  * @brief  This function is used to get a valid address.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_USART_GetAddress(uint32_t *Address)
{
  uint8_t tmpAddress[4] = {0U, 0U, 0U, 0U};
  uint8_t status;
  uint8_t temp_xor;

  tmpAddress[3] = OPENBL_USART_ReadByte();
  tmpAddress[2] = OPENBL_USART_ReadByte();
  tmpAddress[1] = OPENBL_USART_ReadByte();
  tmpAddress[0] = OPENBL_USART_ReadByte();

  temp_xor = tmpAddress[3] ^ tmpAddress[2] ^ tmpAddress[1] ^ tmpAddress[0];

  /* Check the integrity of received data */
  if (OPENBL_USART_ReadByte() != temp_xor)
  {
    status = NACK_BYTE;
  }
  else
  {
    /* Get the start address */
    *Address = (((uint32_t)tmpAddress[3] << 24U)
                | ((uint32_t)tmpAddress[2] << 16U)
                | ((uint32_t)tmpAddress[1] << 8U)
                | (uint32_t)tmpAddress[0]);

    /* Get the operation type */
    OperationType = (uint8_t)(*Address >> 24U);

    /* Get the packet number */
    PacketNum = (uint32_t)((*Address << 8U) >> 8U);

    /* Check if the address is supported */
    if ((*Address == 0xFFFFFFFFU) || (OperationType == PHASE_OTP))
    {
      status = ACK_BYTE;
    }
    else
    {

      /* Build the real memory address */
      *Address = DestinationAddress + (PacketNum * OPENBL_USART_PACKET_SIZE);

      /* Check if the memory address is valid */
      if (OPENBL_MEM_GetAddressArea(*Address) == AREA_ERROR)
      {
        status = NACK_BYTE;
      }
      else
      {
        status = ACK_BYTE;
      }
    }
  }

  return status;
}

/**
  * @brief  This function is used to execute special command commands.
  * @retval None.
  */
void OPENBL_USART_SpecialCommand(void)
{
  OPENBL_SpecialCmdTypeDef *special_cmd;
  uint16_t op_code;
  uint8_t index;
  uint8_t data;
  uint8_t xor;

  /* Point to the RAM USART buffer to gain size and reliability */
  special_cmd = (OPENBL_SpecialCmdTypeDef *)(uint32_t)USART_RAM_Buf;

  /* Send special command code acknowledgment */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the command operation code */
  if (OPENBL_USART_GetSpecialCmdOpCode(&op_code, OPENBL_SPECIAL_CMD) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    /* Send Operation code acknowledgment */
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Initialize the special command frame */
    special_cmd->CmdType = OPENBL_SPECIAL_CMD;
    special_cmd->OpCode  = op_code;

    /* Initialize the xor variable */
    xor = 0U;

    /* Get the number of bytes to be received */
    /* Read the MSB of the size byte */
    data                     = OPENBL_USART_ReadByte();
    special_cmd->SizeBuffer1 = ((uint16_t)data) << 8;
    xor                     ^= data;

    /* Read the LSB of the size byte */
    data                      = OPENBL_USART_ReadByte();
    special_cmd->SizeBuffer1 |= (uint16_t)data;
    xor                      ^= data;

    if (special_cmd->SizeBuffer1 > SPECIAL_CMD_SIZE_BUFFER1)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      if (special_cmd->SizeBuffer1 != 0U)
      {
        /* Read received bytes */
        for (index = 0U; index < special_cmd->SizeBuffer1; index++)
        {
          data                        = OPENBL_USART_ReadByte();
          special_cmd->Buffer1[index] = data;
          xor                        ^= data;
        }
      }

      /* Check data integrity */
      if (OPENBL_USART_ReadByte() != xor)
      {
        OPENBL_USART_SendByte(NACK_BYTE);
      }
      else
      {
        /* Send received size acknowledgment */
        OPENBL_USART_SendByte(ACK_BYTE);

        /* Process the special command */
        OPENBL_USART_SpecialCommandProcess(special_cmd);

        /* NOTE: In case of any operation inside "SpecialCommandProcess" function that prevents the code
         * from returning to here (reset operation...), to be compatible with the OpenBL protocol,
         * the user must ensure sending the last ACK in the application side.
         */

        /* Send last acknowledgment */
        OPENBL_USART_SendByte(ACK_BYTE);
      }
    }
  }
}

/**
  * @brief  This function is used to execute extended special command commands.
  * @retval None.
  */
void OPENBL_USART_ExtendedSpecialCommand(void)
{
  OPENBL_SpecialCmdTypeDef *special_cmd;
  uint16_t op_code;
  uint16_t index;
  uint8_t xor;
  uint8_t data;

  /* Point to the RAM USART buffer to gain size and reliability */
  special_cmd = (OPENBL_SpecialCmdTypeDef *)(uint32_t) USART_RAM_Buf;

  /* Send extended special command code acknowledgment */
  OPENBL_USART_SendByte(ACK_BYTE);

  /* Get the command operation code */
  if (OPENBL_USART_GetSpecialCmdOpCode(&op_code, OPENBL_EXTENDED_SPECIAL_CMD) == NACK_BYTE)
  {
    OPENBL_USART_SendByte(NACK_BYTE);
  }
  else
  {
    /* Send Operation code acknowledgment */
    OPENBL_USART_SendByte(ACK_BYTE);

    /* Initialize the special command frame */
    special_cmd->CmdType = OPENBL_EXTENDED_SPECIAL_CMD;
    special_cmd->OpCode  = op_code;

    /* Initialize the xor variable */
    xor = 0U;

    /* Get the number of bytes to be received */
    /* Read the MSB of the size byte */
    data                     = OPENBL_USART_ReadByte();
    special_cmd->SizeBuffer1 = ((uint16_t)data) << 8;
    xor                     ^= data;

    /* Read the LSB of the size byte */
    data                      = OPENBL_USART_ReadByte();
    special_cmd->SizeBuffer1 |= (uint16_t)data;
    xor                      ^= data;

    if (special_cmd->SizeBuffer1 > SPECIAL_CMD_SIZE_BUFFER1)
    {
      OPENBL_USART_SendByte(NACK_BYTE);
    }
    else
    {
      if (special_cmd->SizeBuffer1 != 0U)
      {
        /* Read received bytes */
        for (index = 0U; index < special_cmd->SizeBuffer1; index++)
        {
          data                        = OPENBL_USART_ReadByte();
          special_cmd->Buffer1[index] = data;
          xor                        ^= data;
        }
      }

      /* Check data integrity */
      if (OPENBL_USART_ReadByte() != xor)
      {
        OPENBL_USART_SendByte(NACK_BYTE);
      }
      else
      {
        /* Send receive size acknowledgment */
        OPENBL_USART_SendByte(ACK_BYTE);

        /* Get the number of bytes to be written */
        /* Read the MSB of the size byte */
        xor                      = 0U;
        data                     = OPENBL_USART_ReadByte();
        special_cmd->SizeBuffer2 = ((uint16_t)data) << 8;
        xor                     ^= data;

        /* Read the LSB of the size byte */
        data                      = OPENBL_USART_ReadByte();
        special_cmd->SizeBuffer2 |= (uint16_t)data;
        xor                      ^= data;

        if (special_cmd->SizeBuffer2 > SPECIAL_CMD_SIZE_BUFFER2)
        {
          OPENBL_USART_SendByte(NACK_BYTE);
        }
        else
        {
          if (special_cmd->SizeBuffer2 != 0U)
          {
            /* Read received bytes */
            for (index = 0U; index < special_cmd->SizeBuffer2; index++)
            {
              data                        = OPENBL_USART_ReadByte();
              special_cmd->Buffer2[index] = data;
              xor                        ^= data;
            }
          }

          /* Check data integrity */
          if (OPENBL_USART_ReadByte() != xor)
          {
            OPENBL_USART_SendByte(NACK_BYTE);
          }
          else
          {
            /* Send receive write size acknowledgment */
            OPENBL_USART_SendByte(ACK_BYTE);

            /* Process the special command */
            OPENBL_USART_SpecialCommandProcess(special_cmd);

            /* NOTE: In case of any operation inside "SpecialCommandProcess" function that prevents the code
             * from returning to here (reset operation...), to be compatible with the OpenBL protocol,
             * the user must ensure sending the last ACK in the application side.
             */

            /* Send last acknowledgment */
            OPENBL_USART_SendByte(ACK_BYTE);
          }
        }
      }
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is used to construct the command list table.
  * @return Returns the number of supported commands.
  */
static uint8_t OPENBL_USART_ConstructCommandsTable(const OPENBL_CommandsTypeDef *pUsartCmd)
{
  uint8_t i = 0U;

  if (pUsartCmd->GetCommand != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_GET_COMMAND;
    i++;
  }

  if (pUsartCmd->GetVersion != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_GET_VERSION;
    i++;
  }

  if (pUsartCmd->GetID != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_GET_ID;
    i++;
  }

  if (pUsartCmd->GetPhase != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_GET_PHASE;
    i++;
  }

  if (pUsartCmd->ReadMemory != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_READ_MEMORY;
    i++;
  }

  if (pUsartCmd->ReadPartition != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_READ_PARTITION;
    i++;
  }

  if (pUsartCmd->Download != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_DOWNLOAD;
    i++;
  }

  if (pUsartCmd->Start != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_START;
    i++;
  }

  if (pUsartCmd->EraseMemory != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_EXT_ERASE_MEMORY;
    i++;
  }

  if (pUsartCmd->WriteProtect != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_WRITE_PROTECT;
    i++;
  }

  if (pUsartCmd->WriteUnprotect != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_WRITE_UNPROTECT;
    i++;
  }

  if (pUsartCmd->ReadoutProtect != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_READ_PROTECT;
    i++;
  }

  if (pUsartCmd->ReadoutUnprotect != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_READ_UNPROTECT;
    i++;
  }

  if (pUsartCmd->SpecialCommand != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_SPECIAL_COMMAND;
    i++;
  }

  if (pUsartCmd->ExtendedSpecialCommand != NULL)
  {
    a_OPENBL_USART_CommandsList[i] = CMD_EXTENDED_SPECIAL_COMMAND;
    i++;
  }

  return (i);
}

/**
  * @brief  This function is used to get the operation code.
  * @param  OpCode Pointer to the operation code to be returned.
  * @param  CmdType Type of the command, Special command or extended special command.
  * @retval Returns NACK status in case of error else returns ACK status.
  */
static uint8_t OPENBL_USART_GetSpecialCmdOpCode(uint16_t *OpCode, OPENBL_SpecialCmdTypeTypeDef CmdType)
{
  uint8_t op_code[2];
  uint8_t xor;
  uint8_t status;
  uint8_t index;

  /* Initialize the status variable */
  status = NACK_BYTE;

  /* Get the command OpCode (2 bytes) */
  op_code[0] = OPENBL_USART_ReadByte(); /* Read the MSB byte */
  op_code[1] = OPENBL_USART_ReadByte(); /* Read the LSB byte */

  /* Get the checksum */
  xor  = op_code[0];
  xor ^= op_code[1];

  if (OPENBL_USART_ReadByte() != xor)
  {
    status = NACK_BYTE;
  }
  else
  {
    /* Get the operation code */
    *OpCode = ((uint16_t)op_code[0] << 8) | (uint16_t)op_code[1];

    if (CmdType == OPENBL_SPECIAL_CMD)
    {
      for (index = 0U; index < SPECIAL_CMD_MAX_NUMBER; index++)
      {
        if (SpecialCmdList[index] == *OpCode)
        {
          status = ACK_BYTE;
          break;
        }
      }
    }
    else if (CmdType == OPENBL_EXTENDED_SPECIAL_CMD)
    {
      for (index = 0U; index < EXTENDED_SPECIAL_CMD_MAX_NUMBER; index++)
      {
        if (ExtendedSpecialCmdList[index] == *OpCode)
        {
          status = ACK_BYTE;
          break;
        }
      }
    }
    else
    {
      status = NACK_BYTE;
    }
  }

  return status;
}
