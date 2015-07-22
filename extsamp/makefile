#
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
# This make file has settings for IBM C Set ++ version 2.x for OS/2
# It is may be used with the nmake program.
#

# General settings
MAKEFILE     = makefile
SYS          = OS2
CC           = icc
CPPC         = icc
SC           = sc
LINKER       = ilink
IMPLIB       = implib
RM           = del
OBJ          = obj
CFLAGS       = $(DLL_CFLAGS)
CPPFLAGS     = $(DLL_CPPFLAGS)
SCFLAGS      = -D __SOMIDL__
TARGETS      = $(DLL_TARGET).$(DLL_EXT) company.exe client1.exe client2.exe
CLEANFILES   = $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -W3 -c -Ti+ -Q+ -Gd- -Gm+ -Gs+ -Sm -Ss+
EXE_CPPFLAGS = /D_OS2 /Q- /Gm /Ge+ /c /Sa /Ti+ /N20
EXE_LFLAGS   = /EXE /OPTF /align:16 /pm:vio /NOI /NOL /NOE /C /DEBUG /STACK:32000 /exepack /packd /packc
EXE_OBJS     = util.obj samobj.lib
EXE_SYSOBJS  =
EXE_LIBS     = somtk.lib
EXE_TARGET   =
EXE_EXT      = exe
EXE_IDLS     = samobj
EXE_CSRC     =
EXE_CPPSRC   = client1.cpp company.cpp client2.cpp

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -W3 -c -Ti+ -Q+ -Gd- -Gm+ -Gs+ -Ge- -Sm -Ss+
DLL_CPPFLAGS = /D_OS2 /Q- /Gm /Ge- /Sa /Ti+ /c /N20
DLL_LFLAGS   = /DLL /align:16 /pm:vio /NOI /NOL /NOE /C /DEBUG /exepack /packd /packc
DLL_OBJS     = samobj.obj util.obj sinitmod.obj
DLL_SYSOBJS  =
DLL_LIBS     = somtk.lib
DLL_TARGET   = samobj
DLL_EXT      = dll
DLL_LIBEXT   = lib
DLL_DEF      = def
DLL_IDLS     = samobj.idl
DLL_CSRC     =
DLL_CPPSRC   = samobj.cpp

rebuild: clean ir somdimpl $(TARGETS)
     @echo '........................................................
     @echo '--------------------------------------------------------
     @echo '========================================================
     @echo '########################################################
     @echo '#         Run the following in the given order         #
     @echo '########################################################
     @echo '#  1. Terminate the somossvr named clientSvrThread     #
     @echo '#                                                      #
     @echo '#  2. company.exe                                      #
     @echo '#     A. Wait until the program has completed running  #
     @echo '#                                                      #
     @echo '#  3. You may now run client1 and/or client2           #
     @echo '#                                                      #
     @echo '########################################################
     @echo '========================================================
     @echo '--------------------------------------------------------
     @echo '........................................................

all: ir somdimpl $(TARGETS)
     @echo '........................................................
     @echo '--------------------------------------------------------
     @echo '========================================================
     @echo '########################################################
     @echo '#       Perform the following in the given order       #
     @echo '########################################################
     @echo '#  1. Terminate the somossvr named clientSvrThread     #
     @echo '#      (if running)                                    #
     @echo '#  2. company.exe                                      #
     @echo '#     A. Wait until the program has completed running  #
     @echo '#                                                      #
     @echo '#  3. You may now run client1 and/or client2           #
     @echo '#                                                      #
     @echo '########################################################
     @echo '========================================================
     @echo '--------------------------------------------------------
     @echo '........................................................


# DLL target
$(DLL_TARGET).$(DLL_EXT): $(DLL_OBJS) $(DLL_TARGET).$(DLL_DEF) $(DLL_TARGET).$(DLL_LIBEXT)
 $(LINKER) $(DLL_LFLAGS) $(DLL_OBJS) $(DLL_TARGET).$(DLL_DEF) $(DLL_LIBS)

# DLL "Import" library target
$(DLL_TARGET).$(DLL_LIBEXT): $(DLL_TARGET).$(DLL_DEF)
	$(IMPLIB) $(DLL_TARGET).$(DLL_LIBEXT) $(DLL_TARGET).$(DLL_DEF)

# DLL exports file
$(DLL_TARGET).$(DLL_DEF): $(DLL_IDLS)
	$(SC) $(SCFLAGS) -s$(DLL_DEF) -mdll=$(DLL_TARGET).$(DLL_EXT) $(DLL_IDLS)

