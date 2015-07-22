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
/* @(#) somx/txtep.c 2.6 12/26/95 17:56:57 [1/24/96 17:31:20] */

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

#define txtEnvProcessor_Class_Source
#include "txtep.ih"
#include "tpword.h"
#include "textline.h"
#include "page.h"
#include "colblk.h"

#define PARAGRAPH_INDENTATION 0
#define LINE_INDENTATION 0

/* ************************************************************ */
/*
 *  Default word processor: Add it to the current line, if
 *  space is available, otherwise create a new line.
 */

SOM_Scope void SOMLINK tpProcessWord(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    textLine thisLine = __get_currentLine(somSelf);
    page thisPage;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessWord");

    thisPage = __get_currentPage(somSelf);
    _thisCB = _pgGetColumnBlock(thisPage);
    if (!_cbLinesLeft(_thisCB))
	_tpProcessFullPage(somSelf);
    if (__get_length(_thisWord) > _tlCharsLeft(thisLine)) {
	_tpProcessFullLine(somSelf);
	_tpProcessNewLine(somSelf);
	thisLine = __get_currentLine(somSelf);
    }
    _llAddTail(thisLine, (baseType) _thisWord);
}

/* ************************************************************ */
/*
 *  Default blank processor: Add to current line, if
 *  space is available, otherwise discard (ie don't
 *  use in a new line).
 */

SOM_Scope void SOMLINK tpProcessBlanks(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    textLine thisLine = __get_currentLine(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessBlanks");

    if (!_tlCharsLeft(thisLine)) {
	_somFree(_thisWord);
	return;
    }
    _tpProcessWord(somSelf);

}

/* ************************************************************ */
/*
 *  Default line break processor: Treat line breaks as
 *  blanks (ie an input line break does not cause a line
 *  break in the output file).
 */

SOM_Scope void SOMLINK tpProcessLineBreak(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    textLine thisLine = __get_currentLine(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessLineBreak");

    _somFree(_thisWord);
    if (!_tlCharsInLine(thisLine))
	return;
    if (!_tlCharsLeft(thisLine))
	return;
    _thisWord = TPWordNew();
    _wordInit1(_thisWord, " ");
    _tpProcessWord(somSelf);
}

/* ************************************************************ */
/*
 *  Default full line processor: Add full input line to
 *  columnBlock and start a new line.
 */

SOM_Scope void SOMLINK tpProcessFullLine(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    textLine thisLine = __get_currentLine(somSelf);
    page thisPage;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessFullLine");

    thisPage = __get_currentPage(somSelf);
    _thisCB = _pgGetColumnBlock(thisPage);
    _llAddTail(_thisCB, (baseType) thisLine);

    thisLine = textLineNew();
    _tlInit(thisLine, __get_columnWidth(_thisCB));
    __set_currentLine(somSelf, thisLine);
}

/* ************************************************************ */
/*
 *  Default full page processor: Print the page and start
 *  another.
 */

SOM_Scope void SOMLINK tpProcessFullPage(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    page thisPage;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessFullPage");

    thisPage = __get_currentPage(somSelf);
    _pgPrint(thisPage, stdout);
    _thisCB = _pgGetColumnBlock(thisPage);
}

/* ************************************************************ */
/*
 *  Default new line processor: Handles a new output line.
 *  Default is indent line 2 characters.
 */

SOM_Scope void SOMLINK tpProcessNewLine(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    long charsToIndent;
    TPWord blanks;
    textLine thisLine;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessNewLine");

    charsToIndent = __get_lineIndentation(somSelf);
    if (charsToIndent) {
	thisLine = __get_currentLine(somSelf);
	blanks = TPWordNew();
	_wordInit2(blanks, ' ', charsToIndent);
	_llAddTail(thisLine, (baseType) blanks);
    }
}

/* ************************************************************ */
/*
 *  Default new paragraph processor: Breaks the output line.
 *  Starts a new output line.
 */

SOM_Scope void SOMLINK tpProcessNewParagraph(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    long charsToIndent;
    TPWord blanks;
    textLine thisLine;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "tpProcessNewParagraph");

    charsToIndent = __get_paragraphIndentation(somSelf);
    _tpProcessFullLine(somSelf);
    _tpProcessNewLine(somSelf);
    thisLine = __get_currentLine(somSelf);

    if (charsToIndent) {
	blanks = TPWordNew();
	_wordInit2(blanks, ' ', charsToIndent);
	_llAddTail(thisLine, (baseType) blanks);
    }
    _somFree(_thisWord);
}

/* ************************************************************ */
/*
 *  Gets the current line.
 */

SOM_Scope textLine SOMLINK _get_currentLine(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "_get_currentLine");

    if (!_currentLine) {
	_currentLine = textLineNew();
	_tlInit(_currentLine, __get_columnWidth(_thisCB));
    }
    return _currentLine;
}

