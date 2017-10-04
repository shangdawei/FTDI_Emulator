/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "usb_mem.h"
#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_pwr.h"

#include "fifo.h"

#include "FTDI_Bsp.h"
#include "FTDI_Emulator.h"
#include "FTDI_Eeprom.h"
#include "FTDI_Descriptor.h"

#include "stm32f10x_eeprom.h" // Eeprom Emulator Driver
#include "EEPROM_Emulator.h"

FTDI_DeviceContext Device;

const EEPROM_Configuration Configuration = {
  0x08007000, 4, STM32F10X_EE_FLASH_PAGE_SIZE, 128 };

uint8_t EEPROM_Cache[ 128 ];

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define FTDI_EMULATOR_IN_FRAME_INTERVAL             5

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Request = 0;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table = {
  EP_NUM, 1 };

DEVICE_PROP Device_Property = {           //
    FTDI_Emulator_Init,                     //
    FTDI_Emulator_Reset,                    //
    FTDI_Emulator_Status_In,                //
    FTDI_Emulator_Status_Out,               //
    FTDI_Emulator_Data_Setup,               //
    FTDI_Emulator_NoData_Setup,             //
    FTDI_Emulator_Get_Interface_Setting,    //
    FTDI_Emulator_GetDeviceDescriptor,      //
    FTDI_Emulator_GetConfigDescriptor,      //
    FTDI_Emulator_GetStringDescriptor,      //
    0,                                      //
    0x08 /* EP0 MAX PACKET SIZE */          //
  };

USER_STANDARD_REQUESTS User_Standard_Requests = {    //
    FTDI_Emulator_GetConfiguration,         //
    FTDI_Emulator_SetConfiguration,         //
    FTDI_Emulator_GetInterface,             //
    FTDI_Emulator_SetInterface,             //
    FTDI_Emulator_GetStatus,                //
    FTDI_Emulator_ClearFeature,             //
    FTDI_Emulator_SetEndPointFeature,       //
    FTDI_Emulator_SetDeviceFeature,         //
    FTDI_Emulator_SetDeviceAddress };

ONE_DESCRIPTOR Device_Descriptor;

ONE_DESCRIPTOR Config_Descriptor;

ONE_DESCRIPTOR String_Descriptor[ 4 ] = {
  {
    (uint8_t*) FTDI_Emulator_StringLangID, 4 },           //
  {
    (uint8_t*) FTDI_Emulator_StringManufacturer, 0 },     //
  {
    (uint8_t*) FTDI_Emulator_StringProduct, 0 },          //
  {
    (uint8_t*) FTDI_Emulator_StringSerialNumber, 0 } };

void FTDI_Emulator_OnRxCompleted( FTDI_InterfaceContext * Interface );
uint32_t FTDI_Emulator_OnTxCompleted( FTDI_InterfaceContext * Interface );

static void Halt( void )
{
  for ( ;; )
    ;
}

void FTDI_Emulator_DestoryInterfaces( void )
{
  FTDI_InterfaceContext * Interface;
  for ( uint32_t i = 0; i < Device.InterfaceNum; i++ )
  {
    Interface = Device.Interfaces[ i ];
    if ( Interface )
    {
      FTDI_Emulator_BSP_DeInit( Interface );
      free( Interface );
      Device.Interfaces[ i ] = 0;
    }
  }
}

USB_COMMON_DESCRIPTOR * USB_GetDescriptor( USB_COMMON_DESCRIPTOR * Descriptor, uint8_t Type )
{
  USB_COMMON_DESCRIPTOR * Desc = Descriptor;
  do
  {
    Desc = (USB_COMMON_DESCRIPTOR *) ( ( (uint8_t *) Desc ) + Desc->bLength );
  }while ( Type != Desc->bDescriptorType );

  return Desc;
}

int32_t FTDI_Emulator_CreateInterfaces( void )
{
  USB_CONFIGURATION_DESCRIPTOR * Cfg = (USB_CONFIGURATION_DESCRIPTOR *) FTDI_Emulator_ConfigDescriptor;
  USB_INTERFACE_DESCRIPTOR * Inf;
  USB_ENDPOINT_DESCRIPTOR * Ep;
  USB_COMMON_DESCRIPTOR * Desc;

  FTDI_InterfaceContext * Interface;
  int32_t RetValue = 0;

  Desc = (USB_COMMON_DESCRIPTOR *) ( Cfg );
  for ( uint32_t i = 0; i < Device.InterfaceNum; i++ )
  {
    Interface = malloc( sizeof(FTDI_InterfaceContext) );
    Device.Interfaces[ i ] = Interface;
    if ( 0 == Interface )
    {
      RetValue = -1;
      break;
    }

    Interface->Device = &Device;
    Interface->OnRxCompleted = FTDI_Emulator_OnRxCompleted;
    Interface->OnTxCompleted = FTDI_Emulator_OnTxCompleted;

    Desc = USB_GetDescriptor( Desc, USB_INTERFACE_DESCRIPTOR_TYPE );
    Inf = (USB_INTERFACE_DESCRIPTOR *) Desc;
    Interface->InterfaceNumber = Inf->bInterfaceNumber;

    for ( uint32_t e = 0; e < 2; e++ )
    {
      Desc = USB_GetDescriptor( Desc, USB_ENDPOINT_DESCRIPTOR_TYPE );
      Ep = (USB_ENDPOINT_DESCRIPTOR *) Desc;
      if ( Ep->bEndpointAddress & 0x80 )
      {
        Interface->InEndpoint = Ep->bEndpointAddress;
        Interface->InEndpointSize = 0x40;    // Ep->wMaxPacketSize;
      }
      else
      {
        Interface->OutEndpoint = Ep->bEndpointAddress;
        Interface->OutEndpointSize = 0x40;    // Ep->wMaxPacketSize;
      }
    }

    Interface->LatencyTimerValue = 16;
    Interface->LatencyTimerCount = 0;
    Interface->EventChar = 0x0D;
    Interface->ErrorChar = 0x00;

    // Config TxFifo, RxFifo, Clock and IO
    Interface->BaudRate = 9600;
    Interface->FlowControl = FTDI_SET_FLOW_CTRL_NONE;
    Interface->LineCoding.DataBits = FTDI_SET_DATA_DATA_BITS_8;
    Interface->LineCoding.StopBits = FTDI_SET_DATA_STOP_BITS_1;
    Interface->LineCoding.Parity = FTDI_SET_DATA_PARITY_NONE;

    RetValue = FTDI_Emulator_BSP_Init( Interface );
  }

  if ( RetValue < 0 )
    FTDI_Emulator_DestoryInterfaces( );

  return RetValue;
}

