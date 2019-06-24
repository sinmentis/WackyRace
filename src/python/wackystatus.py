#!/usr/bin/env python

"""Simple terminal program to talk with a wacky board."""

from __future__ import print_function

__author__ = 'Michael Hayes'
__date__ = 'May 2016'

import sys
from wackyserialconnection import WackySerialConnection, autodetect

def main():
    if len(sys.argv) > 1:
        device_name = sys.argv[1]
        conn = WackySerialConnection(device_name, debug=False)
    else:
        conn = autodetect()
    if conn is None:
        print('Could not autodetect Wacky device, exiting')
        sys.exit(1)

    #Run a basic terminal using the given WackyConnection instance
    while True:
        try:
            command = raw_input('> ')
            print(conn.send_command(command))
        except KeyboardInterrupt:
            print('Exiting')
            sys.exit(0)

if __name__ == '__main__':
    main()
