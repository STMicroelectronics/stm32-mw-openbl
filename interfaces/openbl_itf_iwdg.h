/**
  ******************************************************************************
  * @file    openbl_itf_iwdg.h
  * @brief   Header for openbl_itf_iwdg.c module
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
#ifndef OPENBL_ITF_IWDG_H
#define OPENBL_ITF_IWDG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "openbl_types.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
openbl_status_t OPENBL_ITF_IWDG_Init(void *p_hw_context);
OPENBL_RAM_FUNCTION void OPENBL_ITF_IWDG_Refresh(void *p_hw_context);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_ITF_IWDG_H */
