/**
  ******************************************************************************
  * @file    openbl_util.h
  * @author  MCD Application Team
  * @brief   Header for openbl_util.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_UTIL_H
#define OPENBL_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "string.h"
#include "stdlib.h"
#include <stdbool.h>
#include <limits.h>
#include <errno.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  char     *opt[0xF];
  uint32_t  id[0xF];
  char     *name[0xF];
  char     *type[0xF];
  char     *ip[0xF];
  uint32_t  offset[0xF];
  uint32_t  partsize;
} OPENBL_Flashlayout_TypeDef;

/* Exported constants --------------------------------------------------------*/
#define PHASE_FLASHLAYOUT                    0x00U               /* Flash layout phase */
#define PHASE_OTP                            0xF2U               /* OTP partition phase */
#define PHASE_1                              0x01U               /* Phase ID 0x1 */
#define PHASE_3                              0x03U               /* Phase ID 0x3 */
#define PHASE_4                              0x04U               /* Phase ID 0x4 */
#define PHASE_5                              0x05U               /* Phase ID 0x5 */
#define PHASE_END                            0xFEU               /* End phase */
#define PHASE_LAST_USER                      0x0FU               /* Phase last user */
#define PARSE_ERROR                          -1
#define PARSE_OK                             0
#define PHASE_CMD                            0xF1U

/* Exported variables --------------------------------------------------------*/
extern OPENBL_Flashlayout_TypeDef FlashlayoutStruct;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int32_t OPENBL_FlashLayout_Parse_Layout(uint32_t address, uint32_t size);
int32_t OPENBL_Flashlayout_Parse_ID(char *p_string_id, uint32_t idx);
int32_t OPENBL_Flashlayout_Parse_Offset(char *p_string_offset, uint32_t idx);
int32_t OPENBL_Flashlayout_Parse_Name(char *p_string_name, uint32_t idx);
int32_t OPENBL_Flashlayout_Parse_Type(char *p_string_type, uint32_t idx);
int32_t OPENBL_Flashlayout_Parse_IP(char *p_string_ip, uint32_t idx);
int32_t OPENBL_Flashlayout_Parse_Option(char *p_string_option, uint32_t idx);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPENBL_UTIL_H */
