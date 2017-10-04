#ifndef __STM32F10X_EEPROM_H
#define __STM32F10X_EEPROM_H

#include "stm32f10x.h"
#include "EEPROM_Emulator.h"

#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
#define STM32F10X_EE_FLASH_PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */

#elif defined (STM32F10X_HD) || defined (STM32F10X_CL)
#define STM32F10X_EE_FLASH_PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#endif

int32_t STM32F10X_EE_FlashInit( uint32_t PageBase, uint32_t PageCount );
int32_t STM32F10X_EE_FlashErasePage( uint32_t PageBase, uint32_t PageCount );
int32_t STM32F10X_EE_FlashProgram( uint32_t Addr, uint32_t Data );
int32_t STM32F10X_EE_FlashRead( uint32_t Addr, uint32_t * Data );

extern const Driver_EEPROM EEPROM_Driver;

#endif /* __STM32F10X_EEPROM_H */

