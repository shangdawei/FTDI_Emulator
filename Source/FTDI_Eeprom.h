#ifndef __FTDI_EEPROM_H
#define __FTDI_EEPROM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "FTDI_Type.h"
#include "EEPROM_Emulator.h"

extern const UCHAR FTDI_Emulator_FT232R_Eeprom[ 128 ];
extern const UCHAR FTDI_Emulator_Eeprom[ 128 ];
extern const UCHAR FTDI_Emulator_ChipId[ 32 ];
extern const WORD FTDI_Emulator_DummyData;

DWORD FTDI_GetChipID( UCHAR * ChipID_Data );

int32_t FTDI_EepromDecode( FTDI_DeviceContext * Device, UCHAR * Eeprom, DWORD Size );

#ifdef __cplusplus
}
#endif

#endif /* __FTDI_EEPROM_H */
