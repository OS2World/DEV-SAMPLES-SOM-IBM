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
/* @(#) somx/ll.c 2.6 12/26/95 17:52:56 [1/24/96 17:31:15] */

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

#define linkedList_Class_Source
#include "link.h"
#include "ll.ih"
#define MAX_INT 30000

/* ************************************************************ */
/*
 *  Make the head of the list the current link, and
 *  return its contents.
 */

SOM_Scope baseType SOMLINK llHead(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llHead");

    if (!_nlinks)
	return (baseType) NULL;
    _currentLink = _headLink;
    return (_linkGetContents(_currentLink));
}

/* ************************************************************ */
/*
 *  Make the tail of the list the current link, and
 *  returns its contents.
 */

SOM_Scope baseType SOMLINK llTail(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llTail");

    if (!_nlinks)
	return (baseType) NULL;
    _currentLink = _tailLink;
    return (_linkGetContents(_currentLink));
}

/* ************************************************************ */
/*
 *  Return the number of links is the list.
 */

SOM_Scope long SOMLINK llLength(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llLength");
    return _nlinks;
}

/* ************************************************************ */
/*
 *  Set the maximum number of links the list can contain.
 */

SOM_Scope void SOMLINK llSetMax(linkedList somSelf, long newMax)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llSetMax");
    _max = newMax;
}

/* ************************************************************ */
/*
 *  Return the number of links that can still be added to the
 *  list.
 */

SOM_Scope long SOMLINK llLeft(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llLeft");
    return (_max - _llLength(somSelf));
}

/* ************************************************************ */
/*
 *  Make the link after current the new current link, and
 *  return its contents.
 */

SOM_Scope baseType SOMLINK llNext(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llNext");

    if (!_nlinks)
	return (baseType) NULL;
    if (_linkGetNext(_currentLink)) {
	_currentLink = _linkGetNext(_currentLink);
	return (_linkGetContents(_currentLink));
    }
    else {
	return (baseType) NULL;
    }
}

/* ************************************************************ */
/*
 *  Make the link before current the new current link, and
 *  return its contents.
 */

SOM_Scope baseType SOMLINK llPrevious(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llPrevious");

    if (!_nlinks)
	return (baseType) NULL;
    if (_linkGetPrevious(_currentLink)) {
	_currentLink = _linkGetPrevious(_currentLink);
	return (_linkGetContents(_currentLink));
    }
    else {
	return (baseType) NULL;
    }
}

/* ************************************************************ */
/*
 *  Return the contents of the current link.
 */

SOM_Scope baseType SOMLINK llRetrieve(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llRetrieve");

    if (!_nlinks)
	return (baseType) NULL;
    return (_linkGetContents(_currentLink));
}

/* ************************************************************ */
/*
 *  Replace the contents of the current list but this new
 *  element, and return a pointer to the new contents.
 */

SOM_Scope baseType SOMLINK llReplace(linkedList somSelf, baseType newElement)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llReplace");

    if (!_nlinks)
	return (baseType) NULL;
    _linkSetContents(_currentLink, newElement);
    return (_linkGetContents(_currentLink));
}

/* ************************************************************ */
/*
 *  Move the tail link to the head of the list, and return
 *  its contents.
 */

SOM_Scope baseType SOMLINK llPromoteTail(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    link oldTail;
    linkedListMethodDebug("linkedList", "llPromoteTail");
    if (!_nlinks)
	return (baseType) NULL;
    if (_nlinks == 1)
	return (_linkGetContents(_headLink));
    oldTail = _tailLink;
    _tailLink = _linkGetPrevious(_tailLink);
    _linkSetNext(_linkGetPrevious(oldTail), 0);
    _linkSetPrevious(oldTail, 0);
    _linkSetNext(oldTail, _headLink);
    _linkSetPrevious(_headLink, oldTail);
    _headLink = oldTail;
    _currentLink = _headLink;
}

/* ************************************************************ */
/*
 *  Add a link containing this new Element to the head of the
 *  list, and return a pointer to the new Element.
 */

