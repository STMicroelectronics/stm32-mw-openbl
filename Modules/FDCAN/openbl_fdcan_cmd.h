/**
  ******************************************************************************
  * @file    openbl_fdcan_cmd.h
  * @author  MCD Application Team
  * @brief   Header for openbl_fdcan_cmd.c module
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
#ifndef OPENBL_FDCAN_CMD_H
#define OPENBL_FDCAN_CMD_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_FDCAN_VERSION             0x10U                /* Open Bootloader FDCAN protocol V1.0 */
#define FDCAN_RAM_BUFFER_SIZE             1156                /* Size of FDCAN buffer used to store received data from the host */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t TxData[FDCAN_RAM_BUFFER_SIZE];
extern uint8_t RxData[FDCAN_RAM_BUFFER_SIZE];

extern OPENBL_CommandsTypeDef OPENBL_FDCAN_Commands;

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_FDCAN_GetCommandsList(void);

#endif /* OPENBL_FDCAN_CMD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
