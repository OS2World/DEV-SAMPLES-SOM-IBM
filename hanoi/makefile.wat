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


# OS/2 Watcom C++ makefile for SOM TMS "hanoi" sample 

TARGET       = client.exe
TARGETS      = $(TARGET) hanoi.dll hanoi.lib som.ir somdimpl $(%SOMBASE)\include\somxh.bld usage
OBJS         = hanoi.obj traninv.obj trancoor.obj hanres.obj phanres.obj sinitmod.obj
SVRIDL       = hanoi.idl traninv.idl hanres.idl phanres.idl trancoor.idl hancon.idl
SVRXH        = hanoi.xh traninv.xh hanres.xh phanres.xh trancoor.xh hancon.xh
SVRXIH       = hanoi.xih traninv.xih hanres.xih phanres.xih trancoor.xih

CPPC         = wpp386
EXE_CPPFLAGS = -bm -bt=OS2 -s -wx
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=1000000 option caseexact
EXE_LIBS     = hanoi os2386 somtk 
DLL_CPPFLAGS = -bm -bd -bt=OS2 -s -wx
DLL_CFLAGS   = -bm -bd -bt=OS2 -s -wx
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_LIBS     = somtk 
LINKER       = wlink
SC           = sc
EMITXH       = -sxh -S200000
EMITXIH      = -sxih -S200000
EMITDEF      = -sdef -S200000
EMITIMOD     = -simod -S200000

.EXTENSIONS:
.EXTENSIONS: .obj .cpp .xih .xh .idl

.cpp.obj:
	$(CPPC) $(DLL_CFLAGS) $<

.idl.xih:
	$(SC) $(EMITXIH) $<

.idl.xh:
	$(SC) $(EMITXH) $<

all: $(TARGETS) .SYMBOLIC

$(TARGET): hanoi.lib client.obj
	@%create $*.lnk
	@%append $*.lnk $(EXE_LFLAGS)
        @%append $*.lnk file client.obj
	@%append $*.lnk name $@
	@for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
	$(LINKER) @$*.lnk

client.obj: client.cpp $(SVRXH)
	$(CPPC) $(EXE_CPPFLAGS) client.cpp

sinitmod.obj: sinitmod.c
        wcc386 $(DLL_CFLAGS) $<

sinitmod.c: $(SVRIDL)
        $(SC) $(EMITIMOD) -mdll=hanoi -mimod=$@ $(SVRIDL)

hanoi.dll: $(OBJS) hanoi.def
	@%create $*.lnk
	@%append $*.lnk $(DLL_LFLAGS) name $@
	@for %f in ($(DLL_LIBS)) do @%append $*.lnk library %f
	@for %f in ($(OBJS)) do @%append $*.lnk file %f
	@scdef $*.def $*.lnk
	$(LINKER) @$*.lnk

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

usage: .SYMBOLIC
	@echo See the README file in this directory for information on executing the sample.

$(%SOMBASE)\include\somxh.bld: 
	@echo This sample requires that the SOM toolkit C++ bindings have been built
	@echo with the somxh command.
	@exit 1

clean:  cleanimpl .SYMBOLIC
	-del som.ir client.exe hanoi.lib hanoi.dll $(SVRXH) $(SVRXIH) *.obj sinitmod.c >nul 2>&1

cleanimpl: .SYMBOLIC
        -regimpl -D -i hanoiServer1
        -regimpl -D -i hanoiClient1
        -del somdimpl >nul 2>&1

cycle: cyclesvr cyclecli .SYMBOLIC

cyclesvr: .SYMBOLIC
        -regimpl -D -i hanoiServer1
        -regimpl -A -i hanoiServer1 -p somossvr.exe -v somOS::Server -m on
        -regimpl -a -i hanoiServer1 -c Hanoi -c TranInv -c HanoiResource -c PersistentHanoiResource -c TranCoord -c somPersistencePOSIX::PID_POSIX -c somPersistencePOSIX::PDS_POSIX -c somStream::StreamIO

cyclecli: .SYMBOLIC
        -regimpl -D -i hanoiClient1
        -regimpl -A -i hanoiClient1 -p Client -v somOS::Server -m on

