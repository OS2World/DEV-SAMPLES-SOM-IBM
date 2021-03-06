#   File:          MAKEFILE
#
#   Description:   Make file for $partname$
#
#   Written by:    $author$
#
#   Copyright:     (c) $years$ by $company$
#                  - all rights reserved
#
#   Generated by:  $toolname$ $toolversion$

# make options
DEBUGFLAG = 0                  # 0 - debug off, 1 - debug on
PLATFORM = OS2                 # OS2, WIN32
OS2COMPILER = IBMVACPP30       # IBMVACPP30
WINCOMPILER = IBMVACPP35       # IBMVACPP35

# make file variables
FILENAME = $filename$
RUNTIMEPATH = $runtimepath$
PUBLICPATH = $publicpath$
INCLUDEPATH = .;$(PUBLICPATH);

!if ($(DEBUGFLAG) == 1)
DEBUG = /Ti+ /O-
LDEBUG = /de
!else
DEBUG = /Ti- /O+ /DDEBUG
LDEBUG =
!endif

!if ("$(PLATFORM)" == "OS2")
CPLATFORM = /D_PLATFORM_OS2_
CINCLUDE = /I$(INCLUDEPATH)
CFLAGS = /Gm+ /Ge- /Q+ /C+ $(DEBUG) /I$(INCLUDEPATH)
LFLAGS = /Gm+ /Ge- /Q /B"/pmtype:pm $(LDEBUG)"
SCFLAGS = -D _PLATFORM_OS2_ -I. -I$(PUBLICPATH) "-sxh;xih;def" -p -S65000
!else if ("$(PLATFORM)" == "WIN32")
CPLATFORM = /D_PLATFORM_WIN32_ /D_PLATFORM_NT_
CINCLUDE = /I$(INCLUDEPATH) /I$(INCLUDE) /I$(ODBASE)\include /I$(ODBASE)\samples\include
CFLAGS = /Gm+ /Ge- /Q+ /C+ /Gs- /Su4 /Xi+ $(DEBUG) /I$(INCLUDEPATH)
LFLAGS = /Gm+ /Ge- /Q /B"$(LDEBUG)"
SCFLAGS = -D _PLATFORM_WIN32_ -I. -I$(PUBLICPATH) -I$(ODBASE)\include "-sxh;xih;def;ir" -p -u -S65000
LLIB = $(ODBASE)\lib
!endif


# Make file target
all: $(FILENAME).dll

# Clear and set make order
.SUFFIXES:
.SUFFIXES: .idl .xh .def .cpp .c .orc .wrc .res .exp .dll .lib


# Standard command definitions
.idl.xh:
    sc $(SCFLAGS) $(@B).idl

.cpp.obj:
    icc $(CFLAGS) $(CPLATFORM) $(CINCLUDE) $(@B).cpp

!if ("$(PLATFORM)" == "OS2")
.orc.res:
    rc -r $(@B).orc
!else if ("$(PLATFORM)" == "WIN32")
.wrc.res:
    irc -r $(@B).wrc
!endif


# Dependencies
$(FILENAME).xh:  $(FILENAME).idl

$(FILENAME).obj: $(FILENAME).cpp $(FILENAME).xh

!if ("$(PLATFORM)" == "OS2")
#$(FILENAME).res: $(FILENAME).orc
!else if ("$(PLATFORM)" == "WIN32")
#$(FILENAME).res: $(FILENAME).wrc
!endif


# Specific steps

!if ("$(PLATFORM)" == "OS2")
$(FILENAME).dll: $(FILENAME).obj $(FILENAME).def
    icc @<<
        $(LFLAGS) /Fe$(FILENAME).dll
        $(FILENAME).obj
        os2386.lib somtk.lib opendoc.lib
        $(PUBLICPATH)\jmbase.lib
        $(PUBLICPATH)\pubutils.lib
        $(FILENAME).def
<<
   copy $(FILENAME).dll $(RUNTIMEPATH)
!else if ("$(PLATFORM)" == "WIN32")
$(FILENAME).exp: $(FILENAME).obj $(FILENAME).def
    ilib /geni $(FILENAME).obj $(FILENAME).def

$(FILENAME).dll: $(FILENAME).exp
    icc @<<
        $(LFLAGS) /Fe$(FILENAME).dll
        $(FILENAME).obj
        user32.lib gdi32.lib somtk.lib
        $(LLIB)\odbento.lib  $(LLIB)\odbindng.lib $(LLIB)\odcm.lib     $(LLIB)\odcore.lib
        $(LLIB)\oddatax.lib  $(LLIB)\odimagng.lib $(LLIB)\odlayout.lib $(LLIB)\odpubutl.lib
        $(LLIB)\odstorag.lib $(LLIB)\odui.lib     $(LLIB)\odutils.lib  $(LLIB)\odregsty.lib
        $(PUBLICPATH)\jmbase.lib
        $(FILENAME).exp
<<
   copy $(FILENAME).dll $(RUNTIMEPATH)
!endif