void FTDI_Emulator_BuildDescriptors( void )
{
  USB_DEVICE_DESCRIPTOR * Dev = &FTDI_Emulator_DeviceDescriptor;
  USB_CONFIGURATION_DESCRIPTOR * Cfg = (USB_CONFIGURATION_DESCRIPTOR *) FTDI_Emulator_ConfigDescriptor;

  memcpy( Dev, FTDI_FT232R_DeviceDescriptor, sizeof( FTDI_FT232R_DeviceDescriptor ) );

  Dev->bcdUSB = Device.USBVersion;
  Dev->idVendor = Device.VendorID;
  Dev->idProduct = Device.ProductID;
  Dev->bcdDevice = Device.bcdDevice;
  if ( ( FTDI_DEVICE_232H == Device.DeviceType ) || ( FTDI_DEVICE_2232H == Device.DeviceType )
    || ( FTDI_DEVICE_4232H == Device.DeviceType ) )
    Dev->bMaxPacketSize0 = 0x40;
  if ( 0 == Device.SerNumEnable )
    Dev->iSerialNumber = 0;

  memcpy( &FTDI_Emulator_StringManufacturer[ 0 ], Device.ManufactureStr, Device.ManufactureStrSize );
  memcpy( &FTDI_Emulator_StringProduct[ 0 ], Device.ProductStr, Device.ProductStrSize );
  if ( Device.SerNumEnable )
    memcpy( &FTDI_Emulator_StringSerialNumber[ 0 ], Device.SerialNumberStr, Device.SerialNumberStrSize );

  String_Descriptor[ 1 ].Descriptor_Size = Device.ManufactureStrSize;
  String_Descriptor[ 2 ].Descriptor_Size = Device.ProductStrSize;
  if ( Device.SerNumEnable )
    String_Descriptor[ 3 ].Descriptor_Size = Device.SerialNumberStrSize;

  memcpy( Cfg, FTDI_FT4232H_ConfigDescriptor, sizeof( FTDI_FT4232H_ConfigDescriptor ) );
// ( (USB_CONFIGURATION_DESCRIPTOR *) FTDI_FT4232H_ConfigDescriptor )->wTotalLength );

  if ( ( FTDI_DEVICE_2232C == Device.DeviceType ) || ( FTDI_DEVICE_2232H == Device.DeviceType ) )
  {
    Cfg->wTotalLength = 55;
    Cfg->bNumInterfaces = 2;
  }
  else if ( FTDI_DEVICE_4232H != Device.DeviceType )
  {
    Cfg->wTotalLength = 32;
    Cfg->bNumInterfaces = 1;
  }
  else
  {
    // Cfg->wTotalLength = 101;
    // Cfg->bNumInterfaces = 4;
  }

  Cfg->bmAttributes = 0x80;
  Cfg->bMaxPower = Device.MaxPower;
  if ( Device.SelfPowered )
  {
    Cfg->bmAttributes |= 0x40;    // Self Powered
    Cfg->bMaxPower = 0;
  }
  if ( 1 == Device.RemoteWakeup )
    Cfg->bmAttributes |= 0x20;    // Remote Wakeup

  Device_Descriptor.Descriptor = (uint8_t *) Dev;
  Device_Descriptor.Descriptor_Size = Dev->bLength;

  Config_Descriptor.Descriptor = (uint8_t *) FTDI_Emulator_ConfigDescriptor;
  Config_Descriptor.Descriptor_Size = Cfg->wTotalLength;

  Device.InterfaceNum = Cfg->bNumInterfaces;
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_init.
 * Description    : FTDI Emulator Mouse init routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void FTDI_Emulator_Init( void )
{
  int32_t RetValue;

  RetValue = EEPROM_Init( &Configuration, &EEPROM_Driver, EEPROM_Cache );
  if ( ( EEPROM_VIRGIN != RetValue ) && ( EEPROM_SUCCESS != RetValue ) )
    Halt( );

  if ( EEPROM_VIRGIN == RetValue )
  {
    RetValue = EEPROM_Format( (uint8_t *) FTDI_Emulator_Eeprom, sizeof( FTDI_Emulator_Eeprom ) );
    // RetValue = EEPROM_Format( (uint8_t *) FTDI_Emulator_FT232R_Eeprom, sizeof( FTDI_Emulator_FT232R_Eeprom ) );
    if ( RetValue != EEPROM_SUCCESS )
      Halt( );
  }

  if ( 0 > FTDI_EepromDecode( &Device, EEPROM_Cache, 128 ) )
    Halt( );

  FTDI_Emulator_BuildDescriptors( );

  if ( 0 > FTDI_Emulator_CreateInterfaces( ) )
    Halt( );

  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn( );

  /* Perform basic device initialization operations */
  USB_SIL_Init( );

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_Reset
 * Description    : FTDI_Emulator Mouse reset routine
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void FTDI_Emulator_Reset( void )
{
  /* Set FTDI_Emulator DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = ( (USB_CONFIGURATION_DESCRIPTOR *) FTDI_Emulator_ConfigDescriptor )->bmAttributes;

  /* Set FTDI_Emulator DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;

  SetBTABLE( BTABLE_ADDRESS );

  SetEPType( ENDP0, EP_CONTROL );
  SetEPTxStatus( ENDP0, EP_TX_STALL );
  SetEPRxAddr( ENDP0, ENDP0_RXADDR );
  SetEPTxAddr( ENDP0, ENDP0_TXADDR );
  Clear_Status_Out( ENDP0 );
  SetEPRxCount( ENDP0, FTDI_Emulator_DeviceDescriptor.bMaxPacketSize0 );
  SetEPRxValid( ENDP0 );

  if ( Device.InterfaceNum > 0 )          // Any
  {
    SetEPType( ENDP1, EP_BULK );
    SetEPTxAddr( ENDP1, ENDP1_TXADDR );
    SetEPTxStatus( ENDP1, EP_TX_NAK );    // Naked and all transmission requests result in a NAK handshake.
    SetEPRxStatus( ENDP1, EP_RX_DIS );    // all transmission requests addressed are ignored.

    EP1_IN_Callback( );

    SetEPType( ENDP2, EP_BULK );
    SetEPRxAddr( ENDP2, ENDP2_RXADDR );
    SetEPRxCount( ENDP2, 0x40 );
    SetEPRxStatus( ENDP2, EP_RX_VALID );  // this endpoint is enabled for transmission.
    SetEPTxStatus( ENDP2, EP_TX_DIS );    // all transmission requests addressed are ignored.
  }

  if ( Device.InterfaceNum > 1 )    // 2232C, 2232H
  {

  }

  if ( Device.InterfaceNum > 2 )    // 4232H
  {

  }

  /* Set this device to response on default address */
  SetDeviceAddress( 0 );

  bDeviceState = ATTACHED;
}

RESULT FTDI_Emulator_ControlHandler( uint8_t RequestNo );

/*******************************************************************************
 * Function Name  : FTDI_Emulator_SetConfiguration.
 * Description    : Update the device state to configured.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void FTDI_Emulator_SetConfiguration( void )
{
  DEVICE_INFO *pInfo = &Device_Info;

  if ( pInfo->Current_Configuration != 0 )
  {
    ClearDTOG_TX( ENDP1 );
    ClearDTOG_RX( ENDP2 );

    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_SetConfiguration.
 * Description    : Update the device state to addressed.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void FTDI_Emulator_SetDeviceAddress( void )
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_Status_In.
 * Description    : FTDI Emulator Status In Routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *
 * Host has received Status In from Device for Control Out Request
 * Handle Control Out Setup[8] and Data[N]
 *******************************************************************************/
void FTDI_Emulator_Status_In( void )
{
  FTDI_Emulator_ControlHandler( FTDI_LAST_REQUEST );
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_Status_Out
 * Description    : FTDI Emulator Status OUT Routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *
 * Status Out from Host for Control In Request
 * Update Data for Next Control In
 *******************************************************************************/
void FTDI_Emulator_Status_Out( void )
{
  FTDI_Emulator_ControlHandler( FTDI_LAST_REQUEST );
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_Data_Setup
 * Description    : handle the data class specific requests
 * Input          : Request Nb.
 * Output         : None.
 * Return         : USB_UNSUPPORT or USB_SUCCESS.
 *******************************************************************************/
RESULT FTDI_Emulator_Data_Setup( uint8_t RequestNo )
{
  return FTDI_Emulator_ControlHandler( RequestNo );
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_NoData_Setup.
 * Description    : handle the no data class specific requests.
 * Input          : Request Nb.
 * Output         : None.
 * Return         : USB_UNSUPPORT or USB_SUCCESS.
 *******************************************************************************/
RESULT FTDI_Emulator_NoData_Setup( uint8_t RequestNo )
{
  return FTDI_Emulator_ControlHandler( RequestNo );
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_GetDeviceDescriptor.
 * Description    : Gets the device descriptor.
 * Input          : Length.
 * Output         : None.
 * Return         : The address of the device descriptor.
 *******************************************************************************/
uint8_t *FTDI_Emulator_GetDeviceDescriptor( uint16_t Length )
{
  return Standard_GetDescriptorData( Length, &Device_Descriptor );
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_GetConfigDescriptor.
 * Description    : get the configuration descriptor.
 * Input          : Length.
 * Output         : None.
 * Return         : The address of the configuration descriptor.
 *******************************************************************************/
uint8_t *FTDI_Emulator_GetConfigDescriptor( uint16_t Length )
{
  return Standard_GetDescriptorData( Length, &Config_Descriptor );
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_GetStringDescriptor
 * Description    : Gets the string descriptors according to the needed index
 * Input          : Length.
 * Output         : None.
 * Return         : The address of the string descriptors.
 *******************************************************************************/
uint8_t *FTDI_Emulator_GetStringDescriptor( uint16_t Length )
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if ( wValue0 > 3 )
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData( Length, &String_Descriptor[ wValue0 ] );
  }
}

/*******************************************************************************
 * Function Name  : FTDI_Emulator_Get_Interface_Setting.
 * Description    : test the interface and the alternate setting according to the
 *                  supported one.
 * Input1         : uint8_t: Interface : interface number.
 * Input2         : uint8_t: AlternateSetting : Alternate Setting number.
 * Output         : None.
 * Return         : The address of the string descriptors.
 *******************************************************************************/
RESULT FTDI_Emulator_Get_Interface_Setting( uint8_t Interface, uint8_t AlternateSetting )
{
  if ( AlternateSetting > 0 )
  {
    return USB_UNSUPPORT;
  }
  else if ( Interface >= Device.InterfaceNum )
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

static uint8_t * FTDI_Emulator_CopyRoutineBuffer;
static uint16_t FTDI_Emulator_CopyRoutineSize;

// -------------------------------------------------------------------------------------------------
// 40 00 00 00  00 00 00 00 : Reset UART
// 40 00 01 00  00 00 00 00 : Purge Rx Buffer Only
// 40 00 02 00  00 00 00 00 : Purge Tx Buffer Only
// Reset UART:
//  Sets flow control set to 'none'
//  Sets Event char = $0D
//  Sets Event trigger = disabled
//  Clear DTR
//  Clear RTS
//  Purge TX buffer
//  Purge RX buffer
//
//  baud and data format *NOT* reset
//
void FTDI_Emulator_PurgeTxBuffer( FTDI_InterfaceContext * Interface )
{
  FTDI_Emulator_BSP_CancelTx( Interface );
}

void FTDI_Emulator_PurgeRxBuffer( FTDI_InterfaceContext * Interface )
{
  FTDI_Emulator_BSP_CancelRx( Interface );
}

RESULT FTDI_Emulator_ResetRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.w ];

  if ( FTDI_RESET_SIO == Dev->USBwValues.bw.bb0 )
    FTDI_Emulator_BSP_ResetRequest( Interface );

  if ( ( FTDI_RESET_SIO == Dev->USBwValues.bw.bb0 ) || ( FTDI_RESET_RX == Dev->USBwValues.bw.bb0 ) )
    FTDI_Emulator_PurgeRxBuffer( Interface );

  if ( ( FTDI_RESET_SIO == Dev->USBwValues.bw.bb0 ) || ( FTDI_RESET_TX == Dev->USBwValues.bw.bb0 ) )
  {
    FTDI_Emulator_PurgeTxBuffer( Interface );
    SetEPRxStatus( Interface->OutEndpoint & 0x7F, EP_RX_VALID );
  }

  return USB_SUCCESS;
}

/*
 * ReqType -- ReqNum -- USBwValues.bw.bb0 -- USBwValues.bw.bb1
 * USBwIndexs.bw.bb0 -- USBwIndexs.bw.bb1 -- USBwLengths.bw.bb0 -- USBwLengths.bw.bb1
 *
 * bb0 is low byte, bb1 is high byte
 *
 * 40 01 00 01  00 00 00 00 : Clear DTR
 * 40 01 00 02  00 00 00 00 : Clear RTS
 *
 * If the device is in RTS/CTS flow control, the RTS set by this command will
 * be IGNORED without an error being returned.
 *
 * You can *NOT* set DTR and RTS with one control message
 *
 * wValue
 * B0    DTR state
 *          0 = reset
 *          1 = set
 * B1    RTS state
 *          0 = reset
 *          1 = set
 * B2..7 Reserved
 *
 * B8    DTR state enable
 *          0 = ignore
 *          1 = use DTR state
 * B9    RTS state enable
 *          0 = ignore
 *          1 = use RTS state
 * B10..15 Reserved
 */
RESULT FTDI_Emulator_SetModemCtrlRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.w ];
  if ( Interface )
  {
    if ( FTDI_SET_FLOW_CTRL_RTSCTS == Interface->FlowControl )
    {
      if ( Dev->USBwValues.bw.bb0 & ( 1 << FTDI_SET_MODEM_CTRL_RTS ) )
        return USB_SUCCESS;

      Interface->ModemControl = Dev->USBwValues.w;
      FTDI_Emulator_BSP_SetModemCtrlRequest( Interface );
    }
  }

  return USB_SUCCESS;
}

/*
 * BmRequestType:  0100 0000b -----------------> 0x40
 * bRequest:       FTDI_SIO_SET_FLOW_CTRL -----> 0x02
 * wValue:         Xoff/Xon
 * wIndex:         Protocol/Port - wIndexH is protocol / wIndexL is port
 * wLength:        0
 * Data:           None
 *
 * wIndexH protocol is:
 *   B0 Output handshaking using RTS/CTS
 *       0 = disabled
 *       1 = enabled
 *   B1 Output handshaking using DTR/DSR
 *       0 = disabled
 *       1 = enabled
 *   B2 Xon/Xoff handshaking
 *       0 = disabled
 *       1 = enabled
 *
 * A value of zero in the wIndexH field disables handshaking
 *
 * If Xon/Xoff handshaking is specified,
 * the hValue field should contain the XOFF character
 * and the lValue field contains the XON character.
 */
RESULT FTDI_Emulator_SetFlowCtrlRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    Interface->FlowControl = Dev->USBwIndexs.bw.bb1;
    Interface->FlowControlXON = Dev->USBwValues.bw.bb0;
    Interface->FlowControlXOFF = Dev->USBwValues.bw.bb1;
    FTDI_Emulator_BSP_SetFlowCtrlRequest( Interface );
  }

  return USB_SUCCESS;
}

/*
 * FT8U232AM
 *
 *  Clock = 3MHz
 *  {              wIndex[7:0] } : Port
 *  {             wValue[13:0] } : BaudRateDivisorMantissa
 *  {            wValue[15:14] } : BaudRateDivisorFraction
 *                               : 0.000, 0.500, 0.250, 0.125
 *                               : 00     01     10     11
 *
 * FT232BM and FT232R : 3000000 / 2.875 = 1043478 : 40 03 : 02 C0 : 01 00 : 00 00
 *
 *  Clock = 3MHz               0 : Port
 *  {             wValue[13:0] } : BaudRateDivisorMantissa
 *  { wIndex[0], wValue[15:14] } : BaudRateDivisorFraction
 *                               : 0.000, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875
 *                               : 000    001    010    011    100    101    110    111
 * FT2232C, FT2232D and FT2232L
 *
 *  Clock = 3MHz
 *  {              wIndex[7:0] } : Port
 *  {             wValue[13:0] } : BaudRateDivisorMantissa
 *  { wIndex[8], wValue[15:14] } : BaudRateDivisorFraction
 *                               : 0.000, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875
 *                               : 000    001    010    011    100    101    110    111
 * clk/1   <-> 0 <-> 3.0 MBaud
 * clk/1.5 <-> 1 <-> 2.0 MBaud
 * clk/2   <-> 2 <-> 1.5 MBaud
 *
 * FT2232H, FT4232H and FT232H
 *
 *  {              wIndex[7:0] } : Port
 *  {                wIndex[9] } : 0 - 3MHz Clock, 1 = 12MHz Clock
 *  {             wValue[13:0] } : BaudRateDivisorMantissa
 *  { wIndex[8], wValue[15:14] } : BaudRateDivisorFraction
 *                               : 0.000, 0.125, 0.250, 0.375, 0.500, 0.625, 0.750, 0.875
 *                               : 000    001    010    011    100    101    110    111
 *
 * The FT2232H does not support the baud rates of 7 Mbaud 9 Mbaud 10 Mbaud and 11 Mbaud.
 * Support the baud rates of 3 Mbaud 4 Mbaud 6 Mbaud 8 Mbaud 12 Mbaud
 *
 * 000 - add .000 to divisor
 * 001 - add .500 to divisor
 * 010 - add .250 to divisor
 * 011 - add .125 to divisor
 * 100 - add .375 to divisor
 * 101 - add .625 to divisor
 * 110 - add .750 to divisor
 * 111 - add .875 to divisor
 *
 */
RESULT FTDI_Emulator_SetBaudrateRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface =             //
    Device.Interfaces[ Device.InterfaceNum > 1 ? Dev->USBwIndexs.bw.bb0 : 0 ];

  float DivisorFraction, Clock;
  uint32_t DivisorMantissa, FractionIndex;

  const float table[ 8 ] = {
    0.0, 0.5, 0.25, 0.125, 0.375, 0.625, 0.75, 0.875 };

  if ( Interface )
  {
    Clock = 3000000;
    DivisorMantissa = Dev->USBwValues.w & 0x3FFF;
    FractionIndex = Dev->USBwValues.w >> 14;

    if ( ( FTDI_DEVICE_2232H <= Device.DeviceType ) && ( Device.DeviceType <= FTDI_DEVICE_232H ) )
    {
      if ( Dev->USBwIndexs.bw.bb1 & ( 1 << 1 ) )             // wIndex[9]
        Clock = 12000000;
      if ( Dev->USBwIndexs.bw.bb1 & ( 1 << 0 ) )             // wIndex[8]
        FractionIndex |= ( 1 << 2 );
    }
    // FTDI_DEVICE_BM, FTDI_DEVICE_232R, FTDI_DEVICE_203X, FTDI_DEVICE_2232C
    else if ( FTDI_DEVICE_AM != Device.DeviceType )
    {
      if ( Device.InterfaceNum > 1 )    // FTDI_DEVICE_2232C
      {
        if ( Dev->USBwIndexs.bw.bb1 & ( 1 << 0 ) )             // wIndex[8]
          FractionIndex |= ( 1 << 2 );
      }
      // FTDI_DEVICE_BM, FTDI_DEVICE_232R, FTDI_DEVICE_203X
      else
      {
        if ( Dev->USBwIndexs.bw.bb0 & ( 1 << 0 ) )             // wIndex[0]
          FractionIndex |= ( 1 << 2 );
      }
    }

    DivisorFraction = table[ FractionIndex ];
    Interface->BaudRate = (uint32_t) ( Clock / ( DivisorMantissa + DivisorFraction ) );
    FTDI_Emulator_BSP_SetBaudrateRequest( Interface );
  }

  return USB_SUCCESS;
}

