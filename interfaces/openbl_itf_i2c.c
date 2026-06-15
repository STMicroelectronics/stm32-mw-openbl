/**
  ******************************************************************************
  * @file    openbl_itf_i2c.c
  * @brief   Contains I2C HW configuration
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

#include "openbl_itf_i2c.h"
#include "openbl_i2c.h"
#include "openbl_itf_iwdg.h"
#include "openbl_itf_flash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the used I2C instance.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK openbl_status_t OPENBL_ITF_I2C_Init(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_I2C_Init" can be implemented in the user file.
   */

  STM32_UNUSED(p_itf);

  return OPENBL_SUCCESS;
}

/**
  * @brief  This function is used to De-initialize the I2C pins and instance.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK void OPENBL_ITF_I2C_DeInit(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_I2C_DeInit" can be implemented in the user file.
   */

  STM32_UNUSED(p_itf);

  openbl_user_i2c_deinit();
}

/**
  * @brief  This function is used to detect if there is any activity on I2C protocol.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_ITF_I2C_ProtocolDetection(openbl_itf_obj_t *p_itf)
{
  uint8_t detected;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Check if the I2Cx is addressed */
  if (OPENBL_I2C_IsAddressMatched(p_i2c) != 0U)
  {
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
uint8_t OPENBL_ITF_I2C_GetCommandOpcode(openbl_itf_obj_t *p_itf)
{
  uint8_t command_opc;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Wait until the Address Matched flag is set */
  while (OPENBL_I2C_IsAddressMatched(p_i2c) == 0U)
  {
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);
  }

  OPENBL_I2C_ClearAddressMatchedFlag(p_i2c);

  /* Get the command opcode */
  command_opc = OPENBL_ITF_I2C_ReadByte(p_itf);

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_ITF_I2C_ReadByte(p_itf)) != 0xFFU)
  {
    command_opc = OPENBL_ERROR_CMD;
  }

  OPENBL_ITF_I2C_WaitStop(p_itf);

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from I2C pipe.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_ITF_I2C_ReadByte(openbl_itf_obj_t *p_itf)
{
  uint32_t timeout   = 0U;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  while (OPENBL_I2C_IsRxNotEmpty(p_i2c) == 0U)
  {
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      OPENBL_I2C_CommunicationError();
    }
  }

  return OPENBL_I2C_ReceiveByte(p_i2c);
}

/**
  * @brief  This function is used to send one byte through I2C pipe.
  * @param  p_itf Pointer to the interface object.
  * @param  byte The byte to be sent.
  * @retval None.
  */
void OPENBL_ITF_I2C_SendByte(openbl_itf_obj_t *p_itf, uint8_t byte)
{
  uint32_t timeout   = 0U;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  while (OPENBL_I2C_IsTxEmpty(p_i2c) == 0U)
  {
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      OPENBL_I2C_CommunicationError();
    }
  }

  OPENBL_I2C_TransmitByte(p_i2c, byte);
}

/**
  * @brief  This function is used to wait until the address is matched.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_ITF_I2C_WaitAddress(openbl_itf_obj_t *p_itf)
{
  uint32_t timeout   = 0U;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  while (OPENBL_I2C_IsAddressMatched(p_i2c) == 0U)
  {
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      OPENBL_I2C_CommunicationError();
    }
  }

  OPENBL_I2C_ClearAddressMatchedFlag(p_i2c);
}

/**
  * @brief  This function is used to wait until NACK is detected.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_I2C_WaitNack(openbl_itf_obj_t *p_itf)
{
  uint32_t timeout   = 0U;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* While the I2C NACK is not detected, the IWDG is refreshed,
     if the timeout is reached a system reset occurs */
  while (OPENBL_I2C_IsNackDetected(p_i2c) == 0U)
  {
    /* Refresh IWDG counter */
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      OPENBL_I2C_CommunicationError();
    }
  }

  /* Clear Not Acknowledge detection flag */
  (void)OPENBL_I2C_ClearNackFlag(p_i2c);
}

/**
  * @brief  This function is used to wait until STOP is detected.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_I2C_WaitStop(openbl_itf_obj_t *p_itf)
{
  uint32_t timeout   = 0U;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* While the I2C stop is not detected, refresh the IWDG,
     if the timeout is reached a system reset occurs */
  while (OPENBL_I2C_IsStopDetected(p_i2c) == 0U)
  {
    /* Refresh IWDG: reload counter */
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);

    timeout++;

    if (timeout >= OPENBL_I2C_TIMEOUT)
    {
      OPENBL_I2C_CommunicationError();
    }
  }

  /* Clear Stop detection flag */
  (void)OPENBL_I2C_ClearStopFlag(p_i2c);
}

