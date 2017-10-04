#ifndef __USB_BSP_H
#define __USB_BSP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f10x.h"
#include "usb_type.h"

#define USB_DISCONNECT                      GPIOA
#define USB_DISCONNECT_PIN                  GPIO_Pin_8
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOA

void Set_System( void );
void Set_USBClock( void );
void Enter_LowPowerMode( void );
void Leave_LowPowerMode( void );
void USB_Interrupts_Config( void );
void USB_Cable_Config( FunctionalState NewState );

#ifdef __cplusplus
}
#endif

#endif /* __USB_BSP_H */
