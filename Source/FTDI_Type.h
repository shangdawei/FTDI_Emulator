#ifndef __FTDI_TYPE_H
#define __FTDI_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "fifo.h"

typedef enum
{
  FTDI_COM1 = 0,
  FTDI_COM2 = 1,
  FTDI_COM3 = 2,
  FTDI_COM4 = 3
} FTDI_COM_TypeDef;

typedef struct
{
  union
  {
    uint16_t Coding;
    struct
    {
      uint16_t DataBits :8;
      uint16_t StopBits :3;
      uint16_t Parity :3;
      uint16_t TxBreak :1;
      uint16_t Reserved :1;
    };
  };
} FTDI_COM_LineCoding;

#define FTDI_EEPROM_93C46             ( 0x46 )
#define FTDI_EEPROM_93C56             ( 0x56 )
#define FTDI_EEPROM_93C66             ( 0x66 )
#define FTDI_EEPROM_UNKNOWN           ( 0xFF )

#define FTDI_INTERFACE_A                ( 0 )
#define FTDI_INTERFACE_B                ( 1 )
#define FTDI_INTERFACE_C                ( 2 )
#define FTDI_INTERFACE_D                ( 3 )

#define FTDI_INVERT_TXD                 ( 0x01 )
#define FTDI_INVERT_RXD                 ( 0x02 )
#define FTDI_INVERT_RTS                 ( 0x04 )
#define FTDI_INVERT_CTS                 ( 0x08 )
#define FTDI_INVERT_DTR                 ( 0x10 )
#define FTDI_INVERT_DSR                 ( 0x20 )
#define FTDI_INVERT_DCD                 ( 0x40 )
#define FTDI_INVERT_RI                  ( 0x80 )

// Function, VCP Driver, High Current Drive

// FTDI_DEVICE_232R only
#define FTDI_IF_FUNCTION_MASK07         ( 0x07 )

// FTDI_DEVICE_232H only
#define FTDI_IF_FUNCTION_MASK0F         ( 0x0F )

// FTDI_DEVICE_230X only
#define FTDI_IF_FUNCTION_MASKFF         ( 0xFF )

#define FTDI_IF_FUNCTION_232_UART       ( 0x00 )
// Except FTDI_DEVICE_203X (UART only)
#define FTDI_IF_FUNCTION_245_FIFO       ( 0x01 )
#define FTDI_IF_FUNCTION_OPTO           ( 0x02 )
#define FTDI_IF_FUNCTION_CPU_FIFO       ( 0x04 )

// FTDI_DEVICE_232H only
#define FTDI_IF_FUNCTION_FT1284         ( 0x08 )

#define FTDI_IF_FUNCTION_RS485          ( 0x10 )

#define FTDI_232R_USES_EXT_OSC          ( 0x02 )
#define FTDI_232R_HIGH_CURRENT_DRIVE    ( 0x04 )

#define FTDI_DRIVER_VCP                 ( 0x08 )
#define FTDI_HIGH_CURRENT_DRIVE         ( 0x10 )

#define FTDI_232H_DRIVER_VCP            ( 0x10 )

// FTDI_DEVICE_2232H only
#define FTDI_SUSPEND_ON_DBUS7           ( 0x80 )

// FTDI_DEVICE_232H only
#define FTDI_SUSPEND_ON_ABUS7           ( 0x80 )

// FTDI_DEVICE_232H only
#define FTDI_FT1284_CPOL                ( 0x01 )
#define FTDI_FT1284_LSB                 ( 0x02 )
#define FTDI_FT1284_FC                  ( 0x04 )

/* Requests */
#define FTDI_RESET_REQUEST              0x00    /* Reset the port */
#define FTDI_SET_MODEM_CTRL_REQUEST     0x01    /* Set the modem control register */
#define FTDI_SET_FLOW_CTRL_REQUEST      0x02    /* Set flow control register */
#define FTDI_SET_BAUDRATE_REQUEST       0x03    /* Set baud rate */
#define FTDI_SET_LINE_CODING_REQUEST    0x04    /* Set the data characteristics of the port */
#define FTDI_GET_MODEM_STATUS_REQUEST   0x05
#define FTDI_SET_EVENT_CHAR_REQUEST     0x06
#define FTDI_SET_ERROR_CHAR_REQUEST     0x07
#define FTDI_SET_LATENCY_TIMER_REQUEST  0x09
#define FTDI_GET_LATENCY_TIMER_REQUEST  0x0A

