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
#   @(#) 1.8 src/somples.os2/somd/cpp/animal/makefile.wat, somx.somd, som2.5, s259604a 12/27/95 14:09:11 [1/24/96 17:52:00]


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

#  Makefile for DSOM "animal" samples (os2)

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
TARGETS      = $(DLL_TARGET).$(DLL_EXT) $(EXE_TARGET).$(EXE_EXT) testsvr.$(EXE_EXT)
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) $(DLL_INITMOD).* $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -bm -bt=OS2 -s -wx
EXE_CPPFLAGS = -bm -bt=OS2 -s -wx
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     =  anitest.obj
EXE_SYSOBJS  =
EXE_LIBS     = danimal somtk
EXE_TARGET   = anitest
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     =
EXE_CPPSRC   = anitest.cpp

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -bm -bd -bt=OS2 -s -wx
DLL_CPPFLAGS = -bm -bd -bt=OS2 -s -wx
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
DLL_OBJS     =  DANIMAL.obj DBDOG.obj DDOG.obj DLDOG.obj DLOC.obj sinitmod.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk
DLL_TARGET   = danimal
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = DANIMAL.IDL DBDOG.IDL DDOG.IDL DLDOG.IDL DLOC.IDL
DLL_CSRC     =
DLL_CPPSRC   = DANIMAL.cpp DBDOG.cpp DDOG.cpp DLDOG.cpp DLOC.cpp

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

testsvr.$(EXE_EXT): testsvr.obj
        @%create $*.lnk
        @%append $*.lnk $(EXE_LFLAGS)
        @for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
        @for %f in (testsvr.obj) do @%append $*.lnk file %f
        @%append $*.lnk name $@
        @for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
        $(LINKER) @$*.lnk

# Clean up files that can be re-built
clean: cleanup cleanimpl

cleanup: .SYMBOLIC
        -del *.$(OBJ) *.ih *.h *.xih *.xh *.ir *.lnk $(CLEANFILES) *.err >nul 2>&1

# Cleanup the DSOM Implementation Repository.
cleanimpl: .SYMBOLIC
        -regimpl -D -i aniServer2
        -erase somdimpl

$(%SOMBASE)\include\somxh.bld:
$(SOMBASE)\include\somxh.bld:
        @echo This sample requires that the SOM toolkit C++ bindings have been built
        @echo with the somxh command.
        @exit 1

# Build the DSOM Implementation Repository.
somdimpl:
    -regimpl -A -i aniServer2 -p testsvr.exe
    -regimpl -a -i aniServer2 -c dAnimal -c dDog -c dBigDog -c dLittleDog -c dLocation
    -regimpl -L -i aniServer2
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
anitest.obj: anitest.cpp danimal.xh dldog.xh ddog.xh dbdog.xh dloc.xh
        $(CPPC) $(EXE_CPPFLAGS) anitest.cpp

testsvr.obj: testsvr.cpp
        $(CPPC) $(EXE_CPPFLAGS) testsvr.cpp

DANIMAL.ih: DANIMAL.IDL
DANIMAL.h: DANIMAL.IDL

DBDOG.ih: DBDOG.IDL ddog.idl danimal.idl
DBDOG.h: DBDOG.IDL ddog.idl danimal.idl

DDOG.ih: DDOG.IDL danimal.idl
DDOG.h: DDOG.IDL danimal.idl

DLDOG.ih: DLDOG.IDL ddog.idl danimal.idl
DLDOG.h: DLDOG.IDL ddog.idl danimal.idl

DLOC.ih: DLOC.IDL
DLOC.h: DLOC.IDL

DANIMAL.obj: DANIMAL.cpp dloc.xh danimal.xih DANIMAL.xh
        $(CPPC) $(DLL_CPPFLAGS) DANIMAL.cpp

DBDOG.obj: DBDOG.cpp dbdog.xih DBDOG.xh ddog.xh danimal.xh
        $(CPPC) $(DLL_CPPFLAGS) DBDOG.cpp

DDOG.obj: DDOG.cpp ddog.xih DDOG.xh danimal.xh
        $(CPPC) $(DLL_CPPFLAGS) DDOG.cpp

DLDOG.obj: DLDOG.cpp dldog.xih DLDOG.xh ddog.xh danimal.xh
        $(CPPC) $(DLL_CPPFLAGS) DLDOG.cpp

DLOC.obj: DLOC.cpp dloc.xih DLOC.xh
        $(CPPC) $(DLL_CPPFLAGS) DLOC.cpp




