#include "FTDI_Eeprom.h"

#define FTDI_EEPROM_CFG0_OFFSET         ( 0x00 )
#define FTDI_EEPROM_CFG1_OFFSET         ( 0x01 )

#define FTDI_EEPROM_VID_OFFSET          ( 0x02 )
#define FTDI_EEPROM_PID_OFFSET          ( 0x04 )
#define FTDI_EEPROM_REV_OFFSET          ( 0x06 )
#define FTDI_EEPROM_CFG_OFFSET          ( 0x08 )
#define FTDI_EEPROM_PWR_OFFSET          ( 0x09 )

#define FTDI_EEPROM_CFGA_OFFSET         ( 0x0A )
#define FTDI_EEPROM_CFGB_OFFSET         ( 0x0B )

#define FTDI_EEPROM_CFGC_OFFSET         ( 0x0C )
#define FTDI_EEPROM_CFGD_OFFSET         ( 0x0D )

// USB Version for BM and 2232C
#define FTDI_EEPROM_VER_OFFSET          ( 0x0C )

#define FTDI_EEPROM_MANF_STR_OFFSET     ( 0x0E )
#define FTDI_EEPROM_MANF_LEN_OFFSET     ( 0x0F )
#define FTDI_EEPROM_PROD_STR_OFFSET     ( 0x10 )
#define FTDI_EEPROM_PROD_LEN_OFFSET     ( 0x11 )
#define FTDI_EEPROM_SNUM_STR_OFFSET     ( 0x12 )
#define FTDI_EEPROM_SNUM_LEN_OFFSET     ( 0x13 )

// FT232R
const UCHAR FTDI_Emulator_Eeprom[ 128 ] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };

const UCHAR FTDI_Emulator_FT232R_Eeprom[ 128 ] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };

const UCHAR FTDI_Emulator_ChipId[ 32 ] = {
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,    //
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };

const WORD FTDI_Emulator_DummyData = 0xFFFF;

FTDI_DeviceType FTDI_GetDeviceType( WORD bcdDevice )
{
  FTDI_DeviceType DeviceType = FTDI_DEVICE_UNKNOWN;

  if ( bcdDevice == 0x0200 )
    DeviceType = FTDI_DEVICE_AM;
  else if ( bcdDevice == 0x0400 )
    DeviceType = FTDI_DEVICE_BM;
  else if ( bcdDevice == 0x0500 )
    DeviceType = FTDI_DEVICE_2232C;
  else if ( bcdDevice == 0x0600 )
    DeviceType = FTDI_DEVICE_232R;
  else if ( bcdDevice == 0x0700 )
    DeviceType = FTDI_DEVICE_2232H;
  else if ( bcdDevice == 0x0800 )
    DeviceType = FTDI_DEVICE_4232H;
  else if ( bcdDevice == 0x0900 )
    DeviceType = FTDI_DEVICE_232H;
  else if ( bcdDevice == 0x1000 )
    DeviceType = FTDI_DEVICE_203X;
  else
    // if ( bcdDevice == 0x0000 )
    DeviceType = FTDI_DEVICE_232R;

  return DeviceType;
}

UCHAR FTDI_GetChipIDShift( UCHAR value )
{
  return ( ( value & 1 ) << 1 ) | ( ( value & 2 ) << 5 ) | ( ( value & 4 ) >> 2 ) | ( ( value & 8 ) << 4 )
    | ( ( value & 16 ) >> 1 ) | ( ( value & 32 ) >> 1 ) | ( ( value & 64 ) >> 4 ) | ( ( value & 128 ) >> 2 );
}

#define FTDI_CHIPID_OFFSET        ( 0x06 )
DWORD FTDI_GetChipID( UCHAR * ChipID_Data )
{
  DWORD Value = *(DWORD *) ( ChipID_Data + FTDI_CHIPID_OFFSET );
  DWORD ChipId = FTDI_GetChipIDShift( Value >> 0 ) << 0 | FTDI_GetChipIDShift( Value >> 8 ) << 8
    | FTDI_GetChipIDShift( Value >> 16 ) << 16 | FTDI_GetChipIDShift( Value >> 24 ) << 24;

  ChipId ^= 0xA5F0F7D1;

  return ChipId;
}

