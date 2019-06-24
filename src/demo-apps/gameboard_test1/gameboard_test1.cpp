#include "UsartSerial.hpp"
#include "UartSerial.hpp"
#include "UsbSerial.hpp"
#include "Pacer.hpp"
#include "mcu.h"
#include "Pio.hpp"
#include <stdio.h>

#define PACER_RATE 1000
#define LINE_SIZE 80
#define DEBUG 1
#define ADC_BUFFER_SIZE 4

Wacky::UsartSerial serial;
Wacky::UsbSerial usbserial;
Wacky::Pacer pacer (PACER_RATE);

void gameBoardInit()  {serial.begin(Wacky::UsartSerial::USART_1, 250000);}

void play_sound()
{
	
}

void stop_play_sound()
{
	
}
	
	
static void gameboard_task (Wacky::UsartSerial &serial,
                            Wacky::UsbSerial &usbserial)
{
    char buffer[LINE_SIZE];
    const char *response = "";   

    if (! serial.readln (buffer, sizeof (buffer)))
        return;

    if (DEBUG)
    {
        usbserial.print ("<");
        usbserial.print (buffer);
    }
    
    switch (buffer[0])
    {
    default:
        break;
	case 'U':
        response = "300";
        break;
    case 'M':
        response = "comp";
        break;
    case 'N':
        response = "7";
        break;
   	case 'B':
        response = "racer";
        break;
    case 'K':
		response = "";
		play_sound();
		break;
    case 'R':
		response = "";
		stop_play_sound();
		break;
	case 'L':
		switch (buffer[1])
		{
			default:
				break;
			
			case '0':
				response = "<FF0000>";
				break;
			case '1':
				response = "<00FF00>";
				break;
			case '2':
				response = "<0000FF>";
				break;
		}
		break;
    case 'G':
        response = "Group 7";
        break;
    case 'S':
        response = "Shun Lyu, Robert Anderson, Ke Gao, Yat Chung LEUNG";
        break;
    case 'V':
		response = "1";
		break;
	case 'I':
		response = "1";
		break;
	case 'Q':
		response = "1";
		break;
    }
    
    serial.println (response);
    
    if (DEBUG)
    {
        usbserial.print (">");
        usbserial.println (response);
    }
}


int main (void)
{
    
    // Disable the reset pin since when the game board powers up it
    // inadvertently toggles the PIO pin connected to the SAM4S reset
    // input.
    mcu_reset_disable ();
    
    usbserial.begin ();

    serial.begin (Wacky::UsartSerial::USART_1, 250000);

    while (1)
    {
        pacer.wait ();

        gameboard_task (serial, usbserial);
    }
}
