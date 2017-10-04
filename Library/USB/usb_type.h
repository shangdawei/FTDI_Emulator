/**
 ******************************************************************************
 * @file    usb_type.h
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    28-August-2012
 * @brief   Type definitions used by the USB Library
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_TYPE_H
#define __USB_TYPE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef NULL
#define NULL ((void *)0)
#endif

/// USB Standard Device Descriptor
typedef struct _USB_DEVICE_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUSB;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint8_t iManufacturer;
  uint8_t iProduct;
  uint8_t iSerialNumber;
  uint8_t bNumConfigurations;
} USB_DEVICE_DESCRIPTOR;

/// USB 2.0 Device Qualifier Descriptor
typedef struct _USB_DEVICE_QUALIFIER_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUSB;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint8_t bNumConfigurations;
  uint8_t bReserved;
} USB_DEVICE_QUALIFIER_DESCRIPTOR;

/// USB Standard Configuration Descriptor
typedef struct _USB_CONFIGURATION_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t wTotalLength;
  uint8_t bNumInterfaces;
  uint8_t bConfigurationValue;
  uint8_t iConfiguration;
  uint8_t bmAttributes;
  uint8_t bMaxPower;
} USB_CONFIGURATION_DESCRIPTOR;

/// USB Standard Interface Descriptor
typedef struct _USB_INTERFACE_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;
  uint8_t bNumEndpoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
} USB_INTERFACE_DESCRIPTOR;

/// USB Standard Endpoint Descriptor
typedef struct _USB_ENDPOINT_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t bInterval;
} USB_ENDPOINT_DESCRIPTOR;

/// USB String Descriptor
typedef struct _USB_STRING_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bString;
} USB_STRING_DESCRIPTOR;

// USB Common Descriptor
typedef struct _USB_COMMON_DESCRIPTOR
{
  uint8_t bLength;
  uint8_t bDescriptorType;
} USB_COMMON_DESCRIPTOR;

typedef enum
{
  FALSE = 0,
  TRUE = !FALSE
} bool;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/

#endif /* __USB_TYPE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
