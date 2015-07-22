#   Description: Make file for base classes
#
#   Author:      Joseph H McIntyre
#
#   (C) COPYRIGHT International Business Machines Corp. 1995, 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
#   DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
#   sample code created by IBM Corporation. This sample code is not
#   part of any standard or IBM product and is provided to you solely
#   for the purpose of assisting you in the development of your
#   applications.  The code is provided "AS IS", without
#   warranty of any kind.  IBM shall not be liable for any damages
#   arising out of your use of the sample code, even if they have been
#   advised of the possibility of such damages.

# make options
DEBUGFLAG = 0                  # 0 - debug off, 1 - debug on
PLATFORM = OS2                 # OS2, WIN32
OS2COMPILER = IBMVACPP30       # IBMVACPP30
WINCOMPILER = IBMVACPP35       # IBMVACPP35

# make file variables
FILENAME = jmbase
RUNTIMEPATH = \opendoc\bin
PUBLICPATH = ..\public
INCLUDEPATH = .;$(PUBLICPATH);

!if ($(DEBUGFLAG) == 1)
DEBUG = /Ti+ /O- /DDEBUG
!else
DEBUG = /Ti- /O+
!endif

!if ("$(PLATFORM)" == "OS2")
CPLATFORM = /D_PLATFORM_OS2_
CINCLUDE = /I$(INCLUDEPATH)
CFLAGS = /Gm+ /Ge- /Q+ /C+ $(DEBUG)
LFLAGS = /Gm+ /Ge- /Q $(DEBUG) /B"/pmtype:pm"
SCFLAGS = -D _PLATFORM_OS2_ -I. -I$(PUBLICPATH) "-sxh;xih" -p -S65000
RC = rc
RCFLAGS = -r -D_PLATFORM_OS2_
!else if ("$(PLATFORM)" == "WIN32")
CPLATFORM = /D_PLATFORM_WIN32_ /D_PLATFORM_NT_
CINCLUDE = /I$(INCLUDEPATH) /I$(INCLUDE) /I$(ODBASE)\include /I$(ODBASE)\samples\include
CFLAGS = /Gm+ /Ge- /Q+ /C+ /Gs- /Su4 /Xi+ $(DEBUG)
LFLAGS = /Gm+ /Ge- /Q $(DEBUG)
SCFLAGS = -D _PLATFORM_WIN32_ -I. -I$(PUBLICPATH) -I$(ODBASE)\include "-sxh;xih;ir" -u -p -S65000
RC = irc
RCFLAGS = -D_PLATFORM_WIN32_
LLIB = $(ODBASE)\lib
!endif


# Make file target
all: $(PUBLICPATH)\log.h jmbase.dll

# Clear and set make order
.SUFFIXES:
.SUFFIXES: .h .idl .xh .def .cpp .c .rc .res .exp .dll .lib


# Standard command definitions
.idl.xih:
    sc $(SCFLAGS) $(@B).idl
    copy $(@B).idl $(PUBLICPATH)\$(@B).idl
    copy $(@B).xh  $(PUBLICPATH)\$(@B).xh
    del $(@B).xh

.cpp.obj:
    icc $(CFLAGS) $(CPLATFORM) $(CINCLUDE) $(@B).cpp

.rc.res:
    $(RC) $(RCFLAGS) $(@B).rc


# Dependencies
$(PUBLICPATH)\log.h : log.h
    copy log.h $(PUBLICPATH)

baselist.xih : baselist.idl

baselist.obj : baselist.cpp baselist.xih

baseinfo.xih : baseinfo.idl

baseinfo.obj : baseinfo.cpp baseinfo.xih baselist.xih log.h

image.xih    : image.idl

image.obj    : image.cpp image.xih

basenc.xih   : basenc.idl

basenc.obj   : basenc.cpp basenc.xih baselist.xih baseinfo.xih image.xih log.h

bfiter.xih   : bfiter.idl

bfiter.obj   : bfiter.cpp bfiter.xih

basec.xih    : basec.idl

basec.obj    : basec.cpp basec.xih basenc.xih baselist.xih baseinfo.xih image.xih bfiter.xih log.h

jmbase.obj   : jmbase.cpp baselist.xih baseinfo.xih image.xih basenc.xih basec.xih bfiter.xih

log.obj      : log.cpp log.h

jmbase.res   : jmbase.rc


# Specific steps
!if ("$(PLATFORM)" == "OS2")

jmbase.dll : baselist.obj baseinfo.obj image.obj basenc.obj bfiter.obj basec.obj jmbase.obj log.obj jmbase.res jmbase2.def
    icc @<<
      $(LFLAGS) /Fejmbase.dll
      jmbase.obj baselist.obj baseinfo.obj basenc.obj basec.obj image.obj bfiter.obj log.obj
      os2386.lib somtk.lib opendoc.lib $(PUBLICPATH)\pubutils.lib
      jmbase2.def
<<
    rc jmbase.res jmbase.dll
    copy jmbase.dll $(RUNTIMEPATH)
    IMPLIB /NOLOGO $(PUBLICPATH)\jmbase.lib jmbase2.def

!else if ("$(PLATFORM)" == "WIN32")

jmbase.exp: jmbase.obj baselist.obj baseinfo.obj basenc.obj basec.obj image.obj bfiter.obj log.obj jmbasew.def
    ilib /geni jmbase.obj baselist.obj baseinfo.obj basenc.obj basec.obj image.obj bfiter.obj log.obj jmbasew.def

jmbase.dll: jmbase.exp
    icc @<<
        $(LFLAGS) /Fejmbase.dll
        jmbase.obj baselist.obj baseinfo.obj basenc.obj basec.obj image.obj bfiter.obj log.obj
        user32.lib gdi32.lib somtk.lib
        $(LLIB)\odbento.lib  $(LLIB)\odbindng.lib $(LLIB)\odcm.lib     $(LLIB)\odcore.lib
        $(LLIB)\oddatax.lib  $(LLIB)\odimagng.lib $(LLIB)\odlayout.lib $(LLIB)\odpubutl.lib
        $(LLIB)\odstorag.lib $(LLIB)\odui.lib     $(LLIB)\odutils.lib  $(LLIB)\odregsty.lib
        jmbase.exp
<<
   copy jmbase.dll $(RUNTIMEPATH)
   copy jmbase.lib $(PUBLICPATH)
!endif
