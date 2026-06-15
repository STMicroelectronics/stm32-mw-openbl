/**
  ******************************************************************************
  * @file    openbl_itf_usart.c
  * @brief   Contains USART HW configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

#include "openbl_itf_usart.h"
#include "openbl_usart.h"
#include "openbl_itf_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to configure USART pins and then initialize the used USART instance.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK openbl_status_t OPENBL_ITF_USART_Init(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_USART_Init" can be implemented in the user file.
   */

  USART_TypeDef *p_usart = (USART_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  if (OPENBL_USART_IsEnabled(p_usart) == 0U)
  {
    OPENBL_USART_Enable(p_usart);
  }

  return OPENBL_SUCCESS;
}

/**
  * @brief  This function is used to De-initialize the USART pins and instance.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK void OPENBL_ITF_USART_DeInit(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_USART_DeInit" can be implemented in the user file.
   */

  STM32_UNUSED(p_itf);

  openbl_user_usart_deinit();
}

/**
  * @brief  This function is used to detect if there is any activity on USART protocol.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns 1 if USART is detected otherwise 0.
  */
uint8_t OPENBL_ITF_USART_ProtocolDetection(openbl_itf_obj_t *p_itf)
{
  uint8_t detected;
  USART_TypeDef *p_usart = (USART_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Check if the USARTx is addressed */
  if (OPENBL_USART_IsAutoBaudRateDetected(p_usart) == 1U)
  {
    /* Read byte in order to flush the 0x7F synchronization byte */
    (void)OPENBL_ITF_USART_ReadByte(p_itf);

    /* Acknowledge the host */
    OPENBL_ITF_USART_SendByte(p_itf, OPENBL_ACK_BYTE);

    detected = 1U;
  }
  else
  {
    detected = 0U;
  }

  return detected;
}

/**
  * @brief  This function is used to get the command opcode from the host.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns the command.
  */
uint8_t OPENBL_ITF_USART_GetCommandOpcode(openbl_itf_obj_t *p_itf)
{
  uint8_t command_opc;

  /* Get the command opcode */
  command_opc = OPENBL_ITF_USART_ReadByte(p_itf);

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_ITF_USART_ReadByte(p_itf)) != 0xFFU)
  {
    command_opc = OPENBL_ERROR_CMD;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from USART pipe.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_ITF_USART_ReadByte(openbl_itf_obj_t *p_itf)
{
  USART_TypeDef *p_usart = (USART_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  while (OPENBL_USART_IsRxFifoNotEmpty(p_usart) == 0U)
  {
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);
  }

  return OPENBL_USART_ReceiveByte(p_usart);
}

/**
  * @brief  This function is used to send an acknowledgment.
  * @param  p_itf Pointer to the interface object.
  * @param  acknowledge The acknowledge byte to be sent.
  * @retval None.
  */
void OPENBL_ITF_USART_SendAcknowledge(openbl_itf_obj_t *p_itf, uint8_t acknowledge)
{
  OPENBL_ITF_USART_SendByte(p_itf, acknowledge);
}

/**
  * @brief  This function is used to send one byte through USART pipe.
  * @param  p_itf Pointer to the interface object.
  * @param  byte The byte to be sent.
  * @retval None.
  */
void OPENBL_ITF_USART_SendByte(openbl_itf_obj_t *p_itf, uint8_t byte)
{
  USART_TypeDef *p_usart = (USART_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  OPENBL_USART_TransmitByte(p_usart, byte);

  while (OPENBL_USART_IsTransmissionComplete(p_usart) == 0U)
  {
    /* Nothing to do */
  }
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  p_itf Pointer to the interface object.
  * @param  p_special_cmd Pointer to the @arg openbl_special_cmd_t structure.
  * @retval None.
  */
__WEAK void OPENBL_ITF_USART_SpecialCommandProcess(openbl_itf_obj_t *p_itf, openbl_special_cmd_t *p_special_cmd)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_USART_SpecialCommandProcess" can be implemented in the user file.
   */

  switch (p_special_cmd->opcode)
  {
    case OPENBL_CMD_SPECIAL_COMMAND:
      if (p_special_cmd->type == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);

        /* Send NULL status size */
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);
      }
      break;

    case OPENBL_CMD_EXT_SPECIAL_COMMAND:
      if (p_special_cmd->type == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);

        /* Send NULL status size */
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);
        OPENBL_ITF_USART_SendByte(p_itf, 0x00U);
      }
      break;

    /* Unknown command opcode */
    default:
      break;
  }
}

/**
  * @brief  This function is used to De-initialize the USART pins and instance.
  * @retval None.
  */
__WEAK void openbl_user_usart_deinit(void)
{
  /** @warning This function must not be modified, when needed,
               the "openbl_user_usart_deinit" can be implemented in the user file.
   */
}
