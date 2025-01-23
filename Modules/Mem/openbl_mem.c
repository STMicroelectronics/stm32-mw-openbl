/**
  ******************************************************************************
  * @file    openbl_mem.c
  * @author  MCD Application Team
  * @brief   Provides functions that manage operations on embedded memories (Flash, SRAM...).
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019-2021 STMicroelectronics.
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

#include "interfaces_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t NumberOfMemories = 0U;
static OPENBL_MemoryTypeDef a_MemoriesTable[MEMORIES_SUPPORTED];

/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  This function is used to register memory interfaces in Open Bootloader MW.
  * @param  *Memory A pointer to the memory handle.
  * @retval ErrorStatus Returns ERROR in case of no more space in the memories table else returns SUCCESS.
  */
ErrorStatus OPENBL_MEM_RegisterMemory(const OPENBL_MemoryTypeDef *Memory)
{
  ErrorStatus status = SUCCESS;

  if (NumberOfMemories < MEMORIES_SUPPORTED)
  {
    a_MemoriesTable[NumberOfMemories].StartAddress      = Memory->StartAddress;
    a_MemoriesTable[NumberOfMemories].EndAddress        = Memory->EndAddress;
    a_MemoriesTable[NumberOfMemories].Size              = Memory->Size;
    a_MemoriesTable[NumberOfMemories].Type              = Memory->Type;
    a_MemoriesTable[NumberOfMemories].Init              = Memory->Init;
    a_MemoriesTable[NumberOfMemories].Read              = Memory->Read;
    a_MemoriesTable[NumberOfMemories].Write             = Memory->Write;
    a_MemoriesTable[NumberOfMemories].JumpToAddress     = Memory->JumpToAddress;
    a_MemoriesTable[NumberOfMemories].MassErase         = Memory->MassErase;
    a_MemoriesTable[NumberOfMemories].SectorErase       = Memory->SectorErase;
    a_MemoriesTable[NumberOfMemories].Verify            = Memory->Verify;

    NumberOfMemories++;
  }
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  Check if a given address is valid or not and returns the area type.
  * @param  Address The address to be checked.
  * @retval The address area: FLASH_AREA, RAM_AREA... if the address is valid
  *         or AREA_ERROR if the address is not valid.
  */
uint32_t OPENBL_MEM_GetAddressArea(uint32_t Address)
{
  uint32_t mem_area = AREA_ERROR;
  uint32_t counter;

  for (counter = 0U; counter < NumberOfMemories; counter++)
  {
    if ((Address >= a_MemoriesTable[counter].StartAddress) && (Address < a_MemoriesTable[counter].EndAddress))
    {
      mem_area = a_MemoriesTable[counter].Type;
      break;
    }
    else
    {
      mem_area = AREA_ERROR;
    }
  }

  return mem_area;
}

/**
  * @brief  This function returns the index of the memory that matches the address given in parameter.
  * @param  Address This address is used determinate the index of the memory pointed by this address.
  * @retval The index of the memory that corresponds to the address
  */
uint32_t OPENBL_MEM_GetMemoryIndex(uint32_t Address)
{
  uint32_t counter;

  for (counter = 0; counter < NumberOfMemories; counter++)
  {
    if ((Address >= a_MemoriesTable[counter].StartAddress) && (Address < a_MemoriesTable[counter].EndAddress))
    {
      break;
    }
  }

  return counter;
}

/**
  * @brief  This function is used to initialize a given memory.
  * @param  Address The address of the memory to be initialized.
  * @retval Returns 1 in case of success else returns 0.
 */
uint32_t OPENBL_MEM_Init(uint32_t Address)
{
  uint32_t memory_index;

  /* Get the memory index to know from which memory interface we will used */
  memory_index = OPENBL_MEM_GetMemoryIndex(Address);

  if (memory_index < NumberOfMemories)
  {
    if (a_MemoriesTable[memory_index].Init != NULL)
    {
      return a_MemoriesTable[memory_index].Init(Address);
    }
  }

  return 0U;
}

/**
  * @brief  This function returns the value read from the given address.
  *         The memory index is used to know which memory interface will be used to read from the given address.
  * @param  Address The address that will be read.
  * @param  MemoryIndex The memory index of the memory interface that will be used to read from the given address.
  * @retval Returns the read value.
  */
uint8_t OPENBL_MEM_Read(uint32_t Address, uint32_t MemoryIndex)
{
  uint8_t value;

  if (MemoryIndex < NumberOfMemories)
  {
    if (a_MemoriesTable[MemoryIndex].Read != NULL)
    {
      value = a_MemoriesTable[MemoryIndex].Read(Address);
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
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_MEM_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
  uint32_t index;

  /* Get the memory index to know in which memory we will write */
  index = OPENBL_MEM_GetMemoryIndex(Address);

  if (index < NumberOfMemories)
  {
    if (a_MemoriesTable[index].Write != NULL)
    {
      a_MemoriesTable[index].Write(Address, Data, DataLength);
    }
  }
}

/**
  * @brief  Verify flash memory with RAM buffer and calculates checksum value of the programmed memory
  * @param  Address The Flash address.
  * @param  DataAddr The RAM buffer address.
  * @param  DataLength The Size (in WORD).
  * @param  CrcInit The Initial CRC value.
  * @retval R0: Operation failed (address of failure).
  *         R1: Checksum value.
  */
uint64_t OPENBL_MEM_Verify(uint32_t Address, uint32_t DataAddr, uint32_t DataLength, uint32_t CrcInit)
{
  uint32_t index;
  uint64_t value;

  /* Get the memory index to know in which memory we will write */
  index = OPENBL_MEM_GetMemoryIndex(Address);

  if (index < NumberOfMemories)
  {
    if (a_MemoriesTable[index].Verify != NULL)
    {
      value = a_MemoriesTable[index].Verify(Address, DataAddr, DataLength, CrcInit);
    }
    else
    {
      value = 0;
    }
  }
  else
  {
    value = 0;
  }

  return value;
}

/**
  * @brief  Enables or disables the read out protection.
  * @param  Address The address where the memory protection will be.
  * @param  State The readout protection state that will be set.
  * @retval None.
  */
void OPENBL_MEM_SetReadOutProtection(uint32_t Address, FunctionalState State)
{
  /* Nothing todo */

  UNUSED(Address);
  UNUSED(State);
}

/**
  * @brief  This function is used to enable or disable write protection of the specified FLASH areas.
  * @param  State Can be one of these values:
  *         @arg DISABLE: Disable FLASH write protection
  *         @arg ENABLE: Enable FLASH write protection
  * @param  Address Contains the address of the memory where write protection state will be changed
  * @param  Buffer Contains write protection operation options
  * @param  Length The length of the write protection options Buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
ErrorStatus OPENBL_MEM_SetWriteProtection(FunctionalState State, uint32_t Address, const uint8_t *Buffer,
                                          uint32_t Length)
{
  ErrorStatus status = SUCCESS;

  UNUSED(State);
  UNUSED(Address);
  UNUSED(Buffer);
  UNUSED(Length);

  return status;
}

/**
  * @brief  This function is used to jump to an address of an application.
  * @param  Address User application address.
  * @retval None.
  */
void OPENBL_MEM_JumpToAddress(uint32_t Address)
{
  uint32_t memory_index;

  /* Get the memory index to know from which memory interface we will used */
  memory_index = OPENBL_MEM_GetMemoryIndex(Address);

  if (memory_index < NumberOfMemories)
  {
    if (a_MemoriesTable[memory_index].JumpToAddress != NULL)
    {
      a_MemoriesTable[memory_index].JumpToAddress(Address);
    }
  }
}

/**
  * @brief  This function is used to start a mass erase operation on a given memory.
  * @param  Address The address of the memory to be fully erased.
  * @param  *p_Data Pointer to the buffer that contains mass erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Mass erase operation done
  *          - ERROR:   Mass erase operation failed or one parameter is invalid
 */
void OPENBL_MEM_MassErase(uint32_t Address)
{
  uint32_t memory_index;

  /* Get the memory index to know from which memory interface we will used */
  memory_index = OPENBL_MEM_GetMemoryIndex(Address);

  if (memory_index < NumberOfMemories)
  {
    if (a_MemoriesTable[memory_index].MassErase != NULL)
    {
      a_MemoriesTable[memory_index].MassErase(Address);
    }
  }

}

/**
  * @brief  This function is used to erase the specified memory.
  * @param  Address The address of the memory to be erased.
  * @param  *p_Data Pointer to the buffer that contains erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Erase operation done
  *          - ERROR:   Erase operation failed or one parameter is invalid
 */
void OPENBL_MEM_SectorErase(uint32_t Address, uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
  uint32_t memory_index;

  /* Get the memory index to know from which memory interface we will used */
  memory_index = OPENBL_MEM_GetMemoryIndex(Address);

  if (memory_index < NumberOfMemories)
  {
    if (a_MemoriesTable[memory_index].SectorErase != NULL)
    {
      a_MemoriesTable[memory_index].SectorErase(EraseStartAddress, EraseEndAddress);
    }
  }

}

/**
  * @brief  This function is used to erase the specified memory.
  * @param  Address The address of the memory to be erased.
  * @param  *p_Data Pointer to the buffer that contains erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Erase operation done
  *          - ERROR:   Erase operation failed or one parameter is invalid
 */
ErrorStatus OPENBL_MEM_Erase(uint32_t Address, const uint8_t *p_Data, uint32_t DataLength)
{
  ErrorStatus status = ERROR;

  UNUSED(Address);
  UNUSED(p_Data);
  UNUSED(DataLength);

  return status;
}

/**
  * @brief  Check if a given address is valid and can be used for jump operation
  * @param  Address The address to be checked.
  * @retval Returns 1 if the address is valid else returns 0.
  */
uint8_t OPENBL_MEM_CheckJumpAddress(uint32_t Address)
{
  uint32_t memory_index;
  uint8_t status;

  /* Get the memory index to know from which memory interface we will used */
  memory_index = OPENBL_MEM_GetMemoryIndex(Address);

  if (a_MemoriesTable[memory_index].JumpToAddress != NULL)
  {
    status = 1U;
  }
  else
  {
    status = 0U;
  }

  return status;
}
