/* File:   pio_test1.c
   Author: M. P. Hayes, UCECE
   Date:   3 May 2018
   Descr: 
*/
#include "Pio.hpp"
#include "Pacer.hpp"
#include "Led.hpp"

#define PACER_RATE 10

int main (void)
{
    Wacky::Pio pio1 (PA6_PIO); 		// IMU-JOY
    Wacky::Pio pio2 (PA2_PIO);      // Sleep mode     
    
    Wacky::Led led1 (LED1_PIO, Wacky::ActiveLow);
    Wacky::Led led2 (LED2_PIO, Wacky::ActiveLow);
      
    Wacky::Pacer pacer (PACER_RATE);    
    	
	bool control_mode;
	bool sleep_mode;
	 
    pio1.setMode (PIO_PULLUP);
    pio2.setMode (PIO_PULLUP);
    
    while (1)
    {
        pacer.wait ();
        control_mode = pio1.read();
        sleep_mode = pio2.read();
        if(control_mode) {
			led1 = 1;
		} else {
			led1 = 0;
		}
        if(sleep_mode) {
			led2 = 1;
		} else {
			led2 = 0;
		}    }
}
