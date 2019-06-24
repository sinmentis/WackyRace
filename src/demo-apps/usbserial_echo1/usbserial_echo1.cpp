/* File:   usbserial_echo1.c
   Author: M. P. Hayes, UCECE
   Date:   22 Feb 2018
   Descr: 
*/
#include "UsbSerial.hpp"

int main (void)
{
    Wacky::UsbSerial usbserial;

    usbserial.begin ();    

    while (1)
    {
        char buffer[20];
        
        if (! usbserial.readln (buffer, sizeof (buffer)))
            continue;

        usbserial.print ("FOO ");        
        usbserial.println (buffer);
    }
}