#define FTDI_SET_BITMODE_REQUEST        0x0B
#define FTDI_READ_PINS_REQUEST          0x0C
#define FTDI_READ_EEPROM_REQUEST        0x90
#define FTDI_WRITE_EEPROM_REQUEST       0x91
#define FTDI_ERASE_EEPROM_REQUEST       0x92

#define FTDI_LAST_REQUEST               ( 0xFF )

#define FTDI_RESET_SIO                  0x00    /* Reset the device */
#define FTDI_RESET_RX                   0x01    /* Reset Rx Buffer */
#define FTDI_RESET_TX                   0x02    /* Reset Tx Buffer */

#define FTDI_SET_MODEM_CTRL_DTR         0x01    /* Mask <wValueH> and State <wValueL> */
#define FTDI_SET_MODEM_CTRL_RTS         0x02    /* Mask <wValueH> and State <wValueL> */

#define FTDI_SET_FLOW_CTRL_NONE         0x00    /* Set flow control NONE */
#define FTDI_SET_FLOW_CTRL_RTSCTS       0x01    /* Set flow control RTS/CTS  <wIndexH> */
#define FTDI_SET_FLOW_CTRL_DTRDSR       0x02    /* Set flow control DTR/DSR  <wIndexH> */
#define FTDI_SET_FLOW_CTRL_XONXOFF      0x04    /* Set flow control XON/XOFF <wIndexH> :: XON/XOFF : wValueH/wValueL */

#define FTDI_SET_BAUDRATE_DIVISOR       0x3FFF  /* Set baud rate : Divisor[13..0] <wValue[13..0]> */
#define FTDI_SET_BAUDRATE_FRACTIONAL_L  0xC000  /* Set baud rate : Fractional[1..0] <wValue[15..14]> */
#define FTDI_SET_BAUDRATE_FRACTIONAL_H  0x0100  /* Set baud rate : Fractional[2] <wIndex[8]> */
#define FTDI_SET_BAUDRATE_CLOCK_12MHZ   0x0200  /* Set baud rate : Clock 12MHz <wIndex[9]> */

#define FTDI_SET_DATA_REQUEST_DATA_BITS 0x00FF  /* Set the data characteristics Data Bits : <wValue[7..0]> */
#define FTDI_SET_DATA_REQUEST_PARITY    0x0700  /* Set the data characteristics Parity Bits : <wValue[10..8]> */
#define FTDI_SET_DATA_REQUEST_STOP_BITS 0x3800  /* Set the data characteristics Stop Bits : <wValue[13..11]> */
#define FTDI_SET_DATA_REQUEST_BREAK     0x4000  /* Set the data characteristics Break : <wValue[14]> */

#define FTDI_SET_DATA_DATA_BITS_7       0x0007
#define FTDI_SET_DATA_DATA_BITS_8       0x0008

#define FTDI_SET_DATA_PARITY_NONE       0x0000
#define FTDI_SET_DATA_PARITY_ODD        0x0100
#define FTDI_SET_DATA_PARITY_EVEN       0x0200
#define FTDI_SET_DATA_PARITY_MARK       0x0300
#define FTDI_SET_DATA_PARITY_SPACE      0x0400

#define FTDI_SET_DATA_STOP_BITS_1       0x0000
#define FTDI_SET_DATA_STOP_BITS_1_5     0x0800
#define FTDI_SET_DATA_STOP_BITS_2       0x1800

#define FTDI_SET_DATA_BREAK_OFF         0x0000  /* 0 = Inactive, 1 = Active */
#define FTDI_SET_DATA_BREAK_ON          0x4000  /* 0 = Inactive, 1 = Active */

#define FTDI_GET_MODEM_STATUS_CTS       0x10    /* 0 = Inactive, 1 = Active */
#define FTDI_GET_MODEM_STATUS_DSR       0x20    /* 0 = Inactive, 1 = Active */
#define FTDI_GET_MODEM_STATUS_RI        0x40    /* 0 = Inactive, 1 = Active */
#define FTDI_GET_MODEM_STATUS_RLSD      0x80    /* 0 = Inactive, 1 = Active */

