#include "Mpu9250.hpp"
#include "Adc.hpp"
#include <math.h>
#include "stdio.h"

#define PI 3.1415926
#define MIN_ADC 3900
#define MAX_ADC 5459

double_t calc_angle(int16_t a,int16_t b,int16_t c);
int16_t battery_return();
void IMU_detection();
int8_t* old_control_joy_get ();
int8_t* old_control_IMU_get ();
