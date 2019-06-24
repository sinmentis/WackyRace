#ifndef RACER_HPP_
#define RACER_HPP_
#include "Pacer.hpp"
#include "Adc.hpp"
#include "Led.hpp"
#include "Pio.hpp"
//#include "Nrf24.hpp"
#include "Mpu9250.hpp"
#include "UsbSerial.hpp"
#include "UsartSerial.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "mcu.h"
#include "Pwm.hpp"

/*----- define constants -----*/
#define PACER_RATE 3000

/*----- LED pins -----*/
#define LED1_PIN PA16_PIO
#define LED2_PIN PA15_PIO

/*----- input pins -----*/
#define BUTTON_PIN PA18_PIO
#define BATTERY_PIN PA17_PIO
#define USBDTCT_PIN PA5_PIO

#endif