// Set the special event character for the specified communications port
// If the device sees this character it will immediately return the data read so far
// rather than wait 40ms or until 62 bytes are read which is what normally happens.
#define FTDI_SET_EVENT_CHAR_VALUE       0x00FF  /* wValue<7..0> */
#define FTDI_SET_EVENT_CHAR_MASK        0x0100  /* wValue<8> 0 = Disabled, 1 = Enabled */

// Set the parity error replacement character for the specified communications
#define FTDI_SET_ERROR_CHAR_VALUE       0x00FF  /* wValue<7..0> */
#define FTDI_SET_ERROR_CHAR_MASK        0x0100  /* wValue<8> 0 = Disabled, 1 = Enabled */

/* switch off bitbang mode, back to regular serial/FIFO */
#define FTDI_SET_BITMODE_RESET          0x00

/* classical asynchronous bitbang mode, introduced with B-type chips */
#define FTDI_SET_BITMODE_BITBANG        0x01

/* MPSSE mode, available on 2232x chips */
#define FTDI_SET_BITMODE_MPSSE          0x02

/* synchronous bitbang mode, available on 2232x and R-type chips  */
#define FTDI_SET_BITMODE_SYNCBB         0x04

/* MCU Host Bus Emulation mode, available on 2232x chips, CPU-style fifo mode gets set via EEPROM */
#define FTDI_SET_BITMODE_MCU            0x08

/* Fast Opto-Isolated Serial Interface Mode, available on 2232x chips  */
#define FTDI_SET_BITMODE_OPTO           0x10

/* Bitbang on CBUS pins of R-type chips, configure in EEPROM before */
#define FTDI_SET_BITMODE_CBUS           0x20

/* Single Channel Synchronous FIFO mode, available on 2232H chips */
#define FTDI_SET_BITMODE_SYNCFF         0x40

/* FT1284 mode, available on 232H chips */
#define FTDI_SET_BITMODE_FT1284         0x80

#define FTDI_IN_ENDPOINT_MODEM_STATUS_CTS       0x10    /* 0 = Inactive, 1 = Active */
#define FTDI_IN_ENDPOINT_MODEM_STATUS_DSR       0x20    /* 0 = Inactive, 1 = Active */
#define FTDI_IN_ENDPOINT_MODEM_STATUS_RI        0x40    /* 0 = Inactive, 1 = Active */
#define FTDI_IN_ENDPOINT_MODEM_STATUS_RLSD      0x80    /* 0 = Inactive, 1 = Active */

#define FTDI_IN_ENDPOINT_LINE_STATUS_DR         0x01    /* Data Ready (Receive RDR) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_OE         0x02    /* Overrun Error (Receive OE) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_PE         0x04    /* Parity Error (Receive PE) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_FE         0x08    /* Framing Error (Receive FE) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_BI         0x10    /* Break Interrupt (Receive Break BI) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_THRE       0x20    /* Transmitter Holding Register (THRE) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_TEMT       0x40    /* Transmitter Empty (TEMT) */
#define FTDI_IN_ENDPOINT_LINE_STATUS_RXFIFO     0x80    /* Error in RCVR FIFO */

// Cx > FTDI_CBUS_BB_RD : in Synchronous Bit Bang mode
// Running MPROG show that C0..C3 have fixed function in Synchronous Bit Bang mode
// C0 - RXF
// C1 = TXE
// C2 = RD
// C3 = WR
//
// C4 = Dont care whether it is in Synchronous Bit Bang mode
typedef enum
{
  FTDI_CBUS_TXDEN = 0x00,               // Enable transmit data for RS485
  FTDI_CBUS_PWREN = 0x01,               // Output high during USB suspending mode.
  FTDI_CBUS_RXLED = 0x02,               // Receive data LED drive
  FTDI_CBUS_TXLED = 0x03,               // Transmit data LED drive
  FTDI_CBUS_TXRXLED = 0x04,             // LED drive ¨C pulses low when transmitting or receiving data
  FTDI_CBUS_SLEEP = 0x05,               // Goes low during USB suspend mode.
  FTDI_CBUS_CLK48 = 0x06,               // Clock output
  FTDI_CBUS_CLK24 = 0x07,               // Clock output
  FTDI_CBUS_CLK12 = 0x08,               // Clock output
  FTDI_CBUS_CLK6 = 0x09,                // Clock output
  FTDI_CBUS_IOMODE = 0x0A,              // CBUS bit bang mode option
  FTDI_CBUS_BB_WR = 0x0B,               // Synchronous and asynchronous bit bang mode WR# strobe output.
  FTDI_CBUS_BB_RD = 0x0C,               // Synchronous and asynchronous bit bang mode RD# strobe output.
} FTDI_CBUS_Function;