/**
  * @brief  This function is used to send an acknowledgment to the host.
  * @param  p_itf Pointer to the interface object.
  * @param  acknowledge The acknowledge byte to be sent.
  * @retval None.
  */
void OPENBL_ITF_I2C_SendAcknowledge(openbl_itf_obj_t *p_itf, uint8_t acknowledge)
{
  /* Wait until address is matched */
  OPENBL_ITF_I2C_WaitAddress(p_itf);

  /* Send ACK or NACK byte */
  OPENBL_ITF_I2C_SendByte(p_itf, acknowledge);

  /* Wait until NACK is detected */
  OPENBL_ITF_I2C_WaitNack(p_itf);

  /* Wait until STOP byte is detected */
  OPENBL_ITF_I2C_WaitStop(p_itf);
}

/**
  * @brief  This function is used to send busy byte through I2C pipe.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_I2C_SendBusyByte(openbl_itf_obj_t *p_itf)
{
  uint32_t timeout   = 0U;
  I2C_TypeDef *p_i2c = (I2C_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Wait for the received address to match with the device address */
  if (OPENBL_I2C_IsAddressMatched(p_i2c) != 0U)
  {
    /* Clear the flag of address match */
    OPENBL_I2C_ClearAddressMatchedFlag(p_i2c);

    /* While the transmit data is not empty, refresh the IWDG,
       if the timeout is reached a system reset occurs */
    while (OPENBL_I2C_IsTxEmpty(p_i2c) == 0U)
    {
      OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);

      timeout++;

      if (timeout >= OPENBL_I2C_TIMEOUT)
      {
        OPENBL_I2C_CommunicationError();
      }
    }

    /* Send busy byte */
    OPENBL_I2C_TransmitByte(p_i2c, OPENBL_BUSY_BYTE);

    /* Wait until NACK is detected */
    OPENBL_ITF_I2C_WaitNack(p_itf);

    /* Wait until STOP byte is detected */
    OPENBL_ITF_I2C_WaitStop(p_itf);
  }
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  p_itf Pointer to the interface object.
  * @param  p_special_cmd Pointer to the @arg openbl_special_cmd_t structure.
  * @retval None.
  */
__WEAK void OPENBL_ITF_I2C_SpecialCommandProcess(openbl_itf_obj_t *p_itf, openbl_special_cmd_t *p_special_cmd)
{
  /** @warning This function must not be modified, when needed,
  the "OPENBL_ITF_I2C_SpecialCommandProcess" can be implemented in the user file.
  */

  switch (p_special_cmd->opcode)
  {
    case OPENBL_CMD_SPECIAL_COMMAND:
      if (p_special_cmd->type == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);

        /* Wait for address to match */
        OPENBL_ITF_I2C_WaitAddress(p_itf);

        /* Send NULL status size */
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
        to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
        */
      }
      break;

    case OPENBL_CMD_EXT_SPECIAL_COMMAND:
      if (p_special_cmd->type == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);

        /* Wait for address to match */
        OPENBL_ITF_I2C_WaitAddress(p_itf);

        /* Send NULL status size */
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);
        OPENBL_ITF_I2C_SendByte(p_itf, 0x00U);

        /* NOTE: In case of any operation that prevents the code from returning to Middleware (reset operation...),
        to be compatible with the OpenBL protocol, the user must ensure sending the last ACK from here.
        */
      }
      break;

    default:
      break;
  }
}

/**
  * @brief  This function is used to enable busy state sending during flash operations.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_ITF_I2C_EnableBusyStateSending(openbl_itf_obj_t *p_itf)
{
  p_itf->send_busy_byte = OPENBL_ENABLE;
}

/**
  * @brief  This function is used to disable busy state sending during flash operations.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_ITF_I2C_DisableBusyStateSending(openbl_itf_obj_t *p_itf)
{
  p_itf->send_busy_byte = OPENBL_DISABLE;
}

/**
  * @brief  This function is used to handle I2C communication errors.
  * @retval None (infinite loop).
  */
__WEAK OPENBL_RAM_FUNCTION void OPENBL_I2C_CommunicationError(void)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_I2C_CommunicationError" can be implemented in the user file.
   */

  for (;;) {};
}

/**
  * @brief  This function is used to De-initialize the I2C pins and instance.
  * @retval None.
  */
__WEAK void openbl_user_i2c_deinit(void)
{
  /** @warning This function must not be modified, when needed,
               the "openbl_user_i2c_deinit" can be implemented in the user file.
   */
}