# DLL SOMInitModule function
$(DLL_INITMOD).$(OBJ): $(DLL_INITMOD).c
	$(CC) $(DLL_CPPFLAGS) $*.c

$(DLL_INITMOD).c: $(DLL_IDLS)
	$(SC) $(SCFLAGS) -simod -mdll=$(DLL_TARGET) -mimod=$@ $(DLL_IDLS)

# EXE target
$(EXE_TARGET).$(EXE_EXT): $(EXE_OBJS)
	$(LINKER) $(EXE_LFLAGS) $(EXE_OBJS) $(EXE_SYSOBJS) $(EXE_LIBS)

# Build DSOM Implementation Repository
somdimpl: objsvr thrdsvr
 @echo somdimpl > somdimpl

# Register the object server, the classes it is to support, and then
# initialize the server.
objsvr:
      regimpl -A -i objServer -p "somossvr.exe " -v "somOS::Server" -m on
      regimpl -a  -i objServer -c OESSample::Department \
                               -c OESSample::CompanyCounts \
                               -c OESSample::SalesValues\
                               -c OESSample::VeryBigCo \
                               -c somExternalization::StreamFactory
      start somossvr -a objServer -i
      @echo objsvr > objsvr

#Register the classes used by the sample with the interface repository.
ir: $(DLL_IDLS)
       sc $(SCFLAGS) -sir -u  $(DLL_IDLS)

# Register the server thread, the classes it is to support and then
# initialize the server.
thrdsvr:
      regimpl -A -i clientSvrThread -p "somossvr.exe " -v "somOS::Server" -m on
      regimpl -a  -i clientSvrThread -c OESSample::Department \
                                     -c OESSample::CompanyCounts\
                                     -c OESSample::SalesValues\
                                     -c somLifeCycle::FactoryFinder\
                                     -c somLifeCycle::ServerSetLocation
      start somossvr -a clientSvrThread -i
      @echo thrdsvr > thrdsvr

# Clean up files that can be re-built
clean: cleanfiles cleanimpl

cleanfiles:
   -del *.obj                      1>NUL 2>NUL
   -del *.xh                       1>NUL 2>NUL
   -del *.xih                      1>NUL 2>NUL
   -del *.def                      1>NUL 2>NUL
   -del *.exe                      1>NUL 2>NUL
   -del *.dll                      1>NUL 2>NUL
   -del *.lib                      1>NUL 2>NUL
   -del *.ir                       1>NUL 2>NUL
   -del sinitmod.c                 1>NUL 2>NUL
   -del thrdsvr objsvr somdimpl    1>NUL 2>NUL

cleanimpl:
	-regimpl -D -i objServer
 -regimpl -D -i clientSvrThread

# Suffix rules for building source files
.SUFFIXES: .$(OBJ) .cpp .c .xih .xh .ih .h .idl

.def.lib:
	implib  $@ $<

.c.$(OBJ):
	$(CC) $(EXE_CFLAGS) $<

.cpp.$(OBJ):
	$(CC) $(EXE_CPPFLAGS) $<

.idl.cpp:
	$(SC) $(SCFLAGS) -sxc $<

.idl.def:
	$(SC) $(SCFLAGS) -sdef $<

.idl.c:
	$(SC) $(SCFLAGS) -sc $<

.idl.xih:
	$(SC) $(SCFLAGS) -sxih $<

.idl.xh:
	$(SC) $(SCFLAGS) -sxh $<

.idl.ih:
	$(SC) $(SCFLAGS) -sih $<

.idl.h:
	$(SC) $(SCFLAGS) -sh $<


# Dependencies follow
client1.exe : util.obj $*.obj samobj.obj samobj.lib
	$(LINKER) $(EXE_LFLAGS) $*.obj $(EXE_OBJS) $(EXE_LIBS)

client2.exe : util.obj $*.obj samobj.obj samobj.lib
	$(LINKER) $(EXE_LFLAGS) $*.obj $(EXE_OBJS) $(EXE_LIBS)

company.exe : util.obj $*.obj samobj.obj samobj.lib
	$(LINKER) $(EXE_LFLAGS) $*.obj $(EXE_OBJS) $(EXE_LIBS)

samobj.obj: samobj.cpp samobj.def samobj.xih samobj.xh
   $(CPPC) $(DLL_CPPFLAGS) samobj.cpp

