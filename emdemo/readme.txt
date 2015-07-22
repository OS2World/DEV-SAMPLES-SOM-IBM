#
#   COMPONENT_NAME: somx
#
#   ORIGINS: 27
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
/* @(#) somx/readme 2.8 1/11/96 09:12:21 [1/24/96 17:31:12] */

/*
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 * THIS CODE.  IBM shall not be liable for any damages arising out of
 * your use of the sample code, even if they have been advised of the
 * possibility of such damages.
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */

This directory contains a sample program that illustrates the use of
Event Manager.  The executable built is called "emandemo".

Before building the executable make sure the environment SOMBASE is set
to the root directory of SOM. Make sure the environment variable SOMIR
is set appropriately.

To build the executable type the following commands:
        nmake clean
        nmake
        (During the link stage, if you are asked about dde4mbm library
         and you don't have one on your machine, you are probably using
         the latest CSet/2 (C and C++) and you don't need this library.
         Just hit enter and continue.)

Before running the program, make sure that the following environment
variables are set.

- SOMSOCKETS should be set to the name of the Socket implementation
  class (Typically "TCPIPSockets").
- HOSTNAME should be set to machine on which this program is running.

Run the program by invoking the command "emandemo".

NOTES:

1) Running the Program:

The following are the single character commands supported by the test
program.
 h  - help; prints a list of commands available
 c  - Creates a client event with a method callback
 C  - Creates a client event with a procedure callback
 W  - Registers a Work procedure to be called back when there are
      no events.
 w  - Unregisters the above work procedure interest
 T  - Registers a timer that pops every 100 milliseconds
 U  - Changes the above timer registration to pop every 800
      milliseconds instead
 t  - Unregisters the above timer
 P  - Creates a socket, binds it to a port, registers the socket
      with EMan and then listens for stream connections. When a
      connection is made, the new socket created for the connection
      is also registered with EMan for input messages (msg socket).
 p  - Unregisters the above listening socket.
 S  - Connects to the above listening socket and sends a message.
      When the message is received fully, the connection is brought
      down by the callback.
 q  - quit
 Q  - quit

Any other character is a no op.

2) Building with the IBM C Set/2 version 1.0 compiler

This sample uses the multithreaded migration library; In IBM C Set/2 version
1.0 this library is implemented by dde4mbm.lib and dde4mbs.lib. Hence
*** PLEASE ADD THE LIBRARY "dde4mbm" TO THE LIBRARY LIST in "main.lnk" ***
*** BEFORE YOU BUILD THIS SAMPLE.                                      ***


3) There is no makefile for the Watcom compiler (makefile.wat) due to
   differences in how va_list arguments are handled.

