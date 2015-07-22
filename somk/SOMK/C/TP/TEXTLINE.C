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
/* @(#) somx/textline.c 2.6 12/26/95 17:56:01 [1/24/96 17:31:18] */

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

#define textLine_Class_Source
#include "textline.ih"
#include "helper.h"
#include "tpword.h"

/* ************************************************************ */
/*
 *  Initialize a new text line with the number of characters
 *  it can contain.
 */

SOM_Scope void SOMLINK tlInit(textLine somSelf, long newSize)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "tlInit");
    _totalChars = 0;
    _maxSize = newSize;
}

/* ************************************************************ */
/*
 *  Returns the number of characters in a textLine.
 */

SOM_Scope long SOMLINK tlCharsInLine(textLine somSelf)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "tlCharsInLine");
    return _totalChars;
}

/* ************************************************************ */
/*
 *  Returns the number of characters that can still be placed
 *  in the textLine.
 */

SOM_Scope long SOMLINK tlCharsLeft(textLine somSelf)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "tlCharsLeft");
    return _maxSize - _totalChars;
}

/* ************************************************************ */
SOM_Scope baseType SOMLINK llAddTail(textLine somSelf, baseType newElement)
{
    textLineData *somThis = textLineGetData(somSelf);
    TPWord myWord = (TPWord) newElement;
    long newTotal;
    textLineMethodDebug("textLine", "llAddTail");

    newTotal = _totalChars + __get_length(newElement);
    if (newTotal <= _maxSize) {
	_totalChars = newTotal;
	return (parent_llAddTail(somSelf, newElement));
    }
    else
	return newElement;
}

/* ************************************************************ */
SOM_Scope baseType SOMLINK llAddHead(textLine somSelf, baseType newElement)
{
    textLineData *somThis = textLineGetData(somSelf);
    TPWord myWord = (TPWord) newElement;
    long newTotal;
    textLineMethodDebug("textLine", "llAddHead");

    newTotal = _totalChars + __get_length(newElement);
    if (newTotal <= _maxSize) {
	_totalChars = newTotal;
	return (parent_llAddHead(somSelf, newElement));
    }
    else
	return newElement;
}

/* ************************************************************ */
SOM_Scope baseType SOMLINK llReplace(textLine somSelf, baseType newElement)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "llReplace");
    shouldHaveOverridden(somSelf, "llReplace");
    newElement;
    return (baseType) NULL;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(textLine somSelf)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "somInit");
    parent_somInit(somSelf);
    _maxSize = 0;
    _totalChars = 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(textLine somSelf)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "somUninit");

    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(textLine somSelf, long level)
{
    textLineData *somThis = textLineGetData(somSelf);
    textLineMethodDebug("textLine", "somDumpSelfInt");

    parent_somDumpSelfInt(somSelf, level);
}
