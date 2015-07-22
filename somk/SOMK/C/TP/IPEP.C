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
/* @(#) somx/ipep.c 2.6 12/26/95 17:54:34 [1/24/96 17:31:17] */

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

#define indentedParagraphs_Class_Source
#include "ipep.ih"

#define PARAGRAPH_INDENTATION 3
#define LINE_INDENTATION 0

/* ************************************************************ */
SOM_Scope long  SOMLINK _get_paragraphIndentation(indentedParagraphs somSelf)
{
    /* indentedParagraphsData *somThis = indentedParagraphsGetData(somSelf); */
    indentedParagraphsMethodDebug("indentedParagraphs", 
	"_get_paragraphIndentation");
    SOM_IgnoreWarning (somSelf);
    return PARAGRAPH_INDENTATION;
}

/* ************************************************************ */
SOM_Scope long SOMLINK _get_lineIndentation(indentedParagraphs somSelf)
{
    /* indentedParagraphsData *somThis = indentedParagraphsGetData(somSelf); */
    envProcessor lastEnv;
    indentedParagraphsMethodDebug("indentedParagraphs",
	"_get_lineIndentation");
    lastEnv = __get_previousEnv(somSelf);
    if (lastEnv)
	return __get_lineIndentation(lastEnv) + LINE_INDENTATION;
    else
	return LINE_INDENTATION;

}
