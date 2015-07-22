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
/* @(#) somx/suep.c 2.6 12/26/95 17:55:50 [1/24/96 17:31:18] */

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

#define setUpEnvProcessor_Class_Source
#include "suep.ih"

/* ************************************************************ */
/*
 *  Used to process set up options.
 */

SOM_Scope void SOMLINK suProcessOptions(setUpEnvProcessor somSelf)
{
    long n = 0;
    fileMgr myfm;
    page thisPage;
    TPWord thisWord;

    setUpEnvProcessorMethodDebug("setUpEnvProcessor", "suProcessOptions");
    thisPage = __get_currentPage(somSelf);
    myfm = __get_currentFileMgr(somSelf);

    for (;;) {
	thisWord = readToken(myfm);
        /*  _print(thisWord, stdout); */
	if (_tpwType(thisWord) == TP_EOF) {
	    _somFree(thisWord);
	    break;
	}
	else if (_tpwType(thisWord) == TP_TOKEN)
	    _epStartUpNewEnvironment(somSelf, thisWord);
	else if (_match(thisWord, "width"))
	    _pgSetWidth(thisPage, _suReadIntWord(somSelf));
	else if (_match(thisWord, "height"))
	    _pgSetHeight(thisPage, _suReadIntWord(somSelf));
	else if (_match(thisWord, "columns"))
	    _pgSetNumberOfColumns(thisPage, _suReadIntWord(somSelf));
	_somFree(thisWord);
    }
    thisPage = __get_currentPage(somSelf);
    _pgPrint(thisPage, stdout);
}

/* ************************************************************ */
/*
 *  Used to read an integer word.
 */

SOM_Scope long SOMLINK suReadIntWord(setUpEnvProcessor somSelf)
{
    TPWord thisWord;
    long returnValue;
    setUpEnvProcessorMethodDebug("setUpEnvProcessor", "suReadIntWord");

    thisWord = readToken(__get_currentFileMgr(somSelf));  /* Blank */
    _somFree(thisWord);
    thisWord = readToken(__get_currentFileMgr(somSelf));  /* Number */
    returnValue = _wordToInt(thisWord);
    _somFree(thisWord);
    return returnValue;
}
