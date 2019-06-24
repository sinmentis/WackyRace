#include "gameBoard.hpp"
#define PACER_RATE 1000
#define LINE_SIZE 80
#define DEBUG 0

#define ADC_BUFFER_SIZE 4


Wacky::UsartSerial serial;

void gameBoardInit()  {serial.begin(Wacky::UsartSerial::USART_1, 250000);}

void pollGameBoard ()
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
        // Board type (hat or racer)
    case 'B':
        response = "racer";
        break;
        // Group name
    case 'G':
        response = "Group 7";
        break;
        // Mode (test or comp)
    case 'M':
        response = "comp";
        break;        
        // Group number
    case 'N':
        response = "7";
        break;
        // Student names (comma separated)
    case 'S':
        response = "Shun Lyu, Robert Anderson, Ke Gao, Yat Chung LEUNG";
        break;
        // How much current (mA) we can supply over the 5 V rail.
        // This should depend on the source; the battery can supply
        // more than USB.
    case 'U':
        response = "300";
        break;
    case 'V':
		int V = readBattery();
		response = "300";
		break;
		
	case 'K':
		while(1)
		{setMotor(0,0);}
		response = "";
		break;
	case 'Q':
		response = "1";
		break;
    }
    serial.println (response);
}