/*
 * BmRequestType:  0100 0000B ----------> 0x40
 * bRequest:       FTDI_SIO_SET_DATA ---> 0x04
 * wValue:         Data characteristics (see below)
 * wIndex:         Port
 * wLength:        0
 * Data:           No
 *
 * Data characteristics
 *
 *   B0..7   Number of data bits < 7 or 8 >
 *   B8..10  Parity
 *           0 = None
 *           1 = Odd
 *           2 = Even
 *           3 = Mark
 *           4 = Space
 *   B11..13 Stop Bits
 *           0 = 1
 *           1 = 1.5
 *           2 = 2
 *   B14
 *           1 = TX ON (break)
 *           0 = TX OFF (normal state)
 *   B15 Reserved
 *
 */
RESULT FTDI_Emulator_SetLineCodingRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    Interface->LineCoding.Coding = Dev->USBwValues.w;
    FTDI_Emulator_BSP_SetLineCodingRequest( Interface );
  }

  return USB_SUCCESS;
}

/*
 * Get the current values of the modem and line status registers.
 *
 * C0 05 00 00 00 00 01 00 -- ModemSatus : 01
 * C0 05 00 00 00 00 02 00 -- ModemSatus and LineStatus : 01 60
 *
 * Byte 0: Modem Status
 *
 * Offset Description
 * B0 Reserved - must be 1 ------------------------ 1
 * B1 Reserved - must be 0 ------------------------ 0
 * B2 Reserved - must be 0 ------------------------ 0
 * B3 Reserved - must be 0 ------------------------ 0
 * B4 Clear to Send (CTS) ------------------------- 0
 * B5 Data Set Ready (DSR) ------------------------ 0
 * B6 Ring Indicator (RI) ------------------------- 0
 * B7 Receive Line Signal Detect (RLSD) ----------- 0
 *
 * Byte 1: Line Status
 *
 * Offset Description
 * B0 Data Ready (Receive RDR) -------------------- 0
 * B1 Overrun Error (Receive OE) ------------------ 0
 * B2 Parity Error (Receive PE) ------------------- 0
 * B3 Framing Error (Receive FE) ------------------ 0
 * B4 Break Interrupt (Receive Break BI) ---------- 0
 * B5 Transmitter Holding Register (THRE) --------- 1
 * B6 Transmitter Empty (TEMT) -------------------- 1
 * B7 Error in RCVR FIFO -------------------------- 0
 *
 * 01 60 --> 0x6001 --> 0110 0000 0000 0001
 */
