TARGET = hat.bin

SRC = hat.cpp rf.cpp rf.hpp mode.cpp mode.hpp battery.cpp battery.hpp gameboard.cpp gameboard.hpp

BOARD = hat

DRIVERS = syslog

WACKYLIB_DIR = ../../wackylib
include $(WACKYLIB_DIR)/Pacer.mk
include $(WACKYLIB_DIR)/Adc.mk
include $(WACKYLIB_DIR)/UsbSerial.mk
include $(WACKYLIB_DIR)/UsartSerial.mk
include $(WACKYLIB_DIR)/Mpu9250.mk
include $(WACKYLIB_DIR)/Nrf24.mk


include ../../boards/board.mk
