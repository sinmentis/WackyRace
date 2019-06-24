/** @file   racer.cpp
    @author M. P. Hayes, UCECE
    @date   12 February 2018
*/
#include "UsbSerial.hpp"
#include "racer.hpp"
#include "util.hpp"
#include "motor.hpp"
#include "Nrf24.hpp"
//#include "gameBoard.hpp"
#include "stdio.h"


#define MIN_ADC 3900
#define MAX_ADC 5459

int dutyLeft = 800;
int dutyRight = 800;
int dirLeft = 0;
int dirRight = 0;
char UARTBuffer[4];
Wacky::Pio LED2 (LED2_PIN);
Wacky::Pio LED1 (LED1_PIN); 
Wacky::Pio button (BUTTON_PIN);
Wacky::Pio battery (BATTERY_PIN);
Wacky::Pio USBDtct (USBDTCT_PIN);
Wacky::Pacer pacer (PACER_RATE);
Wacky::Nrf24 radio;  

Wacky::UsartSerial USARTserial;
Wacky::UsbSerial usbserial;

Wacky::Adc adc;
bool game_ready=0;
bool kill = 0;

void racerInit()
{  
	mcu_reset_disable();
    mcu_watchdog_enable();
	LED1.setMode(PIO_OUTPUT_HIGH);
    LED2.setMode(PIO_OUTPUT_LOW);
    ADCInit();
    motorinit();
    USARTserial.begin(Wacky::UsartSerial::USART_1, 250000);    
    usbserial.begin ();

}

static void gameboard_task (Wacky::UsartSerial &serial,
                            int16_t voltage)
{
    char buffer[4];
    const char *response = "";   
	char volt[10];

    if (! serial.readln (buffer, sizeof (buffer)))
        return;
    
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
		setMotor(0, 0);
		kill = 1;
		break;
    case 'R':
		response = "";
		kill = 0;
		break;
	case 'L':
		switch (buffer[1])
		{
			default:
				break;
			
			case '0':
				response = "<FF0000,00FF00,0000FF>";
				break;
			case '1':
				response = "<00FF00>";
				break;
			case '2':
				response = "<FF0000>";
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
    	game_ready = 1;
		sprintf(volt, "%d", voltage);
		response = volt;	        
	
		break;
	case 'I':
		response = "Racer";
		break;
	case 'Q':
		response = "Racer";
		break;
    }
    
    serial.println (response);
}

int16_t battery_return()
{
	int16_t result;
	int16_t buffer[1];
	int bytes = adc.read (buffer, sizeof (buffer));
	result = (buffer[0] - MIN_ADC)*1800/ (MAX_ADC-MIN_ADC)+5000;
	return result;
}

void battery_init()
{    
	// Detect the IMU
    adc.begin (BIT (ADC_CHANNEL_1));
}

int main (void)
{
	racerInit();
	radio.begin (77, 0xCCCECCCECC);    
    radio.listen ();
    int8_t buffer[2];
    buffer[0] = -1;
    buffer[1] = -1;
    
    // USB serial Config
	int16_t voltage;

	battery_init();
	int counter = 0;
    while (1)
    {
        pacer.wait();
        mcu_watchdog_reset();

        gameboard_task(USARTserial, voltage);
		if(game_ready == 1 && kill == 0){   	

			for(int i = 0; i<4; i++){
				voltage += battery_return() * 1;		// battery
			}        
			LED1.toggle();
			int a = readBattery();
			if (counter % 100 == 0){
				if (radio.read (buffer, sizeof (buffer)))
				{
					gameboard_task(USARTserial, voltage);
					radioControl(buffer[0],buffer[1]);			
					usbserial.print ("\nbuffer[0]: ");			
					usbserial.print (buffer[0]);			
					usbserial.print ("           buffer[1]: ");			
					usbserial.print (buffer[1]);

				}
			}
		}
		LED2.toggle();
		counter++;
    }
    return 0;
}

