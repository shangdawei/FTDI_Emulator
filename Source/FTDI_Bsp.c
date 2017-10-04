#include "FTDI_Bsp.h"

#include "stm32f10x.h"

#define BSP_UART_COUNT                    ( 1 )

#define BSP_USART1_USED                   ( 1 )
#define BSP_USART2_USED                   ( 1 )
#define BSP_USART3_USED                   ( 0 )
#define BSP_UART4_USED                    ( 0 )

#define UART_TX_BUFFER_WORD_SIZE          ( 16 )
#define UART_RX_BUFFER_WORD_SIZE          ( 16 )

#define UART_TX_FIFO_EP_RATIO             ( 4 )
#define UART_RX_FIFO_EP_RATIO             ( 4 )

/*
 * FT232R
 * -------------------------------------------------------------------------------------------------
 * DataBit : 7, 8
 * StopBit : 1, 1.5, 2
 * Parity  : None, Even, Odd, Space, Mark
 *
 * STM32F10X
 * -------------------------------------------------------------------------------------------------
 * DataBit : 7, 8
 * StopBit : 0.5, 1, 1.5, 2
 * Parity  : None, Even, Odd
 *
 * Word = [ Data + Parity ]
 * WordLength = 8 or 9 bits
 * -------------------------------------------------------------------------------------------------
 * 7, N, 1 <--> 8, N, 1 : TX : ST - 0 1 2 3 4 5 6 '1' SP
 *                        RX : ST - 0 1 2 3 4 5 6  SP ST - 0 1 2 3 4 5 6  SP SP
 *                                                    FrameError happen
 *                                                    Lost Start bit of next byte
 *                                                    Does NOT supports 7, N, 1
 * -------------------------------------------------------------------------------------------------
 * Parity control disabled < PCE==0 >
 * -------------------------------------------------------------------------------------------------
 * 7, N, 2 <--> 8, N, 1 : TX : ST - 0 1 2 3 4 5 6 '1' SP
 *                        RX : ST - 0 1 2 3 4 5 6 SP  SP ST - 0 1 2 3 4 5 6 SP SP ST
 *                                                    FrameError NOT happen
 *                                                    StartBit NOT lost
 *
 * 7, M, 1 <--> 8, N, 1 : Software Parity : Data |=  0x80 : Bit7 = Mark <------> 7N2
 * 7, S, 1 <--> 8, N, 1 : Software Parity : Data &= ~0x80 : Bit7 = Space
 *
 * 8, N, 1 <--> 8, N, 1 : Parity control disabled
 *
 * 8, S, 1 <--> 9, N, 1 : Software Parity : Data |=  0x100
 * 8, M, 1 <--> 9, N, 1 : Software Parity : Data &= ~0x100
 * -------------------------------------------------------------------------------------------------
 * Parity control enabled < PCE==1, PS=E/O >
 * -------------------------------------------------------------------------------------------------
 * 7, E, 1 <--> 8, E, 1 : Hardware Parity : DR[7] is ignored by Hardware, Replaced by parity
 * 7, O, 1 <--> 8, O, 1 : Hardware Parity : DR[7] is ignored by Hardware, Replaced by parity
 *
 * 8, E, 1 <--> 9, E, 1 : Hardware Parity : DR[8] is ignored by Hardware, Replaced by parity
 * 8, O, 1 <--> 9, O, 1 : Hardware Parity : DR[8] is ignored by Hardware, Replaced by parity
 *
 * USART1
 * -------------------------------------------------------------------------------------------------
 * SystemCoreClock = 48 MHz, USART1 Clock = PCLK2 : 48 MHz
 * UART TX CLOCK : 48 MHz / 16 = 3 MHz
 * FT232R UART TX CLOCK = 3MHz
 * UART_BRR = ( PCLK + (BAUD >> 1) ) / BAUD
 */

#define BSP_UART1_RI_RLSD_EANBLED         ( 1 )
#define BSP_UART1_RTS_CTS_EANBLED         ( 1 )
#define BSP_UART1_DTR_DSR_EANBLED         ( 1 )

#define BSP_UART1                         USART1

#define BSP_UART1_CLK                     RCC_APB2Periph_USART1
#define BSP_UART1_IRQ                     USART1_IRQn

#define BSP_UART1_TXDMA_CLK               RCC_AHBPeriph_DMA1
#define BSP_UART1_TXDMA_CH                DMA1_Channel4
#define BSP_UART1_TXDMA_IRQ               DMA1_Channel4_IRQn

#define BSP_UART1_RXDMA_CLK               RCC_AHBPeriph_DMA1
#define BSP_UART1_RXDMA_CH                DMA1_Channel5
#define BSP_UART1_RXDMA_IRQ               DMA1_Channel5_IRQn

#define BSP_UART1_TX_PIN                  GPIO_Pin_9
#define BSP_UART1_TX_GPIO_PORT            GPIOA
#define BSP_UART1_TX_GPIO_CLK             RCC_APB2Periph_GPIOA

#define BSP_UART1_RX_PIN                  GPIO_Pin_10
#define BSP_UART1_RX_GPIO_PORT            GPIOA
#define BSP_UART1_RX_GPIO_CLK             RCC_APB2Periph_GPIOA

#if ( BSP_UART1_RTS_CTS_EANBLED > 0 )
#define BSP_UART1_RTS_PIN                 GPIO_Pin_6                // LED1
#define BSP_UART1_RTS_GPIO_PORT           GPIOC
#define BSP_UART1_RTS_GPIO_CLK            RCC_APB2Periph_GPIOC

#define BSP_UART1_CTS_PIN                 GPIO_Pin_9
#define BSP_UART1_CTS_GPIO_PORT           GPIOB
#define BSP_UART1_CTS_GPIO_CLK            RCC_APB2Periph_GPIOB      // KEY1
#endif

#if ( BSP_UART1_DTR_DSR_EANBLED > 0 )
#define BSP_UART1_DTR_PIN                 GPIO_Pin_7                // LED2
#define BSP_UART1_DTR_GPIO_PORT           GPIOC
#define BSP_UART1_DTR_GPIO_CLK            RCC_APB2Periph_GPIOC

#define BSP_UART1_DSR_PIN                 GPIO_Pin_8                // KEY2
#define BSP_UART1_DSR_GPIO_PORT           GPIOB
#define BSP_UART1_DSR_GPIO_CLK            RCC_APB2Periph_GPIOB
#endif

#if ( BSP_UART1_RI_RLSD_EANBLED > 0 )

#define BSP_UART1_RI_PIN                  GPIO_Pin_7                // KEY3
#define BSP_UART1_RI_GPIO_PORT            GPIOB
#define BSP_UART1_RI_GPIO_CLK             RCC_APB2Periph_GPIOB

#define BSP_UART1_RLSD_PIN                GPIO_Pin_6
#define BSP_UART1_RLSD_GPIO_PORT          GPIOB
#define BSP_UART1_RLSD_GPIO_CLK           RCC_APB2Periph_GPIOB      // KEY4

#endif

/*
 * USART2
 * -------------------------------------------------------------------------------------------------
 * SystemCoreClock = 48 MHz, USART2 Clock = PCLK1 : 24 MHz
 * UART TX CLOCK : 24 MHz / 16 = 1.5 MHz
 * FT232R UART TX CLOCK = 3MHz
 * UART_BRR = ( PCLK + (BAUD >> 1) ) / BAUD
 */
#define BSP_UART2_RI_RLSD_EANBLED         ( 1 )
#define BSP_UART2_RTS_CTS_EANBLED         ( 1 )
#define BSP_UART2_DTR_DSR_EANBLED         ( 1 )

