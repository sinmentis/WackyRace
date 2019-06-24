/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief 
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a racer!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* LEDs  */
#define LED1_PIO PA15_PIO
#define LED2_PIO PA14_PIO

/* General  */
#define APPENDAGE_PIO PA1_PIO
#define SERVO_PWM_PIO PA2_PIO

/* Button  */
#define BUTTON_PIO PA16_PIO

/* H-bridges   */
#define MOTOR_LEFT_PWM_PIO PA11_PIO
#define MOTOR_LEFT_PHASE_PIO PB4_PIO
#define MOTOR_LEFT_MODE_PIO PA27_PIO
#define MOTOR_RIGHT_PWM_PIO PA7_PIO
#define MOTOR_RIGHT_PHASE_PIO PA6_PIO
#define MOTOR_RIGHT_MODE_PIO PA28_PIO
#define MOTOR_SLEEP_PIO PA5_PIO

/* Radio  */
#define RADIO_CS_PIO PA11_PIO
#define RADIO_CE_PIO PA9_PIO
#define RADIO_IRQ_PIO PA15_PIO


#endif /* TARGET_H  */
