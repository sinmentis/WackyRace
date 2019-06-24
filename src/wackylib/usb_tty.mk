DRIVERS += usb_cdc tty

VPATH += $(WACKYLIB_DIR)
SRC += usb_tty.c
INCLUDES += -I$(WACKYLIB_DIR)