#define BSP_UART2                         USART2

#define BSP_UART2_CLK                     RCC_APB1Periph_USART2
#define BSP_UART2_IRQ                     USART2_IRQn

#define BSP_UART2_TXDMA_CLK               RCC_AHBPeriph_DMA1
#define BSP_UART2_TXDMA_CH                DMA1_Channel7
#define BSP_UART2_TXDMA_IRQ               DMA1_Channel7_IRQn

#define BSP_UART2_RXDMA_CLK               RCC_AHBPeriph_DMA1
#define BSP_UART2_RXDMA_CH                DMA1_Channel6
#define BSP_UART2_RXDMA_IRQ               DMA1_Channel6_IRQn

#define BSP_UART2_TX_PIN                  GPIO_Pin_2
#define BSP_UART2_TX_GPIO_PORT            GPIOA
#define BSP_UART2_TX_GPIO_CLK             RCC_APB2Periph_GPIOA

#define BSP_UART2_RX_PIN                  GPIO_Pin_3
#define BSP_UART2_RX_GPIO_PORT            GPIOA
#define BSP_UART2_RX_GPIO_CLK             RCC_APB2Periph_GPIOA

#if ( BSP_UART2_RTS_CTS_EANBLED > 0 )
#define BSP_UART2_RTS_PIN                 GPIO_Pin_6                // LED1
#define BSP_UART2_RTS_GPIO_PORT           GPIOC
#define BSP_UART2_RTS_GPIO_CLK            RCC_APB2Periph_GPIOC

#define BSP_UART2_CTS_PIN                 GPIO_Pin_9
#define BSP_UART2_CTS_GPIO_PORT           GPIOB
#define BSP_UART2_CTS_GPIO_CLK            RCC_APB2Periph_GPIOB      // KEY1
#endif

#if ( BSP_UART2_DTR_DSR_EANBLED > 0 )
#define BSP_UART2_DTR_PIN                 GPIO_Pin_7                // LED2
#define BSP_UART2_DTR_GPIO_PORT           GPIOC
#define BSP_UART2_DTR_GPIO_CLK            RCC_APB2Periph_GPIOC

#define BSP_UART2_DSR_PIN                 GPIO_Pin_8                // KEY2
#define BSP_UART2_DSR_GPIO_PORT           GPIOB
#define BSP_UART2_DSR_GPIO_CLK            RCC_APB2Periph_GPIOB
#endif

#if ( BSP_UART2_RI_RLSD_EANBLED > 0 )

#define BSP_UART2_RI_PIN                  GPIO_Pin_7                // KEY3
#define BSP_UART2_RI_GPIO_PORT            GPIOB
#define BSP_UART2_RI_GPIO_CLK             RCC_APB2Periph_GPIOB

#define BSP_UART2_RLSD_PIN                GPIO_Pin_6
#define BSP_UART2_RLSD_GPIO_PORT          GPIOB
#define BSP_UART2_RLSD_GPIO_CLK           RCC_APB2Periph_GPIOB      // KEY4

#endif

/*
 * USART3
 * -------------------------------------------------------------------------------------------------
 * SystemCoreClock = 48 MHz, USART3 Clock = PCLK1 : 24 MHz
 * UART TX CLOCK : 24 MHz / 16 = 1.5 MHz
 * FT232R UART TX CLOCK = 3MHz
 * UART_BRR = ( PCLK + (BAUD >> 1) ) / BAUD
 */
#define BSP_UART3                         ( 0 )

/*
 * UART4
 * -------------------------------------------------------------------------------------------------
 * SystemCoreClock = 48 MHz, UART4 Clock = PCLK1 : 24 MHz
 * UART TX CLOCK : 24 MHz / 16 = 1.5 MHz
 * FT232R UART TX CLOCK = 3MHz
 * UART_BRR = ( PCLK + (BAUD >> 1) ) / BAUD
 */
#define BSP_UART4                         ( 0 )

typedef enum UART_PIN
{
  UART_TX = 0,
  UART_RX,
  UART_RTS,
  UART_CTS,
  UART_DTR,
  UART_DSR,
  UART_RI,
  UART_RLSD,
} UART_PIN;

typedef enum
{
  FC_NONE = (uint32_t) 0,
  FC_XONXOFF,
  FC_DTRDSR,
} BSP_UART_FlowControl;

typedef struct BSP_UART_Information
{
  USART_TypeDef * UART;
  uint32_t Clock;
  uint32_t IrqNum;

  DMA_Channel_TypeDef * TxDMA;
  uint32_t TxDMAClock;
  uint32_t TxDMAIrqNum;

  DMA_Channel_TypeDef * RxDMA;
  uint32_t RxDMAClock;
  uint32_t RxDMAIrqNum;

  GPIO_TypeDef * Port[ 8 ];
  uint32_t PORT_Clock[ 8 ];
  uint32_t Pin[ 8 ];
} BSP_UART_Information;

/*
 * When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
 * the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
 * because it is replaced by the parity.
 *
 * When receiving with the parity enabled, the value read in the MSB bit
 * is the received parity bit.
 */
typedef struct BSP_UART_Context
{
  const BSP_UART_Information * Infromation;
  BSP_UART_FlowControl FlowControl;
  uint32_t UseHardwareParity;
  uint32_t SoftwareParityType;
  uint32_t SoftwareParityError;
  uint32_t ReceiveOverrun;
  uint32_t TxBrokenState;
  FIFO * TxFifo;
  FIFO * RxFifo;

  uint32_t Transmitting;  // DMA Transmitting
  uint32_t TransmitSize;  // DMA TransferSize

  // for 8S1, 8M1 only , because FiFo is 8 bits, Copy to TxBuffer and Add Parity Bit to HiByte
  uint8_t TxBuffer[ UART_TX_BUFFER_WORD_SIZE * 2 ];

  // for all, Buffer Rx Data while Handle/Copy RxBuffer to RxFiFo
  // 16+16 Words Buffer for RX DMA
  uint32_t RxBufferIndex;
  uint16_t RxBuffers[ 2 ][ UART_RX_BUFFER_WORD_SIZE ];

  FTDI_InterfaceContext * Interface;  // Interface->UART_Context->Infromation->UART is used by ISR
} BSP_UART_Context;                   // To find RxCallback(Interface) and TxCallback(Interface)