RESULT FTDI_Emulator_GetModemStatusRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    FTDI_Emulator_BSP_GetModemLineStatusRequest( Interface );
    FTDI_Emulator_CopyRoutineSize = ( Dev->USBwLengths.w > 2 ) ? 2 : Dev->USBwLengths.w;
    FTDI_Emulator_CopyRoutineBuffer = &Interface->ModemStatus;
  }

  return USB_SUCCESS;
}

/*
 * Set the special event character for the specified communications port.
 * If the device sees this character it will immediately return the data read so far -
 * rather than wait 16ms or until 62 bytes are read which is what normally happens.
 *
 *  BmRequestType:   0100 0000b ---------------> 0x40
 *  bRequest:        FTDI_SIO_SET_EVENT_CHAR --> 0x06
 *  wValue:          EventChar
 *  wIndex:          Port
 *  wLength:         0
 *  Data:            None
 *
 * wValue:
 *   B0..7   Event Character
 *   B8      Event Character Processing
 *             0 = disabled
 *             1 = enabled
 *   B9..15  Reserved
 *
 * MS LS XX YY ZZ EVT ---> write FIFO to send
 */
RESULT FTDI_Emulator_SetEventCharRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    Interface->EventChar = Dev->USBwValues.bw.bb0;
    Interface->EventCharEnabled = Dev->USBwValues.bw.bb1 & 1;

  }
  return USB_SUCCESS;
}

