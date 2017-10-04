#ifndef __EEPROM_EMULATOR_H
#define __EEPROM_EMULATOR_H

#include <stdint.h>

typedef enum
{
  EEPROM_SUCCESS = 0,               // Everything is OK
  EEPROM_FLASH_FAULT = -1,          // Flash Hardware Fault, Erase Error, Program Error

  EEPROM_VIRGIN = -2,               // All Pages are ERASED, Eeprom need be formatted with init data
  EEPROM_PAGE_FULL = -3,            // Can not write Current Page because it is full
} EEPROM_Status;

typedef struct
{
  uint32_t FlashPageBase;           // Page Base Address of Flash
  uint32_t FlashPageCount;          // Physical Pages can be used
  uint32_t FlashPageSize;           // Erasable Page Size of Flash ---------- Physical Page Size
  uint32_t EepromCapacity;          // Variables' number in Byte
} EEPROM_Configuration;

typedef struct
{
  int32_t (*FlashInit)( uint32_t PageBase, uint32_t PageCount );        //
  int32_t (*FlashErasePage)( uint32_t PageBase, uint32_t PageCount );   //
  int32_t (*FlashProgram)( uint32_t Addr, uint32_t Data );              //
  int32_t (*FlashRead)( uint32_t Addr, uint32_t * Data );               //
} Driver_EEPROM;

int32_t EEPROM_Init( const EEPROM_Configuration * Configuration, const Driver_EEPROM * Driver, uint8_t * Cache );

int32_t EEPROM_Format( uint8_t * EepromData, uint32_t Size );

int32_t EEPROM_ReadByte( uint32_t Addr, uint8_t * Data );               // Read a byte from any address
int32_t EEPROM_ReadWord( uint32_t Addr, uint16_t * Data );              // Read a word from Even address
int32_t EEPROM_ReadDWord( uint32_t Addr, uint32_t * Data );             // Read a dword from DWORD alginment address

int32_t EEPROM_WriteByte( uint32_t Addr, uint8_t Data );                // Write a byte into any address
int32_t EEPROM_WriteWord( uint32_t Addr, uint16_t Data );               // Write a word into Even address
int32_t EEPROM_WriteDWord( uint32_t Addr, uint32_t Data );              // Write a dword into DWORD alginment address

#endif /* __EEPROM_EMULATOR_H */