const BSP_UART_Information BSP_UART1_Information = {
  BSP_UART1, BSP_UART1_CLK, BSP_UART1_IRQ,  //
  BSP_UART1_TXDMA_CH, BSP_UART1_TXDMA_CLK, BSP_UART1_TXDMA_IRQ,  //
  BSP_UART1_RXDMA_CH, BSP_UART1_RXDMA_CLK, BSP_UART1_RXDMA_IRQ,  //
  {
    BSP_UART1_TX_GPIO_PORT, BSP_UART1_RX_GPIO_PORT,
#if ( BSP_UART1_RTS_CTS_EANBLED > 0 )
    BSP_UART1_RTS_GPIO_PORT, BSP_UART1_CTS_GPIO_PORT,
#else
    0,0,
#endif

#if ( BSP_UART1_DTR_DSR_EANBLED > 0 )
    BSP_UART1_DTR_GPIO_PORT, BSP_UART1_DSR_GPIO_PORT,
#else
    0,0,
#endif
#if ( BSP_UART1_RI_RLSD_EANBLED > 0 )
    BSP_UART1_RI_GPIO_PORT, BSP_UART1_RLSD_GPIO_PORT,
#else
  0,0,
#endif
}, {
  BSP_UART1_TX_GPIO_CLK, BSP_UART1_RX_GPIO_CLK,
#if ( BSP_UART1_RTS_CTS_EANBLED > 0 )
  BSP_UART1_RTS_GPIO_CLK, BSP_UART1_CTS_GPIO_CLK,
#else
  0,0,
#endif

#if ( BSP_UART1_DTR_DSR_EANBLED > 0 )
  BSP_UART1_DTR_GPIO_CLK, BSP_UART1_DSR_GPIO_CLK,
#else
  0,0,
#endif
#if ( BSP_UART1_RI_RLSD_EANBLED > 0 )
  BSP_UART1_RI_GPIO_CLK, BSP_UART1_RLSD_GPIO_CLK,
#else
  0,0,
#endif
}, {
  BSP_UART1_TX_PIN, BSP_UART1_RX_PIN,
#if ( BSP_UART1_RTS_CTS_EANBLED > 0 )
  BSP_UART1_RTS_PIN, BSP_UART1_CTS_PIN,
#else
  0,0,
#endif

#if ( BSP_UART1_DTR_DSR_EANBLED > 0 )
  BSP_UART1_DTR_PIN, BSP_UART1_DSR_PIN,
#else
  0,0,
#endif
#if ( BSP_UART1_RI_RLSD_EANBLED > 0 )
  BSP_UART1_RI_PIN, BSP_UART1_RLSD_PIN,
#else
  0,0,
#endif
} };

BSP_UART_Context BSP_UART1_Context = {
  &BSP_UART1_Information, FC_NONE };

const BSP_UART_Information BSP_UART2_Information = {
  BSP_UART2, BSP_UART2_CLK, BSP_UART2_IRQ,  //
  BSP_UART2_TXDMA_CH, BSP_UART2_TXDMA_CLK, BSP_UART2_TXDMA_IRQ,  //
  BSP_UART2_RXDMA_CH, BSP_UART2_RXDMA_CLK, BSP_UART2_RXDMA_IRQ,  //
  {
    BSP_UART2_TX_GPIO_PORT, BSP_UART2_RX_GPIO_PORT,
#if ( BSP_UART2_RTS_CTS_EANBLED > 0 )
    BSP_UART2_RTS_GPIO_PORT, BSP_UART2_CTS_GPIO_PORT,
#else
    0,0,
#endif

#if ( BSP_UART2_DTR_DSR_EANBLED > 0 )
    BSP_UART2_DTR_GPIO_PORT, BSP_UART2_DSR_GPIO_PORT,
#else
    0,0,
#endif
#if ( BSP_UART2_RI_RLSD_EANBLED > 0 )
    BSP_UART2_RI_GPIO_PORT, BSP_UART2_RLSD_GPIO_PORT,
#else
  0,0,
#endif
}, {
  BSP_UART2_TX_GPIO_CLK, BSP_UART2_RX_GPIO_CLK,
#if ( BSP_UART2_RTS_CTS_EANBLED > 0 )
  BSP_UART2_RTS_GPIO_CLK, BSP_UART2_CTS_GPIO_CLK,
#else
  0,0,
#endif

#if ( BSP_UART2_DTR_DSR_EANBLED > 0 )
  BSP_UART2_DTR_GPIO_CLK, BSP_UART2_DSR_GPIO_CLK,
#else
  0,0,
#endif
#if ( BSP_UART2_RI_RLSD_EANBLED > 0 )
  BSP_UART2_RI_GPIO_CLK, BSP_UART2_RLSD_GPIO_CLK,
#else
  0,0,
#endif
}, {
  BSP_UART2_TX_PIN, BSP_UART2_RX_PIN,
#if ( BSP_UART2_RTS_CTS_EANBLED > 0 )
  BSP_UART2_RTS_PIN, BSP_UART2_CTS_PIN,
#else
  0,0,
#endif

#if ( BSP_UART2_DTR_DSR_EANBLED > 0 )
  BSP_UART2_DTR_PIN, BSP_UART2_DSR_PIN,
#else
  0,0,
#endif
#if ( BSP_UART2_RI_RLSD_EANBLED > 0 )
  BSP_UART2_RI_PIN, BSP_UART2_RLSD_PIN,
#else
  0,0,
#endif
} };

BSP_UART_Context BSP_UART2_Context = {
  &BSP_UART2_Information, FC_NONE };

BSP_UART_Context BSP_UART3_Context = {
  0, FC_NONE };

BSP_UART_Context BSP_UART4_Context = {
  0, FC_NONE };

BSP_UART_Context * BSP_UART_ARRAY[ 4 ] = {
  &BSP_UART2_Context, &BSP_UART1_Context, &BSP_UART3_Context, &BSP_UART4_Context };

int32_t FTDI_Emulator_BSP_DeInit( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = Interface->UART_Context;
  if ( UART_Context->TxFifo )
    FIFO_Destroy( UART_Context->TxFifo );
  if ( UART_Context->RxFifo )
    FIFO_Destroy( UART_Context->RxFifo );

  return 0;
}

/*
 * STM32F1 DMA has not FIFO, can not pack/unpack
 *
 * Source Data Width < Dest Data Width : Read and Pad 0 for HighHalfWord or High 3 Bytes
 * Source Data Width > Dest Data Width : Read and Discard HighHalfWord or High 3 Bytes
 *
 * Source Addr Inc = SourceDataWidth / 8
 * Dest Addr Inc   = DestDataWidth / 8
 *
 * CNTR : times of data to transfer
 *
 * #define PERIPH_BASE           ( (uint32_t) 0x40000000 )
 * #define AHBPERIPH_BASE        ( PERIPH_BASE + 0x20000 )
 * #define DMA1_BASE             ( AHBPERIPH_BASE + 0x0000 ) = 0x40020000
 * #define DMA1_Channel1_BASE    ( AHBPERIPH_BASE + 0x0008 ) = 0x40020008
 * #define DMA1_Channel2_BASE    ( AHBPERIPH_BASE + 0x001C ) = 0x4002001C
 * #define DMA1_Channel3_BASE    ( AHBPERIPH_BASE + 0x0030 ) = 0x40020030
 * #define DMA1_Channel4_BASE    ( AHBPERIPH_BASE + 0x0044 ) = 0x40020044
 * #define DMA1_Channel5_BASE    ( AHBPERIPH_BASE + 0x0058 ) = 0x40020058
 * #define DMA1_Channel6_BASE    ( AHBPERIPH_BASE + 0x006C ) = 0x4002006C
 * #define DMA1_Channel7_BASE    ( AHBPERIPH_BASE + 0x0080 ) = 0x40020080
 *
 * #define DMA2_BASE             ( AHBPERIPH_BASE + 0x0400 ) = 0x40020400
 * #define DMA2_Channel1_BASE    ( AHBPERIPH_BASE + 0x0408 ) = 0x40020408
 * #define DMA2_Channel2_BASE    ( AHBPERIPH_BASE + 0x041C ) = 0x4002041C
 * #define DMA2_Channel3_BASE    ( AHBPERIPH_BASE + 0x0430 ) = 0x40020430
 * #define DMA2_Channel4_BASE    ( AHBPERIPH_BASE + 0x0444 ) = 0x40020444
 * #define DMA2_Channel5_BASE    ( AHBPERIPH_BASE + 0x0458 ) = 0x40020458
 *
 */