typedef enum
{
  FT232H_CBUS_TRISTATE = 0,
  FT232H_CBUS_TXLED = 1,
  FT232H_CBUS_RXLED = 2,
  FT232H_CBUS_TXRXLED = 3,
  FT232H_CBUS_PWREN = 4,
  FT232H_CBUS_SLEEP = 5,
  FT232H_CBUS_DRIVE_0 = 6,
  FT232H_CBUS_DRIVE1 = 7,
  FT232H_CBUS_IOMODE = 8,
  FT232H_CBUS_TXDEN = 9,
  FT232H_CBUS_CLK30 = 10,
  FT232H_CBUS_CLK15 = 11,
  FT232H_CBUS_CLK7_5 = 12
} FT232H_CBUS_Function;

typedef enum
{
  FT230X_CBUS_TRISTATE = 0,
  FT230X_CBUS_TXLED = 1,
  FT230X_CBUS_RXLED = 2,
  FT230X_CBUS_TXRXLED = 3,
  FT230X_CBUS_PWREN = 4,
  FT230X_CBUS_SLEEP = 5,
  FT230X_CBUS_DRIVE_0 = 6,
  FT230X_CBUS_DRIVE1 = 7,
  FT230X_CBUS_IOMODE = 8,
  FT230X_CBUS_TXDEN = 9,
  FT230X_CBUS_CLK24 = 10,
  FT230X_CBUS_CLK12 = 11,
  FT230X_CBUS_CLK6 = 12,
  FT230X_CBUS_BAT_DETECT = 13,
  FT230X_CBUS_BAT_DETECT_NEG = 14,
  FT230X_CBUS_I2C_TXE = 15,
  FT230X_CBUS_I2C_RXF = 16,
  FT230X_CBUS_VBUS_SENSE = 17,
  FT230X_CBUS_BB_WR = 18,
  FT230X_CBUS_BB_RD = 19,
  FT230X_CBUS_TIME_STAMP = 20,
  FT230X_CBUS_AWAKE = 21
} FT230X_CBUS_Function;

typedef enum
{
  FTDI_DEVICE_UNKNOWN = 0UL,            // bcdDevice = 0x0000
  FTDI_DEVICE_AM,                       // bcdDevice = 0x0200
  FTDI_DEVICE_BM,                       // bcdDevice = 0x0400
  FTDI_DEVICE_2232C,                    // bcdDevice = 0x0500
  FTDI_DEVICE_232R,                     // bcdDevice = 0x0600
  FTDI_DEVICE_2232H,                    // bcdDevice = 0x0700
  FTDI_DEVICE_4232H,                    // bcdDevice = 0x0800
  FTDI_DEVICE_232H,                     // bcdDevice = 0x0900
  FTDI_DEVICE_203X                      // bcdDevice = 0x1000
} FTDI_DeviceType;

typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef uint8_t UCHAR;

typedef struct FTDI_DeviceContext FTDI_DeviceContext;

typedef struct FTDI_InterfaceContext FTDI_InterfaceContext;

typedef void FTDI_Emulator_RxCallback( FTDI_InterfaceContext * Interface );
typedef uint32_t FTDI_Emulator_TxCallback( FTDI_InterfaceContext * Interface );

struct FTDI_InterfaceContext
{
  FTDI_DeviceContext * Device;
  FTDI_Emulator_RxCallback * OnRxCompleted;
  FTDI_Emulator_TxCallback * OnTxCompleted;
  void * UART_Context;
  uint32_t RxCompleted;

  WORD InEndpointSize;
  WORD OutEndpointSize;

  UCHAR InterfaceNumber;
  UCHAR InEndpoint;
  UCHAR OutEndpoint;
  UCHAR EventChar;
  UCHAR EventCharEnabled;
  UCHAR ErrorChar;
  UCHAR ErrorCharEnabled;
  UCHAR LatencyTimerValue;
  UCHAR LatencyTimerCount;

  /* B0 Output handshaking using RTS/CTS
   *     0 = disabled
   *     1 = enabled
   * B1 Output handshaking using DTR/DSR
   *     0 = disabled
   *     1 = enabled
   * B2 Xon/Xoff handshaking
   *     0 = disabled
   *     1 = enabled
   */
  UCHAR FlowControl;
  UCHAR FlowControlXON;
  UCHAR FlowControlXOFF;

