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
SCFLAGS      = -D __SOMIDL__
TARGETS      = $(DLL_TARGET).$(DLL_EXT) company.exe client1.exe client2.exe
CLEANFILES   = $(TARGETS)

# EXE build settings
EXE_CFLAGS   = -od -zp4 -bm -bt=OS2 -s -wx
EXE_CPPFLAGS = -od -zp4 -bm -bt=OS2 -s -wx
EXE_LFLAGS   = sys os2v2 pmcompatible option stack=360000 option caseexact
EXE_OBJS     = util.obj samobj.lib
EXE_SYSOBJS  =
EXE_LIBS     = somtk.lib
EXE_TARGET   =
EXE_EXT      = exe
EXE_IDLS     = samobj
EXE_CSRC     =
EXE_CPPSRC   = company.cpp client1.cpp client2.cpp

# DLL build settings
DLL_INITMOD  = sinitmod
DLL_CFLAGS   = -od -zp4 -bm -bd -bt=OS2 -s -wx
DLL_CPPFLAGS = -od -zp4 -bm -bd -bt=OS2 -s -wx
DLL_LFLAGS   = sys os2v2 dll initinstance terminstance sys begin segment type data loadoncall nonshared end option manyautodata option caseexact
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

rebuild: clean ir somdimpl $(TARGETS) .SYMBOLIC
     @echo '........................................................
     @echo '--------------------------------------------------------
     @echo '========================================================
     @echo '********************************************************
     @echo '*         Run the following in the given order         *
     @echo '********************************************************
     @echo '*  1. Terminate the somossvr named clientSvrThread     *
     @echo '*                                                      *
     @echo '*  2. company.exe                                      *
     @echo '*     A. Wait until the program has completed running  *
     @echo '*                                                      *
     @echo '*  3. You may now run client1 and/or client2           *
     @echo '*                                                      *
     @echo '********************************************************
     @echo '========================================================
     @echo '--------------------------------------------------------
     @echo '........................................................


all: ir somdimpl $(TARGETS) .SYMBOLIC
     @echo '........................................................
     @echo '--------------------------------------------------------
     @echo '========================================================
     @echo '********************************************************
     @echo '*         Run the following in the given order         *
     @echo '********************************************************
     @echo '*  1. Terminate the somossvr named clientSvrThread     *
     @echo '*      (if running)                                    *
     @echo '*  2. company.exe                                      *
     @echo '*     A. Wait until the program has completed running  *
     @echo '*                                                      *
     @echo '*  3. You may now run client1 and/or client2           *
     @echo '*                                                      *
     @echo '********************************************************
     @echo '========================================================
     @echo '--------------------------------------------------------
     @echo '........................................................

# DLL target
# removed from end of next line $(DLL_TARGET).$(DLL_LIBEXT)
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
	$(IMPLIB) $@ +$*.$(DLL_EXT)

# DLL exports file
$(DLL_TARGET).$(DLL_DEF): $(DLL_IDLS)
	$(SC) $(SCFLAGS) -s$(DLL_DEF) -mdll=$(DLL_TARGET) $(DLL_IDLS)

# DLL SOMInitModule function
$(DLL_INITMOD).$(OBJ): $(DLL_INITMOD).c
	$(CC) $(DLL_CPPFLAGS) $*.c

$(DLL_INITMOD).c: $(DLL_IDLS)
	$(SC) $(SCFLAGS) -simod -mdll=$(DLL_TARGET).$(DLL_EXT) -mimod=$@ $(DLL_IDLS)

# EXE target
#$(EXE_TARGET).$(EXE_EXT): $(EXE_OBJS)
#	@%create $*.lnk
#	@%append $*.lnk $(EXE_LFLAGS)
#	@for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
#	@for %f in ($(EXE_OBJS)) do @%append $*.lnk file %f
#	@%append $*.lnk name $@
#	@for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
#	$(LINKER) @$*.lnk

