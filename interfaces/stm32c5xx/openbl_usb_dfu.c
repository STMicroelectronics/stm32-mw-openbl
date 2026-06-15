/**
  ******************************************************************************
  * @file    openbl_usb_dfu.c
  *
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
#include "ux_api.h"
#include "ux_dcd_stm32.h"
#include "ux_stm32_device_descriptors_config.h"

#include "openbl_usb_dfu.h"
#include "openbl_usb_cmd.h"
#include "openbl_itf_usb.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define USBD_DFU_APP_BUFFER_SIZE    (USBD_DFU_XFER_SIZE * 8U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static UX_SLAVE_CLASS_DFU *p_USB_Instance;
static UCHAR usbd_dfu_media[USBD_DFU_APP_BUFFER_SIZE];
static ULONG usbd_dfu_media_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
static ULONG usbd_dfu_last_notification;

static ULONG DfuStatus     = 0U;
static ULONG MemoryAddress = 0U;
static ULONG LeaveDfuState = 0U;

static openbl_itf_obj_t *p_USB_Itf;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static UINT OPENBL_USBD_DFU_Erase(void *dfu_instance, UCHAR *p_data, ULONG length);
static UINT OPENBL_USBD_DFU_ReadMemory(void *dfu_instance, ULONG block_number, uint8_t *p_data, uint32_t length,
                                       ULONG *actual_length);
static UINT OPENBL_USBD_DFU_CheckAddress(uint32_t address);
static UINT OPENBL_USB_SetAddressPointer(void *dfu_instance, UCHAR *p_data, ULONG length);
static UINT OPENBL_USBD_DFU_SetJumpAddress(uint32_t address);
static UINT OPENBL_USBD_DFU_WriteMemory(void *dfu_instance, ULONG block_number, UCHAR *p_data, ULONG length);
static void OPENBL_USB_GetCommands(UCHAR *p_data, ULONG length, ULONG *actual_length);

/* Exported functions --------------------------------------------------------*/
/**
  * @brief Callback function to handle the activation of the DFU instance.
  * This function is called when the DFU instance is activated.
  * @param dfu_instance Pointer to the DFU instance.
  * @return None.
  */
void OPENBL_USB_DFU_Activate(void *dfu_instance)
{
  if (p_USB_Instance == UX_NULL)
  {
    p_USB_Instance = (UX_SLAVE_CLASS_DFU *)dfu_instance;
  }
}

/**
  * @brief Callback function to handle the deactivation of the DFU instance.
  * This function is called when the DFU instance is deactivated.
  * @param dfu_instance Pointer to the DFU instance.
  * @return None.
  */
void OPENBL_USB_DFU_Deactivate(void *dfu_instance)
{
  if (p_USB_Instance == (UX_SLAVE_CLASS_DFU *)dfu_instance)
  {
    p_USB_Instance = UX_NULL;
  }
}

/**
  * @brief This function handles calls to USB upload requests (DFU_UPLOAD) from the host.
  * @param dfu_instance Pointer to the DFU instance.
  * @param block_number Block number for the upload request.
  * @param p_data Pointer to the data buffer.
  * @param length Length of the data buffer.
  * @param actual_length Pointer to store the actual length of data transferred.
  * @return UINT Status of the operation, UX_SUCCESS if successful, else UX_ERROR.
  */
UINT OPENBL_USB_DFU_Upload(void *dfu_instance, ULONG block_number, UCHAR *p_data, ULONG length, ULONG *actual_length)
{
  UINT ux_status = UX_SUCCESS;

  if (block_number == 0U)
  {
    OPENBL_USB_GetCommands(p_data, length, actual_length);
  }
  else
  {
    ux_status = OPENBL_USBD_DFU_ReadMemory(dfu_instance, block_number, p_data, length, actual_length);
  }

  return ux_status;
}

/**
  * @brief This function handles calls to USB download requests (DFU_DNLOAD) from the host.
  * @param dfu_instance Pointer to the DFU instance.
  * @param block_number Block number for the download request.
  * @param p_data Pointer to the data buffer.
  * @param length Length of the data buffer.
  * @param media_status Pointer to store the media status.
  * @return UINT Status of the operation, UX_SUCCESS if successful, else UX_ERROR.
  */
UINT OPENBL_USB_DFU_Download(void *dfu_instance, ULONG block_number, UCHAR *p_data, ULONG length, ULONG *media_status)
{
  UINT ux_status = UX_SUCCESS;
  UINT command_opcode;

  STM32_UNUSED(media_status);

  if (block_number == 0U)
  {
    command_opcode = p_data[0];

    switch (command_opcode)
    {
      case OPENBL_DFU_CMD_SET_ADDRESS_POINTER:
        ux_status = OPENBL_USB_SetAddressPointer(dfu_instance, p_data, length);

        break;

      case OPENBL_DFU_CMD_ERASE:
        ux_status = OPENBL_USBD_DFU_Erase(dfu_instance, p_data, length);

        break;

      default:
        break;
    }
  }
  /* Regular download command */
  else
  {
    ux_status = OPENBL_USBD_DFU_WriteMemory(dfu_instance, block_number, p_data, length);
  }

  return ux_status;
}

