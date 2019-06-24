#include "UsbSerial.hpp"
#include "UsartSerial.hpp"
#include "UartSerial.hpp"
#include "stdio.h"
#include "mcu_sleep.h"
#include "Led.hpp"
#include "Pacer.hpp"
#include "Pio.hpp"
#include "Mpu9250.hpp"
#include "Adc.hpp"
#include <math.h>
#include "rf.hpp"
#include "mode.hpp"
#include "battery.hpp"
#include "gameboard.hpp"

#define PACER_RATE 2000
#define USB_DEBUG 0
#define PI 3.1415926
#define MIN_ADC 3900
#define MAX_ADC 5459

bool gameboard_ready = false;
int8_t RF_buffer[2];

// ADC Config
Wacky::Adc adc;
Wacky::Mpu9250 imu;

/* LED Indicators
 * Red on - voltage below 5V
 * Red flash for RF detection not working
 * Blue on for no receiver for RF - off when packed received
 */


double_t calc_angle(int16_t a,int16_t b,int16_t c)
{
	double_t result;
	double_t equation;
	
	equation = a/sqrt(b*b+c*c);
	result = atan(equation)*405/PI;
	
	if (result >= 90) { result = 90;}
	else if (result <= -90) { result = -90;}
	
	return result; 
}

int16_t battery_return()
{
	int16_t result;
	int16_t buffer[3];
	int bytes = adc.read (buffer, sizeof (buffer));
	result = (buffer[2] - MIN_ADC)*1800/ (MAX_ADC-MIN_ADC)+5000;
	return result;
}

void IMU_detection()
{    
	// Detect the IMU
	while (!imu.begin (Wacky::Mpu9250::MPU_0)){} 
    adc.begin (BIT (ADC_CHANNEL_1) | BIT (ADC_CHANNEL_2) | BIT (ADC_CHANNEL_3));
}

void control_joy_get ()
{      
	int8_t result[2];
	int16_t buffer[3];
	int bytes = adc.read (buffer, sizeof (buffer));
	RF_buffer[0] = (buffer[0]/ 22.78 - 90) * 0.5; 
	RF_buffer[1] = buffer[1]/ 22.78 - 90; 
}

void control_IMU_get ()
{      
	int8_t result[2];
	int16_t acceldata[3]; 	// IMU Buffer
	int16_t buffer[3];
	int bytes = adc.read (buffer, sizeof (buffer));
	imu.readAccel (acceldata);
	int16_t x = acceldata[0];
	int16_t y = acceldata[1];
	int16_t z = acceldata[2];
	RF_buffer[1] = calc_angle(x, y, z); 	// yaw
	RF_buffer[0] = (calc_angle(y, x, z)*0.5); 	// pitch
}

static void gameboard_task (Wacky::UsartSerial &serial,
                            Wacky::UsbSerial &usbserial,
                            int16_t voltage)
{
    char buffer[LINE_SIZE];
    const char *response = "";   
	char volt[10];

    if (! serial.readln (buffer, sizeof (buffer)))
        return;

    if (USB_DEBUG)
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
		gameboard_ready = true;
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
    
    if (USB_DEBUG)
    {
        usbserial.print (">");
        usbserial.println (response);
    }
}



int main (void)
{

    /*################################################################*/
    /*################              Define            ################*/
    /*################################################################*/
    Wacky::Pacer pacer (PACER_RATE);  
    
    // USB serial Config
    Wacky::UsbSerial usbserial;
    Wacky::UsartSerial serial;
	
    // Switch mode variables
	bool control_mode;
	bool sleep_mode;

	// LEDs
	Wacky::Led led_red (LED2_PIO, Wacky::ActiveHigh); // Red
	Wacky::Led led_blue (LED1_PIO, Wacky::ActiveHigh); // blue
	led_blue = 0;
	led_red = 0;
	
	// loop counter
	int counter = 0;

    /*################################################################*/
    /*################              INIT              ################*/
    /*################################################################*/
    // Gameboard 
    Wacky::Pio pio1 (PA9_PIO); 
	pio1.setMode (PIO_OUTPUT_HIGH);
	pio1 = 0;
    mcu_reset_disable ();
    if(USB_DEBUG){usbserial.begin ();}
    serial.begin (Wacky::UsartSerial::USART_1, 250000);
        
    // battery reading
    int16_t voltage;
    
    // PIO init
    init_pio();
    
    // both LED toggle if IMU is fucked
    IMU_detection();
    
    // RF is fucked
	while(rf_detection() == 0){
		pacer.wait();
		led_red.toggle();
	}
	

    /*################################################################*/
    /*################              MAIN              ################*/
    /*################################################################*/
    while (1)
    {
        pacer.wait ();
        pio1 = 0;
		gameboard_task(serial, usbserial, voltage);
		if (gameboard_ready){
			// Read PA6_PIO
			control_mode = read_control_pio();

			// fill in RF_buffer based on current mode
			if(control_mode==0) {
				control_joy_get();
			} else if(control_mode==1) {
				control_IMU_get();
			}
			
			// Flash LEDs if voltage below 5V
			for(int i = 0; i<4; i++){
				voltage += battery_return() * 1;		// battery
			}
			voltage = voltage / 4;
			if(voltage < 5000){
				led_red = 1;
			} else{led_red = 0;}
			
			// Send through RF
			
			if(counter % 100 == 0) {
				if(rf_send_buffer(RF_buffer)==0){
					led_blue.toggle();
				}
			}
			counter += 1;
		
			// Print PC_Serial
			if(USB_DEBUG){
				if(gameboard_ready && (counter % 50 == 0)) {
					usbserial.print ("\nMode: ");
					usbserial.print (control_mode);
					usbserial.print (", buffer[0]: ");
					usbserial.print (RF_buffer[0]);
					usbserial.print (",buffer[1]: ");        
					usbserial.println (RF_buffer[1]);
					usbserial.print (",Battery: ");   
					usbserial.println (voltage); 
				}
			}
		}
    }
}
