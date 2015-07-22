#
# This make file has settings for Watcom C32,C++32 version 10.5 for OS/2
# It is may be used with the wmake program.
#

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
#   @(#) 1.9 src/somples.os2/somd/c/event/makefile.wat, somx.somd, som2.5, s259604a 12/27/95 14:08:40 [1/24/96 17:51:59]


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

#  Makefile for DSOM "event" samples (os2)

# General settings
MAKEFILE     = makefile.wat
SYS          = OS2
CC           = wcc386
CPPC         = wpp386
SC           = sc
LINKER       = wlink
IMPLIB       = wlib -b -n
RM           = del
OBJ          = obj
CFLAGS       = $(DLL_CFLAGS)
CPPFLAGS     = $(DLL_CPPFLAGS)
SCFLAGS      =
TARGETS      = $(DLL_TARGET).$(DLL_EXT) $(EXE_TARGET).$(EXE_EXT) eventsvr.exe consumer.exe
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) $(DLL_INITMOD).* $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -bm -bt=OS2 -s -wx
EXE_CPPFLAGS = -bm -bt=OS2 -s -wx
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     = eventcli.obj
EXE_SYSOBJS  =
EXE_LIBS     = eventc somtk
EXE_TARGET   = eventcli
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     = eventcli.c
EXE_CPPSRC   =

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -bm -bd -bt=OS2 -s -wx
DLL_CPPFLAGS = -bm -bd -bt=OS2 -s -wx
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_OBJS     = CELL.obj EVENTCH.obj EVENTCOM.obj  sinitmod.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk
DLL_TARGET   = eventc
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = CELL.IDL EVENTCH.IDL EVENTCOM.IDL
DLL_CSRC     = CELL.c EVENTCH.c EVENTCOM.c
DLL_CPPSRC   =

all: $(%SOMBASE)\include\somcorba.bld $(TARGETS) somdimpl .SYMBOLIC

# DLL target
$(DLL_TARGET).$(DLL_EXT): $(DLL_OBJS) $(DLL_TARGET).$(DLL_DEF)
        @%create $*.lnk
        @%append $*.lnk $(DLL_LFLAGS) name $@
        @for %f in ($(DLL_LIBS)) do @%append $*.lnk library %f
        @for %f in ($(DLL_OBJS)) do @%append $*.lnk file %f
        @scdef $*.def $*.lnk
        $(LINKER) @$*.lnk
        $(IMPLIB) $*.$(DLL_LIBEXT) +$@
        sc $(SCFLAGS) -sir -u -mnolock *.idl

# DLL "Import" library target
$(DLL_TARGET).$(DLL_LIBEXT): $(DLL_TARGET).$(DLL_EXT)
        $(IMPLIB) $@ +$<


# DLL exports file
$(DLL_TARGET).$(DLL_DEF): $(DLL_IDLS)
        $(SC) -s$(DLL_DEF) -mdll=$(DLL_TARGET) $(DLL_IDLS)

# DLL SOMInitModule function
$(DLL_INITMOD).$(OBJ): $(DLL_INITMOD).c
        $(CC) $(DLL_CFLAGS) $<


$(DLL_INITMOD).c: $(DLL_IDLS)
        $(SC) -simod -mdll=$(DLL_TARGET) -mimod=$@ $(DLL_IDLS)

# EXE target
$(EXE_TARGET).$(EXE_EXT): $(EXE_OBJS)
        @%create $*.lnk
        @%append $*.lnk $(EXE_LFLAGS)
        @for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
        @for %f in ($(EXE_OBJS)) do @%append $*.lnk file %f
        @%append $*.lnk name $@
        @for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
        $(LINKER) @$*.lnk

eventsvr.$(EXE_EXT): eventsvr.obj
        @%create $*.lnk
        @%append $*.lnk $(EXE_LFLAGS)
        @for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
        @for %f in (eventsvr.obj) do @%append $*.lnk file %f
        @%append $*.lnk name $@
        @for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
        $(LINKER) @$*.lnk

consumer.$(EXE_EXT): consumer.obj
        @%create $*.lnk
        @%append $*.lnk $(EXE_LFLAGS)
        @for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
        @for %f in (consumer.obj) do @%append $*.lnk file %f
        @%append $*.lnk name $@
        @for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
        $(LINKER) @$*.lnk

# Clean up files that can be re-built
clean: cleanup cleanimpl

cleanup: .SYMBOLIC
        -del *.$(OBJ) *.ih *.h *.xih *.xh *.ir *.lnk $(CLEANFILES) *.rep *.err >nul 2>&1

# Cleanup the DSOM Implementation Repository.
cleanimpl: .SYMBOLIC
        -regimpl -D -i cellServer1
        -erase somdimpl

$(%SOMBASE)\include\somcorba.bld:
    @echo This sample requires the CORBA-style C bindings
    @echo created using the somcorba command.
    @exit 1

# Build the DSOM Implementation Repository.
somdimpl:
    -regimpl -A -i cellServer1 -p eventsvr.exe
    -regimpl -a -i cellServer1 -c Cell -c EventChannelAdmin::ConsumerAdmin -c EventChannelAdmin::EventChannel -c EventChannelAdmin::SupplierAdmin -c EventComm::Consumer -c EventComm::Supplier -c EventComm::EventConnection
    -regimpl -L -i cellServer1
    @echo "" > somdimpl

# Suffix rules for building source files
.EXTENSIONS:
.EXTENSIONS: .$(OBJ) .cpp .c .xih .xh .ih .h .idl

.cpp.$(OBJ):
        $(CPPC) $(CPPFLAGS) $<


.c.$(OBJ):
        $(CC) $(CFLAGS) $<


.idl.xih:
        $(SC) $(SCFLAGS) -sxih $<


.idl.xh:
        $(SC) $(SCFLAGS) -sxh $<


.idl.ih:
        $(SC) $(SCFLAGS) -sih $<


.idl.h:
        $(SC) $(SCFLAGS) -sh $<


# Dependencies follow
eventcli.obj: eventcli.c cell.h eventcom.ih EVENTCOM.h
        $(CC) $(EXE_CFLAGS) eventcli.c

eventsvr.obj: eventsvr.c cell.h eventcom.ih EVENTCOM.h eventch.h
        $(CC) $(EXE_CFLAGS) eventsvr.c

consumer.obj: consumer.c eventcom.ih eventcom.h eventch.h
        $(CC) $(EXE_CFLAGS) consumer.c

CELL.ih: CELL.IDL eventcom.idl
CELL.h: CELL.IDL eventcom.idl

EVENTCH.ih: EVENTCH.IDL eventcom.idl
EVENTCH.h: EVENTCH.IDL eventcom.idl

EVENTCOM.ih: EVENTCOM.IDL
EVENTCOM.h: EVENTCOM.IDL

CELL.obj: CELL.c cell.ih CELL.h eventcom.ih EVENTCOM.h
        $(CC) $(DLL_CFLAGS) CELL.c

EVENTCH.obj: EVENTCH.c eventcom.ih EVENTCOM.h eventch.ih EVENTCH.h
        $(CC) $(DLL_CFLAGS) EVENTCH.c

EVENTCOM.obj: EVENTCOM.c eventcom.ih EVENTCOM.h
        $(CC) $(DLL_CFLAGS) EVENTCOM.c




