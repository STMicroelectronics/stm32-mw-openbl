/**
  ******************************************************************************
  * @file    openbl_interfaces.h
  * @brief   Header for openbl_interfaces.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_INTERFACES_H
#define OPENBL_INTERFACES_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "openbl_types.h"
#include "openbl_commands.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/**
  * @brief This structure represents the operations that can be performed on an interface.
  *
  * Each function pointer in this structure corresponds to a specific operation that can be performed on the interface,
  * such as initialization, de-initialization, detection, getting command opcode, sending acknowledgment,
  * and sending busy byte.
  * The actual implementation of these functions depends on the specific interface being used (e.g., USART, USB, etc.).
  */
typedef struct
{
  openbl_status_t (*Init)(openbl_itf_obj_t *p_itf);
  void (*DeInit)(openbl_itf_obj_t *p_itf);
  uint8_t (*Detection)(openbl_itf_obj_t *p_itf);
  uint8_t (*GetCommandOpcode)(openbl_itf_obj_t *p_itf);
  void (*SendAcknowledge)(openbl_itf_obj_t *p_itf, uint8_t acknowledge);
  void (*SendBusyByte)(openbl_itf_obj_t *p_itf);
} openbl_itf_ops_t;

/**
  * @brief This structure represents an interface object.
  *
  * The actual implementation depends on the specific interface being used (e.g., USART, USB, etc.).
  */
struct openbl_itf_obj_s
{
  openbl_hw_context_t hw_context;                           /*!< The hardware context of the interface */
  uint32_t itf_type;                                        /*!< The type of the interface (e.g., USART, USB, etc.) */
  uint32_t detected;                                        /*!< The detection status of the interface */
  openbl_state_t send_busy_byte;                            /*!< The state of the send busy byte feature */
  openbl_itf_ops_t *p_ops;                                  /*!< A pointer to the operations structure */
  openbl_commands_t *p_cmd;                                 /*!< A pointer to the commands structure */
  openbl_cmd_opcodes_list_t cmd_opcodes_list;               /*!< The list of supported command opcodes */
  openbl_cmd_opcodes_list_t special_cmd_opcodes_list;       /*!< The list of supported special command opcodes */
  openbl_cmd_opcodes_list_t ext_special_cmd_opcodes_list;   /*!< The list of supported extended special command opcodes */
  openbl_buffer_t buffer;                                   /*!< The buffer used for data transmission and reception */
  openbl_mem_list_t *p_mem_list;                            /*!< A pointer to the memory list */
  void *p_user_data;                                        /*!< A pointer to any additional user data */
};

/**
  * @brief This structure represents a list of supported interfaces.
  *
  * The maximum number of supported interfaces is defined by the @arg OPENBL_INTERFACES_NUMBER constant.
  *
  * This structure is used to manage multiple interfaces in the Open Bootloader application,
  * allowing for flexibility in supporting different communication protocols.
  */
typedef struct
{
  uint32_t interfaces_nbr;                                /*!< The number of supported interfaces */
  openbl_itf_obj_t interfaces[OPENBL_INTERFACES_NUMBER];  /*!< An array of interface objects */
} openbl_itf_list_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_INTERFACES_H */
