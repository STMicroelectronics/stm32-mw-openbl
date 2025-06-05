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
}

/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure I2C pins and then initialize the used I2C instance.
 * @retval None.
 */
void OPENBL_I2C_Configuration(void)
{
}

/**
 * @brief  This function is used to detect if there is any activity on I2C protocol.
 * @retval None.
 */
uint8_t OPENBL_I2C_ProtocolDetection(void)
{
  uint8_t detected;
  return detected;
}

/**
 * @brief  This function is used to get the command opcode from the host.
 * @retval Returns the command.
 */
uint8_t OPENBL_I2C_GetCommandOpcode(void)
{
  uint8_t command_opc = 0x0U;
  return command_opc;
}

/**
  * @brief  This function is used to read one byte from I2C pipe.
  * @retval Returns the read byte.
  */
uint8_t OPENBL_I2C_ReadByte(void)
{
  return LL_I2C_ReceiveData8(I2Cx);
}

/**
  * @brief  This function is used to send one byte through I2C pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_I2C_SendByte(uint8_t Byte)
{
}

/**
  * @brief  This function is used to wait until the address is matched.
  * @retval None.
  */
void OPENBL_I2C_WaitAddress(void)
{
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
}

/**
  * @brief  This function is used to send Acknowledgment.
  * @retval None.
  */
void OPENBL_I2C_SendAcknowledgeByte(uint8_t Byte)
{
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
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
