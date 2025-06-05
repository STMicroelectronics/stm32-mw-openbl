/**
  ******************************************************************************
  * @file    i2c_interface.c
  * @author  MCD Application Team
  * @brief   Contains I2C HW configuration
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
#include "stm32g0xx.h"
#include "stm32g0xx_ll_i2c.h"
#include "openbl_core.h"
#include "openbl_i2c_cmd.h"
#include "i2c_interface.h"
#include "iwdg_interface.h"
#include "interfaces_conf.h"
#include "flash_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void OPENBL_I2C_Init(void);

/* Private functions ---------------------------------------------------------*/

static void OPENBL_I2C_Init(void)
{
  LL_I2C_InitTypeDef I2C_InitStruct;

  I2C_InitStruct.PeripheralMode      = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing              = I2C_TIMING;
  I2C_InitStruct.AnalogFilter        = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter       = 0U;
  I2C_InitStruct.OwnAddress1         = I2C_ADDRESS;
  I2C_InitStruct.TypeAcknowledge     = LL_I2C_NACK;
  I2C_InitStruct.OwnAddrSize         = LL_I2C_OWNADDRESS1_7BIT;

  LL_I2C_Init(I2Cx, &I2C_InitStruct);
  LL_I2C_Enable(I2Cx);
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure I2C pins and then initialize the used I2C instance.
 * @retval None.
 */
void OPENBL_I2C_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all resources clocks --------------------------------------------*/
  /* Enable used GPIOx clocks */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Enable I2C clock */
  __HAL_RCC_I2C1_CLK_ENABLE();

  GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = I2Cx_ALTERNATE;
  HAL_GPIO_Init(I2Cx_SCL_PIN_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = I2Cx_SDA_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = I2Cx_ALTERNATE;
  HAL_GPIO_Init(I2Cx_SDA_PIN_PORT, &GPIO_InitStruct);

  OPENBL_I2C_Init();
}

/**
 * @brief  This function is used to detect if there is any activity on I2C protocol.
 * @retval None.
 */
uint8_t OPENBL_I2C_ProtocolDetection(void)
{
  uint8_t detected;

  /* Check if the I2Cx is addressed */
  if ((I2Cx->ISR & I2C_ISR_ADDR) != 0)
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
 * @retval Returns the command.
 */
uint8_t OPENBL_I2C_GetCommandOpcode(void)
{
  uint8_t command_opc = 0x0U;

  OPENBL_I2C_WaitAddress();

  /* Get the command opcode */
  command_opc = OPENBL_I2C_ReadByte();

  /* Check the data integrity */
  if ((command_opc ^ OPENBL_I2C_ReadByte()) != 0xFFU)
  {
    command_opc = ERROR_COMMAND;
  }

  OPENBL_I2C_WaitStop();

  return command_opc;
}

/**
  * @brief  This function is used to read one byte from I2C pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_I2C_ReadByte(void)
{
  uint32_t timeout = 0U;

  while (LL_I2C_IsActiveFlag_RXNE(I2Cx) == 0)
  {
    OPENBL_IWDG_Refresh();

    if ((timeout++) >= OPENBL_I2C_TIMEOUT)
    {
      /* System Reset */
      NVIC_SystemReset();
    }
  }

  return LL_I2C_ReceiveData8(I2Cx);
}

/**
  * @brief  This function is used to send one byte through I2C pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_I2C_SendByte(uint8_t Byte)
{
  uint32_t timeout = 0U;

  if (LL_I2C_IsActiveFlag_TXIS(I2Cx) == 0)
  {
    while (LL_I2C_IsActiveFlag_TXIS(I2Cx) == 0)
    {
      OPENBL_IWDG_Refresh();

      if ((timeout++) >= OPENBL_I2C_TIMEOUT)
      {
        /* System Reset */
        NVIC_SystemReset();
      }
    }
  }

  LL_I2C_TransmitData8(I2Cx, Byte);
}

/**
  * @brief  This function is used to wait until the address is matched.
  * @retval None.
  */