  FTDI_COM_LineCoding LineCoding;

  DWORD BaudRate;

  /*
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
  WORD ModemControl;

  union
  {
    WORD dummy;
    struct
    {
      /* B0 Reserved - must be 1
       * B1 Reserved - must be 0
       * B2 Reserved - must be 0
       * B3 Reserved - must be 0
       * B4 Clear to Send (CTS)
       * B5 Data Set Ready (DSR)
       * B6 Ring Indicator (RI)
       * B7 Receive Line Signal Detect (RLSD)
       */
      UCHAR ModemStatus;
      /* B0 Data Ready (Receive RDR)
       * B1 Overrun Error (Receive OE)
       * B2 Parity Error (Receive PE)
       * B3 Framing Error (Receive FE)
       * B4 Break Interrupt (Receive Break BI)
       * B5 Transmitter Holding Register (THRE)
       * B6 Transmitter Empty (TEMT)
       * B7 Error in RCVR FIFO
       */
      UCHAR LineStatus;
    };
  };

  uint8_t TxBuffer[ 0x40 ];
  uint32_t TxOffset;
  uint32_t Remaining;
};

typedef struct
{
  //
  // Rev4 (FT232B) extensions
  //
  UCHAR IsoIn;                        // non-zero if in endpoint is isochronous
  UCHAR IsoOut;                       // non-zero if out endpoint is isochronous

  UCHAR USBVersionEnable;             // non-zero if chip uses USBVersion
} FT_Device_BM;

typedef struct
{
  //
  // Rev 5 (FT2232) extensions
  //
  UCHAR IsoInA;                       // non-zero if in endpoint is isochronous
  UCHAR IsoOutA;                      // non-zero if out endpoint is isochronous

  UCHAR IsoInB;                       // non-zero if in endpoint is isochronous
  UCHAR IsoOutB;                      // non-zero if out endpoint is isochronous

  UCHAR AIsHighCurrent;               // non-zero if interface is high current
  UCHAR AIsVCP;                       // non-zero if interface is to use VCP drivers
  UCHAR AFunction;                    // FT_IF_FUNCTION_XXX

  UCHAR BIsHighCurrent;               // non-zero if interface is high current
  UCHAR BIsVCP;                       // non-zero if interface is to use VCP drivers
  UCHAR BFunction;                    // FT_IF_FUNCTION_XXX

  UCHAR IFAIsFifo;                    // non-zero if interface is 245 FIFO
  UCHAR IFAIsFifoTar;                 // non-zero if interface is 245 FIFO CPU target
  UCHAR IFAIsFastSer;                 // non-zero if interface is Fast serial

  UCHAR IFBIsFifo;                    // non-zero if interface is 245 FIFO
  UCHAR IFBIsFifoTar;                 // non-zero if interface is 245 FIFO CPU target
  UCHAR IFBIsFastSer;                 // non-zero if interface is Fast serial
} FT_Device_2232C;

typedef struct
{
  //
  // Rev 6 (FT232R) extensions
  //
  UCHAR HighDriveIOs;                 // High Drive I/Os
  UCHAR IsD2XX;                       // non-zero if using D2XX driver

  UCHAR UseExtOsc;                    // Use External Oscillator
  UCHAR EndpointSize;                 // Endpoint size

  UCHAR InvertSet;                    // Bitmap for All, Bit0 : TXD

  UCHAR InvertTXD;                    // non-zero if invert TXD
  UCHAR InvertRXD;                    // non-zero if invert RXD
  UCHAR InvertRTS;                    // non-zero if invert RTS
  UCHAR InvertCTS;                    // non-zero if invert CTS
  UCHAR InvertDTR;                    // non-zero if invert DTR
  UCHAR InvertDSR;                    // non-zero if invert DSR
  UCHAR InvertDCD;                    // non-zero if invert DCD
  UCHAR InvertRI;                     // non-zero if invert RI

  UCHAR CbusFunctions[ 5 + 1 ];       // Cbus Mux control

  UCHAR Cbus0;                        // Cbus Mux control
  UCHAR Cbus1;                        // Cbus Mux control
  UCHAR Cbus2;                        // Cbus Mux control
  UCHAR Cbus3;                        // Cbus Mux control
  UCHAR Cbus4;                        // Cbus Mux control

} FT_Device_232R;

