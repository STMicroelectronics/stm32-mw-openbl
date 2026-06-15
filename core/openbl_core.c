/**
  ******************************************************************************
  * @file    openbl_core.c
  * @brief   Open Bootloader core file
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
#include <stdbool.h>

#include "openbl_core.h"
#include "openbl_commands.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the registered interfaces.
  * @param  p_openbl_obj The pointer to the Open Bootloader object.
  * @retval Returns @arg OPENBL_SUCCESS if initialization is successful, otherwise returns @arg OPENBL_ERROR.
  */
openbl_status_t OPENBL_Init(openbl_obj_t *p_openbl_obj)
{
  uint32_t counter;

  /* Initialize the user application */
  if (openbl_user_init(p_openbl_obj) == 0)
  {
    /* Initialize the registered interfaces */
    for (counter = 0U; counter < p_openbl_obj->itf_list.interfaces_nbr; counter++)
    {
      if (p_openbl_obj->itf_list.interfaces[counter].p_ops == NULL)
      {
        return OPENBL_ERROR;
      }

      /* Initialize the interface if the initialization function is available */
      if (p_openbl_obj->itf_list.interfaces[counter].p_ops->Init != NULL)
      {
        p_openbl_obj->itf_list.interfaces[counter].p_ops->Init(&p_openbl_obj->itf_list.interfaces[counter]);
      }
    }
  }
  else
  {
    return OPENBL_ERROR;
  }

  return OPENBL_SUCCESS;
}

/**
  * @brief  This function is used to de-initialize the Open Bootloader MW.
  * @retval None.
  */
void OPENBL_DeInit(void)
{
  /* De-initialize the user application */
  openbl_user_deinit();
}

/**
  * @brief  This function is used to de-initialize the interfaces that were not detected.
  * @param  p_openbl_obj The pointer to the Open Bootloader object.
  * @retval None.
  */
void OPENBL_DeInitNotDetectedInterfaces(openbl_obj_t *p_openbl_obj)
{
  uint32_t counter;

  /* Loop through all interfaces and de-initialize the ones that were not detected */
  for (counter = 0U; counter < p_openbl_obj->itf_list.interfaces_nbr; counter++)
  {
    if (p_openbl_obj->itf_list.interfaces[counter].p_ops == NULL)
    {
      break;
    }

    /* De-initialize the interface that was not detected */
    if ((p_openbl_obj->itf_list.interfaces[counter].detected == 0U)
        && (p_openbl_obj->itf_list.interfaces[counter].p_ops->DeInit != NULL))
    {
      p_openbl_obj->itf_list.interfaces[counter].p_ops->DeInit(&p_openbl_obj->itf_list.interfaces[counter]);
    }
  }
}

/**
  * @brief  This function is used to detect if there is any activity on a given interface.
  * @param  p_openbl_obj The pointer to the Open Bootloader object.
  * @retval Returns the index of the detected interface if any interface is detected, otherwise returns -1.
  */
int32_t OPENBL_InterfaceDetection(openbl_obj_t *p_openbl_obj)
{
  uint32_t counter;

  /* Loop through all interfaces and try to detect any activity */
  for (counter = 0U; counter < p_openbl_obj->itf_list.interfaces_nbr; counter++)
  {
    /* Check if the interface operations pointer is valid */
    if (p_openbl_obj->itf_list.interfaces[counter].p_ops == NULL)
    {
      return (-1);
    }

    /* Check if the detection function is available */
    if (p_openbl_obj->itf_list.interfaces[counter].p_ops->Detection == NULL)
    {
      return (-1);
    }

    /* Check if there is any activity on the interface */
    if (p_openbl_obj->itf_list.interfaces[counter].p_ops->Detection(&p_openbl_obj->itf_list.interfaces[counter]) == 1U)
    {
      p_openbl_obj->itf_list.interfaces[counter].detected = 1U;

      OPENBL_MEM_AssociateDetectedInterfaceToMemories((void *)&p_openbl_obj->itf_list.interfaces[counter], \
                                                      p_openbl_obj->itf_list.interfaces[counter].p_mem_list);

      return (int32_t)counter;
    }
    else
    {
      p_openbl_obj->itf_list.interfaces[counter].detected = 0U;
    }
  }

  return (-1);
}