#define DMA( Channel )        ( ( DMA_TypeDef * ) ( ( (uint32_t) (Channel) ) & 0xFFFFFF00 ) )
#define DMA_SHIFT( Channel )  ( ( ( ( ( uint32_t) (Channel) ) & 0x000000FF ) / 0x14 ) << 2 )

void FTDI_Emulator_BSP_StartReceive( FTDI_InterfaceContext * Interface );

int32_t FTDI_Emulator_BSP_Init( FTDI_InterfaceContext * Interface )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  BSP_UART_Context * UART_Context = BSP_UART_ARRAY[ Interface->InterfaceNumber ];
  Interface->UART_Context = UART_Context;

  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  if ( 0 == UART_Information )
    return -1;

  UART_Context->Interface = Interface;

  UART_Context->TxFifo = FIFO_Create( Interface->OutEndpointSize * UART_TX_FIFO_EP_RATIO );
  if ( 0 == UART_Context->TxFifo )
    return -1;

  UART_Context->RxFifo = FIFO_Create( Interface->InEndpointSize * UART_RX_FIFO_EP_RATIO );
  if ( 0 == UART_Context->RxFifo )
    return -1;

  /* Enable GPIO clock */
  uint32_t RCC_APB2Periph = RCC_APB2Periph_AFIO;
  for ( uint32_t i = UART_TX; i <= UART_RLSD; i++ )
    RCC_APB2Periph |= UART_Information->PORT_Clock[ i ];
  RCC_APB2PeriphClockCmd( RCC_APB2Periph, ENABLE );

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = ( 1 << 0 );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init( GPIOC, &GPIO_InitStructure );

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = ( 1 << 1 );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init( GPIOC, &GPIO_InitStructure );

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_TX ];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( UART_Information->Port[ UART_TX ], &GPIO_InitStructure );

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_RX ];
  GPIO_Init( UART_Information->Port[ UART_RX ], &GPIO_InitStructure );

  /* Configure USART RTS as alternate function push-pull */
  if ( UART_Information->Port[ UART_RTS ] )
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_RTS ];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( UART_Information->Port[ UART_RTS ], &GPIO_InitStructure );
  }

  /* Configure USART CTS as input floating */
  if ( UART_Information->Port[ UART_CTS ] )
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_CTS ];
    GPIO_Init( UART_Information->Port[ UART_CTS ], &GPIO_InitStructure );
  }

  /* Configure USART DTR as alternate function push-pull */
  if ( UART_Information->Port[ UART_DTR ] )
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_DTR ];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( UART_Information->Port[ UART_DTR ], &GPIO_InitStructure );
  }

  /* Configure USART DSR as input floating */
  if ( UART_Information->Port[ UART_DSR ] )
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_DSR ];
    GPIO_Init( UART_Information->Port[ UART_DSR ], &GPIO_InitStructure );
  }

  /* Configure USART RI as input floating */
  if ( UART_Information->Port[ UART_RI ] )
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_RI ];
    GPIO_Init( UART_Information->Port[ UART_RI ], &GPIO_InitStructure );
  }

  /* Configure USART RLSD as input floating */
  if ( UART_Information->Port[ UART_RLSD ] )
  {
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_RLSD ];
    GPIO_Init( UART_Information->Port[ UART_RLSD ], &GPIO_InitStructure );
  }

  if ( USART1 == UART_Information->UART )  // Enable UART clock
    RCC_APB2PeriphClockCmd( UART_Information->Clock, ENABLE );
  else
    RCC_APB1PeriphClockCmd( UART_Information->Clock, ENABLE );

  /* Enable UART, UART_TX, UART_RX, RXNEIE: RXNE interrupt enable */
  // Enable RX Interrupt only,  TX interrupt enabled in send routine
  UART_Information->UART->CR1 =
  USART_CR1_UE |          // USART enable
    USART_CR1_IDLEIE |    // IDLE interrupt enabled
    USART_CR1_TE |        // Transmitter enabled
    USART_CR1_RE |        // Receiver enabled
    0;

  FTDI_Emulator_BSP_SetBaudrateRequest( Interface );
  FTDI_Emulator_BSP_SetFlowCtrlRequest( Interface );
  FTDI_Emulator_BSP_SetLineCodingRequest( Interface );

  /* RxDMA (triggered by USART Tx event) Config */
  RCC_AHBPeriphClockCmd( UART_Information->RxDMAClock, ENABLE );

  UART_Information->RxDMA->CCR = 0;   // Disable DMA to Config it
  UART_Information->RxDMA->CPAR = (uint32_t) &UART_Information->UART->DR;

  UART_Information->RxDMA->CCR =
  DMA_M2M_Disable |                     // Memory to memory mode disabled
    DMA_Priority_High |                 // Channel priority level
    DMA_PeripheralDataSize_HalfWord |   // Peripheral size always uses Half Word
    DMA_MemoryDataSize_HalfWord |       // Memory size always uses Half Word
    DMA_MemoryInc_Enable |              // Memory increment mode
    DMA_PeripheralInc_Disable |         // Peripheral increment mode
    DMA_Mode_Normal |                   // Circular mode disabled
    DMA_DIR_PeripheralSRC |             // Data transfer direction, Write to memory
    DMA_CCR1_TCIE | DMA_CCR1_TEIE |     // Transfer error and complete interrupt enable
    0;                                  // Channel disabled

  UART_Context->RxBufferIndex = 0;
  FTDI_Emulator_BSP_StartReceive( Interface );

  /* TxDMA (triggered by USART Tx event) Config */
  RCC_AHBPeriphClockCmd( UART_Information->TxDMAClock, ENABLE );

  UART_Information->TxDMA->CCR = 0;   // Disable DMA to Config it

  UART_Information->TxDMA->CNDTR = 0;
  UART_Information->TxDMA->CMAR = 0;
  UART_Information->TxDMA->CPAR = (uint32_t) &UART_Information->UART->DR;

  UART_Information->TxDMA->CCR =
  DMA_M2M_Disable |                     // Memory to memory mode disabled
    DMA_Priority_High |                 // Channel priority level
    DMA_PeripheralDataSize_HalfWord |   // Peripheral size always uses Half Word
    DMA_MemoryDataSize_Byte |           // Memory size, Byte/HalfWord for 8/9 bits word length
    DMA_MemoryInc_Enable |              // Memory increment mode
    DMA_PeripheralInc_Disable |         // Peripheral increment mode
    DMA_Mode_Normal |                   // Circular mode disabled
    DMA_DIR_PeripheralDST |             // Data transfer direction, Read from memory
    DMA_CCR1_TCIE | DMA_CCR1_TEIE |     // Transfer error and complete interrupt enable
    0;                                  // Channel disabled

  /* RXDMA Interrupt Priority is heigher USART IDLE Interrupt */
  /* USART IDLE Interrupt Priority is heigher TXDMA Interrupt */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_InitStructure.NVIC_IRQChannel = UART_Information->TxDMAIrqNum;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_Init( &NVIC_InitStructure );

  DMA(UART_Information->TxDMA )->IFCR = DMA_IFCR_CGIF1 << DMA_SHIFT( UART_Information->TxDMA );
  NVIC_InitStructure.NVIC_IRQChannel = UART_Information->IrqNum;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_Init( &NVIC_InitStructure );

  DMA(UART_Information->RxDMA )->IFCR = DMA_IFCR_CGIF1 << DMA_SHIFT( UART_Information->RxDMA );
  NVIC_InitStructure.NVIC_IRQChannel = UART_Information->RxDMAIrqNum;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Init( &NVIC_InitStructure );

  /* USB interrupt PreemptionPriority = 1 */
  /* USB Wake-up interrupt PreemptionPriority = 0 */

  //  Enable USART DMA TX and RX request
  UART_Information->UART->CR3 |= ( USART_CR3_DMAR | USART_CR3_DMAT );

  // RxDMA is ready to receive -- RxBuffer[0][0], HalfWord, Transfer UART_RX_BUFFER_WORD_SIZE Times
  // TxDMA is disabled -- MemoryAddr, MemorySize and TransferTimes are unknown now

  return 0;
}

