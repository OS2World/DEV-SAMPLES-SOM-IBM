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
# @(#) somx/makefile 2.4 9/20/94 16:21:01 [9/22/94 15:27:15]


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

# Makefile for: irdump

CC = wcc386
CFLAGS_EXE = /d2 /w2 /3s

SYSTEM     = os2v2

LINKER     = *wlink
LDFLAGS   = debug all option stack=360K,map,c


irdump.exe: $(SOMBASE)\include\somcorba.bld irdump.c
	$(CC) $(CFLAGS_EXE) irdump.c
        $(LINKER) system $(SYSTEM) $(LDFLAGS) name $@ file {$*.obj} library somtk



clean: .SYMBOLIC
	-del irdump.obj irdump.exe som.ir >nul 2>&1

.IGNORE
$(SOMBASE)\include\somcorba.bld: .SYMBOLIC
	@echo This sample requires the CORBA-style C bindings
	@echo created using the somcorba command.
	@exit 1

