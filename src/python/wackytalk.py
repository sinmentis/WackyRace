#!/usr/bin/env python

"""Simple terminal program to talk with a wacky board."""

from __future__ import print_function

__author__ = 'Michael Hayes'
__date__ = 'May 2016'

from wackyconnect import wacky_connect
import sys

def main():

    conn = wacky_connect(sys.argv, debug=False)
    if conn is None:
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