typedef struct
{
  //
  // Rev 7 (FT2232H) Extensions
  //
  UCHAR PowerSaveEnable;              // non-zero if using BCBUS7 to save power for self-powered designs

  UCHAR AIsVCP;                       // non-zero if interface is to use VCP drivers
  UCHAR BIsVCP;                       // non-zero if interface is to use VCP drivers

  UCHAR AFunction;                    // FT_IF_FUNCTION_XXX
  UCHAR BFunction;                    // FT_IF_FUNCTION_XXX

  UCHAR ALSlowSlew;                   // non-zero if AL pins have slow slew
  UCHAR ALSchmittInput;               // non-zero if AL pins are Schmitt input
  UCHAR ALDriveCurrent;               // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR AHSlowSlew;                   // non-zero if AH pins have slow slew
  UCHAR AHSchmittInput;               // non-zero if AH pins are Schmitt input
  UCHAR AHDriveCurrent;               // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR BLSlowSlew;                   // non-zero if BL pins have slow slew
  UCHAR BLSchmittInput;               // non-zero if BL pins are Schmitt input
  UCHAR BLDriveCurrent;               // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR BHSlowSlew;                   // non-zero if BH pins have slow slew
  UCHAR BHSchmittInput;               // non-zero if BH pins are Schmitt input
  UCHAR BHDriveCurrent;               // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR IFAIsFifo;                    // non-zero if interface is 245 FIFO
  UCHAR IFAIsFifoTar;                 // non-zero if interface is 245 FIFO CPU target
  UCHAR IFAIsFastSer;                 // non-zero if interface is Fast serial

  UCHAR IFBIsFifo;                    // non-zero if interface is 245 FIFO
  UCHAR IFBIsFifoTar;                 // non-zero if interface is 245 FIFO CPU target
  UCHAR IFBIsFastSer;                 // non-zero if interface is Fast serial

} FT_Device_2232H;

typedef struct
{
  //
  // Rev 8 (FT4232H) Extensions
  //
  UCHAR AIsVCP;                       // non-zero if interface is to use VCP drivers
  UCHAR BIsVCP;                       // non-zero if interface is to use VCP drivers
  UCHAR CIsVCP;                       // non-zero if interface is to use VCP drivers
  UCHAR DIsVCP;                       // non-zero if interface is to use VCP drivers

  UCHAR ARIIsTXDEN;                   // non-zero if port A uses RI as RS485 TXDEN
  UCHAR BRIIsTXDEN;                   // non-zero if port B uses RI as RS485 TXDEN
  UCHAR CRIIsTXDEN;                   // non-zero if port C uses RI as RS485 TXDEN
  UCHAR DRIIsTXDEN;                   // non-zero if port D uses RI as RS485 TXDEN

  UCHAR ASlowSlew;                    // non-zero if A pins have slow slew
  UCHAR ASchmittInput;                // non-zero if A pins are Schmitt input
  UCHAR ADriveCurrent;                // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR BSlowSlew;                    // non-zero if B pins have slow slew
  UCHAR BSchmittInput;                // non-zero if B pins are Schmitt input
  UCHAR BDriveCurrent;                // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR CSlowSlew;                    // non-zero if C pins have slow slew
  UCHAR CSchmittInput;                // non-zero if C pins are Schmitt input
  UCHAR CDriveCurrent;                // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR DSlowSlew;                    // non-zero if D pins have slow slew
  UCHAR DSchmittInput;                // non-zero if D pins are Schmitt input
  UCHAR DDriveCurrent;                // valid values are 4mA, 8mA, 12mA, 16mA
} FT_Device_4232H;

