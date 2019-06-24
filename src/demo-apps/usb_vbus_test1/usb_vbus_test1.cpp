/* File:   usb_vbus_test1.cpp
   Author: M. P. Hayes, UCECE
   Date:   8 Apr 2018
   Descr:  Drive an LED if USB VBUS detected
*/
#include "Led.hpp"
#include "Pacer.hpp"
#include "Pio.hpp"

#define PACER_RATE 10

int main (void)
{
    Wacky::Led led1 (LED1_PIO, Wacky::ActiveLow);
    Wacky::Led led2 (LED2_PIO, Wacky::ActiveLow);
    Wacky::Pio usb_vbus_pio (USB_VBUS_PIO);            
    Wacky::Pacer pacer (PACER_RATE);    

    led1 = 1;    
    led2 = 0;

    usb_vbus_pio.setMode (PIO_INPUT);
    
    while (1)
    {
        pacer.wait ();

        led2 = usb_vbus_pio.read ();

        led1.toggle ();
    }
}