/**
  * @brief This function is used to get the status of the DFU media.
  * @param dfu_instance Pointer to the DFU instance.
  * @param media_status Pointer to store the media status.
  * @return UINT Status of the operation, always returns UX_SUCCESS if successful.
  */
UINT OPENBL_USB_DFU_GetStatus(void *dfu_instance, ULONG *media_status)
{
  UX_PARAMETER_NOT_USED(dfu_instance);

  *media_status = usbd_dfu_media_status;

  return UX_SUCCESS;
}

/**
  * @brief This function is used to get the status of the DFU media.
  * @param dfu_instance Pointer to the DFU instance.
  * @param notification Notification code for the DFU event.
  * @return UINT Status of the operation, always returns UX_SUCCESS if successful.
  */
UINT OPENBL_USB_DFU_Notify(void *dfu_instance, ULONG notification)
{
  UX_PARAMETER_NOT_USED(dfu_instance);

  usbd_dfu_last_notification = notification;

  switch (notification)
  {
    case UX_SLAVE_CLASS_DFU_NOTIFICATION_ABORT_DOWNLOAD:
      usbd_dfu_media_status = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
      break;

    case UX_SLAVE_CLASS_DFU_NOTIFICATION_ABORT_UPLOAD:
    case UX_SLAVE_CLASS_DFU_NOTIFICATION_BEGIN_DOWNLOAD:
    case UX_SLAVE_CLASS_DFU_NOTIFICATION_END_DOWNLOAD:
    case UX_SLAVE_CLASS_DFU_NOTIFICATION_BEGIN_UPLOAD:
    case UX_SLAVE_CLASS_DFU_NOTIFICATION_END_UPLOAD:
    default:
      break;
  }

  return UX_SUCCESS;
}

/**
  * @brief  Handles the sub-protocol DFU leave DFU mode request (leaves DFU mode
  *         and resets device to jump to user loaded code).
  * @param  dfu Pointer to the DFU instance.
  * @param  transfer Pointer to the transfer request structure.
  * @retval Returns UX_SUCCESS if operation is successful, else UX_ERROR.
  */
UINT OPENBL_USB_FDU_Leave(void *dfu_instance, UX_SLAVE_TRANSFER *transfer)
{
  UCHAR *p_setup;
  UCHAR dfu_state;
  UINT status = UX_ERROR;

  /* Get DFU state */
  dfu_state = _ux_device_class_dfu_state_get((UX_SLAVE_CLASS_DFU *)dfu_instance);

  p_setup = transfer->ux_slave_transfer_request_setup;

  if ((dfu_state == (uint8_t)UX_SYSTEM_DFU_STATE_DFU_IDLE)
      || (dfu_state == (uint8_t)UX_SYSTEM_DFU_STATE_DFU_DNLOAD_IDLE))
  {
    if (p_setup[UX_SETUP_REQUEST] == (uint8_t)UX_SLAVE_CLASS_DFU_COMMAND_DOWNLOAD)
    {
      if ((p_setup[UX_SETUP_LENGTH] == 0U) && (p_setup[UX_SETUP_LENGTH + 1U] == 0U))
      {
        status = OPENBL_USBD_DFU_SetJumpAddress(MemoryAddress);

        if (status != (UINT)UX_SUCCESS)
        {
          return (status);
        }

        /* In the system, state the DFU state machine to DFU MANIFEST SYNCH */
        _ux_system_slave->ux_system_slave_device_dfu_state_machine = UX_SLAVE_CLASS_DFU_STATUS_STATE_DFU_MANIFEST_SYNC;

        /* Set USB leave flag */
        LeaveDfuState = 1;
      }
    }
  }

  return (status);
}

/**
  * @brief  DFU device connection callback.
  * @param  device_state USBx device state.
  * @retval Returns USBx error status.
  */
UINT OPENBL_USB_DFU_ConnectionCallback(ULONG device_state)
{
  if (device_state == (uint32_t)UX_DEVICE_REMOVED)
  {
    if (_ux_system_slave -> ux_system_slave_device_dfu_mode == (uint32_t)UX_DEVICE_CLASS_DFU_MODE_DFU)
    {
      /* Nothing to do */
    }
  }
  else if (device_state == (uint32_t)UX_DCD_STM32_SOF_RECEIVED)
  {
    /* USB detection is based on the Start of Frame (SOF) packets reception from the host.
      If at least one SOF packet is received, the USB interface is considered as detected. */
    OPENBL_ITF_USB_SetSofDetect();
  }
  else if (device_state == (uint32_t)UX_DCD_STM32_DEVICE_DISCONNECTED)
  {
    /* Disconnect USBx stack driver */
    (void)ux_device_stack_disconnect();

    if (LeaveDfuState == 1U)
    {
      OPENBL_ITF_USB_SetJumpRequest();
    }
  }
  else
  {
    /* Nothing to do */
  }

  return UX_SUCCESS;
}