WORD FTDI_EepromCheckSumBuild( FTDI_DeviceType DeviceType, UCHAR * Eeprom, DWORD Size )
{
  WORD * wEeprom = (WORD *) Eeprom;
  WORD Value;
  WORD CheckSum = 0xAAAA;

  for ( DWORD wordAddr = 0; wordAddr < ( ( Size >> 1 ) - 1 ); wordAddr++ )
  {

    // FT230X has a user section in the MTP which is not part of the checksum
    if ( ( DeviceType == FTDI_DEVICE_203X ) && ( wordAddr == 0x12 ) )
    {
      wordAddr = 0x50;    // FIXME
      wordAddr = 0x40;    // FIXME
    }
    Value = wEeprom[ wordAddr ];
    CheckSum ^= Value;
    CheckSum = ( CheckSum << 1 ) | ( CheckSum >> 15 );
  }

  return CheckSum;
}

int32_t FTDI_EepromDecode( FTDI_DeviceContext * Device, UCHAR * Eeprom, DWORD Size )
{
  WORD * wEeprom = (WORD *) Eeprom;

  Device->EepromSize = Size;

  // VID, PID and bcdUSB of Device Descriptor
  // Addr 02: Vendor ID
  Device->VendorID = wEeprom[ FTDI_EEPROM_VID_OFFSET / 2 ];
  // Addr 04: Product ID
  Device->ProductID = wEeprom[ FTDI_EEPROM_PID_OFFSET / 2 ];
  // bcdDevice of Device Descriptor
  // Addr 06: Device release number
  Device->bcdDevice = wEeprom[ FTDI_EEPROM_REV_OFFSET / 2 ];
  Device->DeviceType = FTDI_GetDeviceType( Device->bcdDevice );
  if ( FTDI_DEVICE_UNKNOWN == Device->DeviceType )
    return -1;

  WORD CheckSum = FTDI_EepromCheckSumBuild( Device->DeviceType, Eeprom, Size );

  // Program Eeprom with Error Value
  if ( CheckSum != wEeprom[ Size / 2 - 1 ] )
  {
    // ReProgram Eeprom with Right Value
    EEPROM_WriteWord( Size - 2, CheckSum );
    return -1;
  }

  // bmAttributes of Configuration Descriptor
  // Addr 08: Config descriptor
  // Bit 7: always 1
  // Bit 6: 1 if this device is self powered, 0 if bus powered
  // Bit 5: 1 if this device uses remote wakeup
  // bmAttributes = 0x80 | Device->SefPowered | Device->RemoteWakeup
  Device->SelfPowered = Eeprom[ FTDI_EEPROM_CFG_OFFSET ] & 0x40;
  Device->RemoteWakeup = Eeprom[ FTDI_EEPROM_CFG_OFFSET ] & 0x20;

  // MaxPower of Configuration Descriptor
  // Addr 09: Max power consumption: max power = value * 2 mA
  // Device->MaxPower = 0 if ( Device->SefPowered > 0 )
  Device->MaxPower = Eeprom[ FTDI_EEPROM_PWR_OFFSET ];

  // Addr 0A: Chip configuration
  // Bit 7: 0 - reserved
  // Bit 6: 0 - reserved
  // Bit 5: 0 - reserved
  // Bit 4: 1 - Change USB version on BM and 2232C
  // Bit 3: 1 - Use the serial number string
  // Bit 2: 1 - Enable suspend pull downs for lower power
  // Bit 1: 1 - Out EndPoint is Isochronous
  // Bit 0: 1 - In EndPoint is Isochronous

  // Addr 0C: USB version low byte --- BM and 2232C : 0x0110
  // Addr 0D: USB version high byte -- BM and 2232C : 0x0200
  Device->USBVersionEnable = Eeprom[ FTDI_EEPROM_CFGA_OFFSET ] & 0x10;
  if ( Device->USBVersionEnable )
    Device->USBVersion = wEeprom[ FTDI_EEPROM_VER_OFFSET / 2 ];
  else
    // if ( 0x0000 == Device->USBVersion )
    Device->USBVersion = 0x0200;    // Except BM and 2232C

  // iSerialNumber = 0 if ( Device->SerNumEnable == 0 )
  // iSerialNumber = 3 if ( Device->SerNumEnable > 0 )
  Device->SerNumEnable = Eeprom[ FTDI_EEPROM_CFGA_OFFSET ] & 0x08;

  // Pull IO pins low during suspend
  Device->PullDownEnable = Eeprom[ FTDI_EEPROM_CFGA_OFFSET ] & 0x04;

  // Addr 0E: Offset of the manufacturer string + 0x80, calculated later
  // Addr 0F: Length of manufacturer string
  // Manufacturer String Descriptor : LEN+2 0x03 [ ---- LEN Bytes ---- ]
  Device->ManufactureStr = (char *) ( Eeprom + Eeprom[ FTDI_EEPROM_MANF_STR_OFFSET ] );
  Device->ManufactureStrSize = Eeprom[ FTDI_EEPROM_MANF_LEN_OFFSET ];

  // Addr 10: Offset of the product string + 0x80, calculated later
  // Addr 11: Length of product string
  // Product String Descriptor : LEN+2 0x03 [ ---- LEN Bytes ---- ]
  Device->ProductStr = (char *) ( Eeprom + Eeprom[ FTDI_EEPROM_PROD_STR_OFFSET ] );
  Device->ProductStrSize = Eeprom[ FTDI_EEPROM_PROD_LEN_OFFSET ];

  // Addr 12: Offset of the serial number string + 0x80, calculated later
  // Addr 13: Length of serial number string
  // Serial Number String Descriptor : LEN+2 0x03 [ ---- LEN Bytes ---- ]
  Device->SerialNumberStr = (char *) ( Eeprom + Eeprom[ FTDI_EEPROM_SNUM_STR_OFFSET ] );
  Device->SerialNumberStrSize = Eeprom[ FTDI_EEPROM_SNUM_LEN_OFFSET ];

  if ( FTDI_DEVICE_BM == Device->DeviceType )
  {
    Device->Device_BM.IsoIn = Eeprom[ FTDI_EEPROM_CFGA_OFFSET ] & 0x01;
    Device->Device_BM.IsoOut = Eeprom[ FTDI_EEPROM_CFGA_OFFSET ] & 0x02;
  }
  else if ( FTDI_DEVICE_2232C == Device->DeviceType )
  {
    Device->Device_2232C.AIsVCP = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_DRIVER_VCP;
    Device->Device_2232C.AIsHighCurrent = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_HIGH_CURRENT_DRIVE;
    Device->Device_2232C.AFunction = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_IF_FUNCTION_MASK07;

    Device->Device_2232C.BIsVCP = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_DRIVER_VCP;
    Device->Device_2232C.BIsHighCurrent = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_HIGH_CURRENT_DRIVE;
    Device->Device_2232C.BFunction = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_IF_FUNCTION_MASK07;

    // Device->EepromType = Eeprom[ 0x14 ];    // 93C46, 93C56, 93C66
  }
  else if ( FTDI_DEVICE_232R == Device->DeviceType )
  {
    Device->Device_232R.UseExtOsc = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_232R_USES_EXT_OSC;
    Device->Device_232R.HighDriveIOs = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_232R_HIGH_CURRENT_DRIVE;
    // FT232R uses flags for D2XX, Others use flags for VCP
    Device->Device_232R.IsD2XX = ( Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_DRIVER_VCP ) ? 1 : 0;

    // wMaxPacketSize of Endpoint Descriptor
    Device->Device_232R.EndpointSize = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ];    // 0x40
    if ( 0x40 != Device->Device_232R.EndpointSize )
      Device->Device_232R.EndpointSize = 0x40;

    // Device->EepromType = FTDI_EEPROM_93C46;

    Device->Device_232R.InvertSet = Eeprom[ FTDI_EEPROM_CFGB_OFFSET ];
    if ( 1 )
    {
      for ( DWORD i = 0; i < 3; i++ )
      {
        // FTDI_CBUS_TXDEN, FTDI_CBUS_PWREN...
        Device->Device_232R.CbusFunctions[ i * 2 + 0 ] = ( Eeprom[ 0x14 + i ] >> 0 ) & 0x0F;
        Device->Device_232R.CbusFunctions[ i * 2 + 1 ] = ( Eeprom[ 0x14 + i ] >> 4 ) & 0x0F;
      }
    }
    else
    {
      Device->Device_232R.Cbus0 = ( Eeprom[ 0x14 ] >> 0 ) & FTDI_IF_FUNCTION_MASK0F;
      Device->Device_232R.Cbus1 = ( Eeprom[ 0x14 ] >> 4 ) & FTDI_IF_FUNCTION_MASK0F;
      Device->Device_232R.Cbus2 = ( Eeprom[ 0x15 ] >> 0 ) & FTDI_IF_FUNCTION_MASK0F;
      Device->Device_232R.Cbus3 = ( Eeprom[ 0x15 ] >> 4 ) & FTDI_IF_FUNCTION_MASK0F;
      Device->Device_232R.Cbus4 = ( Eeprom[ 0x16 ] >> 0 ) & FTDI_IF_FUNCTION_MASK0F;
    }
  }
  else if ( FTDI_DEVICE_2232H == Device->DeviceType )
  {
    // Device->EepromType = Eeprom[ 0x18 ];

    // Suspend on DBUS7
    Device->Device_2232H.PowerSaveEnable = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_SUSPEND_ON_DBUS7;

    Device->Device_2232H.AIsVCP = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_DRIVER_VCP;
    Device->Device_2232H.BIsVCP = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_DRIVER_VCP;

    Device->Device_2232H.AFunction = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_IF_FUNCTION_MASK07;
    Device->Device_2232H.BFunction = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_IF_FUNCTION_MASK07;

  }
  else if ( FTDI_DEVICE_4232H == Device->DeviceType )
  {
    // Device->EepromType = Eeprom[ 0x18 ];
    Device->Device_4232H.AIsVCP = ( Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] >> 0 ) & FTDI_DRIVER_VCP;
    Device->Device_4232H.BIsVCP = ( Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] >> 0 ) & FTDI_DRIVER_VCP;
    Device->Device_4232H.CIsVCP = ( Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] >> 4 ) & FTDI_DRIVER_VCP;
    Device->Device_4232H.DIsVCP = ( Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] >> 4 ) & FTDI_DRIVER_VCP;

    // Eeprom[ FTDI_EEPROM_CFGB_OFFSET ] : DCBA-000
    Device->Device_4232H.ARIIsTXDEN = Eeprom[ FTDI_EEPROM_CFGB_OFFSET ] & ( FTDI_IF_FUNCTION_RS485 << 0 );
    Device->Device_4232H.BRIIsTXDEN = Eeprom[ FTDI_EEPROM_CFGB_OFFSET ] & ( FTDI_IF_FUNCTION_RS485 << 1 );
    Device->Device_4232H.CRIIsTXDEN = Eeprom[ FTDI_EEPROM_CFGB_OFFSET ] & ( FTDI_IF_FUNCTION_RS485 << 2 );
    Device->Device_4232H.DRIIsTXDEN = Eeprom[ FTDI_EEPROM_CFGB_OFFSET ] & ( FTDI_IF_FUNCTION_RS485 << 3 );
  }
  else if ( FTDI_DEVICE_232H == Device->DeviceType )
  {
    // FTDI_IF_FUNCTION_FT1284 = 0x08
    Device->Device_232H.Function = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_IF_FUNCTION_MASK0F;
    Device->Device_232H.IsVCP = Eeprom[ FTDI_EEPROM_CFG0_OFFSET ] & FTDI_232H_DRIVER_VCP;

    // Enter low power state on ACBUS7
    Device->Device_232H.PowerSaveEnable = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_SUSPEND_ON_ABUS7;

    // FT1284 Mode Clock is idle HIGH : Clock Polariry high
    Device->Device_232H.FT1248Cpol = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_FT1284_CPOL;

    // Flow control active when the device is not selected.
    Device->Device_232H.FT1248FlowControl = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_FT1284_FC;

    // With this option checked the data will be LSB first.
    Device->Device_232H.FT1248Lsb = Eeprom[ FTDI_EEPROM_CFG1_OFFSET ] & FTDI_FT1284_LSB;

    for ( DWORD i = 0; i < 5; i++ )
    {
      // FTDI_232H_CBUS_TRISTATE, FTDI_232H_CBUS_TXLED...
      Device->Device_232H.CbusFunctions[ i * 2 + 0 ] = ( Eeprom[ 0x18 + i ] >> 0 ) & FTDI_IF_FUNCTION_MASK0F;
      Device->Device_232H.CbusFunctions[ i * 2 + 1 ] = ( Eeprom[ 0x18 + i ] >> 4 ) & FTDI_IF_FUNCTION_MASK0F;
    }
  }
  else if ( FTDI_DEVICE_203X == Device->DeviceType )
  {
    // Device->EepromType = Eeprom[ 0x0B ];
    Device->Device_230X.InvertSet = Eeprom[ FTDI_EEPROM_CFGB_OFFSET ];

    for ( DWORD i = 0; i < 4; i++ )
    {
      // FTDI_230X_CBUS_TRISTATE, FTDI_230X_CBUS_TXLED...
      Device->Device_230X.CbusFunctions[ i ] = Eeprom[ 0x1A + i ];
    }
  }

  if ( 128 == Device->EepromSize )
  {
    Device->ManufactureStr -= 0x80;
    Device->ProductStr -= 0x80;
    Device->SerialNumberStr -= 0x80;
  }

  return 0;
}