void OPENBL_I2C_WaitAddress(void)
{
  uint32_t timeout = 0U;

  while (LL_I2C_IsActiveFlag_ADDR(I2Cx) == 0)
  {
    OPENBL_IWDG_Refresh();

    if ((timeout++) >= OPENBL_I2C_TIMEOUT)
    {
      /* System Reset */
      NVIC_SystemReset();
    }
  }

  LL_I2C_ClearFlag_ADDR(I2Cx);
}

/**
  * @brief  This function is used to wait until NACK is detected.
  * @retval None.
  */
#if defined (__CC_ARM)
void OPENBL_I2C_WaitNack(void)
#else
__ramfunc void OPENBL_I2C_WaitNack(void)
#endif /* (__CC_ARM) */
{
  uint32_t timeout = 0U;

  /* While the i2C NACK is not detected, the IWDG is refreshed,
  if the timeout is reached a system reset occurs */
  while ((I2Cx->ISR & I2C_ISR_NACKF) == 0)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;

    if ((timeout++) >= OPENBL_I2C_TIMEOUT)
    {
      /* System Reset */
      SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                     SCB_AIRCR_SYSRESETREQ_Msk);
    }
  }

  /* Clear Not Acknowledge detection flag. */
  I2Cx->ICR |= I2C_ICR_NACKCF;
}

/**
  * @brief  This function is used to wait until STOP is detected.
  * @retval None.
  */
#if defined (__CC_ARM)
void OPENBL_I2C_WaitStop(void)
#else
__ramfunc void OPENBL_I2C_WaitStop(void)
#endif /* (__CC_ARM) */
{
  uint32_t timeout = 0U;

  /* While the i2C stop is not detected, refresh the IWDG,
  if the timeout is reached a system reset occurs */
  while ((I2Cx->ISR & I2C_ISR_STOPF) == 0)
  {
    /* Refresh IWDG: reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;

    if ((timeout++) >= OPENBL_I2C_TIMEOUT)
    {
      /* System Reset */
      SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                     SCB_AIRCR_SYSRESETREQ_Msk);
    }
  }

  /* Clear Stop detection flag. */
  I2Cx->ICR |= I2C_ICR_STOPCF;
}

/**
  * @brief  This function is used to send Acknowledgment.
  * @retval None.
  */
void OPENBL_I2C_SendAcknowledgeByte(uint8_t Byte)
{
  /* Wait until address is matched */
  OPENBL_I2C_WaitAddress();

  /* Send ACK or NACK byte */
  OPENBL_I2C_SendByte(Byte);

  /* Wait until NACK is detected */
  OPENBL_I2C_WaitNack();

  /* Wait until STOP byte is detected*/
  OPENBL_I2C_WaitStop();
}

/**
  * @brief  This function is used to send busy byte through I2C pipe.
  * @param
  * @retval None.
  */
#if defined (__CC_ARM)
void OPENBL_I2C_SendBusyByte(void)
#else
__ramfunc void OPENBL_I2C_SendBusyByte(void)
#endif /* (__CC_ARM) */
{
  uint32_t timeout = 0;

  /* Wait for the received address to match with the device address */
  if (((I2Cx->ISR & I2C_ISR_ADDR) != 0))
  {
    /* Clear the flag of address match*/
    I2Cx->ICR |= I2C_ICR_ADDRCF;

    /* While the transmit data is not empty, refresh the IWDG,
    if the timeout is reached a system reset occurs */
    while ((I2Cx->ISR & I2C_ISR_TXIS) == 0)
    {
      IWDG->KR = IWDG_KEY_RELOAD;

      if ((timeout++) >= OPENBL_I2C_TIMEOUT)
      {
        /* System Reset */
        SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                       SCB_AIRCR_SYSRESETREQ_Msk);
      }
    }

    /* Send busy byte */
    I2Cx->TXDR = BUSY_BYTE;
  }

  /* Wait until NACK is detected */
  OPENBL_I2C_WaitNack();

  /* Wait until STOP byte is detected*/
  OPENBL_I2C_WaitStop();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
