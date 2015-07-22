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
#   @(#) 1.5 src/somples.os2/somd/cpp/collect/makefile.wat, somx.somd, som2.5, s259604a 12/27/95 14:10:05 [1/24/96 17:52:00]


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

#  Makefile for DSOM "collect" samples (os2)

# General settings
MAKEFILE     = makefile
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
TARGETS      = $(DLL_TARGET).$(DLL_EXT) $(EXE_TARGET).$(EXE_EXT) server.exe
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) $(DLL_INITMOD).* $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -bm -bt=OS2 -s -wx
EXE_CPPFLAGS = -bm -bt=OS2 -s -wx
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     =  client.obj
EXE_SYSOBJS  =
EXE_LIBS     = bank somtk
EXE_TARGET   = client
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     =
EXE_CPPSRC   = client.cpp

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -bm -bd -bt=OS2 -s -wx
DLL_CPPFLAGS = -bm -bd -bt=OS2 -s -wx
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_OBJS     =  ACCT.obj BANK.obj CHECK.obj MF.obj SAVE.obj TRANSACT.obj sinitmod.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk
DLL_TARGET   = bank
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = ACCT.IDL BANK.IDL CHECK.IDL MF.IDL SAVE.IDL TRANSACT.IDL
DLL_CSRC     =
DLL_CPPSRC   = ACCT.cpp BANK.cpp CHECK.cpp MF.cpp SAVE.cpp TRANSACT.cpp

all: $(%SOMBASE)\include\somxh.bld $(TARGETS) somdimpl .SYMBOLIC

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

server.$(EXE_EXT): server.obj bank.lib
        @%create $*.lnk
        @%append $*.lnk $(EXE_LFLAGS)
        @for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
        @for %f in (server.obj) do @%append $*.lnk file %f
        @%append $*.lnk name $@
        @for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
        $(LINKER) @$*.lnk

# Clean up files that can be re-built
clean: cleanup cleanimpl

cleanup: .SYMBOLIC
        -del *.$(OBJ) *.ih *.h *.xih *.xh *.ir *.lnk $(CLEANFILES) *.err >nul 2>&1

# Cleanup the DSOM Implementation Repository.
cleanimpl: .SYMBOLIC
        -regimpl -D -i BankServer
        -erase somdimpl

$(%SOMBASE)\include\somxh.bld:
        @echo This sample requires that the SOM toolkit C++ bindings have been built
        @echo with the somxh command.
        @exit 1

# Build the DSOM Implementation Repository.
somdimpl:
    -regimpl -A -i BankServer -p server.exe
    -regimpl -a -i BankServer -c trans_info -c Account -c savingsAcc -c checkingAcc -c mutualfundsAcc -c Bank
    -regimpl -L -i BankServer
    -regimpl -l -i BankServer
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
client.obj: client.cpp bank.xh transact.xh save.xh acct.xh check.xh mf.xh
        $(CPPC) $(EXE_CPPFLAGS) client.cpp

server.obj: server.cpp bank.xh transact.xh save.xh acct.xh check.xh mf.xh
        $(CPPC) $(EXE_CPPFLAGS) server.cpp

ACCT.ih: ACCT.IDL transact.idl
ACCT.h: ACCT.IDL transact.idl

BANK.ih: BANK.IDL
BANK.h: BANK.IDL

CHECK.ih: CHECK.IDL acct.idl transact.idl
CHECK.h: CHECK.IDL acct.idl transact.idl

MF.ih: MF.IDL acct.idl transact.idl
MF.h: MF.IDL acct.idl transact.idl

SAVE.ih: SAVE.IDL acct.idl transact.idl
SAVE.h: SAVE.IDL acct.idl transact.idl

TRANSACT.ih: TRANSACT.IDL
TRANSACT.h: TRANSACT.IDL

ACCT.obj: ACCT.cpp transact.xh check.xh acct.xh save.xh mf.xh acct.xih
        $(CPPC) $(DLL_CPPFLAGS) ACCT.cpp

BANK.obj: BANK.cpp transact.xh save.xh acct.xh check.xh mf.xh bank.xih BANK.xh acct.xih
        $(CPPC) $(DLL_CPPFLAGS) BANK.cpp

CHECK.obj: CHECK.cpp transact.xh acct.xh check.xih CHECK.xh
        $(CPPC) $(DLL_CPPFLAGS) CHECK.cpp

MF.obj: MF.cpp transact.xh acct.xh mf.xih MF.xh
        $(CPPC) $(DLL_CPPFLAGS) MF.cpp

SAVE.obj: SAVE.cpp transact.xh acct.xh save.xih SAVE.xh
        $(CPPC) $(DLL_CPPFLAGS) SAVE.cpp

TRANSACT.obj: TRANSACT.cpp transact.xih TRANSACT.xh
        $(CPPC) $(DLL_CPPFLAGS) TRANSACT.cpp