/**
  * @brief  This function is used to get the type of the detected interface.
  * @param  p_openbl_obj The pointer to the Open Bootloader object.
  * @retval Returns the type of the detected interface, otherwise returns @arg OPENBL_ITF_TYPE_NONE.
  */
uint32_t OPENBL_GetTypeOfDetectedInterface(openbl_obj_t *p_openbl_obj)
{
  uint32_t com_idx;

  /* Loop through all interfaces and return the type of the detected interface */
  for (com_idx = 0; com_idx < p_openbl_obj->itf_list.interfaces_nbr; com_idx++)
  {
    if (p_openbl_obj->itf_list.interfaces[com_idx].detected == 1U)
    {
      return p_openbl_obj->itf_list.interfaces[com_idx].itf_type;
    }
  }

  return OPENBL_ITF_TYPE_NONE;
}

/**
  * @brief  This function is used to get the command opcode from the given interface and execute the right command.
  * @param  p_openbl_obj The pointer to the Open Bootloader object.
  * @param  com_idx The index of the detected interface.
  * @retval None.
  */
void OPENBL_CommandProcess(openbl_obj_t *p_openbl_obj, uint32_t com_idx)
{
  uint8_t command_opcode;
  openbl_itf_obj_t *p_itf;

  /* Get the detected interface object */
  p_itf = &p_openbl_obj->itf_list.interfaces[com_idx];

  /* Check if the interface operations pointer is valid */
  if (p_itf->p_ops == NULL)
  {
    return;
  }

  if (p_itf->p_ops->GetCommandOpcode != NULL)
  {
    /* Get the command opcode from the interface */
    command_opcode = p_itf->p_ops->GetCommandOpcode(p_itf);

    /* Process the command based on the opcode */
    switch (command_opcode)
    {
      /* Process get command */
      /* =================== */
      case OPENBL_CMD_GET_COMMAND:
        if (p_itf->p_cmd->GetCommand != NULL)
        {
          p_itf->p_cmd->GetCommand(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process get version command */
      /* =========================== */
      case OPENBL_CMD_GET_VERSION:
        if (p_itf->p_cmd->GetVersion != NULL)
        {
          p_itf->p_cmd->GetVersion(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process get ID command */
      /* ====================== */
      case OPENBL_CMD_GET_ID:
        if (p_itf->p_cmd->GetID != NULL)
        {
          p_itf->p_cmd->GetID(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process read memory command */
      /* =========================== */
      case OPENBL_CMD_READ_MEMORY:
        if (p_itf->p_cmd->ReadMemory != NULL)
        {
          p_itf->p_cmd->ReadMemory(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process write memory command */
      /* ============================ */
      case OPENBL_CMD_WRITE_MEMORY:
        if (p_itf->p_cmd->WriteMemory != NULL)
        {
          p_itf->p_cmd->WriteMemory(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process go command */
      /* ================== */
      case OPENBL_CMD_GO:
        if (p_itf->p_cmd->Go != NULL)
        {
          p_itf->p_cmd->Go(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process read protect command */
      /* ============================ */
      case OPENBL_CMD_READ_PROTECT:
        if (p_itf->p_cmd->ReadoutProtect != NULL)
        {
          p_itf->p_cmd->ReadoutProtect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process read unprotect command */
      /* ============================== */
      case OPENBL_CMD_READ_UNPROTECT:
        if (p_itf->p_cmd->ReadoutUnprotect != NULL)
        {
          p_itf->p_cmd->ReadoutUnprotect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process extended erase memory command */
      /* ===================================== */
      case OPENBL_CMD_EXT_ERASE_MEMORY:
        if (p_itf->p_cmd->EraseMemory != NULL)
        {
          p_itf->p_cmd->EraseMemory(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process legacy erase memory command */
      /* =================================== */
      case OPENBL_CMD_LEG_ERASE_MEMORY:
        if (p_itf->p_cmd->EraseMemory != NULL)
        {
          p_itf->p_cmd->EraseMemory(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process write protect command */
      /* ============================= */
      case OPENBL_CMD_WRITE_PROTECT:
        if (p_itf->p_cmd->WriteProtect != NULL)
        {
          p_itf->p_cmd->WriteProtect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process write unprotect command */
      /* =============================== */
      case OPENBL_CMD_WRITE_UNPROTECT:
        if (p_itf->p_cmd->WriteUnprotect != NULL)
        {
          p_itf->p_cmd->WriteUnprotect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process non-stretch write memory command */
      /* ======================================== */
      case OPENBL_CMD_NS_WRITE_MEMORY:
        if (p_itf->p_cmd->NsWriteMemory != NULL)
        {
          p_itf->p_cmd->NsWriteMemory(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process non-stretch erase memory command */
      /* ======================================== */
      case OPENBL_CMD_NS_ERASE_MEMORY:
        if (p_itf->p_cmd->NsEraseMemory != NULL)
        {
          p_itf->p_cmd->NsEraseMemory(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process non-stretch mass erase memory command */
      /* ============================================= */
      case OPENBL_CMD_NS_WRITE_PROTECT:
        if (p_itf->p_cmd->NsWriteProtect != NULL)
        {
          p_itf->p_cmd->NsWriteProtect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process non-stretch read protect command */
      /* ======================================== */
      case OPENBL_CMD_NS_WRITE_UNPROTECT:
        if (p_itf->p_cmd->NsWriteUnprotect != NULL)
        {
          p_itf->p_cmd->NsWriteUnprotect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process non-stretch read protect command */
      /* ======================================== */
      case OPENBL_CMD_NS_READ_PROTECT:
        if (p_itf->p_cmd->NsReadoutProtect != NULL)
        {
          p_itf->p_cmd->NsReadoutProtect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process non-stretch read unprotect command */
      /* ========================================== */
      case OPENBL_CMD_NS_READ_UNPROTECT:
        if (p_itf->p_cmd->NsReadoutUnprotect != NULL)
        {
          p_itf->p_cmd->NsReadoutUnprotect(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process speed command */
      /* ===================== */
      case OPENBL_CMD_SPEED:
        if (p_itf->p_cmd->Speed != NULL)
        {
          p_itf->p_cmd->Speed(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process special command */
      /* ======================= */
      case OPENBL_CMD_SPECIAL_COMMAND:
        if (p_itf->p_cmd->SpecialCommand != NULL)
        {
          p_itf->p_cmd->SpecialCommand(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Process extended special command */
      /* ================================ */
      case OPENBL_CMD_EXT_SPECIAL_COMMAND:
        if (p_itf->p_cmd->ExtendedSpecialCommand != NULL)
        {
          p_itf->p_cmd->ExtendedSpecialCommand(p_itf);
        }
        else
        {
          if (p_itf->p_ops->SendAcknowledge != NULL)
          {
            p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
          }
        }
        break;

      /* Unknown command opcode */
      /* ====================== */
      default:
        if (p_itf->p_ops->SendAcknowledge != NULL)
        {
          p_itf->p_ops->SendAcknowledge(p_itf, OPENBL_NACK_BYTE);
        }
        break;
    }
  }
}

/**
  * @brief  This function is used to initialize Open Bootloader application.
  * @param  p_openbl_obj The pointer to the Open Bootloader object.
  * @retval Returns 0 in case of success otherwise returns -1.
  */
__WEAK int32_t openbl_user_init(openbl_obj_t *p_openbl_obj)
{
  /** @warning This function must not be modified, when needed,
               the "openbl_user_init" can be implemented in the user file.
   */
  STM32_UNUSED(p_openbl_obj);

  return (-1);
}

/**
  * @brief  This function is used to de-initialize Open Bootloader application.
  * @retval None.
  */
__WEAK void openbl_user_deinit(void)
{
  /** @warning This function must not be modified, when needed,
               the "openbl_user_deinit" can be implemented in the user file.
   */
}
