#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#

 #
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
 #

#
# This make file has settings for Watcom C32,C++32 version 10.5 for OS/2
# It is may be used with the wmake program.
#

# General settings
MAKEFILE     = nametest.mak
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
SCFLAGS      = -p
TARGETS      = $(DLL_TARGET).$(DLL_EXT) $(EXE_TARGET).$(EXE_EXT) somdimpl
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) $(DLL_INITMOD).* $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -od -zp4 -bm -bt=OS2 -s -wx
EXE_CPPFLAGS = -od -zp4 -bm -bt=OS2 -s -wx
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     =  nametest.obj namemgr.obj
EXE_SYSOBJS  =
EXE_LIBS     = vehicle somtk
EXE_TARGET   = nametest
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     =
EXE_CPPSRC   = nametest.cpp namemgr.cpp

# added for OBJ file
OBJ_CPPFLAGS = -zp4 -bt=OS2 -s -wx

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -od -zp4 -bm -bd -bt=OS2 -s -wx
DLL_CPPFLAGS = -od -zp4 -bm -bd -bt=OS2 -s -wx
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_OBJS     =  vehicle.obj sinitmod.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk
DLL_TARGET   = vehicle
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = vehicle.idl
DLL_CSRC     =
DLL_CPPSRC   = vehicle.cpp

all: $(TARGETS) .SYMBOLIC

# DLL target
$(DLL_TARGET).$(DLL_EXT): $(DLL_OBJS) $(DLL_TARGET).$(DLL_DEF)
	@%create $*.lnk
	@%append $*.lnk $(DLL_LFLAGS) name $@
	@for %f in ($(DLL_LIBS)) do @%append $*.lnk library %f
	@for %f in ($(DLL_OBJS)) do @%append $*.lnk file %f
	@scdef $*.def $*.lnk
	$(LINKER) @$*.lnk
	$(IMPLIB) $*.$(DLL_LIBEXT) +$@

# DLL "Import" library target
$(DLL_TARGET).$(DLL_LIBEXT): $(DLL_TARGET).$(DLL_EXT)
	$(IMPLIB) $@ +$*.(DLL_EXT)

# DLL exports file
$(DLL_TARGET).$(DLL_DEF): $(DLL_IDLS)
	$(SC) -s$(DLL_DEF) -mdll=$(DLL_TARGET) $(DLL_IDLS)

# DLL SOMInitModule function
$(DLL_INITMOD).$(OBJ): $(DLL_INITMOD).c
	$(CC) $(DLL_CFLAGS) $*.c

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

# Build DSOM Implementation Repository
somdimpl:
 -regimpl -A -i vehiclesvr -p somossvr.exe -v somOS::Server -m on
 -regimpl -a -i vehiclesvr -c vehicle

	@echo somdimpl > somdimpl

# Clean up files that can be re-built
clean: cleanfiles cleanimpl .SYMBOLIC

cleanfiles: .SYMBOLIC
	-del *.$(OBJ) >nul 2>&1
	-del *.ih >nul 2>&1
	-del *.xh >nul 2>&1
	-del *.xih >nul 2>&1
	-del *.ir >nul 2>&1
	-del *.lnk >nul 2>&1
	-del *.err >nul 2>&1
	-del $(CLEANFILES) >nul 2>&1

cleanimpl: .SYMBOLIC
 -regimpl -D -i vehicle

# Suffix rules for building source files
.EXTENSIONS:
.EXTENSIONS: .$(OBJ) .cpp .c .xih .xh .ih .h .idl

.cpp.$(OBJ):
	$(CPPC) $(CPPFLAGS) $<


.c.$(OBJ):
	$(CC) $(CFLAGS) $<


.idl.cpp:
	$(SC) $(SCFLAGS) -usir;xc $<


.idl.c:
	$(SC) $(SCFLAGS) -usir;c $<


.idl.xih:
	$(SC) $(SCFLAGS) -usir;xih $<


.idl.xh:
	$(SC) $(SCFLAGS) -usir;xh $<


.idl.ih:
	$(SC) $(SCFLAGS) -usir;ih $<


.idl.h:
	$(SC) $(SCFLAGS) -usir;h $<


# Dependencies follow
vehicle.xih: vehicle.idl
vehicle.xh: vehicle.idl

vehicle.obj: vehicle.cpp vehicle.xih vehicle.xh
 $(CPPC) $(DLL_CPPFLAGS) vehicle.cpp

namemgr.obj: namemgr.cpp namemgr.h
 $(CPPC) $(OBJ_CPPFLAGS) namemgr.cpp

nametest.obj: nametest.cpp
 $(CPPC) $(EXE_CPPFLAGS) nametest.cpp