# Build DSOM Implementation Repository
somdimpl:  objsvr thrdsvr
 @echo somdimpl > somdimpl

# Register the object server, the classes it is to support, and then
# initialize the server.
objsvr:
      regimpl -A -i objServer -p "somossvr.exe " -v "somOS::Server" -m on
      regimpl -a  -i objServer -c OESSample::Department -c OESSample::CompanyCounts -c OESSample::SalesValues -c OESSample::VeryBigCo -c somExternalization::StreamFactory
      start somossvr -a objServer -i
      @echo objsvr > objsvr

#Register the classes used by the sample with the interface repository.
ir: $(DLL_IDLS)
       sc $(SCFLAGS) -sir -u  $(DLL_IDLS)
       @echo ir>ir

# Register the server thread, the classes it is to support and then
# initialize the server.
thrdsvr:
      regimpl -A -i clientSvrThread -p "somossvr.exe " -v "somOS::Server" -m on
      regimpl -a  -i clientSvrThread -c OESSample::Department -c OESSample::CompanyCounts -c OESSample::SalesValues -c somLifeCycle::FactoryFinder -c somLifeCycle::ServerSetLocation
      start somossvr -a clientSvrThread -i
      @echo thrdsvr > thrdsvr


clean: cleanfiles cleanimpl .SYMBOLIC

cleanfiles: .SYMBOLIC
   -del *.obj                      1>NUL 2>NUL
   -del *.xh                       1>NUL 2>NUL
   -del *.xih                      1>NUL 2>NUL
   -del *.def                      1>NUL 2>NUL
   -del *.exe                      1>NUL 2>NUL
   -del *.dll                      1>NUL 2>NUL
   -del *.lib                      1>NUL 2>NUL
   -del *.ir                       1>NUL 2>NUL
   -del *.lnk                      1>NUL 2>NUL
   -del *.err                      1>NUL 2>NUL
   -del sinitmod.c                 1>NUL 2>NUL
   -del thrdsvr objsvr somdimpl ir 1>NUL 2>NUL

cleanimpl: .SYMBOLIC
 -regimpl -D -i objServer
 -regimpl -D -i clientSvrThread

# Suffix rules for building source files
.EXTENSIONS:
.EXTENSIONS: .$(OBJ) .cpp .c .xih .xh .ih .h .lib .def .idl

.def.lib:
	$(IMPLIB) $@ $<

.c.$(OBJ):
	$(CC) $(EXE_CFLAGS) $<

.cpp.$(OBJ):
	$(CPPC) $(EXE_CPPFLAGS) $<

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
client1.exe : util.obj client1.obj samobj.obj samobj.lib
	@%create $*.lnk
	@%append $*.lnk $(EXE_LFLAGS)
	@for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
	@for %f in ($(EXE_OBJS)) do @%append $*.lnk file %f
	@%append $*.lnk name $@
	@for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
	$(LINKER) @$*.lnk

client2.exe : util.obj client2.obj samobj.obj samobj.lib
	@%create $*.lnk
	@%append $*.lnk $(EXE_LFLAGS)
	@for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
	@for %f in ($(EXE_OBJS)) do @%append $*.lnk file %f
	@%append $*.lnk name $@
	@for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
	$(LINKER) @$*.lnk

company.exe : util.obj company.obj samobj.obj samobj.lib
	@%create $*.lnk
	@%append $*.lnk $(EXE_LFLAGS)
	@for %f in ($(EXE_SYSOBJS)) do @%append $*.lnk file %f
	@for %f in ($(EXE_OBJS)) do @%append $*.lnk file %f
	@%append $*.lnk name $@
	@for %f in ($(EXE_LIBS)) do @%append $*.lnk library %f
	$(LINKER) @$*.lnk

samobj.obj: samobj.cpp samobj.def samobj.xih samobj.xh
   $(CPPC) $(DLL_CPPFLAGS) samobj.cpp

