/**
  ******************************************************************************
  * @file    flash_interface.h
  * @author  MCD Application Team
  * @brief   Header for flash_interface.c module
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
#ifndef FLASH_INTERFACE_H
#define FLASH_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include "common_interface.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FLASH_BUSY_STATE_ENABLED       ((uint32_t)0xAAAA0000)
#define FLASH_BUSY_STATE_DISABLED      ((uint32_t)0x0000DDDD)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_FLASH_JumpToAddress(uint32_t Address);
void OPENBL_FLASH_Lock(void);
void OPENBL_FLASH_OB_Unlock(void);
void OPENBL_FLASH_OB_Launch(void);
uint8_t OPENBL_FLASH_Read(uint32_t Address);
void OPENBL_FLASH_SetReadOutProtectionLevel(uint32_t Level);
void OPENBL_FLASH_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength);
void OPENBL_FLASH_Unlock(void);
ErrorStatus OPENBL_FLASH_MassErase(uint8_t *p_Data, uint32_t DataLength);
ErrorStatus OPENBL_FLASH_Erase(uint8_t *p_Data, uint32_t DataLength);
ErrorStatus OPENBL_FLASH_SetWriteProtection(FunctionalState State, uint8_t *ListOfPages, uint32_t Length);
uint32_t OPENBL_FLASH_GetReadOutProtectionLevel(void);
void OPENBL_Enable_BusyState_Sending(Send_BusyByte_Func *pCallback);
void OPENBL_Disable_BusyState_Sending(void);

#endif /* FLASH_INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
