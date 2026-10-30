/**
  ******************************************************************************
  * @file    openbl_usb_dfu_descriptors.c
  * @brief   Open Bootloader USB DFU Device descriptor source file
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
#include "openbl_usb_dfu_descriptors.h"
#include "ux_stm32_device_descriptors_config.h"
#include "stm32_hal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t *OPENBL_USB_SerialNumber_Desc;

#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma data_alignment = 4
#endif /* defined ( __ICCARM__ ) */
static __ALIGN_BEGIN UCHAR USBD_StringFramework[OPENBL_USBD_STRING_FRAMEWORK_MAX_LENGTH]
__ALIGN_END = {0};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4
#endif /* __ICCARM__ */
static __ALIGN_BEGIN uint8_t USBD_StringSerial[OPENBL_USBD_STRING_SERIAL_SIZE] = {0};

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_USB_Desc_GetString(uint8_t *p_desc, uint8_t *p_buffer, uint16_t *p_length);
static uint8_t OPENBL_USB_Desc_GetLength(uint8_t *p_buffer);
static uint8_t *OPENBL_USB_Desc_GetSerialNumber(void);
static void OPENBL_USB_Desc_IntToUnicode(uint32_t value, uint8_t *p_buffer, uint8_t length);

/**
  * @brief  Used to get the device descriptors string.
  * @param  p_length Pointer to the descriptor length.
  * @retval Returns a pointer to descriptors string.
  */
uint8_t *OPENBL_USB_Desc_GetStringFramework(ULONG *p_length)
{
  uint16_t length = 0x0U;
  uint16_t count  = 0x0U;

  /* Set the Manufacturer language Id and index in USBD_StringFramework */
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = USBD_IDX_MFC_STR;

  /* Set the Manufacturer string in string_framework */
  OPENBL_USB_Desc_GetString((uint8_t *)USBD_MANUFACTURER_STRING, USBD_StringFramework + count, &length);

  /* Set the Product language Id and index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = USBD_IDX_PRODUCT_STR;

  /* Set the Product string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)USBD_PRODUCT_STRING, USBD_StringFramework + count, &length);

  /* Set Serial language Id and index in string_framework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = USBD_IDX_SERIAL_STR;

  OPENBL_USB_SerialNumber_Desc = OPENBL_USB_Desc_GetSerialNumber();

  /* Set the Serial number in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USB_SerialNumber_Desc, USBD_StringFramework + count, &length);

  /* Set FLASH descriptor index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = OPENBL_USBD_DFU_STRING_FLASH_DESC_INDEX;

  /* Set FLASH descriptor string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USBD_DFU_STRING_FLASH_DESC, USBD_StringFramework + count, &length);

  /* Set Option Bytes area descriptor index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = OPENBL_USBD_DFU_STRING_OB_DESC_INDEX;

  /* Set Option Bytes area 1 descriptor string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USBD_DFU_STRING_OB_DESC, USBD_StringFramework + count, &length);

  /* Set RO descriptor index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = OPENBL_USBD_DFU_STRING_RO_DESC_INDEX;

  /* Set RO descriptor string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USBD_DFU_STRING_RO_DESC, USBD_StringFramework + count, &length);

  /* Set OTP descriptor index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = OPENBL_USBD_DFU_STRING_OTP_DESC_INDEX;

  /* Set OTP descriptor string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USBD_DFU_STRING_OTP_DESC, USBD_StringFramework + count, &length);

  /* Set ENGI descriptor index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = OPENBL_USBD_DFU_STRING_ENGI_DESC_INDEX;

  /* Set ENGI descriptor string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USBD_DFU_STRING_ENGI_DESC, USBD_StringFramework + count, &length);

  /* Set device ID descriptor index in USBD_StringFramework */
  count += length + 1U;
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING & 0xFFU);
  USBD_StringFramework[count++] = (UCHAR)(USBD_LANG_ID_STRING >> 8U);
  USBD_StringFramework[count++] = OPENBL_USBD_DFU_STRING_DEVICE_ID_DESC_INDEX;

  /* Set device ID descriptor string in USBD_StringFramework */
  OPENBL_USB_Desc_GetString((uint8_t *)OPENBL_USBD_DFU_STRING_DEVICE_ID_DESC, USBD_StringFramework + count, &length);

  /* Get the length of USBD_StringFramework */
  *p_length = strlen((const char *)USBD_StringFramework);

  return USBD_StringFramework;
}

/**
  * @brief  Create the serial number string descriptor.
  * @retval Returns the serial number string descriptor.
  */
static uint8_t *OPENBL_USB_Desc_GetSerialNumber(void)
{
  uint32_t deviceserial0;
  uint32_t deviceserial1;
  uint32_t deviceserial2;

  deviceserial0 = *(uint32_t *)OPENBL_USBD_DEVICE_ID1;
  deviceserial1 = *(uint32_t *)OPENBL_USBD_DEVICE_ID2;
  deviceserial2 = *(uint32_t *)OPENBL_USBD_DEVICE_ID3;

  deviceserial0 += deviceserial2;

  if (deviceserial0 != 0U)
  {
    OPENBL_USB_Desc_IntToUnicode(deviceserial0, &USBD_StringSerial[0], 8);
    OPENBL_USB_Desc_IntToUnicode(deviceserial1, &USBD_StringSerial[8], 4);
  }

  return (uint8_t *)USBD_StringSerial;
}

/**
  * @brief  Convert Hex 32 bits value into char.
  * @param  value Value to convert.
  * @param  p_buffer Pointer to the buffer.
  * @param  length Buffer length.
  * @retval None.
  */
static void OPENBL_USB_Desc_IntToUnicode(uint32_t value, uint8_t *p_buffer, uint8_t length)
{
  uint8_t index;

  for (index = 0U; index < length; index ++)
  {
    if (((value >> 28U)) < 0xAU)
    {
      p_buffer[index] = (uint8_t)((value >> 28U) + (uint8_t)'0');
    }
    else
    {
      p_buffer[index] = (uint8_t)((value >> 28U) + (uint8_t)'A' - 10U);
    }

    value = value << 4U;

    p_buffer[index + 1U] = 0U;
  }
}

/**
  * @brief  Converts ASCII string into Unicode one.
  * @param  p_desc Pointer to the descriptor buffer.
  * @param  p_unicode Pointer to the formatted string buffer (unicode).
  * @param  p_length Pointer to the descriptor length.
  * @retval None
  */
static void OPENBL_USB_Desc_GetString(uint8_t *p_desc, uint8_t *p_unicode, uint16_t *p_length)
{
  uint8_t index = 0x0U;
  uint8_t *p_temp_desc;

  p_temp_desc = p_desc;

  *p_length = (uint16_t)OPENBL_USB_Desc_GetLength(p_temp_desc);

  p_unicode[index++] = *(uint8_t *)p_length;

  while (*p_temp_desc != (uint8_t)'\0')
  {
    p_unicode[index++] = *p_temp_desc;
    p_temp_desc++;
  }
}

/**
  * @brief  Returns the string length.
  * @param  p_buffer Pointer to the ASCII string buffer.
  * @retval Returns the string length.
  */
static uint8_t OPENBL_USB_Desc_GetLength(uint8_t *p_buffer)
{
  uint8_t length         = 0x0U;
  uint8_t *p_temp_buffer = p_buffer;

  while (*p_temp_buffer != (uint8_t)'\0')
  {
    length++;
    p_temp_buffer++;
  }

  return length;
}
