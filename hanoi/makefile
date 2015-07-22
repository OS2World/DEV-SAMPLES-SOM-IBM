#
# COMPONENT_NAME: somtr::SAMPLES
#
# ORIGINS: 27
#
# Licensed Materials - Property of IBM                                     
#                                                                          
# 25H7-912, 5697-197 (EMEA), 17H5-843 (JAPAN)                              
#                                                                          
# (c) Copyright IBM Corp. 1995                                             
#
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


# OS/2 C Set ++ makefile for SOM TMS "hanoi" sample 

TARGET     = client.exe
TARGETS    = $(TARGET) hanoi.dll hanoi.lib som.ir somdimpl $(SOMBASE)\include\somxh.bld usage
OBJS       = hanoi.obj traninv.obj trancoor.obj hanres.obj phanres.obj initmod.obj
SVRIDL     = hanoi.idl traninv.idl hanres.idl phanres.idl trancoor.idl hancon.idl
SVRXH      = hanoi.xh traninv.xh hanres.xh phanres.xh trancoor.xh hancon.xh
SVRXIH     = hanoi.xih traninv.xih hanres.xih phanres.xih trancoor.xih

CC         = icc
#CDEBUG    = -Ti+
CDEBUG     = 
#LDEBUG    = /debug
LDEBUG     = 
CPPFLAGS   = -W3 -c $(CDEBUG) -Q+ -Gd- -Gm+ -Gs+ -Sm 
LDFLAGS    = /exepack /packd /packc /align:4 /stack:1000000 /pm:vio /noe /noi /nol $(LDEBUG)
LIBLIST    = os2386+somtk
LINKER     = link386
SC         = sc
EMITXH     = -sxh -S200000
EMITXIH    = -sxih -S200000
EMITDEF    = -sdef -S200000
DELETE     = del

.SUFFIXES:
.SUFFIXES: .obj .cpp .xih .xh .idl

.cpp.obj:
        $(CC) -fo$*.obj -I. -I$(SOMBASE)\include $(CPPFLAGS) -Ge- $<

.idl.xih:
        $(SC) $(EMITXIH) $<

.idl.xh:
        $(SC) $(EMITXH) $<

all: $(TARGETS)

$(TARGET): hanoi.lib client.obj 
        $(LINKER) $(LDFLAGS) client.obj,$@,NUL,$(LIBLIST)+hanoi;

client.obj: client.cpp $(SVRXH)
        $(CC) -fo$@  -I. -I$(SOMBASE)\include $(CPPFLAGS) -Ge+ client.cpp

hanoi.dll: $(OBJS) hanoi.def 
        $(LINKER) $(LDFLAGS) $(OBJS),$@,nul,$(LIBLIST),hanoi.def

hanoi.def: $(SVRIDL)
        $(SC) $(EMITDEF) hanoi.idl
        $(SC) $(EMITDEF) traninv.idl
        $(SC) $(EMITDEF) trancoor.idl
        $(SC) $(EMITDEF) hanres.idl
        $(SC) $(EMITDEF) phanres.idl

hanoi.obj: hanoi.xih $(SVRXH) hanoi.cpp 

traninv.obj: traninv.xih $(SVRXH) traninv.cpp 

hanres.obj: hanres.xih $(SVRXH) hanres.cpp 

phanres.obj: phanres.xih $(SVRXH) phanres.cpp 

trancoor.obj: trancoor.xih $(SVRXH) trancoor.cpp

initmod.obj: $(SVRXH) initmod.cpp

hanoi.lib: hanoi.dll hanoi.def
        implib hanoi.lib hanoi.def

hanoi.xh: $(SVRIDL)

hanoi.xih: $(SVRIDL)

traninv.xh: $(SVRIDL)

traninv.xih: $(SVRIDL)

hanres.xh: $(SVRIDL)

hanres.xih: $(SVRIDL)

phanres.xh: $(SVRIDL)

phanres.xih: $(SVRIDL)

trancoor.xh: $(SVRIDL)

trancoor.xih: $(SVRIDL)

som.ir:
        $(SC) -sir -u -S200000 -mnolock hanoi.idl
        $(SC) -sir -u -S200000 -mnolock traninv.idl
        $(SC) -sir -u -S200000 -mnolock trancoor.idl
        $(SC) -sir -u -S200000 -mnolock hanres.idl
        $(SC) -sir -u -S200000 -mnolock phanres.idl

### -- Build the DSOM Implementation Repository.
somdimpl:
        -regimpl -A -i hanoiServer1 -p somossvr.exe -v somOS::Server -m on
        -regimpl -a -i hanoiServer1 -c Hanoi -c TranInv -c HanoiResource -c PersistentHanoiResource -c TranCoord -c somPersistencePOSIX::PID_POSIX -c somPersistencePOSIX::PDS_POSIX -c somStream::StreamIO
        -regimpl -A -i hanoiClient1 -p Client -v somOS::Server -m on
        @echo x > somdimpl

usage:
	@echo See the README file in this directory for information on executing the sample.

$(SOMBASE)\include\somxh.bld:
	@echo This sample requires that the SOM toolkit C++ bindings have been built
	@echo with the somxh command.
	@exit 1

clean:  cleanimpl
	-del som.ir client.exe hanoi.lib hanoi.dll $(SVRXH) $(SVRXIH) *.obj >nul 2>&1

cleanimpl:
        -regimpl -D -i hanoiServer1
        -regimpl -D -i hanoiClient1
        -del somdimpl >nul 2>&1

cycle: cyclesvr cyclecli

cyclesvr: 
        -regimpl -D -i hanoiServer1
        -regimpl -A -i hanoiServer1 -p somossvr.exe -v somOS::Server -m on
        -regimpl -a -i hanoiServer1 -c Hanoi -c TranInv -c HanoiResource -c PersistentHanoiResource -c TranCoord -c somPersistencePOSIX::PID_POSIX -c somPersistencePOSIX::PDS_POSIX -c somStream::StreamIO

cyclecli:
        -regimpl -D -i hanoiClient1
        -regimpl -A -i hanoiClient1 -p Client -v somOS::Server -m on

