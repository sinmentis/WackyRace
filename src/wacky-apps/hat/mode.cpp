#include "mode.hpp"


// PIO Config
Wacky::Pio Pio_control (PA6_PIO);
Wacky::Pio Pio_sleep (PA2_PIO);

// LED Config
Wacky::Led led_blue (LED1_PIO, Wacky::ActiveHigh); // Blue

void init_pio()
{
	Pio_control.setMode (PIO_PULLUP);
	Pio_sleep.setMode (PIO_PULLUP);
	led_blue = 0;
}

// BLUE toggle for Joy
int read_control_pio()
{
	if (Pio_control.read() == 1)
	{
		led_blue.toggle();
	} else {
		led_blue = 1;
	}
	
	return Pio_control.read();
}

int read_sleep_pio()
{
	return Pio_sleep.read();
}
