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
/* @(#) somx/tpword.c 2.6 12/26/95 17:56:34 [1/24/96 17:31:19] */

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

#define TPWord_Class_Source
#include <string.h>
#include "tpword.ih"

/* ************************************************************ */
/*
 *  Returns the type of a TPWord.
 */

SOM_Scope long SOMLINK tpwType(TPWord somSelf)
{
    /* TPWordData *somThis = TPWordGetData(somSelf); */
    TPWordMethodDebug("TPWord", "tpwType");

    if (_match(somSelf, "[[EOF]]"))
	return TP_EOF;
    else if (_match(somSelf, "[["))
	return TP_TOKEN;
    else if (_match(somSelf, "\n\n"))
	return TP_PARAGRAPH_BREAK;
    else if (_match(somSelf, "\n"))
	return TP_LINE_BREAK;
    else if (_match(somSelf, " "))
	return TP_BLANK_SPACE;
    else
	return TP_WORD;
}

TPWord readToken(fileMgr myFile)
{
    char buffer[100];
    long nxtChr;
    long n = 0;
    long brackets;
    TPWord newWord;
    nxtChr = _fmPeekChar(myFile, 0);

/* Check for string of blanks.
   --------------------------- */
    if (nxtChr == ' ') {
	while (_fmPeekChar(myFile, 0) == ' ') {
	    buffer[n++] = (char) _fmGetChar(myFile);
	}
    }
/* Check for string of newlines.
   ----------------------------- */
    else if (nxtChr == '\n') {
	while (_fmPeekChar(myFile, 0) == '\n') {
	    buffer[n++] = (char) _fmGetChar(myFile);
	}
    }
/* Check for EOF.
   -------------- */
    else if (nxtChr == EOF) {
	strcpy(buffer, "[[EOF]]");
	n = strlen(buffer);
    }
/* Check for special token.
   ------------------------ */
    else if ((nxtChr == '[') && (_fmPeekChar(myFile, 1) == '[')) {
	brackets = 0;
	while (brackets < 2) {
	    nxtChr = _fmGetChar(myFile);
	    buffer[n++] = (char) nxtChr;
	    if (nxtChr == ']')
		brackets++;
	    else
		brackets = 0;
	}
    }
/* Otherwise, handle as word.
   -------------------------- */
    else {
	for (;;) {
	    nxtChr = _fmPeekChar(myFile, 0);
	    if (nxtChr == ' ')
		break;
	    if (nxtChr == '\n')
		break;
	    if (nxtChr == EOF)
		break;
	    if (nxtChr == '[')
		break;
	    buffer[n++] = (char) _fmGetChar(myFile);
	}
    }
/* Return converted buffer.
   ------------------------ */
    buffer[n] = '\0';
    newWord = TPWordNew();
    _wordInit1(newWord, buffer);
    return newWord;
}