/**
  * @brief This callback function is used to process the USB DFU interface.
  * @return None.
  */
void OPENBL_USB_DFU_Process(void)
{
  UX_PARAMETER_NOT_USED(p_USB_Instance);
  UX_PARAMETER_NOT_USED(usbd_dfu_last_notification);
}

/**
  * @brief Set the USB DFU interface instance.
  * @param p_itf Pointer to the interface object.
  * @return None.
  */
void OPENBL_USBD_DFU_SetInterfaceInstance(openbl_itf_obj_t *p_itf)
{
  p_USB_Itf = p_itf;
}

/**
  * @brief This functions handles the set address pointer command.
  * @param dfu_instance Pointer to the DFU instance.
  * @param p_data Pointer to the data buffer.
  * @param length Length of the data.
  * @return None.
  */
static UINT OPENBL_USB_SetAddressPointer(void *dfu_instance, UCHAR *p_data, ULONG length)
{
  UINT status;
  uint32_t address;

  UX_PARAMETER_NOT_USED(dfu_instance);

  (void)ux_utility_memory_copy(usbd_dfu_media, p_data, length);

  /* Get address from the buffer */
  address  = (uint32_t)usbd_dfu_media[1U];
  address += (uint32_t)usbd_dfu_media[2U] << 8U;
  address += (uint32_t)usbd_dfu_media[3U] << 16U;
  address += (uint32_t)usbd_dfu_media[4U] << 24U;

  /* Check if the address is supported or not */
  status = OPENBL_USBD_DFU_CheckAddress(address);

  if (status != (UINT)UX_SUCCESS)
  {
    DfuStatus  = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_ERROR_ADDRESS << 4U);
  }
  else
  {
    /* Update the memory address only if the received address is valid */
    MemoryAddress = address;

    DfuStatus  = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_OK << 4U);
  }

  return status;
}

/**
  * @brief This function is used to set the supported commands opcodes.
  * @param p_data Pointer to the data buffer.
  * @param length Length of the data buffer.
  * @param actual_length Pointer to store the actual length of data.
  * @return None.
  */
static void OPENBL_USB_GetCommands(UCHAR *p_data, ULONG length, ULONG *actual_length)
{
  /* Store the values of all supported commands */
  p_data[0U] = OPENBL_DFU_CMD_GET_COMMANDS;
  p_data[1U] = OPENBL_DFU_CMD_SET_ADDRESS_POINTER;
  p_data[2U] = OPENBL_DFU_CMD_ERASE;
  p_data[3U] = 0U;

  /* Update the actual length */
  *actual_length = length;
}

/**
  * @brief  Jumps to a user application.
  * @param  address: Address to jump to.
  * @retval Returns UX_SUCCESS if operation is successful, UX_ERROR else.
  */
static UINT OPENBL_USBD_DFU_SetJumpAddress(uint32_t address)
{
  UINT status;

  /* Check if the address is supported or not */
  status = OPENBL_USBD_DFU_CheckAddress(address);

  if (status == (UINT)UX_SUCCESS)
  {
    OPENBL_ITF_USB_SetJumpAddress(address);
  }

  return status;
}

/**
  * @brief  This function is used to check if a given address is valid or not.
  * @param  address Address to check.
  * @retval Returns UX_SUCCESS if the address is valid, else returns UX_ERROR.
  */
static UINT OPENBL_USBD_DFU_CheckAddress(uint32_t address)
{
  UINT status;

  /* Check if the address is valid or not */
  if (OPENBL_MEM_GetAreaFromAddress(p_USB_Itf->p_mem_list, address) == OPENBL_MEM_AREA_ERROR)
  {
    status = UX_ERROR;
  }
  else
  {
    status = UX_SUCCESS;
  }

  return status;
}

/**
  * @brief This function is used to read data from a given address.
  * @param dfu_instance Pointer to the DFU instance.
  * @param block_number Block number to read.
  * @param p_data Pointer to the data buffer.
  * @param length Length of data to read.
  * @param actual_length Pointer to store the actual length of data read.
  * @return UINT Returns UX_SUCCESS if operation is successful, else returns UX_ERROR.
  */