typedef struct
{
  //
  // Rev 9 (FT232H) Extensions
  //
  UCHAR IsVCP;                        // non-zero if interface is to use VCP drivers
  UCHAR PowerSaveEnable;              // non-zero if using ACBUS7 to save power for self-powered designs

  UCHAR ACSlowSlew;                   // non-zero if AC pins have slow slew
  UCHAR ACSchmittInput;               // non-zero if AC pins are Schmitt input
  UCHAR ACDriveCurrent;               // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR ADSlowSlew;                   // non-zero if AD pins have slow slew
  UCHAR ADSchmittInput;               // non-zero if AD pins are Schmitt input
  UCHAR ADDriveCurrent;               // valid values are 4mA, 8mA, 12mA, 16mA

  UCHAR CbusFunctions[ 10 ];          // Cbus Mux control

  UCHAR Cbus0;                        // Cbus Mux control
  UCHAR Cbus1;                        // Cbus Mux control
  UCHAR Cbus2;                        // Cbus Mux control
  UCHAR Cbus3;                        // Cbus Mux control
  UCHAR Cbus4;                        // Cbus Mux control
  UCHAR Cbus5;                        // Cbus Mux control
  UCHAR Cbus6;                        // Cbus Mux control
  UCHAR Cbus7;                        // Cbus Mux control
  UCHAR Cbus8;                        // Cbus Mux control
  UCHAR Cbus9;                        // Cbus Mux control

  UCHAR Function;                     // FT_IF_FUNCTION_XXX

  UCHAR IsFifo;                       // non-zero if interface is 245 FIFO
  UCHAR IsFifoTar;                    // non-zero if interface is 245 FIFO CPU target
  UCHAR IsFastSer;                    // non-zero if interface is Fast serial
  UCHAR IsFT1248;                     // non-zero if interface is FT1248

  UCHAR FT1248Cpol;                   // FT1248 clock polarity - clock idle high (1) or clock idle low (0)
  UCHAR FT1248Lsb;                    // FT1248 data is LSB (1) or MSB (0)
  UCHAR FT1248FlowControl;            // FT1248 flow control enable

} FT_Device_232H;

typedef struct
{
  //
  // Rev 9 (FT232H) Extensions
  //
  UCHAR InvertSet;                    // Bitmap for All, Bit0 : TXD

  UCHAR InvertTXD;                    // non-zero if invert TXD
  UCHAR InvertRXD;                    // non-zero if invert RXD
  UCHAR InvertRTS;                    // non-zero if invert RTS
  UCHAR InvertCTS;                    // non-zero if invert CTS
  UCHAR InvertDTR;                    // non-zero if invert DTR
  UCHAR InvertDSR;                    // non-zero if invert DSR
  UCHAR InvertDCD;                    // non-zero if invert DCD
  UCHAR InvertRI;                     // non-zero if invert RI

  UCHAR CbusFunctions[ 4 ];          // Cbus Mux control

  UCHAR Cbus0;                        // Cbus Mux control
  UCHAR Cbus1;                        // Cbus Mux control
  UCHAR Cbus2;                        // Cbus Mux control
  UCHAR Cbus3;                        // Cbus Mux control

} FT_Device_230X;

struct FTDI_DeviceContext
{
  FTDI_InterfaceContext * Interfaces[ 4 ];
  FTDI_DeviceType DeviceType;           // By bcdDevice

  char * ManufactureID;               // "FT"
  char * ManufactureStr;              // "FTDI"
  char * ProductStr;                  // "USB HS Serial Converter"
  char * SerialNumberStr;             // "FT000001" if fixed, or NULL

  WORD bcdDevice;                     // To DeviceType
  WORD VendorID;                      // 0x0403
  WORD ProductID;                     // 0x0601
  WORD USBVersion;                    // BCD (0x0200 => USB2)

  WORD EepromSize;                    // 128, 256, 512
  // UCHAR EepromType;                // 0x46, 0x56, 0x66 : 93C46, 93C56, 93C66
  WORD InterfaceNum;                  // 1, 2, 4

  UCHAR MaxPower;                     // 0xFA = 500 mA
  UCHAR SelfPowered;                  // 0 = bus powered, 1 = self powered
  UCHAR RemoteWakeup;                 // 0 = not capable, 1 = capable

  UCHAR ManufactureStrSize;           //
  UCHAR ProductStrSize;
  UCHAR SerialNumberStrSize;

  UCHAR USBVersionEnable;             // BM and 2232C
  UCHAR PullDownEnable;               // non-zero if pull down enabled
  UCHAR SerNumEnable;                 // non-zero if serial number to be used
  UCHAR PnP;                          // 0 = disabled, 1 = enabled

  union
  {
    FT_Device_BM Device_BM;
    FT_Device_2232C Device_2232C;
    FT_Device_232R Device_232R;
    FT_Device_2232H Device_2232H;
    FT_Device_4232H Device_4232H;
    FT_Device_232H Device_232H;
    FT_Device_230X Device_230X;
  };
};

#ifdef __cplusplus
}
#endif

#endif /* __FTDI_TYPE_H */
