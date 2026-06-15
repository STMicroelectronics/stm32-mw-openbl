/**
  ******************************************************************************
  * @file    openbl_itf_spi.c
  * @brief   Contains SPI HW configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "openbl_itf_spi.h"
#include "openbl_spi.h"
#include "openbl_itf_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static volatile uint8_t SpiRxNotEmpty = 0U;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the SPI peripheral.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns OPENBL_SUCCESS if initialization is successful, otherwise returns OPENBL_ERROR.
  */
__WEAK openbl_status_t OPENBL_ITF_SPI_Init(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_SPI_Init" can be implemented in the user file.
   */

  SPI_TypeDef *p_spi = (SPI_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Set the underrun pattern that will be sent when SPI is busy */
  OPENBL_SPI_SetUnderRunPattern(p_spi, OPENBL_SPI_BUSY_BYTE);

  /* Enable the SPI peripheral */
  if (OPENBL_SPI_IsEnabled(p_spi) == 0U)
  {
    OPENBL_SPI_Enable(p_spi);
  }

  return OPENBL_SUCCESS;
}

/**
  * @brief  This function is used to De-initialize the SPI pins and instance.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK void OPENBL_ITF_SPI_DeInit(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_SPI_DeInit" can be implemented in the user file.
   */

  STM32_UNUSED(p_itf);

  openbl_user_spi_deinit();
}

/**
  * @brief  This function is used to detect if there is any activity on SPI protocol.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns 1 if spi is detected otherwise 0.
  */
uint8_t OPENBL_ITF_SPI_ProtocolDetection(openbl_itf_obj_t *p_itf)
{
  uint8_t detected;
  SPI_TypeDef *p_spix = (SPI_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Check if there is any activity on SPI */
  if (OPENBL_SPI_IsActiveFlag_RXP(p_spix) != 0U)
  {
    /* Check that Synchronization byte has been received on SPI */
    if (OPENBL_SPI_ReceiveByte(p_spix) == OPENBL_SPI_SYNC_BYTE)
    {
      detected = 1U;

      /* Enable the interrupt of Rx not empty buffer */
      OPENBL_SPI_EnableIT_RXP(p_spix);

      /* Send synchronization byte */
      OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_SPI_SYNC_BYTE);

      /* Send acknowledgment */
      OPENBL_ITF_SPI_SendAcknowledge(p_itf, OPENBL_ACK_BYTE);
    }
    else
    {
      detected = 0U;
    }
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
uint8_t OPENBL_ITF_SPI_GetCommandOpcode(openbl_itf_obj_t *p_itf)
{
  uint8_t command_opc;

  /* Wait until synchronization byte is received */
  while (OPENBL_ITF_SPI_ReadByte(p_itf) != OPENBL_SPI_SYNC_BYTE)
  {
    /* Nothing to do */
  }

  /* Get the command opcode */
  command_opc = OPENBL_ITF_SPI_ReadByte(p_itf);

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_ITF_SPI_ReadByte(p_itf)) != 0xFFU)
  {
    command_opc = OPENBL_ERROR_CMD;
  }

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from SPI pipe.
  *         Read operation is synchronized on SPI Rx buffer not empty interrupt.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns the read byte.
  */
OPENBL_RAM_FUNCTION uint8_t OPENBL_ITF_SPI_ReadByte(openbl_itf_obj_t *p_itf)
{
  uint8_t data;
  SPI_TypeDef *p_spix = (SPI_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Wait until SPI Rx buffer not empty interrupt */
  while (OPENBL_ITF_SPI_GetRxNotEmptyState() == 0U)
  {
    /* Refresh IWDG counter */
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);
  }

  /* Reset the RX not empty token */
  OPENBL_ITF_SPI_SetRxNotEmptyState(0U);

  /* Read the SPI data register */
  data = OPENBL_SPI_ReceiveByte(p_spix);

  /* Enable the interrupt of Rx not empty buffer */
  OPENBL_SPI_EnableIT_RXP(p_spix);

  return data;
}

/**
  * @brief  This function is used to send one busy byte each receive interrupt through SPI pipe.
  *         Read operation is synchronized on SPI Rx buffer not empty interrupt.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_SPI_SendBusyByte(openbl_itf_obj_t *p_itf)
{
  SPI_TypeDef *p_spix = (SPI_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Wait until SPI Rx buffer not empty interrupt */
  while (OPENBL_ITF_SPI_GetRxNotEmptyState() == 0U)
  {
    /* Refresh IWDG counter */
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);
  }

  /* Reset the RX not empty token */
  OPENBL_ITF_SPI_SetRxNotEmptyState(0U);

  /* Transmit the busy byte */
  OPENBL_SPI_TransmitByte(p_spix, OPENBL_SPI_BUSY_BYTE);

  /* Read bytes from the host to avoid the overrun */
  if (OPENBL_SPI_IsActiveFlag_OVR(p_spix) != 0U)
  {
    OPENBL_SPI_ClearFlag_OVR(p_spix);
  }

  /* Enable the interrupt of Rx not empty buffer */
  OPENBL_SPI_EnableIT_RXP(p_spix);
}

