/**
  ******************************************************************************
  * @file    openbl_core.c
  * @author  MCD Application Team
  * @brief   Open Bootloader core file
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
#include "openbl_core.h"
#include <stdbool.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t NumberOfInterfaces = 0U;
static OPENBL_HandleTypeDef a_InterfacesTable[INTERFACES_SUPPORTED];
static OPENBL_HandleTypeDef *p_Interface;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the registered interfaces in the Open Bootloader MW.
  * @retval None.
  */
void OPENBL_Init(void)
{
  uint32_t counter;

  for (counter = 0U; counter < NumberOfInterfaces; counter++)
  {
    if (a_InterfacesTable[counter].p_Ops->Init != NULL)
    {
      a_InterfacesTable[counter].p_Ops->Init();
    }
  }
}

/**
  * @brief  This function is used to de-initialize the registered interfaces in the Open Bootloader MW.
  * @retval None.
  */
void OPENBL_DeInit(void)
{
  uint32_t counter;

  for (counter = 0U; counter < NumberOfInterfaces; counter++)
  {
    if (a_InterfacesTable[counter].p_Ops->DeInit != NULL)
    {
      a_InterfacesTable[counter].p_Ops->DeInit();
    }
  }
}

/**
  * @brief  This function is used to register a given interface in the Open Bootloader MW.
  * @retval None.
  */
ErrorStatus OPENBL_RegisterInterface(OPENBL_HandleTypeDef *Interface)
{
  ErrorStatus status = SUCCESS;

  if (NumberOfInterfaces < INTERFACES_SUPPORTED)
  {
    a_InterfacesTable[NumberOfInterfaces].p_Ops = Interface->p_Ops;
    a_InterfacesTable[NumberOfInterfaces].p_Cmd = Interface->p_Cmd;

    NumberOfInterfaces++;
  }
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to detect if there is any activity on a given interface.
  * @retval None.
  */
uint32_t OPENBL_InterfaceDetection(void)
{
  uint32_t counter;
  uint8_t detected = 0U;

  for (counter = 0U; counter < NumberOfInterfaces; counter++)
  {
    if (a_InterfacesTable[counter].p_Ops->Detection != NULL)
    {
      detected = a_InterfacesTable[counter].p_Ops->Detection();

      if (detected == 1U)
      {
        p_Interface = &(a_InterfacesTable[counter]);
        break;
      }
    }
  }

  return detected;
}

/**
  * @brief  This function is used to get the command opcode from the given interface and execute the right command.
  * @retval None.
  */
void OPENBL_CommandProcess(void)
{
  uint8_t command_opcode;

  /* Get the user command opcode */
  if (p_Interface->p_Ops->GetCommandOpcode != NULL)
  {
    command_opcode = p_Interface->p_Ops->GetCommandOpcode();

    switch (command_opcode)
    {
      case CMD_GET_COMMAND:
        if (p_Interface->p_Cmd->GetCommand != NULL)
        {
          p_Interface->p_Cmd->GetCommand();
        }
        break;

      case CMD_GET_VERSION:
        if (p_Interface->p_Cmd->GetVersion != NULL)
        {
          p_Interface->p_Cmd->GetVersion();
        }
        break;

      case CMD_GET_ID:
        if (p_Interface->p_Cmd->GetID != NULL)
        {
          p_Interface->p_Cmd->GetID();
        }

        break;

      case CMD_READ_MEMORY:
        if (p_Interface->p_Cmd->ReadMemory != NULL)
        {
          p_Interface->p_Cmd->ReadMemory();
        }
        break;

      case CMD_WRITE_MEMORY:
        if (p_Interface->p_Cmd->WriteMemory != NULL)
        {
          p_Interface->p_Cmd->WriteMemory();
        }
        break;

      case CMD_GO:
        if (p_Interface->p_Cmd->Go != NULL)
        {
          p_Interface->p_Cmd->Go();
        }
        break;

      case CMD_READ_PROTECT:
        if (p_Interface->p_Cmd->ReadoutProtect != NULL)
        {
          p_Interface->p_Cmd->ReadoutProtect();
        }
        break;

      case CMD_READ_UNPROTECT:
        if (p_Interface->p_Cmd->ReadoutUnprotect != NULL)
        {
          p_Interface->p_Cmd->ReadoutUnprotect();
        }
        break;

      case CMD_EXT_ERASE_MEMORY:
        if (p_Interface->p_Cmd->EraseMemory != NULL)
        {
          p_Interface->p_Cmd->EraseMemory();
        }
        break;

      case CMD_WRITE_PROTECT:
        if (p_Interface->p_Cmd->WriteProtect != NULL)
        {
          p_Interface->p_Cmd->WriteProtect();
        }
        break;

      case CMD_WRITE_UNPROTECT:
        if (p_Interface->p_Cmd->WriteUnprotect != NULL)
        {
          p_Interface->p_Cmd->WriteUnprotect();
        }
        break;

      case CMD_NS_WRITE_MEMORY:
        if (p_Interface->p_Cmd->NsWriteMemory != NULL)
        {
          p_Interface->p_Cmd->NsWriteMemory();
        }
        break;

      case CMD_NS_ERASE_MEMORY:
        if (p_Interface->p_Cmd->NsEraseMemory != NULL)
        {
          p_Interface->p_Cmd->NsEraseMemory();
        }
        break;

      case CMD_NS_WRITE_PROTECT:
        if (p_Interface->p_Cmd->NsWriteProtect != NULL)
        {
          p_Interface->p_Cmd->NsWriteProtect();
        }
        break;

      case CMD_NS_WRITE_UNPROTECT:
        if (p_Interface->p_Cmd->NsWriteUnprotect != NULL)
        {
          p_Interface->p_Cmd->NsWriteUnprotect();
        }
        break;

      case CMD_NS_READ_PROTECT:
        if (p_Interface->p_Cmd->NsReadoutProtect != NULL)
        {
          p_Interface->p_Cmd->NsReadoutProtect();
        }
        break;

      case CMD_NS_READ_UNPROTECT:
        if (p_Interface->p_Cmd->NsReadoutUnprotect != NULL)
        {
          p_Interface->p_Cmd->NsReadoutUnprotect();
        }
        break;

      /* Unknown command opcode */
      default:
        if (p_Interface->p_Ops->SendByte != NULL)
        {
          p_Interface->p_Ops->SendByte(NACK_BYTE);
        }
        break;
    }
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
