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
/* @(#) somx/ep.c 2.6 12/26/95 17:52:00 [1/24/96 17:31:14] */

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

#define envProcessor_Class_Source

#include <stdlib.h>

#include "helper.h"
#include "tpword.h"
#include "page.h"

#include "txtep.h"
#include "header.h"
#include "footer.h"
#include "ipep.h"
#include "blep.h"

#include "ep.ih"

/* ************************************************************ */
/*
 *  Used to initialize a new environment from an
 *  existing environment.
 */

SOM_Scope void SOMLINK epInit1(envProcessor somSelf, envProcessor lastEnv)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "epInit1");

    _previousEnv = lastEnv;
    _currentFileMgr = __get_currentFileMgr(_previousEnv);
    _currentPage = __get_currentPage(_previousEnv);

}

/* ************************************************************ */
/*
 *  Defines what it means to process a given
 *  environment.  This method MUST be overridden in
 *  derived classes.
 */

SOM_Scope void SOMLINK epProcessEnvironment(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "epProcessEnvironment");
    shouldHaveOverridden(somSelf, "epProcessEnvironment");
}

/* ************************************************************ */
/*
 *  Defines what steps need to be taken at the start
 *  of this environment.
 */

SOM_Scope void SOMLINK epStartUpNewEnvironment(envProcessor somSelf,
    TPWord envName)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessor nextEnv;
    envProcessorMethodDebug("envProcessor", "epStartUpNewEnvironment");

    if (_match(envName, "[[standard_text]]")) {
	nextEnv = txtEnvProcessorNew();
	_epInit1(nextEnv, somSelf);
    }
    else if (_match(envName, "[[header]]")) {
	nextEnv = headerNew();
	_epInit1(nextEnv, somSelf);
    }
    else if (_match(envName, "[[footer]]")) {
	nextEnv = footerNew();
	_epInit1(nextEnv, somSelf);
    }
    else if (_match(envName, "[[standard_text]]")) {
	nextEnv = txtEnvProcessorNew();
	_epInit1(nextEnv, somSelf);
    }
    else if (_match(envName, "[[indent_paragraphs]]")) {
	nextEnv = indentedParagraphsNew();
	_epInit1(nextEnv, somSelf);
    }
    else if (_match(envName, "[[bulleted_list]]")) {
	nextEnv = bulletedListNew();
	_epInit1(nextEnv, somSelf);
    }
    else {
	printf("Unknown environment: ");
	_print(envName, stdout);
	exit(2);
    }

    _epPrepareForNewEnvironment(somSelf);
    _epProcessEnvironment(nextEnv);
    _somFree(nextEnv);
}

/* ************************************************************ */
/*
 *  Defines what steps need to be taken to initialize
 *  a given environment.
 */

SOM_Scope void SOMLINK epInitializeEnvironment(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "epInitializeEnvironment");
}

/* ************************************************************ */
/*
 *  Defines what steps need to be taken to shutdown this
 *  environment.
 */

SOM_Scope void SOMLINK epShutdownEnvironment(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "epShutdownEnvironment");
}

/* ************************************************************ */
/*
 *  Defines what steps this environment should take as it
 *  prepares to transfer control to a new environment.
 */

SOM_Scope void SOMLINK epPrepareForNewEnvironment(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "epPrepareForNewEnvironment");
}

/* ************************************************************ */
/*
 *  Get the indentation for a new paragraph for this environment.
 */

SOM_Scope long SOMLINK _get_paragraphIndentation(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "_get_paragraphIndentation");
    return (0);
}

/* ************************************************************ */
/*
 *  Get the indentation for a new line for this environment.
 */

SOM_Scope long SOMLINK _get_lineIndentation(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "_get_lineIndentation");
    return (0);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "somInit");

    parent_somInit(somSelf);
    _previousEnv = 0;
    _currentFileMgr = 0;
    _currentPage = 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(envProcessor somSelf)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "somUninit");

    if (!_previousEnv) {
	_somFree(_currentFileMgr);
	_somFree(_currentPage);
	printf("\n");                  /* Last line feed for document. */
    }
    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(envProcessor somSelf, long level)
{
    envProcessorData *somThis = envProcessorGetData(somSelf);
    envProcessorMethodDebug("envProcessor", "somDumpSelfInt");

    parent_somDumpSelfInt(somSelf, level);
}
