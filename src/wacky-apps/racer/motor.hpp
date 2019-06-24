#ifndef MOTOR_HPP_
#define MOTOR_HPP_

#include "Pio.hpp"
#include "Pwm.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mcu.h"

/*----- motor output pin -----*/
#define PWM_PIN4 PA2_PIO
#define PWM_PIN3 PA0_PIO
#define PWM_PIN2 PA1_PIO
#define PWM_PIN1 PA7_PIO
#define DAC1_PIN PB14_PIO
#define DAC2_PIN PB13_PIO

#define PWM_FREQ 1000

void motorinit();
void setMotor(int dutyLeft, int dutyRight);
void USBControl(char command);
void radioControl(int8_t angle_x, int8_t angle_y);


#endif
