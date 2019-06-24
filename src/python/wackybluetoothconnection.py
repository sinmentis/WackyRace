"""Wacky connection class for a Bluetooth connection."""

from __future__ import print_function

__author__ = 'Michael Hayes'
__date__ = 'May 2017'

import sys
try:
    import bluetooth
except:
    pass
    
from wackyconnection import WackyConnection


class WackyBluetoothConnection(WackyConnection):

    def __init__(self, mac_address, debug=False):

        print('Connecting to %s...' % mac_address, file=sys.stderr)

        self.debug = debug
        self.mac_address = mac_address
        self.connection = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.connection.connect((mac_address, 1))        

        # Clear buffer in case anything is left from a previous connection
        # I'm not sure if this happens with Bluetooth
        print('Connected...')        
        self.connection.settimeout(0.5)
        #self.connection.recv(1000)

    def _readline(self):
        """Read a line or time out"""
        # The RN41 seems to return strings in 8 byte bursts...
        try:
            return self.connection.recv(1000)
        except bluetooth.btcommon.BluetoothError:
            return ''

    def _write(self, string):
        """Write a string"""        
        return self.connection.send(string)


def discover():

    devices = bluetooth.discover_devices(lookup_names=True)
    return devices