/*
 * Set the parity error replacement character for the specified communications
 *
 *  BmRequestType:  0100 0000b ----------------> 0x40
 *  bRequest:       FTDI_SIO_SET_EVENT_CHAR ---> 0x07
 *  wValue:         Error Char
 *  wIndex:         Port
 *  wLength:        0
 *  Data:           None
 *
 *Error Char
 *  B0..7  Error Character
 *  B8     Error Character Processing
 *           0 = disabled
 *           1 = enabled
 *  B9..15 Reserved
 *
 */
RESULT FTDI_Emulator_SetErrorCharRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    Interface->ErrorChar = Dev->USBwValues.bw.bb0;
    Interface->ErrorCharEnabled = Dev->USBwValues.bw.bb1 & 1;

  }
  return USB_SUCCESS;
}

/*
 * Set the timeout interval.
 * The FTDI collects data from the slave device, transmitting it to the host when either
 *
 * A) 62 bytes are received, or
 * B) the timeout interval has elapsed and the buffer contains at least 1 byte.
 *
 * Setting this value to a small number can dramatically improve performance
 * for applications which send small packets, since the default value is 16ms.
 *
 *  BmRequestType:   0100 0000b -------------------> 0x40
 *  bRequest:        FTDI_SIO_SET_LATENCY_TIMER ---> 0x09
 *  wValue:          Latency (milliseconds)
 *  wIndex:          Port
 *  wLength:         0
 *  Data:            None
 *
 * wValue:
 *   B0..7   Latency timer
 *   B8..15  0
 *
 */
