/* File:   serial_test1.c
   Author: M. P. Hayes, UCECE
   Date:   22 Feb 2018
   Descr: 
*/
#include "UsartSerial.hpp"

int main (void)
{
    Wacky::UsartSerial serial;

    serial.begin (Wacky::UsartSerial::USART_0);

    while (1)
    {
        serial.println ("Hello world");
    }
}