// Reset UART:
//  Sets flow control set to 'none'
//  Sets Event char = $0D
//  Sets Event trigger = disabled
//  Clear DTR
//  Clear RTS
int32_t FTDI_Emulator_BSP_ResetRequest( FTDI_InterfaceContext * Interface )
{
  Interface->EventChar = 0x0D;
  Interface->EventCharEnabled = 0;

  Interface->FlowControl = FTDI_SET_FLOW_CTRL_NONE;
  FTDI_Emulator_BSP_SetFlowCtrlRequest( Interface );
  Interface->ModemControl =  // Mask and Value
    ( ( ( 1 << FTDI_SET_MODEM_CTRL_DTR ) | ( 1 << FTDI_SET_MODEM_CTRL_RTS ) ) << 8 )
      | ( ( ( 0 << FTDI_SET_MODEM_CTRL_DTR ) | ( 0 << FTDI_SET_MODEM_CTRL_RTS ) ) << 0 );
  FTDI_Emulator_BSP_SetModemCtrlRequest( Interface );
  return 0;
}

/* Interface->ModemControl
 *
 *  B0    DTR state
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
int32_t FTDI_Emulator_BSP_SetModemCtrlRequest( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  uint8_t Mask, Value;

  Mask = Interface->ModemControl >> 8;
  Value = Interface->ModemControl >> 0;

  if ( Mask & FTDI_SET_MODEM_CTRL_DTR )
  {
    if ( UART_Context->Infromation->Port[ UART_DTR ] )
    {
      // BSRR : BR:BS : If both BSx and BRx are set, BSx has priority -- Set
      if ( Value & FTDI_SET_MODEM_CTRL_DTR )
        UART_Context->Infromation->Port[ UART_DTR ]->BSRR = FTDI_SET_MODEM_CTRL_DTR;
      else
        UART_Context->Infromation->Port[ UART_DTR ]->BRR = FTDI_SET_MODEM_CTRL_DTR;
    }
  }

  if ( Mask & FTDI_SET_MODEM_CTRL_RTS )
  {
    if ( UART_Context->Infromation->Port[ UART_RTS ] )
    {
      // BSRR : BR:BS : If both BSx and BRx are set, BSx has priority -- Set
      if ( Value & FTDI_SET_MODEM_CTRL_RTS )
        UART_Context->Infromation->Port[ UART_RTS ]->BSRR = FTDI_SET_MODEM_CTRL_RTS;
      else
        UART_Context->Infromation->Port[ UART_RTS ]->BRR = FTDI_SET_MODEM_CTRL_RTS;
    }
  }

  return 0;
}

/* Interface->FlowControl
 *
 * B0 Output handshaking using RTS/CTS
 *     0 = disabled
 *     1 = enabled
 * B1 Output handshaking using DTR/DSR
 *     0 = disabled
 *     1 = enabled
 * B2 Xon/Xoff handshaking
 *     0 = disabled
 *     1 = enabled
 */
int32_t FTDI_Emulator_BSP_SetFlowCtrlRequest( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  USART_TypeDef * USARTx = UART_Context->Infromation->UART;
  uint32_t USART_CR3;

#define CR3_CLEAR_Mask            ((uint16_t)0xFCFF)  /*!< USART CR3 Mask */

  // Disable FlowControl
  UART_Context->FlowControl = FC_NONE;

  // Disable RTS/CTS FlowControl
  USART_CR3 = USARTx->CR3;
  USART_CR3 &= CR3_CLEAR_Mask;

  if ( Interface->FlowControl & FTDI_SET_FLOW_CTRL_RTSCTS )
  {
    USART_CR3 |= USART_HardwareFlowControl_RTS_CTS;
    USARTx->CR3 = (uint16_t) USART_CR3;
  }
  else if ( Interface->FlowControl & FTDI_SET_FLOW_CTRL_DTRDSR )
    UART_Context->FlowControl = FC_DTRDSR;
  else if ( Interface->FlowControl & FTDI_SET_FLOW_CTRL_XONXOFF )
    UART_Context->FlowControl = FC_XONXOFF;

  return 0;
}

/* Interface->BaudRate
 */
int32_t FTDI_Emulator_BSP_SetBaudrateRequest( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  USART_TypeDef * USARTx = UART_Context->Infromation->UART;

  uint32_t apbclock;
  uint16_t Div, DIV_Mantissa, DIV_Fraction;
  uint32_t BaudRate = Interface->BaudRate;

  RCC_ClocksTypeDef RCC_ClocksStatus;
  RCC_GetClocksFreq( &RCC_ClocksStatus );
  if ( USART1_BASE == (uint32_t) ( USARTx ) )
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  else
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;

  Div = ( apbclock + ( BaudRate >> 1 ) ) / BaudRate;

  /* USART OVER8 mode Enable Mask */
#define CR1_OVER8_Set ((uint16_t)0x8000)

  if ( USARTx->CR1 & CR1_OVER8_Set )
  {
    DIV_Mantissa = ( Div & ~0x7 ) << 1;
    DIV_Fraction = Div & 0x07;
    Div = DIV_Mantissa | DIV_Fraction;
  }

  USARTx->BRR = Div;

  return 0;
}

