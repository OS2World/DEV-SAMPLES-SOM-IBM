
#
#  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
#  All Rights Reserved
#  Licensed Materials - Property of IBM
#  US Government Users Restricted Rights - Use, duplication or
#  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
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
# This make file has settings for IBM C Set ++ version 2.x for OS/2
# It is may be used with the nmake program.
#

# General settings
MAKEFILE     = makefile
SYS          = OS2
CC           = icc
CPPC         = icc
SC           = sc
LINKER       = icc
IMPLIB       = implib
RM           = del
OBJ          = obj
CPPFLAGS     = $(DLL_CPPFLAGS)
SCFLAGS      = -p -u
TARGETS      = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_EXT)  $(EXE_TARGET).$(EXE_EXT) somdimpl
CLEANFILES   = $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF) $(DLL_TARGET).$(DLL_EXT) $(EXE_TARGET).$(OBJ) $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -W3 -c -Ti+ -Q+ -Gd- -Gm+ -Gs+ -Sm -Ss+
EXE_CPPFLAGS = -W3 -c -Ti+ -Q+ -Ge- -Gd- -Gm+ -Gs+ -Sm
EXE_LFLAGS   = -B"/exepack /packd /packc /align:16 /pm:vio /MAP /NOI /NOL /NOE" -D_OS2 -D_c -Q+ -Gm+ -Ge+ -Sa -Ti+ -Fe
EXE_OBJS     =  phone.obj
EXE_SYSOBJS  =
EXE_LIBS     = somtk.lib os2386.lib phone.lib
EXE_TARGET   = phonmain
EXE_EXT      = exe
EXE_IDLS     =
EXE_CSRC     =
EXE_CPPSRC   =

# DLL build settings
DLL_INITMOD  =
DLL_CFLAGS   = -W3 -D_OS2 -D_c -Ti+ -Q+ -Gd- -Gm+ -Gs+ -Ge- -Sm -Ss+
DLL_CPPFLAGS = -W3 -D_OS2 -D_c -Ti+ -Q+ -Gd- -Gm+ -Ge+ -Sa -Fe
DLL_LFLAGS   =  -B"/exepack /packd /packc /align:16 /pm:vio /MAP /NOI /NOL /NOE" -D_OS2 -D_c -Q+ -Gm+ -Ge- -Sa -Ti+ -Fe
DLL_OBJS     =  phone.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk.lib os2386.lib phone.lib
DLL_TARGET   = phone
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = phone.idl
DLL_CSRC     =
DLL_CPPSRC   = phonmain.cpp phone.cpp

all: $(TARGETS)

# DLL target
$(DLL_TARGET).$(DLL_EXT): $(DLL_OBJS) $(DLL_TARGET).$(DLL_DEF)
	$(LINKER) $(DLL_LFLAGS) $(DLL_TARGET).$(DLL_EXT) $(DLL_OBJS) $(DLL_TARGET).$(DLL_DEF)  $(DLL_LIBS) $(DLL_TARGET).$(DLL_DEF)

# DLL "Import" library target
$(DLL_TARGET).$(DLL_LIBEXT): $(DLL_TARGET).$(DLL_DEF)
	$(IMPLIB) $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF)

# DLL exports file
$(DLL_TARGET).$(DLL_DEF): $(DLL_IDLS)
	$(SC) -s$(DLL_DEF) -mdll=$(DLL_TARGET) $(DLL_IDLS)

# EXE target
$(EXE_TARGET).$(EXE_EXT): $(DLL_TARGET).$(DLL_LIBEXT) $(EXE_TARGET).$(OBJ)
           $(LINKER) $(EXE_LFLAGS) $(EXE_TARGET).$(EXE_EXT) $(EXE_TARGET).$(OBJ) $(EXE_LIBS)

# Build DSOM Implementation Repository
somdimpl:
            -regimpl -A -i SampleServer -p "somossvr.exe" -v somOS::Server -m on
           -regimpl -a -i SampleServer -c phoneEntry -c somPersistencePOSIX::PID_POSIX -c somPersistencePOSIX::PDS_POSIX
            -regimpl -L -i SampleServer
            -regimpl -l -i SampleServer
            start somossvr.exe -a SampleServer -i
	@echo somdimpl > somdimpl

# Clean up files that can be re-built
clean: cleanfiles cleanimpl

cleanfiles:
	-del *.$(OBJ) >nul 2>&1
	-del $(EXE_IDLS:.idl=.h) $(DLL_IDLS:.idl=.h) >nul 2>&1
	-del $(EXE_IDLS:.idl=.ih) $(DLL_IDLS:.idl=.ih) >nul 2>&1
	-del $(EXE_IDLS:.idl=.xh) $(DLL_IDLS:.idl=.xh) >nul 2>&1
	-del $(EXE_IDLS:.idl=.xih) $(DLL_IDLS:.idl=.xih) >nul 2>&1
	-del *.ir >nul 2>&1
           -del *.map >nul 2>&1
	-del $(CLEANFILES) >nul 2>&1

cleanimpl:
	-regimpl -D -i SampleServer

# Suffix rules for building source files
.SUFFIXES: .$(OBJ) .cpp .c .xih .xh .ih .h .idl

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
phonmain.obj: phonmain.cpp phone.xh
	$(CPPC) -fo $@ $(EXE_CPPFLAGS) -Ge+ phonmain.cpp

phone.xih: phone.idl
phone.xh: phone.idl

phone.obj: phone.cpp phone.xih phone.xh
	$(CPPC) $(EXE_CPPFLAGS) phone.cpp




