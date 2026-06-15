/**
  ******************************************************************************
  * @file    openbl_usart.h
  * @brief   Contains Open Bootloader USART low level functions.
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
#ifndef OPENBL_USART_H
#define OPENBL_USART_H

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
  * @brief  USART Enable.
  * @param  p_usart USART Instance.
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_USART_Enable(USART_TypeDef *p_usart)
{
  LL_USART_Enable(p_usart);
}

/**
  * @brief  Indicates if USART is enabled.
  * @param  p_usart USART Instance.
  * @retval State of bit (1 if enabled / 0 if disabled).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_USART_IsEnabled(USART_TypeDef *p_usart)
{
  return LL_USART_IsEnabled(p_usart);
}

/**
  * @brief  Indicates if auto baud rate is detected.
  * @param  p_usart USART Instance.
  * @retval State of auto baud rate detection (1 if detected / 0 if not detected).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_USART_IsAutoBaudRateDetected(USART_TypeDef *p_usart)
{
  if (LL_USART_IsActiveFlag_ABR(p_usart) == 0U)
  {
    return 0U;
  }

  if (LL_USART_IsActiveFlag_ABRE(p_usart) != 0U)
  {
    return 0U;
  }

  return 1U;
}

/**
  * @brief  Check whether the USART Read Data Register or USART RX FIFO Not Empty Flag is set.
  * @param  p_usart USART Instance.
  * @retval State of bit (1 if not empty / 0 if empty).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_USART_IsRxFifoNotEmpty(USART_TypeDef *p_usart)
{
  return LL_USART_IsActiveFlag_RXNE_RXFNE(p_usart);
}

/**
  * @brief  Read Receiver Data register (Receive Data value, 8 bits).
  * @param  p_usart USART Instance.
  * @retval Value between Min_Data=0x00 and Max_Data=0xFF
  */
__STATIC_FORCEINLINE uint8_t OPENBL_USART_ReceiveByte(USART_TypeDef *p_usart)
{
  return LL_USART_ReceiveData8(p_usart);
}

/**
  * @brief  Write in Transmitter Data Register (Transmit Data value, 8 bits).
  * @param  p_usart USART Instance.
  * @param  byte Value between Min_Data=0x00 and Max_Data=0xFF.
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_USART_TransmitByte(USART_TypeDef *p_usart, uint8_t byte)
{
  LL_USART_TransmitData8(p_usart, byte);
}

/**
  * @brief  Check whether the USART Transmission Complete Flag is set.
  * @param  p_usart USART Instance.
  * @retval State of bit (1 if complete / 0 if not complete).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_USART_IsTransmissionComplete(USART_TypeDef *p_usart)
{
  return LL_USART_IsActiveFlag_TC(p_usart);
}

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_USART_H */
