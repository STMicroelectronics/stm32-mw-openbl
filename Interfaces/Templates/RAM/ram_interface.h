/**
  ******************************************************************************
  * @file    ram_interface.h
  * @author  MCD Application Team
  * @brief   Header for ram_interface.c module
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
#ifndef RAM_INTERFACE_H
#define RAM_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OPENBL_RAM_JumpToAddress(uint32_t Address);
uint8_t OPENBL_RAM_Read(uint32_t Address);
void OPENBL_RAM_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength);

#endif /* RAM_INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