RESULT FTDI_Emulator_SetLatencyTimerRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    Interface->LatencyTimerValue = Dev->USBwValues.bw.bb0;
  }
  return USB_SUCCESS;
}

RESULT FTDI_Emulator_GetLatencyTimerRequest( void )
{
  DEVICE_INFO * Dev = &Device_Info;
  FTDI_InterfaceContext * Interface = Device.Interfaces[ Dev->USBwIndexs.bw.bb0 ];
  if ( Interface )
  {
    FTDI_Emulator_CopyRoutineSize = 1;
    FTDI_Emulator_CopyRoutineBuffer = &Interface->LatencyTimerValue;
  }

  return USB_SUCCESS;
}

RESULT FTDI_Emulator_SetBitModeRequest( void )
{
  return USB_SUCCESS;
}

RESULT FTDI_Emulator_ReadPinsRequest( void )
{
  return USB_SUCCESS;
}

RESULT FTDI_Emulator_EraseEepromRequest( void )
{
  return USB_SUCCESS;
}

// c0 90 00 00  5f 00 02 00
RESULT FTDI_Emulator_ReadEepormRequest( void )
{
  uint16_t Addr = Device_Info.USBwIndexs.w;

  FTDI_Emulator_CopyRoutineSize = 2;
  FTDI_Emulator_CopyRoutineBuffer = &EEPROM_Cache[ Addr << 1 ];

  if ( Addr >= ( 0xA0 >> 1 ) )
    FTDI_Emulator_CopyRoutineBuffer = (uint8_t*) &FTDI_Emulator_DummyData;

  else if ( Addr >= ( 0x80 >> 1 ) )
  {
    Addr -= ( 0x80 >> 1 );
    FTDI_Emulator_CopyRoutineBuffer = (uint8_t *) &FTDI_Emulator_ChipId[ Addr << 1 ];
  }

  return USB_SUCCESS;
}

