/**
  **********************************************************************************************************************
  * @file    openbl_types.h
  * @brief   This file provides Open Bootloader exported types definitions.
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_TYPES_H
#define OPENBL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/**
  * @brief This enumeration represents the state of a given feature.
  */
typedef enum
{
  OPENBL_DISABLE                          = 0xDADADADAU,  /*!< The feature is disabled */
  OPENBL_ENABLE                           = 0xEAEAEAEAU   /*!< The feature is enabled */
} openbl_state_t;

/**
  * @brief This enumeration represents the status of an Open Bootloader operation.
  */
typedef enum
{
  OPENBL_SUCCESS                          = 0xAEAEAEAEU,  /*!< The operation was successful */
  OPENBL_ERROR                            = 0xF5F5F5F5U   /*!< An error occurred during the operation */
} openbl_status_t;


/**
  * @brief This structure represents the hardware context of an interface.
  */

typedef struct
{
  void *p_itf_hw_context;     /*!< A pointer to the hardware-specific context of the interface */
  void *p_data;               /*!< A pointer to any additional data associated with the hardware context */
} openbl_hw_context_t;

/**
  * @brief This structure represents a buffer used for data transmission and reception.
  */
typedef struct
{
  uint32_t max_length;        /*!< The maximum length of the buffer */
  uint8_t *p_data;            /*!< A pointer to the actual data buffer */
} openbl_buffer_t;

/* Exported macros -----------------------------------------------------------*/
#if defined (__ICCARM__)
#define OPENBL_RAM_FUNCTION               __ramfunc
#else
#define OPENBL_RAM_FUNCTION               __attribute__((section("__ramfunc")))
#endif /* (__ICCARM__) */

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_TYPES_H */
