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
/* @(#) somx/colblk.c 2.6 12/26/95 17:52:22 [1/24/96 17:31:15] */

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

#define columnBlock_Class_Source
#include "colblk.ih"
#include "textline.h"

#define SPACE_BETWEEN 3
#define HEADER_SPACE 3
#define FOOTER_SPACE 3

/* ************************************************************ */
/*
 *    Initialize a new columnBlock.
 */

SOM_Scope void SOMLINK cbInit(columnBlock somSelf, long newColumns,
    long newHeight, long newWidth)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    long n;
    linkedList newBlock;
    columnBlockMethodDebug("columnBlock", "cbInit");

    _columnWidth = newWidth - ((newColumns - 1) * SPACE_BETWEEN);
    _columnWidth = _columnWidth / newColumns;

    _llSetMax(somSelf, newColumns);
    for (n = 0; n < newColumns; n++) {
	newBlock = linkedListNew();
	_llSetMax(newBlock, newHeight - HEADER_SPACE - FOOTER_SPACE);
	parent_llAddTail(somSelf, (baseType) newBlock);
    }
    _llHead(somSelf);
}

/* ************************************************************ */
/*
 *   Returns TRUE if there is room for at least one more
 *   line in the columnBlock, FALSE otherwise.
 */

SOM_Scope long SOMLINK cbLinesLeft(columnBlock somSelf)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    linkedList thisBlock;
    columnBlockMethodDebug("columnBlock", "cbLinesLeft");

    thisBlock = (linkedList) _llRetrieve(somSelf);
    return (_llLeft(thisBlock) || !_llIsTail(somSelf)) ? 1 : 0;
}

/* ************************************************************ */
SOM_Scope baseType SOMLINK llAddTail(columnBlock somSelf, baseType newLine)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    linkedList thisBlock;
    columnBlockMethodDebug("columnBlock", "llAddTail");

    thisBlock = (linkedList) _llRetrieve(somSelf);
    if (!thisBlock)
	return (baseType) NULL;
    if (!_llLeft(thisBlock)) {
	if (!(thisBlock = (linkedList) _llNext(somSelf))) {
	    return (baseType) NULL;
	}
    }
    return _llAddTail(thisBlock, newLine);
}

/* ************************************************************ */
SOM_Scope void SOMLINK print(columnBlock somSelf, FILE *output)
{
/* Set up.
   ------- */
    columnBlockData *somThis = columnBlockGetData(somSelf);
    int n, blockLength, blankLines;
    linkedList thisBlock;
    textLine thisLine;
    columnBlockMethodDebug("columnBlock", "print");

/* Determine number of lines in typical block.
   ------------------------------------------- */
    thisBlock = (linkedList) _llHead(somSelf);
    blockLength = _llLength(thisBlock);
    blankLines = _llLeft(thisBlock);

/* For each horizontal line...
   --------------------------- */
    for (n = 0; n < blockLength; n++) {

/*     For each text block...
       ---------------------- */
	thisBlock = (linkedList) _llHead(somSelf);
	while (thisBlock) {

/*	  Print the column line.
	  ---------------------- */
	    thisLine = (textLine) _llHead(thisBlock);
	    if (thisLine) {
		_print(thisLine, output);
		_llFreeContents(thisLine);
		_cbPrintBlanks(somSelf, output, _tlCharsLeft(thisLine));
	    }
	    else {
		_cbPrintBlanks(somSelf, output, __get_columnWidth(somSelf));
	    }
	    _llRemoveHead(thisBlock);

/*	  Print space between columns.
	  ---------------------------- */
	    _cbPrintBlanks(somSelf, output, SPACE_BETWEEN);

/*	  Move to next block.
	  ------------------- */
	    thisBlock = (linkedList) _llNext(somSelf);
	}
	fprintf(output, "\n");
    }
/* Print blank lines.
   ------------------ */
    for (n = 0; n < blankLines; n++) {
	fprintf(output, "\n");
    }

}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(columnBlock somSelf)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    columnBlockMethodDebug("columnBlock", "somInit");
    parent_somInit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(columnBlock somSelf)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    columnBlockMethodDebug("columnBlock", "somUninit");
    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(columnBlock somSelf, long level)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    columnBlockMethodDebug("columnBlock", "somDumpSelfInt");
    parent_somDumpSelfInt(somSelf, level);
}

/* ************************************************************ */
SOM_Scope void SOMLINK cbPrintBlanks(columnBlock somSelf, FILE *output,
    long nblanks)
{
    columnBlockData *somThis = columnBlockGetData(somSelf);
    long n;
    columnBlockMethodDebug("columnBlock", "cbPrintBlanks");

    for (n = 0; n < nblanks; n++) {
	putc(' ', output);
    }
}