/**
  * @brief  This function is used to send one byte through SPI pipe.
  * @param  p_itf Pointer to the interface object.
  * @param  byte The byte to be sent.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_SPI_SendByte(openbl_itf_obj_t *p_itf, uint8_t byte)
{
  SPI_TypeDef *p_spix = (SPI_TypeDef *)p_itf->hw_context.p_itf_hw_context;

  /* Wait until SPI transmit buffer is empty */
  while (OPENBL_SPI_IsActiveFlag_TXP(p_spix) == 0U)
  {
    /* Nothing to do */
  }

  /* Transmit the data */
  OPENBL_SPI_TransmitByte(p_spix, byte);

  /* Clear underrun flag */
  if (OPENBL_SPI_IsActiveFlag_UDR(p_spix) != 0U)
  {
    OPENBL_SPI_ClearFlag_UDR(p_spix);
  }
}

/**
  * @brief  This function is used to send acknowledge byte through SPI pipe.
  * @param  p_itf Pointer to the interface object.
  * @param  byte The byte to be sent.
  * @retval None.
  */
void OPENBL_ITF_SPI_SendAcknowledge(openbl_itf_obj_t *p_itf, uint8_t byte)
{
  /* Check the AN4286 for details about SPI acknowledge procedure */
  if (byte == OPENBL_ACK_BYTE)
  {
    /* Send dummy byte */
    OPENBL_ITF_SPI_SendByte(p_itf, OPENBL_SPI_DUMMY_BYTE);
  }

  OPENBL_ITF_SPI_SendByte(p_itf, byte);

  /* Wait for the host to send ACK synchronization byte */
  while (OPENBL_ITF_SPI_ReadByte(p_itf) != OPENBL_ACK_BYTE)
  {
    /* Nothing to do */
  }
}

/**
  * @brief  Used to set the RX not empty state.
  * @param  state The Rx state to be set.
  * @retval None.
  */
OPENBL_RAM_FUNCTION void OPENBL_ITF_SPI_SetRxNotEmptyState(uint8_t state)
{
  SpiRxNotEmpty = state;
}

/**
  * @brief  Used to get the RX not empty state.
  * @retval Returns the Rx state.
  */
OPENBL_RAM_FUNCTION uint8_t OPENBL_ITF_SPI_GetRxNotEmptyState(void)
{
  return SpiRxNotEmpty;
}

/**
  * @brief  This function enables the send of busy state.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK void OPENBL_ITF_SPI_EnableBusyState(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_SPI_EnableBusyState" can be implemented in the user file.
   */

  /* Since underrun configuration is enabled, there is no specific code used to handle sending busy bytes */
  p_itf->send_busy_byte = OPENBL_ENABLE;
}

/**
  * @brief  This function disables the send of busy state.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK void OPENBL_ITF_SPI_DisableBusyState(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_SPI_DisableBusyState" can be implemented in the user file.
   */

  /* Since underrun configuration is enabled, there is no specific code used to handle sending busy bytes */
  p_itf->send_busy_byte = OPENBL_DISABLE;
}

/**
  * @brief  This function is used to process and execute the special commands.
  *         The user must define the special commands routine here.
  * @param  p_itf Pointer to the interface object.
  * @param  p_special_cmd Pointer to the @arg openbl_special_cmd_t structure.
  * @retval None.
  */
void OPENBL_ITF_SPI_SpecialCommandProcess(openbl_itf_obj_t *p_itf, openbl_special_cmd_t *p_special_cmd)
{
  switch (p_special_cmd->opcode)
  {
    case OPENBL_CMD_SPECIAL_COMMAND:
      if (p_special_cmd->type == OPENBL_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);

        /* Send NULL status size */
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);
      }
      break;

    case OPENBL_CMD_EXT_SPECIAL_COMMAND:
      if (p_special_cmd->type == OPENBL_EXTENDED_SPECIAL_CMD)
      {
        /* Send NULL data size */
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);

        /* Send NULL status size */
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);
        OPENBL_ITF_SPI_SendByte(p_itf, 0x00U);
      }
      break;

    /* Unknown command opcode */
    default:
      break;
  }
}

/**
  * @brief  This function is used to De-initialize the SPI pins and instance.
  * @retval None.
  */
__WEAK void openbl_user_spi_deinit(void)
{
  /** @warning This function must not be modified, when needed,
               the "openbl_user_spi_deinit" can be implemented in the user file.
   */
}
