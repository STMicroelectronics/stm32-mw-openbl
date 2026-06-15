/**
  ******************************************************************************
  * @file    openbl_core.h
  * @brief   Header for openbl_core.c module
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
#ifndef OPENBL_CORE_H
#define OPENBL_CORE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "openbl_types.h"
#include "openbl_interfaces.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  openbl_itf_list_t itf_list;
} openbl_obj_t;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int32_t openbl_user_init(openbl_obj_t *p_openbl_obj);
void openbl_user_deinit(void);
openbl_status_t OPENBL_Init(openbl_obj_t *p_openbl_obj);
void OPENBL_DeInit(void);
void OPENBL_DeInitNotDetectedInterfaces(openbl_obj_t *p_openbl_obj);
int32_t OPENBL_InterfaceDetection(openbl_obj_t *p_openbl_obj);
void OPENBL_CommandProcess(openbl_obj_t *p_openbl_obj, uint32_t com_idx);
uint32_t OPENBL_GetTypeOfDetectedInterface(openbl_obj_t *p_openbl_obj);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_CORE_H */
