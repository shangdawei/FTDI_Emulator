#include "usb_bsp.h"
#include "usb_lib.h"

__root uint8_t dummy[256];

int main(void)
{
  for ( uint32_t i=0; i<256; i++)
    dummy[i] = i;
  Set_System();
  Set_USBClock();

  USB_Init();
  USB_Interrupts_Config();

  while (1)
  {
  }
}
