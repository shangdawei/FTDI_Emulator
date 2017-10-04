/* Includes ------------------------------------------------------------------*/
#include "usb_type.h"
#include "FTDI_Descriptor.h"

USB_DEVICE_DESCRIPTOR FTDI_Emulator_DeviceDescriptor;
uint8_t FTDI_Emulator_ConfigDescriptor[ 256 ];

/* Manufacturer: "FTDI" */
uint8_t FTDI_Emulator_StringManufacturer[ 256 ] =
{
  10, 0x03, 'F', 0, 'T', 0, 'D', 0, 'I', 0 };

/* Product name: "USB <-> Serial Cable" */
uint8_t FTDI_Emulator_StringProduct[ 256 ] =
{
  42, 0x03, 'U', 0, 'S', 0, 'B', 0, ' ', 0, '<', 0, '-', 0, '>', 0, ' ', 0, 'S', 0, 'e', 0, 'r', 0, 'i', 0, 'a', 0, 'l',
  0, ' ', 0, 'C', 0, 'a', 0, 'b', 0, 'l', 0, 'e', 0 };

/* Serial Number: "FT12345678" */
uint8_t FTDI_Emulator_StringSerialNumber[ 256 ] =
{
  22, 0x03, 'F', 0, 'T', 0, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0 };

/* USB String LangID Descriptors */
const uint8_t FTDI_Emulator_StringLangID[ 4 ] =
{
  4, 0x03, 0x09, 0x04 /* LangID = 0x0409: U.S. English */
};

const uint8_t FTDI_FT2232C_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x08, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x10, 0x60, /* idProduct = 0x6010: */
  0x00, 0x05, /* bcdDevice = 5.00 : FT2232C */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT2232C_ConfigDescriptor[ 55 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  55, 0x00, /* wTotalLength:no of returned bytes */

  0x02, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x01, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x83, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x04, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */
};

const uint8_t FTDI_FT232R_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x08, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x01, 0x60, /* idProduct = 0x6001: */
  0x00, 0x06, /* bcdDevice = 6.00 : FT232R */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT232R_ConfigDescriptor[ 32 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  32, 0x00, /* wTotalLength:no of returned bytes */

  0x01, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x40, 0x00, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x40, 0x00, /* wMaxPacketSize: */
  0x00, /* bInterval */
};

const uint8_t FTDI_FT245R_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x08, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x01, 0x60, /* idProduct = 0x6001: */
  0x00, 0x06, /* bcdDevice = 6.00 : FT232R */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT245R_ConfigDescriptor[ 32 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  32, 0x00, /* wTotalLength:no of returned bytes */

  0x01, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x40, 0x00, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x40, 0x00, /* wMaxPacketSize: */
  0x00, /* bInterval */
};

const uint8_t FTDI_FT232H_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x40, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x14, 0x60, /* idProduct = 0x6014: */
  0x00, 0x09, /* bcdDevice = 9.00 : FT232H */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT232H_ConfigDescriptor[ 32 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  32, 0x00, /* wTotalLength:no of returned bytes */

  0x01, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */
};

const uint8_t FTDI_FT2232H_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x40, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x10, 0x60, /* idProduct = 0x6010: */
  0x00, 0x07, /* bcdDevice = 7.00 : FT2232C */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT2232H_ConfigDescriptor[ 55 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  55, 0x00, /* wTotalLength:no of returned bytes */

  0x02, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x01, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x83, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x04, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */
};

const uint8_t FTDI_FT4232H_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x40, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x11, 0x60, /* idProduct = 0x6011: */
  0x00, 0x08, /* bcdDevice = 8.00 : FT2232C */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT4232H_ConfigDescriptor[ 101 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  101, 0x00, /* wTotalLength:no of returned bytes */

  0x04, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x01, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x83, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x04, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x02, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x85, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x06, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x03, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x87, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x08, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x00, 0x02, /* wMaxPacketSize: */
  0x00, /* bInterval */
};

const uint8_t FTDI_FT230X_DeviceDescriptor[ 18 ] =
{
  0x12, /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE, /* bDescriptorType */
  0x00, 0x02, /* bcdUSB = 2.00 */

  0x00, /* bDeviceClass: This is an Interface Class Defined Device */
  0x00, /* bDeviceSubClass */
  0x00, /* bDeviceProtocol */

  0x08, /* bMaxPacketSize0 */

  0x03, 0x04, /* idVendor = 0x0403: Future Technology Devices International Limited */
  0x15, 0x60, /* idProduct = 0x6015: */
  0x00, 0x10, /* bcdDevice = 10.00 : FT230X */

  1, /* Index of string descriptor describing manufacturer */
  2, /* Index of string descriptor describing product */
  3, /* Index of string descriptor describing the device's serial number */

  0x01 /* bNumConfigurations */
};

const uint8_t FTDI_FT230X_ConfigDescriptor[ 32 ] =
{
  /*Configuration Descriptor*/
  0x09, /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */

  32, 0x00, /* wTotalLength:no of returned bytes */

  0x01, /* bNumInterfaces: 1 interface */
  0x01, /* bConfigurationValue: Configuration value */
  0x00, /* iConfiguration: Index of string descriptor describing the configuration */

  0x80, /* bmAttributes: Bus powered */
  0xFA, /* MaxPower 500 mA */

  /*Interface Descriptor*/
  0x09, /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00, /* bInterfaceNumber: Number of Interface */
  0x00, /* bAlternateSetting: Alternate setting */
  0x02, /* bNumEndpoints: 2 endpoints used */

  0xFF, /* bInterfaceClass: Vendor Specific Device Interface Class */
  0xFF, /* bInterfaceSubClass: */
  0xFF, /* bInterfaceProtocol: */

  0x02, /* iInterface: Index of string descriptor describing product */

  /*Endpoint 1 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x81, /* bEndpointAddress: (IN1) */
  0x02, /* bmAttributes: Bulk */
  0x40, 0x00, /* wMaxPacketSize: */
  0x00, /* bInterval */

  /*Endpoint 2 Descriptor*/
  0x07, /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: Endpoint */
  0x02, /* bEndpointAddress: (OUT2) */
  0x02, /* bmAttributes: Bulk */
  0x40, 0x00, /* wMaxPacketSize: */
  0x00, /* bInterval */
};
