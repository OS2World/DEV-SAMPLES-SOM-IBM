# 3/8/96 15:42:11
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
#
#  DISCLAIMER OF WARRANTIES.
#  The following [enclosed] code is sample code created by IBM
#  Corporation. This sample code is not part of any standard or IBM
#  product and is provided to you solely for the purpose of assisting
#  you in the development of your applications.  The code is provided
#  "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
#  NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
#  THIS CODE.  IBM shall not be liable for any damages arising out of
#  your use of the sample code, even if they have been advised of the
#  possibility of such damages.
#
#  DISTRIBUTION.
#  This sample code can be freely distributed, copied, altered, and
#  incorporated into other software, provided that it bears the above
#  Copyright notice and DISCLAIMER intact.

#
# This make file has settings for Watcom C32,C++32 version 10.5 for OS/2
# It may be used with the wmake program.
#

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
SCFLAGS      = -S100000 -p
TARGETS      = $(DLL_TARGET).$(DLL_EXT) $(EXE_TARGET).$(EXE_EXT) somdimpl
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) $(DLL_INITMOD).* $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -bm -bt=OS2 -s -wx -zp4
EXE_CPPFLAGS = -bm -bt=OS2 -s -wx -zp4
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     =  phase3.obj checkenv.obj
EXE_SYSOBJS  =
EXE_LIBS     = apptbk somtk
EXE_TARGET   = sample
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     =
EXE_CPPSRC   = phase3.cpp checkenv.cpp

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -bm -bd -bt=OS2 -s -wx -zp4
DLL_CPPFLAGS = -bm -bd -bt=OS2 -s -wx -zp4
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_OBJS     =  appt.obj apptbook.obj ccall.obj dapptbk.obj fapptbk.obj key.obj mtg.obj dappt.obj dmtg.obj dccall.obj sinitmod.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk
DLL_TARGET   = apptbk
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = appt.idl apptbook.idl ccall.idl dapptbk.idl fapptbk.idl key.idl mtg.idl dappt.idl dmtg.idl dccall.idl
DLL_CSRC     =
DLL_CPPSRC   = appt.cpp apptbook.cpp ccall.cpp dapptbk.cpp fapptbk.cpp key.cpp mtg.cpp dappt.cpp dmtg.cpp dccall.cpp

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
	$(SC) $(SCFLAGS) -s$(DLL_DEF) -mdll=$(DLL_TARGET) $(DLL_IDLS)

# DLL SOMInitModule function
$(DLL_INITMOD).$(OBJ): $(DLL_INITMOD).c
	$(CC) $(DLL_CFLAGS) $*.c

$(DLL_INITMOD).c: $(DLL_IDLS)
	$(SC) $(SCFLAGS) -simod -mdll=$(DLL_TARGET) -mimod=$@ $(DLL_IDLS)

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
	-regimpl -A -i BookServer -p somossvr.exe -v somOS::Server -m on
	-regimpl -a -i BookServer -c Appointment
	-regimpl -a -i BookServer -c AppointmentBook
	-regimpl -a -i BookServer -c ConferenceCall
	-regimpl -a -i BookServer -c DistributedAppointmentBook
	-regimpl -a -i BookServer -c AppointmentBookFactory
	-regimpl -a -i BookServer -c Appt_Key
	-regimpl -a -i BookServer -c Meeting
	-regimpl -a -i BookServer -c DistributeAppointment
	-regimpl -a -i BookServer -c DistributedMeeting
	-regimpl -a -i BookServer -c DistributedConferenceCall
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
	-regimpl -D -i BookServer

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
phase3.obj: phase3.cpp appt.xh mtg.xh ccall.xh apptbook.xh key.xh fapptbk.xh dapptbk.xh samptype.h
	$(CPPC) $(EXE_CPPFLAGS) phase3.cpp

checkenv.obj: checkenv.cpp appt.xh mtg.xh ccall.xh apptbook.xh key.xh samptype.h
	$(CPPC) $(EXE_CPPFLAGS) checkenv.cpp

appt.xih: appt.idl
appt.xh: appt.idl

apptbook.xih: apptbook.idl mtg.idl appt.idl ccall.idl
apptbook.xh: apptbook.idl mtg.idl appt.idl ccall.idl

ccall.xih: ccall.idl appt.idl
ccall.xh: ccall.idl appt.idl

dapptbk.xih: dappt.idl dmtg.idl dccall.idl apptbook.idl DAPPTBK.IDL
dapptbk.xh: dappt.idl dmtg.idl dccall.idl apptbook.idl DAPPTBK.IDL

fapptbk.xih: fapptbk.idl appt.idl apptbook.idl mtg.idl ccall.idl dappt.idl dmtg.idl dccall.idl
fapptbk.xh: fapptbk.idl appt.idl apptbook.idl mtg.idl ccall.idl dappt.idl dmtg.idl dccall.idl

key.xih: key.idl appt.idl
key.xh: key.idl appt.idl

mtg.xih: mtg.idl appt.idl
mtg.xh: mtg.idl appt.idl

dappt.xih: dappt.idl appt.idl
dappt.xh: dappt.idl appt.idl

dmtg.xih: dmtg.idl dappt.idl appt.idl mtg.idl
dmtg.xh: dmtg.idl dappt.idl appt.idl mtg.idl

dccall.xih: dappt.idl dccall.idl appt.idl ccall.idl
dccall.xh: dappt.idl dccall.idl appt.idl ccall.idl

appt.obj: appt.cpp appt.xih appt.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) appt.cpp

apptbook.obj: apptbook.cpp appt.xh mtg.xh ccall.xh apptbook.xih apptbook.xh samptype.h key.xh
	$(CPPC) $(DLL_CPPFLAGS) apptbook.cpp

ccall.obj: ccall.cpp ccall.xih ccall.xh appt.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) ccall.cpp

dapptbk.obj: dapptbk.cpp appt.xh mtg.xh ccall.xh apptbook.xh key.xh dapptbk.xih dapptbk.xh samptype.h dappt.xh fapptbk.xh
	$(CPPC) $(DLL_CPPFLAGS) dapptbk.cpp

fapptbk.obj: fapptbk.cpp appt.xh mtg.xh ccall.xh dappt.xh dmtg.xh dccall.xh apptbook.xh key.xh dapptbk.xh fapptbk.xih fapptbk.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) fapptbk.cpp

key.obj: key.cpp appt.xh key.xih key.xh
	$(CPPC) $(DLL_CPPFLAGS) key.cpp

mtg.obj: mtg.cpp mtg.xih mtg.xh appt.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) mtg.cpp

dappt.obj: dappt.cpp dappt.xih dappt.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) dappt.cpp

dmtg.obj: dmtg.cpp dmtg.xih dmtg.xh mtg.xh appt.xh dappt.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) dmtg.cpp

dccall.obj: dccall.cpp dccall.xih dccall.xh ccall.xh appt.xh dappt.xh samptype.h
	$(CPPC) $(DLL_CPPFLAGS) dccall.cpp





