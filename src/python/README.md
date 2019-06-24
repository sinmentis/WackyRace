Wackyracers
===========

This directory contains a number of rough-as-guts demo programs for
communicating with a wacky racer via USB CDC or Bluetooth (if
supported).


Requirements
------------

These programs require the following python packages to be installed:

python2.7
python-numpy
python-matplotlib
python-serial
python-pygame
python-bluetooth


Programs
--------

    * wackydiscover prints a list of discoverable Bluetooth devices
    * wackychat is a simple terminal program
    * wackycrasher sends commands to the motor card to crash the racer into things
    
These programs can be run either using:

    $ python wackytalk.py
or

    $ ./wackytalk.py

With no command line arguments, the programs try to find a USB CDC
device.  If the argument has a colon in it, then it is assumed to be a
Bluetooth MAC address.  For example:

    ./wackytalk.py 00:11:67:00:C1:A9

Otherwise the argument is assumed to be a device name.  For example:

    ./wackytalk.py /dev/ttyACM0


Bluetooth
---------

To determine discoverable Bluetooth devices from python use:

    >>> import bluetooth
    >>> devices = bluetooth.discover_devices(lookup_names=True)
    >>> print(devices)

The variable devices is a list of tuples.  The first tuple element is
the MAC address; the second element is the device name.  Given a MAC
address you can connect to a Bluetooth device using something like:

    >>> mac_address = '00:06:66:8E:C3:D2'
    >>> s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
    >>> s.connect((mac_address, 1))

You can then send strings to the device using:

    >>> s.send('Hello\n')

Strings can be received using:

    >>> print(s.recv(80))

The magic number is the maximum number of characters to return.  Note,
this blocks until a newline character is received.