int32_t FTDI_Emulator_BSP_SetLineBreak( FTDI_InterfaceContext * Interface, uint32_t BreakOn )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  if ( BreakOn )
  {
    /* Configure USART Tx as general purpose push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_TX ];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( UART_Information->Port[ UART_TX ], &GPIO_InitStructure );

    UART_Information->Port[ UART_TX ]->BRR = UART_Information->Pin[ UART_TX ];
  }
  else
  {
    UART_Information->Port[ UART_TX ]->BSRR = UART_Information->Pin[ UART_TX ];

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = UART_Information->Pin[ UART_TX ];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( UART_Information->Port[ UART_TX ], &GPIO_InitStructure );
  }
  return 0;
}

/*   B0..7   Number of data bits < 7 or 8 >
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
 */
int32_t FTDI_Emulator_BSP_SetLineCodingRequest( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  USART_TypeDef * USARTx = UART_Context->Infromation->UART;

  uint32_t USART_CR1;
  uint32_t USART_CR2;

  uint16_t DataBit = Interface->LineCoding.Coding & FTDI_SET_DATA_REQUEST_DATA_BITS;
  uint16_t Parity = Interface->LineCoding.Coding & FTDI_SET_DATA_REQUEST_PARITY;
  uint16_t StopBit = Interface->LineCoding.Coding & FTDI_SET_DATA_REQUEST_STOP_BITS;
  uint16_t TxBreak = Interface->LineCoding.Coding & FTDI_SET_DATA_REQUEST_BREAK;

  // Set/Clear Tx Pin Level
  if ( UART_Context->TxBrokenState ^ TxBreak )                    // Break State Changed
  {
    FTDI_Emulator_BSP_SetLineBreak( Interface, TxBreak );         // Change Tx Level
    UART_Context->TxBrokenState ^= FTDI_SET_DATA_REQUEST_BREAK;   // Save New State
  }

  // WordLength=8, ParityEnable=0, ParitySelection=Even
  USART_CR1 = USARTx->CR1 & ~( USART_CR1_M | USART_CR1_PS | USART_CR1_PCE );

  if ( FTDI_SET_DATA_PARITY_NONE != Parity )
  {
    if ( ( FTDI_SET_DATA_PARITY_ODD == Parity ) || ( FTDI_SET_DATA_PARITY_EVEN == Parity ) )
    {
      UART_Context->UseHardwareParity = 1;        // Execute by Hardware
      UART_Context->SoftwareParityType = FTDI_SET_DATA_PARITY_NONE;
      USART_CR1 |= USART_CR1_PCE;                 // Parity control enabled
      if ( FTDI_SET_DATA_PARITY_ODD == Parity )
        USART_CR1 |= USART_CR1_PS;                // PS=1 Odd parity

      if ( FTDI_SET_DATA_DATA_BITS_8 == DataBit )
        USART_CR1 |= USART_CR1_M;                 // M=1 Start bit, 9 Data bits, n Stop bit
    }
    else  // FTDI_SET_DATA_PARITY_MARK or FTDI_SET_DATA_PARITY_SPACE
    {
      UART_Context->UseHardwareParity = 0;        // Execute by Software
      UART_Context->SoftwareParityType = Parity;
      if ( FTDI_SET_DATA_DATA_BITS_8 == DataBit )
        USART_CR1 |= USART_CR1_M;     // M=1 Start bit, 9 Data bits, n Stop bit
    }
  }
  else
  {
    UART_Context->SoftwareParityType = FTDI_SET_DATA_PARITY_NONE;
    UART_Context->UseHardwareParity = 0;        // Execute by Software
    // ( FTDI_SET_DATA_PARITY_NONE == Parity )
    // WordLength=8, ParityEnable=0
  }

  USARTx->CR1 = USART_CR1;

  USART_CR2 = USARTx->CR2 & ~( ( 1 << 13 ) | ( 1 << 12 ) );  // STOP
  if ( FTDI_SET_DATA_STOP_BITS_1_5 == StopBit )
    USART_CR2 |= ( 1 << 13 ) | ( 1 << 12 );       // 1.5 STOP
  else if ( FTDI_SET_DATA_STOP_BITS_2 == StopBit )
    USART_CR2 |= ( 1 << 13 ) | ( 0 << 12 );       // 2 STOP
  USARTx->CR2 |= USART_CR2;

  return 0;
}

int32_t FTDI_Emulator_BSP_GetModemLineStatusRequest( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  USART_TypeDef * USARTx = UART_Context->Infromation->UART;

  Interface->ModemStatus = 0x01;
  if ( UART_Context->Infromation->Port[ UART_CTS ] )
  {
    if ( UART_Context->Infromation->Port[ UART_CTS ]->IDR  //
    & UART_Context->Infromation->Pin[ UART_CTS ] )
    {
      Interface->ModemStatus |= FTDI_GET_MODEM_STATUS_CTS;
    }
  }

  if ( UART_Context->Infromation->Port[ UART_DSR ] )
  {
    if ( UART_Context->Infromation->Port[ UART_DSR ]->IDR  //
    & UART_Context->Infromation->Pin[ UART_DSR ] )
    {
      Interface->ModemStatus |= FTDI_GET_MODEM_STATUS_DSR;
    }
  }

  if ( UART_Context->Infromation->Port[ UART_RI ] )
  {
    if ( UART_Context->Infromation->Port[ UART_RI ]->IDR  //
    & UART_Context->Infromation->Pin[ UART_RI ] )
    {
      Interface->ModemStatus |= FTDI_GET_MODEM_STATUS_RI;
    }
  }

  if ( UART_Context->Infromation->Port[ UART_RLSD ] )
  {
    if ( UART_Context->Infromation->Port[ UART_RLSD ]->IDR  //
    & UART_Context->Infromation->Pin[ UART_RLSD ] )
    {
      Interface->ModemStatus |= FTDI_GET_MODEM_STATUS_RLSD;
    }
  }

  Interface->LineStatus = 0x00;   // FTDI_IN_ENDPOINT_LINE_STATUS_RXFIFO = 0

  uint16_t USART_SR = USARTx->SR;
  if ( USART_SR & ( 1 << 5 ) )    // RXNE
    Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_DR;

  if ( USART_SR & ( 1 << 3 ) )    // ORE
    Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_OE;

  if ( UART_Context->UseHardwareParity )
  {
    if ( USART_SR & ( 1 << 0 ) )  // PE
      Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_PE;
  }

  if ( FTDI_SET_DATA_PARITY_NONE != UART_Context->SoftwareParityType )
  {
    if ( UART_Context->SoftwareParityError )
      Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_PE;
  }

  // This bit is set by hardware when the LIN break is detected.
  // It is cleared by software (by writing it to 0).
  if ( USART_SR & ( 1 << 8 ) )    // LBD
  {
    Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_BI;
    USARTx->SR &= ~( USART_SR_LBD );
  }

  if ( USART_SR & ( 1 << 7 ) )    // TXE -- Transmit data register -- TDR
    Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_THRE;

  if ( USART_SR & ( 1 << 6 ) )    // TC -- Transmit Shift Register -- TSR
    Interface->LineStatus |= FTDI_IN_ENDPOINT_LINE_STATUS_TEMT;

  return 0;
}

/* -------------------------------------------------------------------------------------------------
 * USART IRQ Handler
 */
void FTDI_Emulator_BSP_IRQHandler( DMA_Channel_TypeDef* DMA_Channel, USART_TypeDef * USARTx );
void FTDI_Emulator_BSP_TXDMAHandler( DMA_Channel_TypeDef* DMA_Channel, USART_TypeDef * USARTx );
void FTDI_Emulator_BSP_RXDMAHandler( DMA_Channel_TypeDef* DMA_Channel, USART_TypeDef * USARTx );

#if ( BSP_USART1_USED > 0 )
void USART1_IRQHandler( void )
{
  FTDI_Emulator_BSP_IRQHandler( BSP_UART1_RXDMA_CH, USART1 );
}

void DMA1_Channel4_IRQHandler( void )
{
  FTDI_Emulator_BSP_TXDMAHandler( BSP_UART1_TXDMA_CH, USART1 );
}

void DMA1_Channel5_IRQHandler( void )
{
  FTDI_Emulator_BSP_RXDMAHandler( BSP_UART1_RXDMA_CH, USART1 );
}
#endif

#if ( BSP_USART2_USED > 0 )
void USART2_IRQHandler( void )
{
  FTDI_Emulator_BSP_IRQHandler( BSP_UART2_RXDMA_CH, USART2 );
}

void DMA1_Channel6_IRQHandler( void )
{
  FTDI_Emulator_BSP_RXDMAHandler( BSP_UART2_RXDMA_CH, USART2 );
}

void DMA1_Channel7_IRQHandler( void )
{
  FTDI_Emulator_BSP_TXDMAHandler( BSP_UART2_TXDMA_CH, USART2 );
}

#endif

#if ( BSP_USART3_USED > 0 )
void USART3_IRQHandler( void )
{
  FTDI_Emulator_BSP_IRQHandler( USART3 );
}
#endif

#if ( BSP_UART4_USED > 0 )
void UART4_IRQHandler( void )
{
  FTDI_Emulator_BSP_IRQHandler( UART4 );
}
#endif

/*
 * For example, the parity bit can be computed as follows,
 * assuming we are sending simple 4-bit values 1001.
 *
 * Even parity
 * A computes parity bit value: 1^0^0^1 = 0 ^ 0 = 0
 * A adds parity bit and sends: 1001-0
 *
 * B receives: 10010
 * B computes parity: 1^0^0^1^0 = 0
 * B reports correct transmission after observing expected even result.
 *
 * Odd parity
 * A computes parity bit value: 1^0^0^1 = 0 ^ 1 = 1
 * A adds parity bit and sends: 1001-1
 *
 * B receives: 10011
 * B computes overall parity: 1^0^0^1^1 = 1
 * B reports correct transmission after observing expected odd result.
 */
uint32_t FTDI_Emulator_BSP_BuildParity( uint8_t Data, uint32_t BitNum, uint32_t ParityType )
{
  uint32_t RetValue = 0;
  for ( uint32_t i = 0; i < BitNum; i++ )
  {
    if ( Data & ( 1 << i ) )
      RetValue ^= 1;
  }

  if ( FTDI_SET_DATA_PARITY_ODD == ParityType )
    RetValue ^= 1;

  return RetValue;
}

uint32_t FTDI_Emulator_BSP_ReceiveOverrun( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  uint32_t RetValue = UART_Context->ReceiveOverrun;
  UART_Context->ReceiveOverrun = 0;
  return RetValue;
}

int32_t FTDI_Emulator_BSP_CancelTx( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  // Disable Tx DMA
  UART_Context->TxFifo->readIndex = UART_Context->TxFifo->writeIndex;
  return 0;
}

int32_t FTDI_Emulator_BSP_CancelRx( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  UART_Context->RxFifo->readIndex = UART_Context->RxFifo->writeIndex;
  return 0;
}

#define PARITY_BIT_SPACE    ( 0 )
#define PARITY_BIT_MARk     ( 1 )
#define PARITY_BIT_SKIP     ( 2 )

void FTDI_Emulator_BSP_AfterReceived( FTDI_InterfaceContext * Interface, uint32_t RxSize )
{
  uint8_t ParityBit;
  uint16_t UART_DR;
  uint32_t Index, Writable, Written;

  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  uint32_t RxBufferIndex = ( UART_Context->RxBufferIndex - 1 ) & 1;
  uint32_t RxByteIndex = 0;

  while ( RxSize )
  {
    Writable = FIFO_Writable( UART_Context->RxFifo, &Index );
    if ( 0 == Writable )
    {
      UART_Context->ReceiveOverrun = 1;
      return;
    }

    if ( RxSize > Writable )
      Written = Writable;
    else
      Written = RxSize;

    for ( uint32_t i = 0; i < Written; i++ )
    {
      UART_DR = UART_Context->RxBuffers[ RxBufferIndex ][ RxByteIndex ];
      RxByteIndex++;

      // When receiving with the parity enabled, the value read in the MSB bit is the received parity bit.
      // Parity bit in DR[7] or DR[8]
      if ( UART_Information->UART->CR1 & USART_CR1_M )
      {
        // 1 Start bit, 9 Data bits, n Stop bit : 8O1, 8E1, 8S1, 8M1
        // ToDo
        ParityBit = 0;
        (void) ParityBit;
      }
      else
      {
        // 1 Start bit, 8 Data bits, n Stop bit : 8N1, 7X1
        // ToDo
        ParityBit = 0;
        (void) ParityBit;
      }

      UART_Context->RxFifo->memory[ Index + i ] = UART_DR;
    }
    FIFO_Update( UART_Context->RxFifo, Written );
    Interface->OnRxCompleted( Interface );  // Notify to Read from RxFifo

    RxSize -= Written;
  }
}

uint32_t FTDI_Emulator_BSP_BeforeTransmit( FTDI_InterfaceContext * Interface )
{
  uint8_t ParityBit;
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  uint16_t UART_CR1 = UART_Information->UART->CR1;

  uint32_t Index, TransmitSize = FIFO_Readable( UART_Context->TxFifo, &Index );
  if ( 0 == TransmitSize )
    return 0;

  UART_Context->TransmitSize = TransmitSize;

  // We want to transmit TxFiFo directly with MemorySize=Byte
  UART_Information->TxDMA->CMAR = (uint32_t) ( UART_Context->TxFifo->memory + Index );
  UART_Information->TxDMA->CCR &= ~DMA_CCR1_MSIZE;  // DMA_MemoryDataSize_Byte

  // 7N2, 8N1
  if ( FTDI_SET_DATA_PARITY_NONE == UART_Context->SoftwareParityType )
  {
    if ( FTDI_SET_DATA_DATA_BITS_7 == Interface->LineCoding.DataBits )
      ParityBit = PARITY_BIT_MARk;  // 7N2
    else
      ParityBit = PARITY_BIT_SKIP;  // 8N1
  }
  // 7S1 or 7M1, 8S1 or 8M1
  else
  {
    // 8S1 or 8M1 : 1 Start bit, 9 Data bits, n Stop bit
    // 8S1 or 8M1 : Copy from TxFifo to TxBuffer[ x ] and build parity
    if ( UART_CR1 & USART_CR1_M )
    {
      if ( FTDI_SET_DATA_PARITY_SPACE == UART_Context->SoftwareParityType )
        ParityBit = PARITY_BIT_SPACE;
      else
        ParityBit = PARITY_BIT_MARk;

      TransmitSize = ( TransmitSize > UART_TX_BUFFER_WORD_SIZE ) ? UART_TX_BUFFER_WORD_SIZE : TransmitSize;
      for ( uint32_t i = 0; i < TransmitSize; i++ )
      {
        UART_Context->TxBuffer[ i * 2 + 0 ] = UART_Context->TxFifo->memory[ Index + i ];
        UART_Context->TxBuffer[ i * 2 + 1 ] = ParityBit;
      }

      FIFO_Flush( UART_Context->TxFifo, TransmitSize );   // TxFifo can be writen now
      UART_Context->TransmitSize = 0;                       // Need not Flush TxFifo After DMA Done

      UART_Information->TxDMA->CCR |= DMA_MemoryDataSize_HalfWord;
      UART_Information->TxDMA->CMAR = (uint32_t) ( &UART_Context->TxBuffer[ 0 ] );
      ParityBit = PARITY_BIT_SKIP;
    }
    else
    {
      // 7S1 or 7M1 : 1 Start bit, 8 Data bits, n Stop bit
      if ( FTDI_SET_DATA_PARITY_SPACE == UART_Context->SoftwareParityType )
        ParityBit = PARITY_BIT_SPACE;     // 7S1
      else
        ParityBit = PARITY_BIT_MARk;      // 7M1
    }
  }

  if ( PARITY_BIT_SKIP != ParityBit )
  {
    for ( uint32_t i = 0; i < TransmitSize; i++ )
    {
      if ( PARITY_BIT_MARk == ParityBit )
        UART_Context->TxFifo->memory[ Index + i ] |= 0x80;
      else
        UART_Context->TxFifo->memory[ Index + i ] &= 0x7F;
    }
  }

  return TransmitSize;
}

void FTDI_Emulator_BSP_StartTransmit( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;

  // FTDI_Emulator_BSP_TXDMAHandler() : DMA_CCR1_EN = 0 : TransferDone
  // EP2_OUT_Callback() : DMA_CCR1_EN = 1 : Transferring
  // EP2_OUT_Callback() : DMA_CCR1_EN = 0 : TransferIdle
  if ( 0 == UART_Context->Transmitting )
  {
    uint32_t TransmitSize = FTDI_Emulator_BSP_BeforeTransmit( Interface );

    // MemorySize and MemoryAddr are set
    if ( TransmitSize )
    {
      if ( 1 )
      {
        // ToDo : Hanlder FlowControl for XON/XOFF, RTS/CTS, DTR/DSR
        UART_Information->TxDMA->CNDTR = TransmitSize;   // DMA Transfer Times
        UART_Information->TxDMA->CCR |= DMA_CCR1_EN;
        UART_Context->Transmitting = 1;
      }
      else
      {
        uint8_t * Buffer = (uint8_t *) ( UART_Information->TxDMA->CMAR );
        USART_TypeDef * UART = UART_Information->UART;
        for ( uint32_t i = 0; i < TransmitSize; i++ )
        {
          while ( 0 == ( UART->SR & USART_SR_TXE ) )
            ;
          UART->DR = Buffer[ i ];
        }
      }
    }
  }
}

// Called by FTDI_Emulator_OnTxCompleted()
// Write more byte to TxFifo and Start Transmit
int32_t FTDI_Emulator_BSP_Transmit( FTDI_InterfaceContext * Interface, uint8_t * Buffer, uint32_t Transferred )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  Transferred = FIFO_Write( UART_Context->TxFifo, Buffer, Transferred );
  FTDI_Emulator_BSP_StartTransmit( Interface );
  return Transferred;
}

