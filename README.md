Wackyracers-2018
===============

This contains a directory structure for your assignment.

doc -- documentation

hw  -- hardware

src -- source code


Downloading
-----------

Your group leader should create a forked copy of the wacky-racers-2018
git project and then add the other group members to the project.  This
can be done by:

1. Go to https://eng-git.canterbury.ac.nz/mph/wacky-racers-2018

2. Click 'Fork' button.  This will create your own copy of the repository.

3. Click on the ‘Settings’ menu then click the 'Expand' button for
'Sharing and permissions'.  Change 'Project Visibility' to 'Private'.

4. Click on the 'Members' menu and add group members as Developers.

5. Using bash terminal (or other useful shell), enter the command:

```
$ git clone --recursive https://eng-git.canterbury.ac.nz/your-userid/wacky-racers.git
```

If you do not want to have to enter your password for every git
push/pull operation, you should set up ssh-keys and use:

```
$ git clone --recursive git@eng-git.canterbury.ac.nz:your-userid/wacky-racers.git
```

Note, this project uses two submodules (mmculib and mat91lib) hosted
on github.


Updating
--------

Updating is a nuisance since each submodule needs to be pulled
separately.  This can be achieved using:

```
$ git pull && git submodule update
```

Note, the submodules mmculib and mat91lib have detached heads.

A simpler way is to run (from the top-level directory):

```
$ make update
```

Configuration
-------------

The src/boards directory contains a number of configurations; one for
the hat and racer boards.  Each configuration directory contains three
files:

* board.mk  --- this is a makefile fragment that specifies the MCU model, optimisation level, etc.
* target.h  --- this is a C header file that defines the PIO pins and clock speeds.
* config.h  --- this is a C header file that wraps target.h.  It's purpose is for porting to different compilers.

You will need to edit the target.h file for your board.


Compilation
-----------

Compilation requires a bare-metal ARM GCC toolchain arm-none-eabi.
See
http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/ARM_toolchain
for installation details.

Compilation is performed using makefiles, since each application
requires many files.  Rather than having large makefiles, a heirarchy
of makefile fragments is employed.  Dependencies are automatically generated.

A board description can be selected with the BOARD environment
variable.  This can be defined for each command, for example,

```
$ BOARD=racer make program
```

Alternatively, this can be defined for a session

```
$ export BOARD=racer
$ make program
```

Before you can flash any code to the MCU, you need to disable the write-protection
bit in the non-volatile memory. This will need to be done once, and only needed again
if you erase your flash memory.
```
$ make bootflash
```

Each of the makefiles has the following phony targets:
* all  --- compile the application
* program --- compile the application and download to the MCU (you need Openocd running as a daemon)
* debug --- start the debugger GDB (you need Openocd running as a daemon)
* reset --- reset your MCU (you need Openocd running as a daemon)
* clean --- delete the executable and object files.   This is necessary when compiling for a different board.
* bootflash -- this needs to done once to set a bit in the SAM4S so that it will run your application and not its bootloader on reset.


Demo applications
-----------------

These are found in the demo-apps directory.  These are board
independent and thus the specific board must be selected with the
BOARD environment variable.  Note, when debugging hardware it is much easier
to use a small test program rather than a large application.

There a number of variations of each test program.  As a rule, the larger the number, the fancier the level of abstraction.

I suggest trying the following programs:
* ledflash1 You will need to correctly define LED1_PIO in boards/xxxx/target.h  If this program does not run see  http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/SAM4S_getting_started.
* pwm_test1 You will need to edit pwm_test1.c and define PWM1_PIO for an available PWM pin.  This program is useful for checking that the phase-locked loop (PLL) is correctly configured to generate the correct CPU clock frequency.  You should see a 100 kHz square wave using an oscilloscope connected to the specified PIO pin (provided it can be driven from the PWM peripheral).
* usb_vbus_test1 This will turn a LED on if the USB VBUS signal is detected when a USB cable is connected to a computer.  It is a simple example of using a PIO as an input.
* usbserial_hello1 This will repeatedly send "Hello world" to a computer via the USB CDC protocol.  If you have connected a USB cable to a computer running Linux, the Linux dmesg command should say someting about ttyACM0 detected.  If not see the troubleshooting section.   For more details, see http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/USB_CDC.
* adc_test1 This reads from the AD5 and AD6 analogue inputs and prints the values to a computer with the USB CDC protocol.


Wacky racer applications
------------------------

There is skeleton code for these in the wacky-apps directory.  There
are two programs; one for the racer and hat boards.  *Note, try the
test-apps first before trying to write these programs to ensure you
have the correct pin definitions, etc.*

Files common to each program are found in the wackylib directory.


Troubleshooting
---------------

If nothing runs, see  http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/SAM4S_getting_started.

### USB

If USB does not work check boards/xxxx/target.h that the definition
of UDP_VBUS_PIO is correct for your board.  The default is

#define UDP_VBUS_PIO PA5_PIO

Note, the USB driver fires up when this PIO pin goes high in response to the
USB connector being plugged in to a computer.

If there is no VBUS detection, do not define this macro.


Watchdog timers
---------------

By default, the watchdog timers are disabled.  To enable them, use:
    
    #include "mcu.h"
    
    mcu_watchdog_enable ();
    
To reset the watchdog, in your main loop use:

    mcu_watchdog_reset ();


I/O
---

I/O is non-blocking so if data is not available, the functions return without waiting.   The underlying drivers use the POSIX I/O model.


Coding style
------------

For the curious, the GNU C coding style is used.  I worked as a
contractor for the USA company Cygnus Solutions (taken over by Redhat)
as a compiler engineer working on the GNU C compiler (GCC).  The
first-rule when working as a programmer for a company is to follow the
required coding style.  My text editor (emacs) is set up to default to
this style.

