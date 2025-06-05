/**
  ******************************************************************************
  * @file    interfaces_conf.h
  * @author  MCD Application Team
  * @brief   Contains Interfaces configuration
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACES_CONF_H
#define INTERFACES_CONF_H

#define MEMORIES_SUPPORTED                7U

/* ------------------------- Definitions for USART -------------------------- */
#define USARTx                            USART1

#define USARTx_TX_PIN                     GPIO_PIN_9
#define USARTx_TX_GPIO_PORT               GPIOA
#define USARTx_RX_PIN                     GPIO_PIN_10
#define USARTx_RX_GPIO_PORT               GPIOA
#define USARTx_ALTERNATE                  GPIO_AF1_USART1

/* ------------------------- Definitions for FDCAN -------------------------- */
#define FDCANx                            FDCAN1

#define FDCANx_TX_PIN                     GPIO_PIN_13
#define FDCANx_TX_GPIO_PORT               GPIOD
#define FDCANx_TX_AF                      GPIO_AF3_FDCAN1
#define FDCANx_RX_PIN                     GPIO_PIN_12
#define FDCANx_RX_GPIO_PORT               GPIOD
#define FDCANx_RX_AF                      GPIO_AF3_FDCAN1

#define FDCANx_IT0_IRQn                   TIM16_FDCAN_IT0_IRQn
#define FDCANx_IT1_IRQn                   TIM17_FDCAN_IT1_IRQn
#define FDCANx_IRQHandler                 TIM16_FDCAN_IT0_IRQHandler

#define FDCANx_FORCE_RESET()              __HAL_RCC_FDCAN_FORCE_RESET()
#define FDCANx_RELEASE_RESET()            __HAL_RCC_FDCAN_RELEASE_RESET()

/* ------------------------- Definitions for I2C -------------------------- */

#define I2Cx                              I2C1

#define I2Cx_SCL_PIN                      GPIO_PIN_6
#define I2Cx_SCL_PIN_PORT                 GPIOB
#define I2Cx_SDA_PIN                      GPIO_PIN_7
#define I2Cx_SDA_PIN_PORT                 GPIOB
#define I2Cx_ALTERNATE                    GPIO_AF6_I2C1;
#define I2C_ADDRESS                       0x000000BAU
#define OPENBL_I2C_TIMEOUT                0x001F4000U
#define I2C_TIMING                        0x00800000U

#endif /* INTERFACES_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
