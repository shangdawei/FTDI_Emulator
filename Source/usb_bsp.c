#include "usb_bsp.h"
#include "stm32_it.h"
#include "usb_lib.h"
#include "usb_pwr.h"

EXTI_InitTypeDef EXTI_InitStructure;

/*******************************************************************************
 * Function Name  : Set_System
 * Description    : Configures Main system clocks & power
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Set_System( void )
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /*!< At this stage the microcontroller clock setting is already configured,
   this is done through SystemInit() function which is called from startup
   file (startup_stm32f10x_xx.s) before to branch to application main.
   To reconfigure the default setting of SystemInit() function, refer to
   system_stm32f10x.c file
   */

  /* 4 bit for pre-emption priority, 0 bits for subpriority */
  NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIO_DISCONNECT, ENABLE );

  /* Configure USB pull-up pin */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init( USB_DISCONNECT, &GPIO_InitStructure );

  /* Configure the EXTI line 18 connected internally to the USB IP */
  EXTI_ClearITPendingBit( EXTI_Line18 );
  EXTI_InitStructure.EXTI_Line = EXTI_Line18;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init( &EXTI_InitStructure );
}

/*******************************************************************************
 * Function Name  : Set_USBClock
 * Description    : Configures USB Clock input (48MHz)
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Set_USBClock( void )
{
  /* Select USBCLK source */
  RCC_USBCLKConfig( RCC_USBCLKSource_PLLCLK_Div1 );

  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USB, ENABLE );
}

/*******************************************************************************
 * Function Name  : Enter_LowPowerMode
 * Description    : Power-off system clocks and power while entering suspend mode
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Enter_LowPowerMode( void )
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
 * Function Name  : Leave_LowPowerMode
 * Description    : Restores system clocks and power while exiting suspend mode
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Leave_LowPowerMode( void )
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if ( pInfo->Current_Configuration != 0 )
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
  /*Enable SystemCoreClock*/
  SystemInit( );
}

/*******************************************************************************
 * Function Name  : USB_Interrupts_Config
 * Description    : Configures the USB interrupts
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void USB_Interrupts_Config( void )
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 4 bit for pre-emption priority, 0 bits for subpriority */
  // Set_System()
  //    NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // Higher Priority
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init( &NVIC_InitStructure );

  /* USART NVIC_IRQChannelPreemptionPriority = 1 */

  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // Highest Priority
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init( &NVIC_InitStructure );
}

/*******************************************************************************
 * Function Name  : USB_Cable_Config
 * Description    : Software Connection/Disconnection of USB Cable
 * Input          : None.
 * Return         : Status
 *******************************************************************************/
void USB_Cable_Config( FunctionalState NewState )
{
  if ( NewState != DISABLE )
  {
    GPIO_SetBits( USB_DISCONNECT, USB_DISCONNECT_PIN );
  }
  else
  {
    GPIO_ResetBits( USB_DISCONNECT, USB_DISCONNECT_PIN );
  }
}

