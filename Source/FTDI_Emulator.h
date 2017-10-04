/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FTDI_EMULATOR_H
#define __FTDI_EMULATOR_H

#include "FTDI_Type.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define FTDI_Emulator_GetConfiguration          NOP_Process
#define FTDI_Emulator_GetInterface              NOP_Process
#define FTDI_Emulator_SetInterface              NOP_Process
#define FTDI_Emulator_GetStatus                 NOP_Process
#define FTDI_Emulator_ClearFeature              NOP_Process
#define FTDI_Emulator_SetEndPointFeature        NOP_Process
#define FTDI_Emulator_SetDeviceFeature          NOP_Process
//#define FTDI_Emulator_SetDeviceAddress          NOP_Process
//#define FTDI_Emulator_SetConfiguration          NOP_Process

/* Exported functions ------------------------------------------------------- */
void FTDI_Emulator_Init( void );
void FTDI_Emulator_Reset(void);

void FTDI_Emulator_SetConfiguration(void);
void FTDI_Emulator_SetDeviceAddress (void);

RESULT FTDI_Emulator_NoData_Setup( uint8_t );
RESULT FTDI_Emulator_Data_Setup( uint8_t );
void FTDI_Emulator_Status_In (void);
void FTDI_Emulator_Status_Out (void);

RESULT FTDI_Emulator_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);

uint8_t *FTDI_Emulator_GetDeviceDescriptor(uint16_t );
uint8_t *FTDI_Emulator_GetConfigDescriptor(uint16_t);
uint8_t *FTDI_Emulator_GetStringDescriptor(uint16_t);

#endif /* __FTDI_EMULATOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

