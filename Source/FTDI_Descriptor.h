/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

#include <stdint.h>

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05
#define STANDARD_ENDPOINT_DESC_SIZE             0x09

extern uint8_t FTDI_Emulator_StringManufacturer[ 256 ];
extern uint8_t FTDI_Emulator_StringProduct[ 256 ];
extern uint8_t FTDI_Emulator_StringSerialNumber[ 256 ];
extern USB_DEVICE_DESCRIPTOR FTDI_Emulator_DeviceDescriptor;
extern uint8_t FTDI_Emulator_ConfigDescriptor[ 256 ];

extern const uint8_t FTDI_Emulator_StringLangID[ 4 ];

extern const uint8_t FTDI_FT230X_ConfigDescriptor[ 32 ];
extern const uint8_t FTDI_FT230X_DeviceDescriptor[ 18 ];

extern const uint8_t FTDI_FT232R_ConfigDescriptor[ 32 ];
extern const uint8_t FTDI_FT232R_DeviceDescriptor[ 18 ];

extern const uint8_t FTDI_FT245R_ConfigDescriptor[ 32 ];
extern const uint8_t FTDI_FT245R_DeviceDescriptor[ 18 ];

extern const uint8_t FTDI_FT232H_ConfigDescriptor[ 32 ];
extern const uint8_t FTDI_FT232H_DeviceDescriptor[ 18 ];

extern const uint8_t FTDI_FT2232C_ConfigDescriptor[ 55 ];
extern const uint8_t FTDI_FT2232C_DeviceDescriptor[ 18 ];

extern const uint8_t FTDI_FT2232H_ConfigDescriptor[ 55 ];
extern const uint8_t FTDI_FT2232H_DeviceDescriptor[ 18 ];

extern const uint8_t FTDI_FT4232H_ConfigDescriptor[ 101 ];
extern const uint8_t FTDI_FT4232H_DeviceDescriptor[ 18 ];



#endif /* __USB_DESC_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
