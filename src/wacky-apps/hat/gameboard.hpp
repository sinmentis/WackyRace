#include "Pacer.hpp"
#include "mcu.h"
#include "Pio.hpp"
#include <stdio.h>
#include "UsartSerial.hpp"
#include "UartSerial.hpp"
#include "UsbSerial.hpp"

#define PACER_RATE_gameboard 1000
#define LINE_SIZE 80
#define DEBUG 1
#define ADC_BUFFER_SIZE 4

void gameBoardInit(Wacky::UsartSerial &serial,
                   Wacky::UsbSerial &usbserial);
void play_sound();
void stop_play_sound();
//void gameboard_task (Wacky::UsartSerial &serial,
  //                          Wacky::UsbSerial &usbserial);
