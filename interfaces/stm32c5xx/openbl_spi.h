/**
  ******************************************************************************
  * @file    openbl_spi.h
  * @brief   Contains Open Bootloader SPI interface low level functions.
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
#ifndef OPENBL_SPI_H
#define OPENBL_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "openbl_itf_spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Enable SPI peripheral.
  * @param  p_spix SPI Instance to be enabled.
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_SPI_Enable(SPI_TypeDef *p_spix)
{
  LL_SPI_Enable(p_spix);
}

/**
  * @brief  Check if SPI peripheral is enabled.
  * @param  p_spix SPI Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_SPI_IsEnabled(SPI_TypeDef *p_spix)
{
  return LL_SPI_IsEnabled(p_spix);
}

/**
  * @brief  Clear overrun error flag.
  * @note   Clearing this flag is done by a read access to the SPIx_DR
  *         register followed by a read access to the SPIx_SR register.
  * @retval None.
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE void OPENBL_SPI_ClearFlag_OVR(SPI_TypeDef *p_spix)
{
  STM32_SET_BIT(p_spix->IFCR, SPI_IFCR_OVRC);
}

/**
  * @brief  Clear underrun error flag.
  * @param  p_spix SPI Instance.
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE void OPENBL_SPI_ClearFlag_UDR(SPI_TypeDef *p_spix)
{
  STM32_SET_BIT(p_spix->IFCR, SPI_IFCR_UDRC);
}

/**
  * @brief  Check if there is enough data in FIFO to read a full packet.
  * @param  p_spix SPI Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_SPI_IsActiveFlag_RXP(const SPI_TypeDef *p_spix)
{
  return LL_SPI_IsActiveFlag_RXP(p_spix);
}

/**
  * @brief  Get underrun error flag.
  * @param  p_spix SPI Instance.
  * @retval State of bit (1 or 0).
  */
__STATIC_FORCEINLINE uint32_t OPENBL_SPI_IsActiveFlag_UDR(const SPI_TypeDef *p_spix)
{
  return ((STM32_READ_BIT(p_spix->SR, SPI_SR_UDR) == (SPI_SR_UDR)) ? 1UL : 0UL);
}

/**
  * @brief  Get overrun error flag.
  * @param  p_spix SPI Instance.
  * @retval State of bit (1 or 0).
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE uint32_t OPENBL_SPI_IsActiveFlag_OVR(const SPI_TypeDef *p_spix)
{
  return ((STM32_READ_BIT(p_spix->SR, SPI_SR_OVR) == (SPI_SR_OVR)) ? 1UL : 0UL);
}

/**
  * @brief  Check if there is enough space in FIFO to hold a full packet.
  * @param  p_spix SPI Instance.
  * @retval State of bit (1 or 0).
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE uint32_t OPENBL_SPI_IsActiveFlag_TXP(const SPI_TypeDef *p_spix)
{
  return ((STM32_READ_BIT(p_spix->SR, SPI_SR_TXP) == (SPI_SR_TXP)) ? 1UL : 0UL);
}

/**
  * @brief  Read Data Register.
  * @param  p_spix SPI Instance.
  * @retval Returns the received byte value.
  */
__STATIC_FORCEINLINE uint8_t OPENBL_SPI_ReceiveByte(SPI_TypeDef *p_spix)
{
  return (*((__IM uint8_t *)&p_spix->RXDR));
}

/**
  * @brief  Write Data Register.
  * @param  p_spix SPI Instance
  * @param  byte The byte to be transmitted.
  * @retval None.
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE void OPENBL_SPI_TransmitByte(SPI_TypeDef *p_spix, uint8_t byte)
{
  *((__IO uint8_t *)&p_spix->TXDR) = byte;
}

/**
  * @brief  Enable Rx Packet available IT.
  * @param  p_spix SPI Instance.
  * @retval None.
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE void OPENBL_SPI_EnableIT_RXP(SPI_TypeDef *p_spix)
{
  STM32_SET_BIT(p_spix->IER, SPI_IER_RXPIE);
}

/**
  * @brief  Disable Rx Packet available IT.
  * @param  p_spix SPI Instance
  * @retval None.
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE void OPENBL_SPI_DisableIT_RXP(SPI_TypeDef *p_spix)
{
  STM32_ATOMIC_CLEAR_BIT_32(p_spix->IER, SPI_IER_RXPIE);
}

/**
  * @brief  Set the underrun pattern.
  * @param  p_spix SPI Instance.
  * @param  pattern 0..0xFFFFFFFF.
  * @retval None.
  */
__STATIC_FORCEINLINE void OPENBL_SPI_SetUnderRunPattern(SPI_TypeDef *p_spix, uint32_t pattern)
{
  LL_SPI_SetUDRPattern(p_spix, pattern);
}

/**
  * @brief  Handle SPI interrupt request.
  * @param  p_spix SPI Instance.
  * @retval None.
  */
OPENBL_RAM_FUNCTION __STATIC_FORCEINLINE void OPENBL_SPI_IRQHandler(SPI_TypeDef *p_spix)
{
  uint32_t overrun_flag;
  uint32_t read_packet_avaiable;
  uint32_t read_packet_interrpt_enable;

  /* Read SPIx Status Register flags */
  overrun_flag                = p_spix->SR & SPI_SR_OVR;
  read_packet_avaiable        = p_spix->SR & SPI_SR_RXP;
  read_packet_interrpt_enable = p_spix->IER & SPI_IER_RXPIE;

  /* Check that SPIx Rx buffer not empty interrupt has been raised */
  if ((overrun_flag                   != SPI_SR_OVR)
      && (read_packet_avaiable        == SPI_SR_RXP)
      && (read_packet_interrpt_enable == SPI_IER_RXPIE))
  {
    /* Set the RX not empty token */
    OPENBL_ITF_SPI_SetRxNotEmptyState(1U);

    /* Disable the interrupt of Rx not empty buffer */
    OPENBL_SPI_DisableIT_RXP(p_spix);
  }

  if ((overrun_flag                   == SPI_SR_OVR)
      && (read_packet_avaiable        == SPI_SR_RXP)
      && (read_packet_interrpt_enable == SPI_IER_RXPIE))
  {
    /* Read bytes from the host to avoid the overrun */
    OPENBL_SPI_ClearFlag_OVR(p_spix);
  }
}

#ifdef __cplusplus
}
#endif

#endif /* OPENBL_SPI_H */
