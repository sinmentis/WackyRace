DRIVERS += usb_cdc tty

VPATH += $(WACKYLIB_DIR)
SRC += UsbSerial.cpp Print.cpp
INCLUDES += -I$(WACKYLIB_DIR)

