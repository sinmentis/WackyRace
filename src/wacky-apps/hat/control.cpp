#include "control.hpp"


// MPU Config
Wacky::Mpu9250 imu;

// ADC Config
Wacky::Adc adc;

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

int8_t* old_control_joy_get ()
{      
	int8_t result[2];
	int16_t buffer[3];
	int bytes = adc.read (buffer, sizeof (buffer));
	result[0] = buffer[0]/ 22.78 - 90; 
	result[1] = buffer[1]/ 22.78 - 90; 
	return result;
}

int8_t* old_control_IMU_get ()
{      
	int8_t result[2];
	int16_t acceldata[3]; 	// IMU Buffer
	int16_t buffer[3];
	int bytes = adc.read (buffer, sizeof (buffer));
	imu.readAccel (acceldata);
	int16_t x = acceldata[0];
	int16_t y = acceldata[1];
	int16_t z = acceldata[2];
	result[0] = calc_angle(x, y, z); 	// yaw
	result[1] = calc_angle(y, x, z); 	// pitch
	return result;
}
