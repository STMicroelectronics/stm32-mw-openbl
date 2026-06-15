/**
  ******************************************************************************
  * @file    openbl_mem.h
  * @brief   Header for openbl_mem.c module
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
#ifndef OPENBL_MEM_H
#define OPENBL_MEM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "openbl_conf.h"
#include "openbl_types.h"

/* Exported types ------------------------------------------------------------*/
typedef struct openbl_mem_s openbl_mem_t;

/**
  * @brief This structure represents a memory descriptor.
  */
typedef struct
{
  uint32_t start_address;      /*!< The start address of the memory region */
  uint32_t end_address;        /*!< The end address of the memory region */
  uint32_t size;               /*!< The size of the memory region */
  uint32_t type;               /*!< The type of the memory region */
  uint32_t program_data_size;  /*!< The size of the data to be programmed */
  uint32_t program_timeout;    /*!< The timeout for programming operations */
  uint32_t erase_timeout;      /*!< The timeout for erase operations */
} openbl_mem_desc_t;

/**
  * @brief This structure represents the operations that can be performed on a memory.
  *
  * Each function pointer in this structure corresponds to a specific operation that can be performed on the memory,
  * such as reading, writing, erasing, setting write protection, etc.
  *
  * The actual implementation of these functions depends on the specific memory being used (e.g., internal flash,
  * external flash, etc.).
  */
typedef struct
{
  uint8_t (*Read)(openbl_mem_t *p_mem, uint32_t address);
  void (*Write)(openbl_mem_t *p_mem, uint32_t address, uint8_t *p_data, uint32_t data_length);
  void (*JumpToAddress)(openbl_mem_t *p_mem, uint32_t address);
  openbl_status_t(*MassErase)(openbl_mem_t *p_mem, uint8_t *p_data, uint32_t data_length);
  openbl_status_t(*Erase)(openbl_mem_t *p_mem, uint8_t *p_data, uint32_t data_length);
  void (*SetReadoutProtect)(openbl_mem_t *p_mem, openbl_state_t State);
  openbl_status_t(*SetWriteProtect)(openbl_mem_t *p_mem, openbl_state_t State, uint8_t *p_buffer, uint32_t Length);
  void (*PostProcessing)(openbl_mem_t *p_mem);
} openbl_mem_ops_t;

/**
  * @brief This structure represents a memory instance.
  */
struct openbl_mem_s
{
  openbl_hw_context_t hw_context; /*!< Hardware context for the memory instance */
  openbl_mem_desc_t mem_desc;     /*!< Memory descriptor */
  openbl_mem_ops_t mem_ops;       /*!< Memory operations */
  void *p_parent;                 /*!< Pointer to the parent object */
};

/**
  * @brief This structure represents a list of memory instances.
  */
typedef struct
{
  uint32_t memories_nbr;                          /*!< Number of memory instances */
  openbl_mem_t memories[OPENBL_MEMORIES_NUMBER];  /*!< Array of memory instances */
} openbl_mem_list_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_MEM_AssociateDetectedInterfaceToMemories(void *p_parent, openbl_mem_list_t *p_mem_list);
void OPENBL_MEM_JumpToAddress(openbl_mem_list_t *p_mem_list, uint32_t address);
void OPENBL_MEM_SetReadOutProtection(openbl_mem_list_t *p_mem_list, uint32_t address, openbl_state_t state);
void OPENBL_MEM_Write(openbl_mem_list_t *p_mem_list, uint32_t address, uint8_t *p_data, uint32_t data_length);

void OPENBL_MEM_PostProcessing(openbl_mem_list_t *p_mem_list, uint32_t address);

uint8_t OPENBL_MEM_Read(openbl_mem_t *p_mem, uint32_t address);
uint32_t OPENBL_MEM_GetAreaFromAddress(openbl_mem_list_t *p_mem_list, uint32_t address);
uint32_t OPENBL_MEM_GetMemoryIndex(openbl_mem_list_t *p_mem_list, uint32_t address);
openbl_mem_t *OPENBL_MEM_GetMemoryFromAddress(openbl_mem_list_t *p_mem_list, uint32_t address);
uint8_t OPENBL_MEM_CheckJumpAddress(openbl_mem_list_t *p_mem_list, uint32_t address);

openbl_status_t OPENBL_MEM_Erase(openbl_mem_list_t *p_mem_list, uint32_t address, uint8_t *p_data,
                                 uint32_t data_length);
openbl_status_t OPENBL_MEM_MassErase(openbl_mem_list_t *p_mem_list, uint32_t address, uint8_t *p_data,
                                     uint32_t data_length);
openbl_status_t OPENBL_MEM_SetWriteProtection(openbl_mem_list_t *p_mem_list, openbl_state_t state, uint32_t address,
                                              uint8_t *p_buffer, uint32_t length);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_MEM_H */
