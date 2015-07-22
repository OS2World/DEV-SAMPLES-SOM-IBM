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
/* @(#) somx/footer.c 2.6 12/26/95 17:53:28 [1/24/96 17:31:16] */

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

#define footer_Class_Source
#include "footer.ih"

#include "page.h"
#include "tpword.h"

/* ************************************************************ */
SOM_Scope void SOMLINK epProcessEnvironment(footer somSelf)
{
/* footerData *somThis = footerGetData(somSelf); */
    page thisPage;
    textLine thisFt;
    TPWord thisWord;

    footerMethodDebug("footer", "epProcessEnvironment");

    thisPage = __get_currentPage(somSelf);
    thisFt = _pgGetFooterBlock(thisPage);

    for (;;) {
	thisWord = readToken(__get_currentFileMgr(somSelf));
	if (_match(thisWord, "[[end_environment]]")) {
	    _somFree(thisWord);
	    return;
	}
	if (_tpwType(thisWord) == TP_LINE_BREAK) {
	    _somFree(thisWord);
	    continue;
	}
	_llAddTail(thisFt, thisWord);
	if (_match(thisWord, "[[page_number]]"))
	    _pgSetPageNumber(thisPage, thisWord);
    }
}
