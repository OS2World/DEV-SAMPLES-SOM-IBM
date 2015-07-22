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
/* @(#) somx/link.c 2.6 12/26/95 17:54:56 [1/24/96 17:31:17] */

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

#define link_Class_Source
#include "link.ih"

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(link somSelf)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "somInit");
    parent_somInit(somSelf);

    _next = 0;
    _previous = 0;
    _contents = 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(link somSelf, long level)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "somDumpSelfInt");
    parent_somDumpSelfInt(somSelf, level);
}

/* ************************************************************ */
/*
 *  Set the next pointer on target link to new link.
 */

SOM_Scope void SOMLINK linkSetNext(link somSelf, link nextLink)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "linkSetNext");
    _next = nextLink;
}

/* ************************************************************ */
/*
 *  Set the previous pointer on target link to new link.
 */

SOM_Scope void SOMLINK linkSetPrevious(link somSelf, link nextLink)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "linkSetPrevious");
    _previous = nextLink;
}

/* ************************************************************ */
/*
 *  Set the contents pointer on target link to new object.
 */

SOM_Scope void SOMLINK linkSetContents(link somSelf, baseType newContents)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "linkSetContents");
    _contents = newContents;
}

/* ************************************************************ */
/*
 *  Get the next link after the target link.
 */

SOM_Scope link SOMLINK linkGetNext(link somSelf)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "linkGetNext");
    return (_next);
}

/* ************************************************************ */
/*
 *  Get the previous link before the target link.
 */

SOM_Scope link SOMLINK linkGetPrevious(link somSelf)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "linkGetPrevious");
    return (_previous);
}

/* ************************************************************ */
/*
 *  Get the contents of the target link.
 */

SOM_Scope baseType SOMLINK linkGetContents(link somSelf)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "linkGetContents");
    return (_contents);
}

/* ************************************************************ */
SOM_Scope void SOMLINK print(link somSelf, FILE *outputFile)
{
    linkData *somThis = linkGetData(somSelf);
    linkMethodDebug("link", "print");
    fprintf(outputFile, "\n");
    fprintf(outputFile, "    Link\n");
    fprintf(outputFile, "    self: %x \n", somSelf);
    fprintf(outputFile, "    next: %x \n", _next);
    fprintf(outputFile, "previous: %x\n", _previous);
    fprintf(outputFile, "contents: %x\n", _contents);
}
