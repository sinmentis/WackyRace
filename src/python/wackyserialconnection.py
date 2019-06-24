"""Wacky connection class for a USB CDC (serial) connection."""

from __future__ import print_function

__author__ = 'Matthew Edwards/Michael Hayes'
__date__ = 'January 2015'

import sys
import serial
import os.path

from wackyconnection import WackyConnection


class WackySerialConnection(WackyConnection):

    SERIAL_DEVICES = [
        '/dev/tty.usbmodem1411', '/dev/tty.usbmodem1431',  # USB on Mac OSX
        '/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyACM2'  # USB on Linux
    ]

    def __init__(self, device_name, debug=False):

        print('Connecting to %s...' % device_name, file=sys.stderr)

        self.debug = debug
        self.device_name = device_name
        # The baudrate is ignored for USB CDC
        self.connection = serial.Serial(port=device_name, baudrate=57600)

        # Clear buffer in case anything is left from a previous connection
        # (this actually happens)
        self.connection.timeout = 0.5
        self.connection.read(1000)

        # Read timeout in seconds
        if device_name in self.SERIAL_DEVICES:
            self.connection.timeout = 0.1
        else:
            # Who knows?
            self.connection.timeout = 2

    def _readline(self):
        """Read a line or time out"""        
        return self.connection.readline()

    def _write(self, string):
        """Write a string"""
        return self.connection.write(string)

def autodetect():
    """Attempt to automatically detect and connect to a Wacky board over
    serial."""

    potential_devices = WackySerialConnection.SERIAL_DEVICES
    for potential_device in potential_devices:
        if not os.path.exists(potential_device):
            continue
        try:
            connection = WackySerialConnection(potential_device)
        except serial.SerialException, e:
            print("Caught exception while trying to connect to %r: %r",
                  potential_device, e)
            continue
        for i in range(2):  # May need to wake up device
            version = connection.send_command('?')
            if version:
                return connection
    return None
