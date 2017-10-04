#ifndef __FTDI_BSP_H
#define __FTDI_BSP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "FTDI_Type.h"

int32_t FTDI_Emulator_BSP_Init( FTDI_InterfaceContext * Interface );
int32_t FTDI_Emulator_BSP_DeInit( FTDI_InterfaceContext * Interface );

// Reset UART:
//  Sets flow control set to 'none'
//  Sets Event char = $0D
//  Sets Event trigger = disabled
//  Clear DTR
//  Clear RTS
int32_t FTDI_Emulator_BSP_ResetRequest( FTDI_InterfaceContext * Interface );

int32_t FTDI_Emulator_BSP_SetModemCtrlRequest( FTDI_InterfaceContext * Interface );
int32_t FTDI_Emulator_BSP_SetBaudrateRequest( FTDI_InterfaceContext * Interface );
int32_t FTDI_Emulator_BSP_SetLineCodingRequest( FTDI_InterfaceContext * Interface );
int32_t FTDI_Emulator_BSP_SetFlowCtrlRequest( FTDI_InterfaceContext * Interface );

int32_t FTDI_Emulator_BSP_GetModemLineStatusRequest( FTDI_InterfaceContext * Interface );

uint32_t FTDI_Emulator_BSP_ReceiveOverrun( FTDI_InterfaceContext * Interface );

int32_t FTDI_Emulator_BSP_Transmit( FTDI_InterfaceContext * Interface, uint8_t * Buffer, uint32_t Size );
int32_t FTDI_Emulator_BSP_Receive( FTDI_InterfaceContext * Interface, uint8_t * Buffer, uint32_t Size );

int32_t FTDI_Emulator_BSP_CancelTx( FTDI_InterfaceContext * Interface );
int32_t FTDI_Emulator_BSP_CancelRx( FTDI_InterfaceContext * Interface );

#ifdef __cplusplus
}
#endif

#endif /* __FTDI_BSP_H */
