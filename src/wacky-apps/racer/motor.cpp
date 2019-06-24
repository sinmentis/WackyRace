#include "motor.hpp"

Wacky::Pwm pwmLF;
Wacky::Pwm pwmLB;
Wacky::Pwm pwmRF;
Wacky::Pwm pwmRB;

Wacky::Pio motorLF(PWM_PIN4);
Wacky::Pio motorLB(PWM_PIN3);
Wacky::Pio motorRF(PWM_PIN2);
Wacky::Pio motorRB(PWM_PIN1);

int duty = 500;
char MotorMode = ' ';
char lastCommand = ' ';
void motorinit() {
    motorLF.setMode(PIO_OUTPUT_LOW);
    motorLB.setMode(PIO_OUTPUT_LOW);
    motorRF.setMode(PIO_OUTPUT_LOW);
    motorRB.setMode(PIO_OUTPUT_LOW);
    pwmLF.begin(PWM_PIN4, 1000, 0);
    pwmRF.begin(PWM_PIN3, 1000, 0);
    pwmRB.begin(PWM_PIN2, 1000, 0);
    pwmLB.begin(PWM_PIN1, 1000, 0);
}

void setMotor(int dutyRight, int dutyLeft) {
	if (dutyLeft > 0 && dutyRight > 0) {
		pwmLF.setDuty(dutyLeft);
		pwmRF.setDuty(dutyRight);
		pwmLB.setDuty(0);
		pwmRB.setDuty(0);
	} else if (dutyLeft < 0 && dutyRight < 0) {
		pwmLF.setDuty(0);
		pwmRF.setDuty(0);
		pwmLB.setDuty(-1*dutyLeft);
		pwmRB.setDuty(-1*dutyRight);
	} else if (dutyLeft > 0 && dutyRight < 0) {
		pwmLF.setDuty(dutyLeft);
		pwmRF.setDuty(0);
		pwmLB.setDuty(0);
		pwmRB.setDuty(-1*dutyRight);
	} else if (dutyLeft < 0 && dutyRight > 0) {
		pwmLF.setDuty(0);
		pwmRF.setDuty(dutyRight);
		pwmLB.setDuty(-1*dutyLeft);
		pwmRB.setDuty(0);
	} else if (dutyLeft == 0 && dutyRight == 0) {
		pwmLF.setDuty(0);
		pwmRF.setDuty(0);
		pwmLB.setDuty(0);
		pwmRB.setDuty(0);
	}
}

void USBControl(char command) {
	if (command == 'q') {
		duty -= 100;
		command = lastCommand;
	} else if (command == 'e') {
		duty += 100;
		command = lastCommand;
	}
	
	if (command == 'w') {
		setMotor(duty, duty);
	} else if (command == 's') {
		setMotor(-1*duty, -1*duty);
	} else if (command == 'a') {
		setMotor(-1*duty, duty);
	} else if (command == 'd') {
		setMotor(duty, -1*duty);
	} else if (command == ' ') {
		setMotor(0,0);
	}
	lastCommand = command;
}

void radioControl(int8_t angle_x, int8_t angle_y) {
	int a_x = angle_x + 90;
	int a_y = angle_y + 90; 
	int dutyLeft = 0;
	int dutyRight = 0;
	dutyLeft = (a_y * 2000 / 180 - 1000) + (a_x * 2000 / 180 - 1000);
	dutyRight = (a_y * 2000 / 180 - 1000) - (a_x * 2000 / 180 - 1000);
	setMotor(dutyLeft, dutyRight);
}
