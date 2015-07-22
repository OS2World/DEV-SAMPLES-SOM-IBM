/*
 *   COMPONENT_NAME: somx
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) somx/fm.c 2.6 12/26/95 17:52:33 [1/24/96 17:31:15] */

/*
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 * THIS CODE.  IBM shall not be liable for any damages arising out of
 * your use of the sample code, even if they have been advised of the
 * possibility of such damages.
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */

/*
 * This class is adapted from the book
 *   Class Construction in C and C++, Object Oriented Fundamentals
 *   by Roger Sessions, Copyright (c) 1992 Prentice Hall.
 * Reprinted with permission.
 */

#define fileMgr_Class_Source

#include <stdio.h>
#include <stdlib.h>
#include "fm.ih"

/* ************************************************************ */
/*
 *  Initialize and open a new file.
 */

SOM_Scope void SOMLINK fmInit(fileMgr somSelf, string newFile)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    fileMgrMethodDebug("fileMgr", "fmInit");
    if (!(_funit = fopen(newFile, "r"))) {
	printf("Error: unable to open input file \"%s\".\n", newFile);
	exit(3);
    }
}

/* ************************************************************ */
/*
 *  Get the next character in the file.
 */

SOM_Scope long SOMLINK fmGetChar(fileMgr somSelf)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    long newChar;
    fileMgrMethodDebug("fileMgr", "fmGetChar");

    if (_fmSize(somSelf)) {
	newChar = _buffer[_getSide];
	_getSide = _fmIncr(somSelf, _getSide);
    }
    else {
	newChar = getc(_funit);
    }
    return newChar;
}

/* ************************************************************ */
/*
 *  Peek ahead in the file.
 */

SOM_Scope long SOMLINK fmPeekChar(fileMgr somSelf, long offset)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    long newChar;
    long n;
    fileMgrMethodDebug("fileMgr", "fmPeekChar");

    for (;;) {
	n = _fmSize(somSelf);
	if (n > offset)
	    break;
	newChar = getc(_funit);
	_buffer[_putSide] = newChar;
	_putSide = _fmIncr(somSelf, _putSide);
    }
    offset = _fmAdd(somSelf, _getSide, offset);
    newChar = _buffer[offset];
    return newChar;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(fileMgr somSelf)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    fileMgrMethodDebug("fileMgr", "somInit");
    parent_somInit(somSelf);

    _funit = 0;
    _putSide = 0;
    _getSide = 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(fileMgr somSelf)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    fileMgrMethodDebug("fileMgr", "somUninit");

    if (_funit)
	fclose(_funit);

    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(fileMgr somSelf, long level)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    fileMgrMethodDebug("fileMgr", "somDumpSelfInt");
    parent_somDumpSelfInt(somSelf, level);
}

/* ************************************************************ */
SOM_Scope long SOMLINK fmSize(fileMgr somSelf)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    long result;
    fileMgrMethodDebug("fileMgr", "fmSize");

    if (_getSide == _putSide)
	result = 0;
    else if (_getSide < _putSide)
	result = _putSide - _getSide;
    else
	result = (BUFF_SIZE - _getSide - 1) + (_putSide + 1);
    return result;
}

/* ************************************************************ */
SOM_Scope long SOMLINK fmIncr(fileMgr somSelf, long oldNum)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    fileMgrMethodDebug("fileMgr", "fmIncr");

    if (oldNum == (BUFF_SIZE - 1))
	return 0;
    else
	return (oldNum + 1);
}

/* ************************************************************ */
SOM_Scope long SOMLINK fmAdd(fileMgr somSelf, long oldNum, long addNum)
{
    fileMgrData *somThis = fileMgrGetData(somSelf);
    fileMgrMethodDebug("fileMgr", "fmAdd");
    return ((oldNum + addNum) % BUFF_SIZE);
}
