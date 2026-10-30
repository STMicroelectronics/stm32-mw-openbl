/**
  ******************************************************************************
  * @file    openbl_i2c.h
  * @brief   Contains Open Bootloader I2C interface low level functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_I2C_H
#define OPENBL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Checks if the I2C address matched.
  * @param  p_i2c Pointer to the I2C instance.
  * @retval Returns 1 if address matched, 0 otherwise.
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_IsAddressMatched(I2C_TypeDef *p_i2c)
{
  return ((STM32_READ_BIT(p_i2c->ISR, I2C_ISR_ADDR) == (I2C_ISR_ADDR)) ? 1UL : 0UL);
}

/**
  * @brief  Checks if the I2C RX buffer is not empty.
  * @param  p_i2c Pointer to the I2C instance.
  * @retval Returns 1 if RX buffer is not empty, 0 otherwise.
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_IsRxNotEmpty(I2C_TypeDef *p_i2c)
{
  return (LL_I2C_IsActiveFlag_RXNE(p_i2c));
}

/**
  * @brief  Checks if the I2C TX buffer is empty.
  * @param  p_i2c Pointer to the I2C instance.
  * @retval Returns 1 if TX buffer is empty, 0 otherwise.
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_IsTxEmpty(I2C_TypeDef *p_i2c)
{
  return ((STM32_READ_BIT(p_i2c->ISR, I2C_ISR_TXIS) == (I2C_ISR_TXIS)) ? 1UL : 0UL);
}

/**
  * @brief  Read Receive data register.
  * @param  p_i2c I2C instance.
  * @retval Value between Min_Data=0x00 and Max_Data=0xFF
  */
__STATIC_FORCEINLINE uint8_t OPENBL_I2C_ReceiveByte(I2C_TypeDef *p_i2c)
{
  return LL_I2C_ReceiveData8(p_i2c);
}

/**
  * @brief  Write in Transmit data Register.
  * @param  p_i2c I2C instance.
  * @param  data Value between Min_Data=0x00 and Max_Data=0xFF
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_I2C_TransmitByte(I2C_TypeDef *p_i2c, uint8_t byte)
{
  STM32_WRITE_REG(p_i2c->TXDR, byte);
}

/**
  * @brief  Clear Address Matched flag.
  * @param  p_i2c I2C instance.
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_I2C_ClearAddressMatchedFlag(I2C_TypeDef *p_i2c)
{
  STM32_SET_BIT(p_i2c->ICR, I2C_ICR_ADDRCF);
}

/**
  * @brief  Indicate the status of Not Acknowledge received flag.
  * @param  p_i2c I2C instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_IsNackDetected(I2C_TypeDef *p_i2c)
{
  return (p_i2c->ISR & I2C_ISR_NACKF);
}

/**
  * @brief  Clear Not Acknowledge flag.
  * @param  p_i2c I2C instance.
  * @retval None.
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_ClearNackFlag(I2C_TypeDef *p_i2c)
{
  return (p_i2c->ICR |= I2C_ICR_NACKCF);
}

/**
  * @brief  Indicate the status of Stop detection flag.
  * @param  p_i2c I2C instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_IsStopDetected(I2C_TypeDef *p_i2c)
{
  return (p_i2c->ISR & I2C_ISR_STOPF);
}

/**
  * @brief  Clear Stop detection flag.
  * @param  p_i2c I2C instance.
  * @retval None.
  */
__STATIC_FORCEINLINE uint32_t OPENBL_I2C_ClearStopFlag(I2C_TypeDef *p_i2c)
{
  return (p_i2c->ICR |= I2C_ICR_STOPCF);
}

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_I2C_H */