static UINT OPENBL_USBD_DFU_ReadMemory(void *dfu_instance,
                                       ULONG block_number,
                                       uint8_t *p_data,
                                       uint32_t length,
                                       ULONG *actual_length)
{
  uint32_t address;
  UINT status;

  /* Return the physical address from which the host requests to read data */
  address = ((block_number - 2U) * (uint32_t)UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH) + MemoryAddress;

  status = OPENBL_USBD_DFU_CheckAddress(address);

  if (status != (UINT)UX_SUCCESS)
  {
    /* Set DFU media status OK */
    DfuStatus  = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_OK;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_OK << 4U);

    /* syncs the USB DFU device state */
    (void)ux_device_class_dfu_state_sync(dfu_instance);
  }
  else
  {
    /* Set DFU media status Error */
    DfuStatus  = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_ERROR_WRITE << 4U);

    /* syncs the USB DFU device state */
    (void)ux_device_class_dfu_state_sync(dfu_instance);

    OPENBL_USB_ReadMemory(p_USB_Itf, address, p_data, length);
  }

  *actual_length = length;

  return status;
}

/**
  * @brief This function is used to write data sent by host to device memory.
  * @param dfu_instance Pointer to the DFU instance.
  * @param block_number Block number to write.
  * @param p_data Pointer to the data buffer.
  * @param length Length of data to write.
  * @return UINT Returns UX_SUCCESS if operation is successful, else returns UX_ERROR.
  */
static UINT OPENBL_USBD_DFU_WriteMemory(void *dfu_instance, ULONG block_number, UCHAR *p_data, ULONG length)
{
  uint32_t address;
  UINT status;
  ULONG dfu_poll_timeout = DFU_MEDIA_PROGRAM_TIME;

  UX_PARAMETER_NOT_USED(dfu_instance);

  (void)ux_utility_memory_copy(usbd_dfu_media, p_data, length);

  /* Get the address where the host requests to write data */
  address = ((block_number - 2U) * (uint32_t)UX_SLAVE_REQUEST_CONTROL_MAX_LENGTH) + MemoryAddress;

  status = OPENBL_USBD_DFU_CheckAddress(address);

  if (status != (UINT)UX_SUCCESS)
  {
    DfuStatus =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_ERROR_WRITE << 4U);
  }
  else
  {
    /* Set DFU media status Busy, dfu poll timeout in program phase */
    DfuStatus =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_BUSY;
    DfuStatus += (ULONG) UX_SLAVE_CLASS_DFU_STATUS_OK << 4U;
    DfuStatus += (ULONG)(dfu_poll_timeout) << 8U;

    OPENBL_USB_WriteMemory(p_USB_Itf, address, usbd_dfu_media, length);
  }

  return status;
}

/**
  * @brief This function is used to erase a page from a given address.
  * @param dfu_instance Pointer to the DFU instance.
  * @param p_data Pointer to the data buffer.
  * @param length Length of data to read.
  * @return None.
  */
static UINT OPENBL_USBD_DFU_Erase(void *dfu_instance, UCHAR *p_data, ULONG length)
{
  UINT status;
  uint32_t address;
  ULONG dfu_poll_timeout = DFU_MEDIA_ERASE_TIME;

  UX_PARAMETER_NOT_USED(dfu_instance);

  (void)ux_utility_memory_copy(usbd_dfu_media, p_data, length);

  address =  usbd_dfu_media[1U];
  address += (uint32_t)usbd_dfu_media[2U] << 8U;
  address += (uint32_t)usbd_dfu_media[3U] << 16U;
  address += (uint32_t)usbd_dfu_media[4U] << 24U;

  /* Check if the address is supported or not */
  status = OPENBL_USBD_DFU_CheckAddress(address);

  if (status != (UINT)UX_SUCCESS)
  {
    DfuStatus  = UX_SLAVE_CLASS_DFU_MEDIA_STATUS_ERROR;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_ERROR_ADDRESS << 4U);
  }
  else
  {
    /* Set DFU media status Busy, dfu poll timeout in erase phase */
    DfuStatus =  UX_SLAVE_CLASS_DFU_MEDIA_STATUS_BUSY;
    DfuStatus += (ULONG)((uint32_t)UX_SLAVE_CLASS_DFU_STATUS_OK << 4U);
    DfuStatus += (ULONG)(dfu_poll_timeout) << 8U;

    UX_INTERRUPT_SAVE_AREA

    /* Disable interrupts to prevent the kernel disruption */
    UX_DISABLE

    (void)OPENBL_USB_EraseMemory(p_USB_Itf, address);

    /* Restore interrupts */
    (void)UX_RESTORE
  }

  return status;
}

/**
  * @brief  Gets the page of a given address.
  * @param  address Address of the FLASH Memory.
  * @retval The page of a given address.
  */
uint32_t OPENBL_USB_DFU_GetPageFromAddress(uint32_t address)
{
  uint32_t page;

  page = (address - FLASH_BASE) / FLASH_PAGE_SIZE;

  return page;
}
