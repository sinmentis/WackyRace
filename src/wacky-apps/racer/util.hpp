#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "Adc.hpp"
#include "UsbSerial.hpp"

#define ADC_BUFFER_SIZE 4

int readBattery();
void ADCInit();
int GetAve(int* buffer);
#endif
