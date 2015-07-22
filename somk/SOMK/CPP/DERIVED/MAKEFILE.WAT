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
#   @(#) 1.4 src/somples.os2/somk/cpp/derived/makefile.wat, somx, som2.5, s259604a 12/27/95 14:10:55 [1/24/96 17:52:01]


# DISCLAIMER OF WARRANTIES.
# The following [enclosed] code is sample code created by IBM
# Corporation. This sample code is not part of any standard or IBM
# product and is provided to you solely for the purpose of assisting
# you in the development of your applications.  The code is provided
# "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
# NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
# THIS CODE.  IBM shall not be liable for any damages arising out of
# your use of the sample code, even if they have been advised of the
# possibility of such damages.
#
# DISTRIBUTION.
# This sample code can be freely distributed, copied, altered, and
# incorporated into other software, provided that it bears the above
# Copyright notice and DISCLAIMER intact.

OBJS       = hello.obj mello.obj derived.obj main.obj
TARGET     = test.exe

# SOM compiler
SC = sc
SCFLAGS   = 

# watcom 32 bit c++ compiler and linker
CC = $(%WCDIR)\binw\wpp386
CPPFLAGS = /d2 /w0 /3s -I. -I$(%SOMBASE)\include -I$(%WCDIR)\h 
LINKER     = $(%WCDIR)\binp\wlink
LDFLAGS   = debug all option stack=360K,map,c
LIBLIST    = os2386,somtk
LIBPATH   = libpath $(%WCDIR)\lib386;$(%WCDIR)\lib386\os2
OBJSL      =   hello.obj,mello.obj,derived.obj,main.obj

.EXTENSIONS:
.EXTENSIONS: .obj .cpp .xih .xh .idl


.cpp.obj:
	  $(CC) $(CPPFLAGS) $<
.idl.xh:
	  $(SC) -sxh $(SCFLAGS) $*.idl
.idl.xih:
	  $(SC) -sxih $(SCFLAGS) $*.idl

$(TARGET): $(SOMBASE)\include\somxh.bld $(WCDIR)\binw\wcc386.exe $(OBJS)
        $(LINKER) $(LDFLAGS) name $@ file $(OBJSL) library $(LIBLIST) $(LIBPATH)
       $(TARGET)

clean: .SYMBOLIC
	-del *.obj *.exe *.map *.xh *.xih >nul 2>&1

.IGNORE
$(%SOMBASE)\include\somxh.bld: .SYMBOLIC
	@echo This sample requires the C++ bindings
	@echo created using the somxh command.
	@exit 1

$(%WCDIR)\binw\wcc386.exe: .SYMBOLIC
 @echo This makefile needs WCDIR to be set. Please do the following:
 @echo set WCDIR=where\watcom\compiler\is\installed
 @exit 1 


hello.obj: hello.xh hello.xih hello.cpp
mello.obj: mello.xh mello.xih mello.cpp
derived.obj: derived.xh derived.xih derived.cpp
main.obj: derived.xh main.cpp

