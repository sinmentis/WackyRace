#include "battery.hpp"

Wacky::Adc batteryADC;

void battery_Init() 
{
	batteryADC.begin(BIT(ADC_CHANNEL_3));
}

int16_t GetAve(int16_t* buffer)
{
	int16_t temp = 0;
	for(int ii = 0;ii<4;ii++)
	{
		temp+=buffer[ii];
	}
	return temp/4;
}

int16_t readBattery()
{
	/*
	int16_t ADCBuffer[4];
	int16_t result;
	batteryADC.read(ADCBuffer,ADC_BUFFER_SIZE);
	int16_t BatteryValue = GetAve(ADCBuffer);
	double_t calc = (BatteryValue - MIN_ADC)*2000/ (MAX_ADC-MIN_ADC);
	result = calc + 5000;
	*/
	int16_t ADCBuffer[1];
	batteryADC.read(ADCBuffer,1);
	
	return ADCBuffer[0];
}