RESULT FTDI_Emulator_WriteEepromRequest( void )
{
  uint16_t Addr = Device_Info.USBwIndexs.w;
  uint16_t Data = Device_Info.USBwValues.w;

  if ( EEPROM_SUCCESS == EEPROM_WriteWord( Addr << 1, Data ) )
    return USB_SUCCESS;

  return USB_ERROR;
}

uint8_t * FTDI_Emulator_CopyRoutine( uint16_t Length )
{
  if ( Length == 0 )
  {
    pInformation->Ctrl_Info.Usb_wLength = FTDI_Emulator_CopyRoutineSize;
    return NULL;
  }
  else
  {
    return FTDI_Emulator_CopyRoutineBuffer;
  }
}

RESULT FTDI_Emulator_ControlHandler( uint8_t RequestNo )
{
  RESULT RetValue = USB_UNSUPPORT;
  uint8_t *(*CopyRoutine)( uint16_t ) = FTDI_Emulator_CopyRoutine;

  FTDI_Emulator_CopyRoutineBuffer = 0;
  FTDI_Emulator_CopyRoutineSize = 0;

  DEVICE_INFO * pInfo = &Device_Info;      // SetupPacket
  ENDPOINT_INFO * pCtrlInfo = &pInfo->Ctrl_Info;

  if ( FTDI_LAST_REQUEST == RequestNo )     // StatusIn() or StatusOut()
  {

    RetValue = USB_SUCCESS;
  }
  else    // DataSetup() or NoDataSetup()
  {
    switch ( RequestNo )
    {
      case FTDI_RESET_REQUEST:
        RetValue = FTDI_Emulator_ResetRequest( );
        break;
      case FTDI_SET_MODEM_CTRL_REQUEST:
        RetValue = FTDI_Emulator_SetModemCtrlRequest( );
        break;
      case FTDI_SET_FLOW_CTRL_REQUEST:
        RetValue = FTDI_Emulator_SetFlowCtrlRequest( );
        break;
      case FTDI_SET_BAUDRATE_REQUEST:
        RetValue = FTDI_Emulator_SetBaudrateRequest( );
        break;
      case FTDI_SET_LINE_CODING_REQUEST:
        RetValue = FTDI_Emulator_SetLineCodingRequest( );
        break;
      case FTDI_GET_MODEM_STATUS_REQUEST:
        RetValue = FTDI_Emulator_GetModemStatusRequest( );
        break;
      case FTDI_SET_EVENT_CHAR_REQUEST:
        RetValue = FTDI_Emulator_SetEventCharRequest( );
        break;
      case FTDI_SET_ERROR_CHAR_REQUEST:
        RetValue = FTDI_Emulator_SetErrorCharRequest( );
        break;
      case FTDI_SET_LATENCY_TIMER_REQUEST:
        RetValue = FTDI_Emulator_SetLatencyTimerRequest( );
        break;
      case FTDI_GET_LATENCY_TIMER_REQUEST:
        RetValue = FTDI_Emulator_GetLatencyTimerRequest( );
        break;
      case FTDI_SET_BITMODE_REQUEST:
        RetValue = FTDI_Emulator_SetBitModeRequest( );
        break;
      case FTDI_READ_PINS_REQUEST:
        RetValue = FTDI_Emulator_ReadPinsRequest( );
        break;
      case FTDI_READ_EEPROM_REQUEST:
        RetValue = FTDI_Emulator_ReadEepormRequest( );
        break;
      case FTDI_WRITE_EEPROM_REQUEST:
        RetValue = FTDI_Emulator_WriteEepromRequest( );
        break;
      case FTDI_ERASE_EEPROM_REQUEST:
        RetValue = FTDI_Emulator_EraseEepromRequest( );
        break;
      default:
        RetValue = USB_UNSUPPORT;
        break;
    }
  }

  if ( ( USB_SUCCESS == RetValue ) && ( FTDI_Emulator_CopyRoutineSize ) )
  {
    // Call again by ISR to Read/Write More Data
    pCtrlInfo->CopyData = CopyRoutine;
    pCtrlInfo->Usb_wOffset = 0;
    pCtrlInfo->Usb_wLength = FTDI_Emulator_CopyRoutineSize;
    ( *CopyRoutine )( 0 );
  }

  return RetValue;
}

