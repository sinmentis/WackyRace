#include "gameboard.hpp"

Wacky::Pacer pacer (PACER_RATE_gameboard);
Wacky::Pio pio1 (PA9_PIO);   

void gameBoardInit(Wacky::UsartSerial &serial, Wacky::UsbSerial &usbserial)  
{  
	mcu_reset_disable ();
    serial.begin (Wacky::UsartSerial::USART_1, 250000);
    pio1 = 0;
}

void play_sound()
{
	for(int i = 0; i < 200; i++) {
		pacer.wait();
		pio1.toggle();
	}
	
	for(int i = 0; i < 200; i++) {
		pacer.wait();
		pacer.wait();
		pio1.toggle();
	}
		
	for(int i = 0; i < 200; i++) {
		pacer.wait();
		pacer.wait();
		pacer.wait();
		pio1.toggle();
	}
	
	for(int i = 0; i < 300; i++) {
		pacer.wait();
		pacer.wait();
		pacer.wait();
		pacer.wait();
		pio1.toggle();
	}
}

void stop_play_sound()
{
	for(int i = 0; i < 200; i++) {
		pacer.wait();
		pacer.wait();
		pacer.wait();
		pacer.wait();
		pio1.toggle();
	}
	
	for(int i = 0; i < 200; i++) {
		pacer.wait();
		pacer.wait();
		pacer.wait();
		pio1.toggle();
	}
	
	for(int i = 0; i < 200; i++) {
		pacer.wait();
		pacer.wait();
		pio1.toggle();
	}
		
	for(int i = 0; i < 300; i++) {
		pacer.wait();
		pio1.toggle();
	}

}
	
	
void gameboard_task (Wacky::UsartSerial& serial,
                            Wacky::UsbSerial& usbserial)
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
        response = "hat";
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
