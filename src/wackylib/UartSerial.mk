DRIVERS += buart tty

VPATH += $(WACKYLIB_DIR)
SRC += UartSerial.cpp Print.cpp
INCLUDES += -I$(WACKYLIB_DIR)