/* -------------------------------------------------------------------------------------------------
 * Handler USB ----> TxBuffer ---> UART
 * Handler UART ---> RxBuffer ---> USB
 */
void SOF_Callback( void )
{
  FTDI_InterfaceContext * Interface;
  if ( 0 )
  {
    if ( bDeviceState == CONFIGURED )
    {
      for ( uint32_t i = 0; i < Device.InterfaceNum; i++ )
      {
        Interface = Device.Interfaces[ i ];
        if ( 0 == Interface )
          continue;

        if ( Interface->LatencyTimerCount < Interface->LatencyTimerValue )
          Interface->LatencyTimerCount++;

        if ( Interface->LatencyTimerCount == Interface->LatencyTimerValue )
          Interface->LatencyTimerCount = 0;
      }
    }
  }
}

uint32_t FTDI_Emulator_OnTxCompleted( FTDI_InterfaceContext * Interface )
{
  uint32_t RetValue;
  // The endpoint is naked and all transmission requests result in a NAK handshake.
  // until set RxStatus = EP_RX_VALID, this endpoint is enabled for transmission
  if ( 0 == Interface->Remaining )
  {
    GPIOC->BRR = ( 1 << 0 );
    SetEPRxStatus( ENDP2, EP_RX_VALID );
    RetValue = 0;
  }
  else
  {
    uint32_t Transferred = FTDI_Emulator_BSP_Transmit( Interface, Interface->TxBuffer, Interface->Remaining );
    Interface->TxOffset += Transferred;
    Interface->Remaining -= Transferred;
    if ( Interface->Remaining )
      RetValue = 1;
    else
    {
      GPIOC->BRR = ( 1 << 0 );
      SetEPRxStatus( ENDP2, EP_RX_VALID );
      RetValue = 0;
    }
  }

  return RetValue;
}

__root uint32_t out_cnt = 0;
void EP2_OUT_Callback( void )
{
  GPIOC->BSRR = ( 1 << 0 );
  out_cnt++;

  // Come here, EP2R.STAT_RX = 0x02 : NAK
  // NAK: the endpoint is naked and all transmission requests result in a NAK handshake.
  FTDI_InterfaceContext * Interface = Device.Interfaces[ 0 ];
  uint16_t Data_Len = USB_SIL_Read( EP2_OUT, Interface->TxBuffer );
  Interface->TxOffset = 0;
  Interface->Remaining = Data_Len;
  FTDI_Emulator_OnTxCompleted( Interface );
}

// Receive 1..N bytes before Timeout < LatencyTimerValue >
void FTDI_Emulator_OnRxCompleted( FTDI_InterfaceContext * Interface )
{
  Interface->RxCompleted = 1;
}

uint32_t state = 0;

void EP1_IN_Callback( void )
{
  uint32_t ReceivedCount;
  uint8_t Buffer[ 0x40 ];

  FTDI_InterfaceContext * Interface = Device.Interfaces[ 0 ];
  FTDI_Emulator_BSP_GetModemLineStatusRequest( Interface );
  Interface->ModemStatus = 0x01;
  if ( FTDI_Emulator_BSP_ReceiveOverrun( Interface ) )
    Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_RXFIFO;

  // Write Modem and Line Status
  *(uint16_t*) &Buffer[ 0 ] = *(uint16_t *) &( Interface->ModemStatus );

  // Write Received Data in RxBuffer
  ReceivedCount = 0;

  // FTDI_Emulator_RxCompleted() be called by BSP
  // 1. Timeout after received first byte
  // 2. RxFifo is full, No space to receive more
  ReceivedCount = FTDI_Emulator_BSP_Receive( Interface, &Buffer[ 2 ], 0x40 - 2 );
  if ( Interface->RxCompleted )
  {
    Interface->RxCompleted = 0;
  }

  if ( ReceivedCount )
  {
    if ( state & 1 )
      GPIOC->BSRR = ( 1 << 1 );
    else
      GPIOC->BRR = ( 1 << 1 );

    state++;
  }

  // Never wait, always send Modem and Line Status, received data < maybe 0 byte>
  USB_SIL_Write( EP1_IN, Buffer, 2 + ReceivedCount );
  SetEPTxStatus( ENDP1, EP_TX_VALID );
}