SOM_Scope baseType SOMLINK llAddHead(linkedList somSelf, baseType newElement)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    link newLink = linkNew();
    linkedListMethodDebug("linkedList", "llAddHead");

    _linkSetContents(newLink, newElement);
    if (!_llLeft(somSelf)) {
	_llHead(somSelf);
	return (_llReplace(somSelf, newElement));
    }
    if (_llHead(somSelf)) {
	_linkSetPrevious(_currentLink, newLink);
    }
    else
	_tailLink = newLink;
    _linkSetNext(newLink, _currentLink);
    _headLink = _currentLink = newLink;
    _nlinks++;
    return (_linkGetContents(_currentLink));
}

/* ************************************************************ */
/*
 *  Add a link containing this new Element to the tail of the
 *  list, and return a pointer to the new Element.
 */

SOM_Scope baseType SOMLINK llAddTail(linkedList somSelf, baseType newElement)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    link newLink = linkNew();
    linkedListMethodDebug("linkedList", "llAddTail");

    _linkSetContents(newLink, newElement);
    if (!_llLeft(somSelf)) {
	_llTail(somSelf);
	return (_llReplace(somSelf, newElement));
    }
    if (_llTail(somSelf)) {
	_linkSetNext(_currentLink, newLink);
    }
    else
	_headLink = newLink;
    _linkSetPrevious(newLink, _currentLink);
    _tailLink = _currentLink = newLink;
    _nlinks++;
    return (_linkGetContents(_currentLink));
}

/* ************************************************************ */
/*
 *  Delete the link at the head of the list.
 */

SOM_Scope baseType SOMLINK llRemoveHead(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    baseType thisItem;
    linkedListMethodDebug("linkedList", "llRemoveHead");

    if (!_nlinks)
	return (baseType) NULL;
    thisItem = _llHead(somSelf);
    if (_nlinks == 1) {
	_somFree(_headLink);
	_headLink = _tailLink = _currentLink = 0;
    }
    if (_nlinks > 1) {
	_llNext(somSelf);
	_somFree(_headLink);
	_headLink = _currentLink;
	_linkSetPrevious(_headLink, 0);
    }
    _nlinks--;
    return thisItem;
}

/* ************************************************************ */
/*
 *  Returns TRUE if the current link is the head of the list,
 *  FALSE otherwise.
 */

SOM_Scope long SOMLINK llIsTail(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llIsTail");
    return (_currentLink == _tailLink);
}

/* ************************************************************ */
/*
 *  Frees the entire list, including the contents of each link.
 */

SOM_Scope void SOMLINK llFreeContents(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    baseType thisItem;

    linkedListMethodDebug("linkedList", "llFreeContents");

    while (thisItem = _llRemoveHead(somSelf))
	_somFree(thisItem);
}

/* ************************************************************ */
SOM_Scope void SOMLINK print(linkedList somSelf, FILE *outputFile)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    baseType thisItem;

    linkedListMethodDebug("linkedList", "print");
    thisItem = _llHead(somSelf);
    if (thisItem) {
	while (thisItem) {
	    _print(thisItem, outputFile);
	    thisItem = _llNext(somSelf);
	}
    }
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "somInit");
    parent_somInit(somSelf);

    _currentLink = 0;
    _headLink = 0;
    _tailLink = 0;
    _nlinks = 0;
    _max = MAX_INT;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(linkedList somSelf)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "somUninit");

    while (_llRemoveHead(somSelf))
	;
    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(linkedList somSelf, long level)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "somDumpSelfInt");
    parent_somDumpSelfInt(somSelf, level);
}

/* ************************************************************ */
/*
 *  Trace the linkedList, useful for debugging.
 */

SOM_Scope void SOMLINK llTrace(linkedList somSelf, FILE *output)
{
    linkedListData *somThis = linkedListGetData(somSelf);
    linkedListMethodDebug("linkedList", "llTrace");
    fprintf(output, "\n");
    fprintf(output, " Linked List\n");
    fprintf(output, "        max: %d\n", _max);
    fprintf(output, "     nlinks: %d\n", _nlinks);
    fprintf(output, "currentLink: %x\n", _currentLink);
    fprintf(output, "   headLink: %x\n", _headLink);
    fprintf(output, "   tailLink: %x\n", _tailLink);

    _print(_headLink, output);
    _print(_currentLink, output);
    _print(_tailLink, output);
}
