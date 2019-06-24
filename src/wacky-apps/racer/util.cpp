#include "util.hpp"

int ADCBuffer[ADC_BUFFER_SIZE];
Wacky::Adc batteryADC;

void ADCInit() {batteryADC.begin(BIT(ADC_CHANNEL_0));}

int readBattery()
{
	batteryADC.read(ADCBuffer,ADC_BUFFER_SIZE);
	int BatteryValue = GetAve(ADCBuffer);
	return BatteryValue;
}

int GetAve(int* buffer)
{
	float temp = 0;
	int size = sizeof(buffer);
	for(int ii = 0;ii<size;ii++)
	{temp+=buffer[ii];}
	return int(temp/size);
}
