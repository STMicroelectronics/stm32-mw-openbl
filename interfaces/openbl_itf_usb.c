/**
  ******************************************************************************
  * @file    openbl_itf_usb.c
  * @brief   Contains USB protocol configuration
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

/* Includes ------------------------------------------------------------------*/
#include "openbl_core.h"
#include "openbl_usb_cmd.h"

#include "openbl_itf_usb.h"
#include "openbl_usb_dfu.h"
#include "openbl_itf_iwdg.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
static uint32_t IsJumpRequested  = 0U;
static uint32_t IsUsbSofDetected = 0U;
static uint32_t JumpAddress      = 0U;

/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to initialize the used USB instance.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns OPENBL_SUCCESS if initialization is successful, otherwise returns OPENBL_ERROR.
  */
__WEAK openbl_status_t OPENBL_ITF_USB_Init(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_USB_Init" can be implemented in the user file.
   */

  STM32_UNUSED(p_itf);

  return OPENBL_SUCCESS;
}

/**
  * @brief  This function is used to De-initialize the I2C pins and instance.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
__WEAK void OPENBL_ITF_USB_DeInit(openbl_itf_obj_t *p_itf)
{
  /** @warning This function must not be modified, when needed,
               the "OPENBL_ITF_USB_DeInit" can be implemented in the user file.
   */

  STM32_UNUSED(p_itf);

  openbl_user_usb_deinit();
}

/**
  * @brief  This function is used to detect if there is any activity on USB protocol.
  * @param  p_itf Pointer to the interface object.
  * @retval Returns 1 if interface is detected else 0.
  */
uint8_t OPENBL_ITF_USB_ProtocolDetection(openbl_itf_obj_t *p_itf)
{
  uint8_t detected;

  STM32_UNUSED(p_itf);

  /* USB detection is based on the Start of Frame (SOF) packets reception from the host.
     If at least one SOF packet is received, the USB interface is considered as detected. */
  if (IsUsbSofDetected == 1U)
  {
    detected = 1U;
  }
  else
  {
    detected = 0U;
  }

  return detected;
}

/**
  * @brief  USB process function.
  * @param  p_itf Pointer to the interface object.
  * @retval None.
  */
void OPENBL_ITF_USB_Process(openbl_itf_obj_t *p_itf)
{
  /* Wait till user asks for jump to application */
  if (p_itf->detected == 1U)
  {
    /* Reload the watchdog (In case user have already enabled it by HW) */
    OPENBL_ITF_IWDG_Refresh(p_itf->hw_context.p_data);
  }

  if (IsJumpRequested == 1U)
  {
    OPENBL_USB_Jump(p_itf, JumpAddress);
  }
}

/**
  * @brief  Sets the SOF detected flag.
  * @retval None.
  */
void OPENBL_ITF_USB_SetSofDetect(void)
{
  IsUsbSofDetected = 1U;
}

/**
  * @brief  This function is used to set the jump request flag.
  * @retval None.
  */
void OPENBL_ITF_USB_SetJumpRequest(void)
{
  IsJumpRequested = 1U;
}

/**
  * @brief  This function is used to set the jump address.
  * @param  address Contains the jump address.
  * @retval None.
  */
void OPENBL_ITF_USB_SetJumpAddress(uint32_t address)
{
  JumpAddress = address;
}

/**
  * @brief  Gets the page of a given address.
  * @param  address Address of the FLASH Memory.
  * @retval The page of a given address.
  */
uint32_t OPENBL_ITF_USB_GetPage(uint32_t address)
{
  uint32_t page;

  page = OPENBL_USB_DFU_GetPageFromAddress(address);

  return page;
}

/**
  * @brief  This function is used to De-initialize the USB instance.
  * @retval None.
  */
__WEAK void openbl_user_usb_deinit(void)
{
  /** @warning This function must not be modified, when needed,
               the "openbl_user_usb_deinit" can be implemented in the user file.
   */
}
