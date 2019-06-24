#include "Adc.hpp"
#include <math.h>
#include "stdio.h"

#define ADC_BUFFER_SIZE 4
#define MIN_ADC 3900
#define MAX_ADC 5459

void battery_Init(); 
int16_t GetAve(int16_t* buffer);
int16_t readBattery();
