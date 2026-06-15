/**
  ******************************************************************************
  * @file    openbl_mem.c
  * @brief   Provides functions that manage operations on embedded memories (Flash, SRAM...).
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

/* Includes ------------------------------------------------------------------*/
#include "openbl_mem.h"
#include "openbl_core.h"
#include "openbl_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief This function is used to associate the detected interface to the available memories.
  * This association is used later to know which memory interface will be used to access a given memory area.
  * @param p_parent Pointer to the detected interface.
  * @param p_mem_list Pointer to the memory list structure.
  */
void OPENBL_MEM_AssociateDetectedInterfaceToMemories(void *p_parent, openbl_mem_list_t *p_mem_list)
{
  uint32_t mem_idx;

  for (mem_idx = 0U; mem_idx < p_mem_list->memories_nbr; mem_idx++)
  {
    p_mem_list->memories[mem_idx].p_parent = p_parent;
  }
}

/**
  * @brief  Check if a given address is valid or not and returns the area type.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address The address to be checked.
  * @retval The address area: OPENBL_FLASH_AREA, OPENBL_RAM_AREA... if the address is valid
  *         or OPENBL_MEM_AREA_ERROR if the address is not valid.
  */
uint32_t OPENBL_MEM_GetAreaFromAddress(openbl_mem_list_t *p_mem_list, uint32_t address)
{
  uint32_t mem_area = OPENBL_MEM_AREA_ERROR;
  uint32_t counter;

  for (counter = 0U; counter < p_mem_list->memories_nbr; counter++)
  {
    if ((address >= p_mem_list->memories[counter].mem_desc.start_address)
        && (address < p_mem_list->memories[counter].mem_desc.end_address))
    {
      mem_area = p_mem_list->memories[counter].mem_desc.type;
      break;
    }
    else
    {
      mem_area = OPENBL_MEM_AREA_ERROR;
    }
  }

  return mem_area;
}

/**
  * @brief  This function returns the index of the memory that matches the address given in parameter.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address This address is used determinate the index of the memory pointed by this address.
  * @retval The index of the memory that corresponds to the address
  */
uint32_t OPENBL_MEM_GetMemoryIndex(openbl_mem_list_t *p_mem_list, uint32_t address)
{
  uint32_t counter;

  for (counter = 0; counter < p_mem_list->memories_nbr; counter++)
  {
    if ((address >= p_mem_list->memories[counter].mem_desc.start_address)
        && (address < p_mem_list->memories[counter].mem_desc.end_address))
    {
      break;
    }
  }

  return counter;
}

/**
  * @brief  This function returns the index of the memory that matches the address given in parameter.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address This address is used determinate the index of the memory pointed by this address.
  * @retval Returns the pointer on the memory struct that corresponds to the address or NULL in case of error.
  */
openbl_mem_t *OPENBL_MEM_GetMemoryFromAddress(openbl_mem_list_t *p_mem_list, uint32_t address)
{
  uint32_t counter;

  for (counter = 0; counter < p_mem_list->memories_nbr; counter++)
  {
    if ((address >= p_mem_list->memories[counter].mem_desc.start_address)
        && (address < p_mem_list->memories[counter].mem_desc.end_address))
    {
      break;
    }
  }

  if (counter < p_mem_list->memories_nbr)
  {
    return &p_mem_list->memories[counter];
  }
  else
  {
    return NULL;
  }
}

/**
  * @brief  This function returns the value read from the given address.
  *         The memory index is used to know which memory interface will be used to read from the given address.
  * @param  p_mem Pointer to the memory structure.
  * @param  address The address to read from.
  * @retval Returns the read value.
  */
uint8_t OPENBL_MEM_Read(openbl_mem_t *p_mem, uint32_t address)
{
  uint8_t value;

  if (p_mem != NULL)
  {
    if (p_mem->mem_ops.Read != NULL)
    {
      value = p_mem->mem_ops.Read(p_mem, address);
    }
    else
    {
      value = 0U;
    }
  }
  else
  {
    value = 0U;
  }

  return value;
}

/**
  * @brief  This function is used to write data in to a given memory.
  * @param  address The address where that data will be written.
  * @param  p_data The data to be written.
  * @param  data_length The length of the data to be written.
  * @retval None.
  */
void OPENBL_MEM_Write(openbl_mem_list_t *p_mem_list, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;

  /* Get the memory index to know in which memory we will write */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.Write != NULL)
    {
      p_mem_list->memories[index].mem_ops.Write(&p_mem_list->memories[index], address, p_data, data_length);
    }
  }
}

/**
  * @brief  Enables or disables the read out protection.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address The address where the memory protection will be.
  * @param  state The readout protection state that will be set.
  * @retval None.
  */
void OPENBL_MEM_SetReadOutProtection(openbl_mem_list_t *p_mem_list, uint32_t address, openbl_state_t state)
{
  uint32_t index;

  /* Get the memory index to know in which memory we will write */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.SetReadoutProtect != NULL)
    {
      p_mem_list->memories[index].mem_ops.SetReadoutProtect(&p_mem_list->memories[index], state);
    }
  }
}

