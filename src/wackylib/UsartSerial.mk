DRIVERS += busart tty

VPATH += $(WACKYLIB_DIR)
SRC += UsartSerial.cpp Print.cpp
INCLUDES += -I$(WACKYLIB_DIR)
