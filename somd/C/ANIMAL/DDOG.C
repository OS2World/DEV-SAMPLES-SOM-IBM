/*
 *   COMPONENT_NAME: somx
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) somx/ddog.c 2.7 12/26/95 17:40:36 [1/24/96 17:31:02] */

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


#define dDog_Class_Source

#include <stdio.h>
#include <string.h>
#include "ddog.ih"

/*
 *  Noise dog makes.
 */

SOM_Scope void  SOMLINK _set_noise(dDog somSelf, Environment *ev,
		string noise)
{
    dDogData *somThis = dDogGetData(somSelf);
    dDogMethodDebug("dDog","_set_noise");

    if (_noise) SOMFree(_noise);
    _noise = (string) SOMMalloc(strlen(noise) + 1);
    strcpy(_noise, noise);
}

/*
 *  Trick dog performs.
 */

SOM_Scope void  SOMLINK _set_trick(dDog somSelf, Environment *ev,
		string trick)
{
    dDogData *somThis = dDogGetData(somSelf);
    dDogMethodDebug("dDog","_set_trick");

    if (_trick) SOMFree(_trick);
    _trick = (string) SOMMalloc(strlen(trick) + 1);
    strcpy(_trick, trick);
}

SOM_Scope string  SOMLINK display(dDog somSelf, Environment *ev)
{
    string mybuff, bp;

    dDogData *somThis = dDogGetData(somSelf);
    dDogMethodDebug("dDog","display");

    mybuff = parent_display(somSelf, ev);
    bp = mybuff;
    bp += strlen(mybuff);
    sprintf(bp, "My trick is %s.\n", _trick);
    return mybuff;
}

SOM_Scope void SOMLINK somDefaultInit(dDog somSelf, somInitCtrl* ctrl)
{
    dDogData *somThis; /* set in BeginInitializer */
    Environment *ev;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    dDogMethodDebug("dDog","somDefaultInit");
    dDog_BeginInitializer_somDefaultInit;

    dDog_Init_dAnimal_somDefaultInit(somSelf, ctrl);

    ev = somGetGlobalEnvironment();
    _set_noise(somSelf, ev, "unknown noise");
    _set_trick(somSelf, ev, "unknown trick");
}

SOM_Scope void SOMLINK somDestruct(dDog somSelf, octet doFree, somDestructCtrl* ctrl)
{
   dDogData *somThis; /* set in BeginDestructor */
   somDestructCtrl globalCtrl;
   somBooleanVector myMask;
   dDogMethodDebug("dDog","somDestruct");
   dDog_BeginDestructor;

   if (_trick) SOMFree (_trick);
   if (_noise) SOMFree (_noise);

   dDog_EndDestructor;
}
