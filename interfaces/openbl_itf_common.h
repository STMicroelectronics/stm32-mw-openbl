/**
  ******************************************************************************
  * @file    openbl_common.h
  * @brief   Header for openbl_common.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_COMMON_H
#define OPENBL_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_types.h"

/* Exported types ------------------------------------------------------------*/
typedef void (*Function_Pointer)(void);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void OPENBL_Common_SetMsp(uint32_t top_of_main_stack);
void OPENBL_Common_EnableIrq(void);
void OPENBL_Common_DisableIrq(void);
openbl_state_t OPENBL_Common_GetProtectionStatus(void);
void OPENBL_Common_JumpToAddress(uint32_t address);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_COMMON_H */