int32_t FTDI_Emulator_BSP_Receive( FTDI_InterfaceContext * Interface, uint8_t * Buffer, uint32_t Size )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  Size = FIFO_Read( UART_Context->RxFifo, Buffer, Size );
  return Size;
}

void FTDI_Emulator_BSP_StartReceive( FTDI_InterfaceContext * Interface )
{
  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  uint32_t RxBufferIndex = UART_Context->RxBufferIndex & 1;
  UART_Information->RxDMA->CNDTR = UART_RX_BUFFER_WORD_SIZE;  // DMA Transfer Times
  UART_Information->RxDMA->CMAR = (uint32_t) ( &UART_Context->RxBuffers[ RxBufferIndex ][ 0 ] );
  UART_Information->RxDMA->CCR |= DMA_CCR1_EN;
}

void FTDI_Emulator_BSP_RXDMAHandler( DMA_Channel_TypeDef * DMA_Channel, USART_TypeDef * USARTx )
{
  FTDI_InterfaceContext * Interface;
  for ( uint32_t i = 0; i < 4; i++ )
  {
    if ( BSP_UART_ARRAY[ i ]->Infromation->UART == USARTx )
    {
      Interface = BSP_UART_ARRAY[ i ]->Interface;
      break;
    }
  }

  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  UART_Information->RxDMA->CCR &= ~DMA_CCR1_EN;         // RxDMA Disabled, then Clear All Flags
  DMA(UART_Information->RxDMA )->IFCR = DMA_IFCR_CGIF1 << DMA_SHIFT( UART_Information->RxDMA );
  uint32_t DMA_CNDTR = UART_Information->RxDMA->CNDTR;  // Save Counter

  UART_Context->RxBufferIndex++;                        // Newer Buffer will be filled
  FTDI_Emulator_BSP_StartReceive( Interface );          // Older Buffer will be handled
  // UART_Information->RxDMA->CCR |= DMA_CCR1_EN;

  uint32_t RxSize = UART_RX_BUFFER_WORD_SIZE - DMA_CNDTR;

  // Now RxDMA is enabled and RxDMA has higher interrup priority
  // ISR can be interrupted here
  FTDI_Emulator_BSP_AfterReceived( Interface, RxSize );
}

