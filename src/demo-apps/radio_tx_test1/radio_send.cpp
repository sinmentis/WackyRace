/* File:   radio_send.c
   Author: 
   Date:   
   Descr: 
*/
#include "Nrf24.hpp"
#include "Pio.hpp"
#include "Pacer.hpp"
#include "stdio.h"


int main (void)
{
    Wacky::Pio led_blue (LED1_PIO); // blue
    Wacky::Pio led_red (LED2_PIO); // red
    Wacky::Pacer pacer(10);
    Wacky::Nrf24 radio;
    Wacky::Nrf24::status status;
    
    
    uint8_t count = 0;

    led_blue.setMode (PIO_OUTPUT_HIGH);
    led_red.setMode (PIO_OUTPUT_LOW); 
        
    status = radio.begin (77, 0xCCCECCCECC);
    if (status != Wacky::Nrf24::OK)
    {
        while (1)
        {
            pacer.wait ();
            led_blue = 1;
        }
	}
	
	led_blue = 0;
	
    while (1)
    {
        char buffer[32];
        
        pacer.wait ();

        sprintf (buffer, "Test #%d", count++);
        
        if (! radio.write (buffer, sizeof (buffer))) {
            led_red.toggle();  // no receiver
		} else {
			led_blue.toggle(); // Received
		}
    }
}
