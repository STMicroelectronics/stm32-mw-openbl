/**
  ******************************************************************************
  * @file    openbl_i2c_cmd.h
  * @author  MCD Application Team
  * @brief   Header for openbl_i2c_cmd.c module
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
#ifndef OPENBL_I2C_CMD_H
#define OPENBL_I2C_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_I2C_VERSION                 0x12U               /* Open Bootloader I2C protocol V1.2 */
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern OPENBL_CommandsTypeDef OPENBL_I2C_Commands;

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_I2C_GetCommandsList(void);

#endif /* OPENBL_I2C_CMD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
