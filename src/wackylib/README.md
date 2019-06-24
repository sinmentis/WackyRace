Wackylib provides a set of C++ classes for some of the SAM4S
peripherals.  The dirty work is provided by the mmculib and mat91lib
libraries.  The goal of wackylib is to provide a simple (Arduino-like)
interface rather than being comprehensive.

mmculib is a microprocessor agnostic library that provides
higher-level drivers with a POSIX-like interface.

mat91lib is a library for Atmel AT91SAM4S (and AT91SAM7)
microcontrollers.  It provides a low-level hardware abstraction using
Atmel's macros and structures for mapping onto the hardware registers.

All these libraries were written by M.P. Hayes.  In hindsight, I wish
 I wrote mmculib and mat91lib in C++.
