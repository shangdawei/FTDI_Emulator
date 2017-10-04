#include "stm32f10x_eeprom.h"

const Driver_EEPROM EEPROM_Driver = {
  STM32F10X_EE_FlashInit, STM32F10X_EE_FlashErasePage, STM32F10X_EE_FlashProgram, STM32F10X_EE_FlashRead };

int32_t STM32F10X_EE_FlashInit( uint32_t PageBase, uint32_t PageCount )
{
  /* Unlock the Flash Program Erase controller */
  FLASH_Unlock( );

  return EEPROM_SUCCESS;
}

int32_t STM32F10X_EE_FlashErasePage( uint32_t PageBase, uint32_t PageCount )
{
  uint32_t ExecuteErase;
  uint16_t FlashStatus;
  for ( uint32_t PageIndex = 0; PageIndex < PageCount; PageIndex++ )
  {
    ExecuteErase = 0;
    for ( uint32_t Addr = 0; Addr < STM32F10X_EE_FLASH_PAGE_SIZE ; Addr += 4 )
    {
      if ( 0xFFFFFFFF != *(__IO uint32_t *) ( PageBase + Addr ) )
      {
        ExecuteErase = 1;
        break;
      }
    }

    if ( ExecuteErase )
    {
      FlashStatus = FLASH_ErasePage( PageBase );
      if ( FLASH_COMPLETE == FlashStatus )
      {
        for ( uint32_t Addr = 0; Addr < STM32F10X_EE_FLASH_PAGE_SIZE ; Addr += 4 )
        {
          if ( 0xFFFFFFFF != *(__IO uint32_t *) ( PageBase + Addr ) )
            return EEPROM_FLASH_FAULT;
        }
      }
    }

    PageBase += STM32F10X_EE_FLASH_PAGE_SIZE;
  }

  return EEPROM_SUCCESS;
}

int32_t STM32F10X_EE_FlashProgram( uint32_t Addr, uint32_t Data )
{
  if ( *(__IO uint32_t *) Addr == Data )
    return EEPROM_SUCCESS;

  uint16_t FlashStatus;
  FlashStatus = FLASH_ProgramHalfWord( Addr, Data );
  if ( FLASH_COMPLETE == FlashStatus )
  {
    FlashStatus = FLASH_ProgramHalfWord( Addr + 2, Data >> 16 );
    if ( FLASH_COMPLETE == FlashStatus )
    {
      if ( *(__IO uint32_t *) Addr == Data )
        return EEPROM_SUCCESS;
    }
  }

  return EEPROM_FLASH_FAULT;
}

int32_t STM32F10X_EE_FlashRead( uint32_t Addr, uint32_t * Data )
{
  *Data = *(__IO uint32_t *) Addr;
  return EEPROM_SUCCESS;
}
