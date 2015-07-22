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
# @(#) 1.5 src/somples.os2/somk/c/tp/makefile.wat, somx.somk, som2.5, s259604a 12/27/95 14:10:45 [1/24/96 17:52:01]


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

OBJS    = bt.obj helper.obj word.obj link.obj ll.obj fm.obj tpword.obj textline.obj colblk.obj page.obj ep.obj suep.obj footer.obj header.obj txtep.obj ipep.obj blep.obj main.obj

LINKOBJS = bt, helper, word, link, ll, fm, tpword, textline, colblk, page, ep, suep, footer, header, txtep, ipep, blep, main

TARGET      = tp.exe

# SOM compiler
SC = sc
SCFLAGS = -p -mnoaddstar

# Watcom 32 bit compiler
CC = $(%WCDIR)\binw\wcc386.exe
CFLAGS = /d2 /w1 /3s -I. -I$(%SOMBASE)\include -I$(%WCDIR)\h 
LINKER     = $(%WCDIR)\binp\wlink
LDFLAGS   = debug all option stack=360K,map,c
LIBLIST    = os2386,somtk
LIBPATH   = libpath $(%WCDIR)\lib386;$(%WCDIR)\lib386\os2

.EXTENSIONS:
.EXTENSIONS: .obj .c .ih .h .idl

.c.obj:
       $(CC) $(CFLAGS) $<

.idl.ih:
       $(SC) $(SCFLAGS) -sih $<

.idl.h:
       $(SC) $(SCFLAGS) -sh $<

$(TARGET): $(SOMBASE)\include\somcorba.bld $(WCDIR)\binw\wcc386.exe $(OBJS)
        $(LINKER) $(LDFLAGS) name $@ file $(LINKOBJS) library $(LIBLIST) $(LIBPATH)
       $(TARGET)

clean: .SYMBOLIC
	ren helper.h helper.sav
	-del *.obj *.exe *.map *.h *.ih $(CLEANFILES) >nul 2>&1
	ren helper.sav helper.h

.IGNORE
$(%SOMBASE)\include\somcorba.bld: .SYMBOLIC
	@echo This sample requires the CORBA-style C bindings
	@echo created using the somcorba command.
	@exit 1

$(%WCDIR)\binw\wcc386.exe: .SYMBOLIC
 @echo This makefile needs WCDIR to be set. Please do the following:
 @echo set WCDIR=where\watcom\compiler\is\installed
 @exit 1
 
bt.ih: bt.idl
bt.h: bt.idl
bt.obj: bt.c bt.h bt.ih

helper.obj: helper.c helper.h

word.ih: word.idl
word.h: word.idl
word.obj: word.c word.h word.ih

link.ih: link.idl
link.h: link.idl
link.obj: link.c link.h link.ih

ll.ih: ll.idl
ll.h: ll.idl
ll.obj: ll.c ll.h ll.ih

fm.ih: fm.idl
fm.h: fm.idl
fm.obj: fm.c fm.h fm.ih

tpword.ih: tpword.idl
tpword.h: tpword.idl
tpword.obj: tpword.c tpword.h tpword.ih

textline.ih: textline.idl
textline.h: textline.idl
textline.obj: textline.c textline.h textline.ih

colblk.ih: colblk.idl
colblk.h: colblk.idl
colblk.obj: colblk.c colblk.h colblk.ih

page.ih: page.idl
page.h: page.idl
page.obj: page.c page.h page.ih

ep.ih: ep.idl
ep.h: ep.idl
ep.obj: ep.c tpword.h page.h txtep.h header.h footer.h ipep.h blep.h ep.h ep.ih

suep.ih: suep.idl
suep.h: suep.idl
suep.obj: suep.c suep.h suep.ih

footer.ih: footer.idl
footer.h: footer.idl
footer.obj: footer.c footer.ih page.h tpword.h

header.ih: header.idl
header.h: header.idl
header.obj: header.c header.h header.ih

txtep.ih: txtep.idl
txtep.h: txtep.idl
txtep.obj: txtep.c txtep.h txtep.ih

ipep.ih: ipep.idl
ipep.h: ipep.idl
ipep.obj: ipep.c ipep.h ipep.ih

blep.ih: blep.idl
blep.h: blep.idl
blep.obj: blep.c blep.h blep.ih

main.obj: main.c suep.h


