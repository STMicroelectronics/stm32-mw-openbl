/**
  ******************************************************************************
  * @file    openbl_core.h
  * @author  MCD Application Team
  * @brief   Header for openbl_core.c module
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
#ifndef OPENBL_CORE_H
#define OPENBL_CORE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "openbootloader_conf.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  void (*Init)(void);
  void (*DeInit)(void);
  uint8_t (*Detection)(void);
  uint8_t (*GetCommandOpcode)(void);
  void (*SendByte)(uint8_t Byte);
} OPENBL_OpsTypeDef;

typedef struct
{
  void (*GetCommand)(void);
  void (*GetVersion)(void);
  void (*GetID)(void);
  void (*ReadMemory)(void);
  void (*WriteMemory)(void);
  void (*Go)(void);
  void (*ReadoutProtect)(void);
  void (*ReadoutUnprotect)(void);
  void (*EraseMemory)(void);
  void (*WriteProtect)(void);
  void (*WriteUnprotect)(void);
  void (*NsWriteMemory)(void);
  void (*NsEraseMemory)(void);
  void (*NsWriteProtect)(void);
  void (*NsWriteUnprotect)(void);
  void (*NsReadoutProtect)(void);
  void (*NsReadoutUnprotect)(void);
} OPENBL_CommandsTypeDef;

typedef struct
{
  OPENBL_OpsTypeDef *p_Ops;
  OPENBL_CommandsTypeDef *p_Cmd;
} OPENBL_HandleTypeDef;

/* Exported constants --------------------------------------------------------*/
#define ERROR_COMMAND                     0xECU             /* Error command */
#define ACK_BYTE                          0x79U             /* Acknowledge Byte ID */
#define NACK_BYTE                         0x1FU             /* No Acknowledge Byte ID */
#define BUSY_BYTE                         0x76U             /* Busy Byte */
#define SYNC_BYTE                         0xA5U             /* synchronization byte */

/* ---------------------- Open Bootloader Commands ---------------------------*/
#define CMD_GET_COMMAND                   0x00U             /* Get commands command */
#define CMD_GET_VERSION                   0x01U             /* Get Version command */
#define CMD_GET_ID                        0x02U             /* Get ID command */
#define CMD_READ_MEMORY                   0x11U             /* Read Memory command */
#define CMD_WRITE_MEMORY                  0x31U             /* Write Memory command */
#define CMD_GO                            0x21U             /* GO command */
#define CMD_READ_PROTECT                  0x82U             /* Readout Protect command */
#define CMD_READ_UNPROTECT                0x92U             /* Readout Unprotect command */
#define CMD_EXT_ERASE_MEMORY              0x44U             /* Erase Memory command */
#define CMD_WRITE_PROTECT                 0x63U             /* Write Protect command */
#define CMD_WRITE_UNPROTECT               0x73U             /* Write Unprotect command */
#define CMD_NS_WRITE_MEMORY               0x32U             /* No Stretch Write Memory command */
#define CMD_NS_ERASE_MEMORY               0x45U             /* No Stretch Erase Memory command */
#define CMD_NS_WRITE_PROTECT              0x64U             /* No Stretch Write Protect command */
#define CMD_NS_WRITE_UNPROTECT            0x74U             /* No Stretch Write Unprotect command */
#define CMD_NS_READ_PROTECT               0x83U             /* No Stretch Read Protect command */
#define CMD_NS_READ_UNPROTECT             0x93U             /* No Stretch Read Unprotect command */
#define CMD_CHECKSUM                      0xA1U

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_Init(void);
void OPENBL_DeInit(void);
uint32_t OPENBL_InterfaceDetection(void);
void OPENBL_CommandProcess(void);
ErrorStatus OPENBL_RegisterInterface(OPENBL_HandleTypeDef *Interface);

#endif /* OPENBL_CORE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
