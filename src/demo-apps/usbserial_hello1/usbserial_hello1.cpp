/* File:   usbserial_hello1.c
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
        usbserial.println ("Hello world");
    }
}
