/* File:   twi_test1.c
   Author: M. P. Hayes, UCECE
   Date:   28 Feb 2018
   Descr: 
*/
#include "UsbSerial.hpp"
#include "Twi.hpp"
#include "stdio.h"
#include "Pio.hpp"

int main (void)
{
    Wacky::UsbSerial usbserial;
    Wacky::Twi twi;

    usbserial.begin ();
    usbserial.print ("> ");
        
    while (1)
    {
        char buffer[80];
        int speed;

        if (!usbserial.readln (buffer, sizeof (buffer)))
            continue;
            
        if (sscanf (buffer, "%d", &speed) != 1)
            continue;

        // TODO
        
        usbserial.print ("> ");        
    }
}
