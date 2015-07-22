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
/* @(#) somx/page.c 2.6 12/26/95 17:54:01 [1/24/96 17:31:16] */

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

#define page_Class_Source
#include <stdio.h>
#include "page.ih"

#define MAX_WORDS_IN_HEADER 100
#define MAX_WORDS_IN_FOOTER 100

/* ************************************************************ */
SOM_Scope void SOMLINK pgPrint(page somSelf, FILE *output)
{
    pageData *somThis = pageGetData(somSelf);
    char buffer[10];
    pageMethodDebug("page", "pgPrint");

/* Print header.
   ------------- */
    _pageNumber++;
    if (_pageWord) {
	sprintf(buffer, "%d", _pageNumber);
	_wrdReplace(_pageWord, buffer);
    }
    fprintf(output, "\n\f");
    if (_header)
	_print(_header, output);
    fprintf(output, "\n\n");

/* Print text.
   ----------- */
    if (_columns)
	_print(_columns, output);
    fprintf(output, "\n\n");

/* Print footer.
   ------------- */
    if (_footer)
	_print(_footer, output);

/* Clear out text from somThis column block.
   -------------------------------------- */
    _somFree(_columns);
    _columns = 0;
}

/* ************************************************************ */
/*
 *  Set the height of a page in characters.
 */

SOM_Scope void SOMLINK pgSetHeight(page somSelf, long newHeight)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgSetHeight");
    _height = newHeight;
}

/* ************************************************************ */
/*
 *  Set the width of a page in characters.
 */

SOM_Scope void SOMLINK pgSetWidth(page somSelf, long newWidth)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgSetWidth");
    _width = newWidth;

}

/* ************************************************************ */
/*
 *  Set the number of columns in a page.
 */

SOM_Scope void  SOMLINK pgSetNumberOfColumns(page somSelf, long ncolumns)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgSetNumberOfColumns");
    _ncolumns = ncolumns;
}

/* ************************************************************ */
/*
 *  Tell the page about a special word to be used to
 *  store page numbers.
 */

SOM_Scope void SOMLINK pgSetPageNumber(page somSelf, TPWord newPageWord)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgSetPageNumber");
    _pageWord = newPageWord;
}

/* ************************************************************ */
/*
 *  Get the columnBlock being used by this page.
 */

SOM_Scope columnBlock SOMLINK pgGetColumnBlock(page somSelf)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgGetColumnBlock");

    if (!_columns) {
	_columns = columnBlockNew();
	_cbInit(_columns, _ncolumns, _height, _width);
    }
    return _columns;
}

/* ************************************************************ */
/*
 *  Get the textLine representing the page header.
 */

SOM_Scope textLine SOMLINK pgGetHeaderBlock(page somSelf)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgGetHeaderBlock");

    if (!_header) {
	_header = textLineNew();
	_tlInit(_header, MAX_WORDS_IN_HEADER);
    }
    return _header;
}

/* ************************************************************ */
/*
 *  Get the textLine representing the page footer.
 */

SOM_Scope textLine SOMLINK pgGetFooterBlock(page somSelf)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "pgGetFooterBlock");
    if (!_footer) {
	_footer = textLineNew();
	_tlInit(_footer, MAX_WORDS_IN_FOOTER);
    }
    return _footer;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(page somSelf)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "somInit");

    parent_somInit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(page somSelf)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "somUninit");

    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(page somSelf, long level)
{
    pageData *somThis = pageGetData(somSelf);
    pageMethodDebug("page", "somDumpSelfInt");

    parent_somDumpSelfInt(somSelf, level);
}