/**
  * @brief  This function is used to enable or disable write protection of the specified FLASH areas.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  state Can be one of these values:
  *         @arg OPENBL_DISABLE: Disable FLASH write protection
  *         @arg OPENBL_ENABLE: Enable FLASH write protection
  * @param  address Contains the address of the memory where write protection state will be changed
  * @param  buffer Contains write protection operation options
  * @param  length The length of the write protection options buffer.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Enable or disable of the write protection is done
  *          - OPENBL_ERROR:   Enable or disable of the write protection is not done
  */
openbl_status_t OPENBL_MEM_SetWriteProtection(openbl_mem_list_t *p_mem_list, openbl_state_t state, uint32_t address,
                                              uint8_t *p_buffer, uint32_t length)
{
  uint32_t index;
  openbl_status_t status = OPENBL_SUCCESS;

  /* Get the memory index to know in which memory we will write */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.SetWriteProtect != NULL)
    {
      p_mem_list->memories[index].mem_ops.SetWriteProtect(&p_mem_list->memories[index], state, p_buffer, length);
    }
    else
    {
      status = OPENBL_ERROR;
    }
  }
  else
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to jump to an address of an application.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address User application address.
  * @retval None.
  */
void OPENBL_MEM_JumpToAddress(openbl_mem_list_t *p_mem_list, uint32_t address)
{
  uint32_t index;

  /* Get the memory index to know from which memory interface we will used */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.JumpToAddress != NULL)
    {
      p_mem_list->memories[index].mem_ops.JumpToAddress(&p_mem_list->memories[index], address);
    }
  }
}

/**
  * @brief  This function is used to start a mass erase operation on a given memory.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address The address of the memory to be fully erased.
  * @param  p_data Pointer to the buffer that contains mass erase operation options.
  * @param  data_length Size of the Data buffer.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Mass erase operation done
  *          - OPENBL_ERROR:   Mass erase operation failed or one parameter is invalid
 */
openbl_status_t OPENBL_MEM_MassErase(openbl_mem_list_t *p_mem_list, uint32_t address, uint8_t *p_data,
                                     uint32_t data_length)
{
  uint32_t index;
  openbl_status_t status;

  /* Get the memory index to know from which memory interface we will used */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.MassErase != NULL)
    {
      status = p_mem_list->memories[index].mem_ops.MassErase(&p_mem_list->memories[index], p_data, data_length);
    }
    else
    {
      status = OPENBL_ERROR;
    }
  }
  else
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to erase the specified memory.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address The address of the memory to be erased.
  * @param  p_data Pointer to the buffer that contains erase operation options.
  * @param  data_length Size of the Data buffer.
  * @retval An openbl_status_t enumeration value:
  *          - OPENBL_SUCCESS: Erase operation done
  *          - OPENBL_ERROR:   Erase operation failed or one parameter is invalid
 */
openbl_status_t OPENBL_MEM_Erase(openbl_mem_list_t *p_mem_list, uint32_t address, uint8_t *p_data, uint32_t data_length)
{
  uint32_t index;
  openbl_status_t status;

  /* Get the memory index to know from which memory interface we will used */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.Erase != NULL)
    {
      status = p_mem_list->memories[index].mem_ops.Erase(&p_mem_list->memories[index], p_data, data_length);
    }
    else
    {
      status = OPENBL_ERROR;
    }
  }
  else
  {
    status = OPENBL_ERROR;
  }

  return status;
}

/**
  * @brief  Check if a given address is valid and can be used for jump operation.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address The address to be checked.
  * @retval Returns 1 if the address is valid else returns 0.
  */
uint8_t OPENBL_MEM_CheckJumpAddress(openbl_mem_list_t *p_mem_list, uint32_t address)
{
  uint32_t index;
  uint8_t status;

  /* Get the memory index to know from which memory interface we will used */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (p_mem_list->memories[index].mem_ops.JumpToAddress != NULL)
  {
    status = 1U;
  }
  else
  {
    status = 0U;
  }

  return status;
}

/**
  * @brief  This function is used to perform post-processing operations on a given memory.
  * @param  p_mem_list Pointer to the memory list structure.
  * @param  address The address of the memory to be processed.
  * @retval None.
  */
void OPENBL_MEM_PostProcessing(openbl_mem_list_t *p_mem_list, uint32_t address)
{
  uint32_t index;

  /* Get the memory index to know from which memory interface we will used */
  index = OPENBL_MEM_GetMemoryIndex(p_mem_list, address);

  if (index < p_mem_list->memories_nbr)
  {
    if (p_mem_list->memories[index].mem_ops.PostProcessing != NULL)
    {
      p_mem_list->memories[index].mem_ops.PostProcessing(&p_mem_list->memories[index]);
    }
  }
}