/*
 * DMA Error management
 * A DMA transfer error can be generated by reading from or writing to a reserved address space.
 * When a DMA transfer error occurs during a DMA read or a write access, the faulty channel
 * is automatically disabled through a hardware clear of its EN bit in the corresponding Channel
 * configuration register (DMA_CCRx).
 *
 * The channel's transfer error interrupt flag (TEIF) in the DMA_IFR register is set and an interrupt
 * is generated if the transfer error interrupt enable bit (TEIE) in the DMA_CCRx register is set.
 */
void FTDI_Emulator_BSP_TXDMAHandler( DMA_Channel_TypeDef * DMA_Channel, USART_TypeDef * USARTx )
{
  // 1) Last But One Byte has been written to USART->TDR
  // 2) Last But One Byte in USART->DR has been transferred to USART->TSR
  // 3) USART->SR.TXE=1, DMA Tx request triggered : TXE=1 and TC=0
  // 4) DMA Tx Last Byte to USART->DR : TXE=0 and TC=0
  // 5) Last Byte in USART->DR has been transferred to Shift Register
  // 6) USART->SR.TXE=1, DMA Tx request triggered : TXE=1 and TC=0
  // DMA Tx request will be triggered again after USART->SR.TXE=1
  //
  // NOW TXE=0 and TC=0
  FTDI_InterfaceContext * Interface;
  for ( uint32_t i = 0; i < 4; i++ )
  {
    if ( BSP_UART_ARRAY[ i ]->Infromation->UART == USARTx )
    {
      Interface = BSP_UART_ARRAY[ i ]->Interface;
      break;
    }
  }

  BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
  const BSP_UART_Information * UART_Information = UART_Context->Infromation;
  UART_Information->TxDMA->CCR &= ~DMA_CCR1_EN;         // RxDMA Disabled, then Clear All Flags
  DMA(UART_Information->TxDMA )->IFCR = DMA_IFCR_CGIF1 << DMA_SHIFT( UART_Information->TxDMA );
  UART_Context->Transmitting = 0;

  // Flush TxFifo, Call OnTxCompleted() to fill TxFifo and Send again
  FIFO_Flush( UART_Context->TxFifo, UART_Context->TransmitSize );  // - CNTR

  if ( Interface->OnTxCompleted( Interface ) )
    FTDI_Emulator_BSP_StartTransmit( Interface );
}

/*
 * Switch RXDMA RxBuffer Time -- 2 Byte @ 3Mbps = 3.33 uS * 2 = 6.66 uS
 * RxPin ---> RSR ---> RDR
 *
 * SystemCoreClock - 48MHz, T=0.021 uS
 * Instruction count = 6.66 / 0.021 = 320
 *
 */
void FTDI_Emulator_BSP_IRQHandler( DMA_Channel_TypeDef * DMA_Channel, USART_TypeDef * USARTx )
{
  FTDI_InterfaceContext * Interface;

  // IDLE: IDLE line detected
  // This bit is set by hardware when an Idle Line is detected.
  // The IDLE bit will not be set again until the RXNE bit has been set itself
  // (i.e. a new idle line occurs).
  // It is cleared by a software sequence (an read to the USART_SR register
  // followed by a read to the USART_DR register).
  uint16_t USART_SR = USARTx->SR;   // Clear TC
  uint16_t USART_DR = USARTx->DR;   // Clear RXNE and IDLE, IDLE can be set again

  for ( uint32_t i = 0; i < 4; i++ )
  {
    if ( BSP_UART_ARRAY[ i ]->Infromation->UART == USARTx )
    {
      Interface = BSP_UART_ARRAY[ i ]->Interface;
      break;
    }
  }

  if ( USART_SR & USART_SR_IDLE )
  {
    BSP_UART_Context * UART_Context = (BSP_UART_Context *) ( Interface->UART_Context );
    const BSP_UART_Information * UART_Information = UART_Context->Infromation;

    FTDI_Emulator_BSP_RXDMAHandler( UART_Information->RxDMA, USARTx );
  }
}