/* ************************************************************ */
SOM_Scope void SOMLINK epProcessEnvironment(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);

/* Declare local variables.
   ------------------------ */
    page thisPage;
    fileMgr thisFile;
    long n = 0;
    thisPage = __get_currentPage(somSelf);
    thisFile = __get_currentFileMgr(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "epProcessEnvironment");

/* Prepare for looping.
   -------------------- */
    _epInitializeEnvironment(somSelf);

/* Loop until end of file.
   ----------------------- */
    while (_thisWord = readToken(thisFile)) {
/*    _print(_thisWord, stdout); */
	if (_tpwType(_thisWord) == TP_EOF) {
	    _somFree(_thisWord);
	    break;
	}
	if (_match(_thisWord, "[[end_environment]]")) {
	    _epShutdownEnvironment(somSelf);
	    break;
	}
	switch (_tpwType(_thisWord)) {
	    case TP_LINE_BREAK:
		_tpProcessLineBreak(somSelf);
		continue;
	    case TP_WORD:
		_tpProcessWord(somSelf);
		continue;
	    case TP_PARAGRAPH_BREAK:
		_tpProcessNewParagraph(somSelf);
		continue;
	    case TP_BLANK_SPACE:
		_tpProcessBlanks(somSelf);
		continue;
	    case TP_TOKEN:
		_epStartUpNewEnvironment(somSelf, _thisWord);
		_somFree(_thisWord);
		continue;
	}
    }
    _tpProcessFullLine(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK epShutdownEnvironment(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    envProcessor lastEnv;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "epShutdownEnvironment");

    lastEnv = __get_previousEnv(somSelf);
    if (lastEnv)
	_epInitializeEnvironment(lastEnv);
}

/* ************************************************************ */
SOM_Scope void SOMLINK epInitializeEnvironment(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    long charsToIndent;
    TPWord blanks;
    TPWord lineFeed;
    textLine thisLine;
    fileMgr thisFile;

    txtEnvProcessorMethodDebug("txtEnvProcessor", "epInitializeEnvironment");

    thisFile = __get_currentFileMgr(somSelf);
    lineFeed = readToken(thisFile);
    _somFree(lineFeed);
    charsToIndent = __get_paragraphIndentation(somSelf);
    _tpProcessNewLine(somSelf);

    thisLine = __get_currentLine(somSelf);
    if (charsToIndent) {
	blanks = TPWordNew();
	_wordInit2(blanks, ' ', charsToIndent);
	_llAddTail(thisLine, (baseType) blanks);
    }
}

/* ************************************************************ */
SOM_Scope void SOMLINK epPrepareForNewEnvironment(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "epPrepareForNewEnvironment");
    _tpProcessFullLine(somSelf);
}

/* ************************************************************ */
SOM_Scope long SOMLINK _get_paragraphIndentation(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    envProcessor previousEnv;
    txtEnvProcessorMethodDebug("txtEnvProcessor",
			       "_get_paragraphIndentation");

    previousEnv = __get_previousEnv(somSelf);
    if (previousEnv)
	return __get_paragraphIndentation(previousEnv) +
	    PARAGRAPH_INDENTATION;
    else
	return 0;
}

/* ************************************************************ */
SOM_Scope long SOMLINK _get_lineIndentation(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    envProcessor previousEnv;
    txtEnvProcessorMethodDebug("txtEnvProcessor", "_get_lineIndentation");

    previousEnv = __get_previousEnv(somSelf);
    if (previousEnv)
	return __get_lineIndentation(previousEnv) + LINE_INDENTATION;
    else
	return 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "somUninit");
    if (_thisWord)
	_somFree(_thisWord);
    if (_currentLine)
	_somFree(_currentLine);
    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(txtEnvProcessor somSelf)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    txtEnvProcessorMethodDebug("txtEnvProcessor", "somInit");
    parent_somInit(somSelf);

    _thisWord = 0;
    _currentLine = 0;
    _thisCB = 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK epInit1(txtEnvProcessor somSelf, envProcessor lastEnv)
{
    txtEnvProcessorData *somThis = txtEnvProcessorGetData(somSelf);
    page thisPage;

    txtEnvProcessorMethodDebug("txtEnvProcessor", "epInit1");
    parent_epInit1(somSelf, lastEnv);

    thisPage = __get_currentPage(somSelf);
    _thisCB = _pgGetColumnBlock(thisPage);
}
