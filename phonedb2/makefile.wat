#
#  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
#  All Rights Reserved
#  Licensed Materials - Property of IBM
#  US Government Users Restricted Rights - Use, duplication or
#  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
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

#
# This make file has settings for Watcom C32,C++32 version 10.5 for OS/2
# It is may be used with the wmake program.
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
SCFLAGS      = -p -u
TARGETS      = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_EXT) smap.dll $(EXE_TARGET).$(EXE_EXT) somdimpl
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) smap.def $(DLL_INITMOD).* $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -bm -bt=OS2 -s -wx -zp4
EXE_CPPFLAGS = -bm -bt=OS2 -s -wx -zp4
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     = maindb2.obj phone.obj
EXE_SYSOBJS  =
EXE_LIBS     = somtk.lib phone.lib db2api.lib
EXE_TARGET   = maindb2
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     =
EXE_CPPSRC   = maindb2.cpp

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -bm -bd -bt=OS2 -s -wx -zp4
DLL_CPPFLAGS = -bm -bd -bt=OS2 -s -wx -zp4
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_OBJS     = phone.obj sinitmod.obj smap.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk.lib db2api.lib
DLL_TARGET   = phone
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = phone.idl
DLL_CSRC     = smap.c
DLL_CPPSRC   = maindb2.cpp phone.cpp

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

smap.dll: smap.obj smap.def
	@%create $*.lnk
	@%append $*.lnk $(EXE_LFLAGS) name $@
	@for %f in ($(DLL_LIBS)) do @%append $*.lnk library %f
	@for %f in ($(DLL_OBJS)) do @%append $*.lnk file %f
	@scdef $*.def $*.lnk
	$(LINKER) @$*.lnk

# DLL "Import" library target
$(DLL_TARGET).$(DLL_LIBEXT): $(DLL_TARGET).$(DLL_EXT)
	$(IMPLIB) $@ +$*.$(DLL_EXT)

# DLL exports file
$(DLL_TARGET).$(DLL_DEF): $(DLL_IDLS)
	$(SC) -s$(DLL_DEF) -mdll=$(DLL_TARGET) $(DLL_IDLS)

smap.lib: smap.dll
	$(IMPLIB) $@ +$*.$(DLL_EXT)

smap.def: smap.idl
	$(SC) -s$(DLL_DEF) -mdll=smap smap.idl

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
           -regimpl -A -i myServer -p "somossvr.exe" -v somOS::Server -m on
           -regimpl -a -i myServer -c phoneEntry -c smap -c somPersistenceDB2::PID_DB2 -c somPersistenceDB2::PDS_DB2
            start somossvr.exe -a myServer -i
	@echo somdimpl > somdimpl

# Clean up files that can be re-built
clean: cleanfiles cleanimpl .SYMBOLIC

cleanfiles: .SYMBOLIC
 -del *.$(OBJ) >nul 2>&1
 -del phone.h >nul 2>&1
 -del smap.h >nul 2>&1
 -del smap.ih >nul 2>&1
 -del smap.c >nul 2>&1
 -del phone.ih >nul 2>&1
 -del phone.xh >nul 2>&1
 -del phone.xih >nul 2>&1
 -del *.ir >nul 2>&1
 -del *.bnd >nul 2>&1
 -del *.map >nul 2>&1
 -del *.lnk >nul 2>&1
 -del *.err >nul 2>&1
 -del $(CLEANFILES) >nul 2>&1

cleanimpl: .SYMBOLIC
	-regimpl -D -i myServer

# Suffix rules for building source files
.EXTENSIONS:
.EXTENSIONS: .$(OBJ) .cpp .c .sqc .xih .xh .ih .h .idl

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
maindb2.obj: maindb2.cpp phone.xh
	$(CPPC) $(EXE_CPPFLAGS) maindb2.cpp

phone.xih: phone.idl
phone.xh: phone.idl

phone.obj: phone.cpp phone.xih phone.xh
        $(CPPC) $(EXE_CPPFLAGS) phone.cpp

smap.$(OBJ): smap.sqc smap.c smap.h smap.ih
        sqlprep smap.sqc TESTDB /b /p
        $(CC) $(DLL_CFLAGS) /Fosmap.obj smap.c

