/**
  **********************************************************************************************************************
  * @file    openbl_commands.h
  * @brief   This file provides Open Bootloader exported commands definitions.
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
#ifndef OPENBL_COMMANDS_H
#define OPENBL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_conf.h"
#include "openbl_types.h"
#include "openbl_mem.h"

/* Exported types ------------------------------------------------------------*/
typedef struct openbl_itf_obj_s openbl_itf_obj_t;

/**
  * @brief This structure represents a list of command opcodes.
  */
typedef struct
{
  uint16_t number;            /*!< The number of supported command opcodes */
  uint8_t *p_opcodes;         /*!< A pointer to the array of supported command opcodes */
} openbl_cmd_opcodes_list_t;

/**
  * @brief This structure represents the list of available commands for a specific interface.
  *
  * The function pointers in this structure correspond to the different commands that can be executed on the interface,
  * such as GetCommand, GetVersion, GetID, ReadMemory, WriteMemory...
  * The actual implementation of these functions depends on the specific interface being used (e.g., USART, USB, etc.).
  */
typedef struct
{
  void (*GetCommand)(openbl_itf_obj_t *p_itf);
  void (*GetVersion)(openbl_itf_obj_t *p_itf);
  void (*GetID)(openbl_itf_obj_t *p_itf);
  void (*ReadMemory)(openbl_itf_obj_t *p_itf);
  void (*WriteMemory)(openbl_itf_obj_t *p_itf);
  void (*Go)(openbl_itf_obj_t *p_itf);
  void (*ReadoutProtect)(openbl_itf_obj_t *p_itf);
  void (*ReadoutUnprotect)(openbl_itf_obj_t *p_itf);
  void (*EraseMemory)(openbl_itf_obj_t *p_itf);
  void (*WriteProtect)(openbl_itf_obj_t *p_itfst);
  void (*WriteUnprotect)(openbl_itf_obj_t *p_itf);
  void (*NsWriteMemory)(openbl_itf_obj_t *p_itf);
  void (*NsEraseMemory)(openbl_itf_obj_t *p_itf);
  void (*NsWriteProtect)(openbl_itf_obj_t *p_itf);
  void (*NsWriteUnprotect)(openbl_itf_obj_t *p_itf);
  void (*NsReadoutProtect)(openbl_itf_obj_t *p_itf);
  void (*NsReadoutUnprotect)(openbl_itf_obj_t *p_itf);
  void (*Speed)(openbl_itf_obj_t *p_itf);
  void (*SpecialCommand)(openbl_itf_obj_t *p_itf);
  void (*ExtendedSpecialCommand)(openbl_itf_obj_t *p_itf);
} openbl_commands_t;

/**
  * @brief This enumeration represents the type of a special command.
  */
typedef enum
{
  OPENBL_SPECIAL_CMD                        = 0x1U,           /*!< Special Command */
  OPENBL_EXTENDED_SPECIAL_CMD               = 0x2U            /*!< Extended Special Command */
} openbl_special_cmd_type_t;

/**
  * @brief This structure represents a special command.
  */
typedef struct
{
  openbl_special_cmd_type_t type;                             /*!< The type of the special command */
  uint16_t opcode;                                            /*!< The opcode of the special command */
  uint16_t size_buffer_1;                                     /*!< The size of the first buffer */
  uint8_t buffer_1[OPENBL_SPECIAL_CMD_SIZE_BUFFER1];          /*!< The first buffer */
  uint16_t size_buffer_2;                                     /*!< The size of the second buffer */
  uint8_t buffer_2[OPENBL_SPECIAL_CMD_SIZE_BUFFER2];          /*!< The second buffer */
} openbl_special_cmd_t;

/* Exported constants --------------------------------------------------------*/
#define OPENBL_ERROR_CMD                    0xECU             /*!< Error command */
#define OPENBL_ACK_BYTE                     0x79U             /*!< Acknowledge Byte ID */
#define OPENBL_NACK_BYTE                    0x1FU             /*!< No Acknowledge Byte ID */
#define OPENBL_BUSY_BYTE                    0x76U             /*!< Busy Byte */
#define OPENBL_SYNC_BYTE                    0xA5U             /*!< Synchronization byte */

#define OPENBL_FLASH_MASS_ERASE             0xFFFFU
#define OPENBL_FLASH_BANK1_ERASE            0xFFFEU
#define OPENBL_FLASH_BANK2_ERASE            0xFFFDU

/* ---------------------- Open Bootloader Commands ---------------------------*/
#define OPENBL_CMD_GET_COMMAND              0x00U             /*!< Get commands command */
#define OPENBL_CMD_GET_VERSION              0x01U             /*!< Get Version command */
#define OPENBL_CMD_GET_ID                   0x02U             /*!< Get ID command */
#define OPENBL_CMD_SPEED                    0x03U             /*!< Speed command */
#define OPENBL_CMD_READ_MEMORY              0x11U             /*!< Read Memory command */
#define OPENBL_CMD_WRITE_MEMORY             0x31U             /*!< Write Memory command */
#define OPENBL_CMD_GO                       0x21U             /*!< GO command */
#define OPENBL_CMD_READ_PROTECT             0x82U             /*!< Readout Protect command */
#define OPENBL_CMD_READ_UNPROTECT           0x92U             /*!< Readout Unprotect command */
#define OPENBL_CMD_LEG_ERASE_MEMORY         0x43U             /*!< Erase Memory command */
#define OPENBL_CMD_EXT_ERASE_MEMORY         0x44U             /*!< Erase Memory command */
#define OPENBL_CMD_WRITE_PROTECT            0x63U             /*!< Write Protect command */
#define OPENBL_CMD_WRITE_UNPROTECT          0x73U             /*!< Write Unprotect command */
#define OPENBL_CMD_NS_WRITE_MEMORY          0x32U             /*!< No Stretch Write Memory command */
#define OPENBL_CMD_NS_ERASE_MEMORY          0x45U             /*!< No Stretch Erase Memory command */
#define OPENBL_CMD_NS_WRITE_PROTECT         0x64U             /*!< No Stretch Write Protect command */
#define OPENBL_CMD_NS_WRITE_UNPROTECT       0x74U             /*!< No Stretch Write Unprotect command */
#define OPENBL_CMD_NS_READ_PROTECT          0x83U             /*!< No Stretch Read Protect command */
#define OPENBL_CMD_NS_READ_UNPROTECT        0x93U             /*!< No Stretch Read Unprotect command */
#define OPENBL_CMD_SPECIAL_COMMAND          0x50U             /*!< Special Command command */
#define OPENBL_CMD_EXT_SPECIAL_COMMAND      0x51U             /*!< Extended Special Command command */
#define OPENBL_CMD_CHECKSUM                 0xA1U             /*!< Checksum command */

#define OPENBL_DFU_CMD_GET_COMMANDS         0x00U             /*!< Get commands command opcode */
#define OPENBL_DFU_CMD_SET_ADDRESS_POINTER  0x21U             /*!< Set address pointer command opcode */
#define OPENBL_DFU_CMD_ERASE                0x41U             /*!< Erase command opcode */
#define OPENBL_DFU_CMD_READ_PROTECT         0x91U             /*!< Read protect command opcode */
#define OPENBL_DFU_CMD_READ_UNPROTECT       0x92U             /*!< Read unprotect command opcode */
#define OPENBL_DFU_CMD_WRITE_PROTECT        0x81U             /*!< Write protect command opcode */
#define OPENBL_DFU_CMD_WRITE_UNPROTECT      0x82U             /*!< Write unprotect command opcode */

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_COMMANDS_H */
