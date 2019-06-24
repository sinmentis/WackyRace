#!/usr/bin/env python

"""Print Bluetooth devices."""

from __future__ import print_function
import bluetooth

__author__ = 'Michael Hayes'
__date__ = 'May 2017'

def main():

    print('Searching for Bluetooth devices.  This may take a minute...')
    devices = bluetooth.discover_devices(lookup_names=True)    
    print(devices)

if __name__ == '__main__':
    main()
