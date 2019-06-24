from __future__ import print_function
from wackyserialconnection import WackySerialConnection, autodetect
from wackybluetoothconnection import WackyBluetoothConnection
from sys import exit, stderr

def wacky_connect(argv=None, debug=False):

    if argv is None or len(argv) == 1:
        conn = autodetect()
        if conn is None:
            print('Could not autodetect Wacky device using USB CDC.'
                  ' Supply device name or MAC address', file=stderr)
        return conn

    device_name = argv[1]
    if ':' in device_name:
        conn = WackyBluetoothConnection(device_name, debug=debug)
        if conn is None:
            print('Could not connect to bluetooth device %s' % device_name)
        return conn

    conn = WackySerialConnection(device_name, debug=debug)
    if conn is None:
        print('Could not connect to USB CDC device %s' % device_name)
    return conn
