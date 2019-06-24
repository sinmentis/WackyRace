/* File:   pio_test1.c
   Author: M. P. Hayes, UCECE
   Date:   3 May 2018
   Descr: 
*/
#include "Pio.hpp"
#include "Pacer.hpp"
#include "mcu.h"

#define PACER_RATE 2000

int main ()
{
    Wacky::Pio pio1 (PA9_PIO);   
    Wacky::Pacer pacer (PACER_RATE);    	

    /* This is required if PB4 or PB5 is used since they default
       to JTAG pins.   */
    //mcu_jtag_disable ();

    pio1.setMode (PIO_OUTPUT_HIGH);
    int count = 0;
    while (1)
    {
        pacer.wait();
        count += 1;
        if (count %2000 < 500)
        {
			pio1 = 1;
        } else {
			pio1 = 0;
		}
    }
}
