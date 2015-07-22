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
/* @(#) somx/word.c 2.7 12/26/95 17:57:30 [1/24/96 17:31:20] */

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

#define word_Class_Source
#include "word.ih"
#include <stdlib.h>
#include <string.h>

static long nNews = 0;
static long nDeletes = 0;
#define PARRAY_SIZE		1000
static string parray[PARRAY_SIZE];

/* ************************************************************ */
/*
 *  Initialize a word with a character string.
 */

SOM_Scope word SOMLINK wordInit1(word somSelf, string newWord)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "wordInit1");

    if (_storage)
	SOMFree(_storage);
    _storage = (string) SOMMalloc(strlen(newWord) + 1);
    strcpy(_storage, newWord);
    _length = strlen(newWord);
    parray[nNews] = _storage;
    nNews++;
    return somSelf;
}

/* ************************************************************ */
/*
 *  Initialize a word with a character and
 *  count.  The new word will contain a null
 *  terminated buffer containing count number of
 *  newChars.
 */

SOM_Scope word SOMLINK wordInit2(word somSelf, char newChar, long count)
{
    wordData *somThis = wordGetData(somSelf);
    long n;

    wordMethodDebug("word", "wordInit2");

    if (_storage)
	SOMFree(_storage);
    _storage = (string) SOMMalloc(count + 1);
    for (n = 0; n < count; n++) {
	_storage[n] = newChar;
    }
    _storage[n] = '\0';
    _length = strlen(_storage);
    parray[nNews] = _storage;
    nNews++;
    return somSelf;
}

/* ************************************************************ */
/*
 *  Translate a word to an integer, such as "123" to 123.
 */

SOM_Scope long SOMLINK wordToInt(word somSelf)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "wordToInt");
    return atoi(_storage);
}

/* ************************************************************ */

SOM_Scope boolean SOMLINK match(word somSelf, string target)
{
    wordData *somThis = wordGetData(somSelf);
    long targetLength = strlen(target);
    wordMethodDebug("word", "match");
    return (!strncmp(_storage, target, targetLength));
}

/* ************************************************************ */
SOM_Scope void SOMLINK print(word somSelf, FILE *outputFile)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "print");

    fprintf(outputFile, "%s", _storage);
    fflush(outputFile);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somInit(word somSelf)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "somInit");
    parent_somInit(somSelf);
    _storage = 0;
}

/* ************************************************************ */
SOM_Scope void SOMLINK somUninit(word somSelf)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "somUninit");
    if (_storage)
	SOMFree(_storage);
    parent_somUninit(somSelf);
}

/* ************************************************************ */
SOM_Scope void SOMLINK somDumpSelfInt(word somSelf, long level)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "somDumpSelfInt");
    parent_somDumpSelfInt(somSelf, level);
}

void showWordStats()
{
    long n;
    printf("\n");
    printf("   nNews: %d\n", nNews);
    printf("nDeletes: %d\n", nDeletes);
    for (n = 0; n < PARRAY_SIZE; n++) {
	if (parray[n]) {
	    printf("n: %ld storage: %s\n", n, parray[n]);
	}
    }
}


/* ************************************************************ */
/*
 *  Replace the contents of a word by another word.
 */

SOM_Scope void SOMLINK wrdReplace(word somSelf, string newWord)
{
    wordData *somThis = wordGetData(somSelf);
    wordMethodDebug("word", "wrdReplace");

    _length = strlen(newWord);
    if (_storage)
	SOMFree(_storage);
    _storage = (string) SOMMalloc(_length + 1);
    strcpy(_storage, newWord);
}
