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
/* @(#) somx/dloc.c 2.8 12/26/95 17:41:19 [1/24/96 17:31:03] */

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


#define dLocation_Class_Source

#include <string.h>
#include "dloc.ih"

/*
 * Method from the IDL attribute statement:
 * "attribute string city"
 */

SOM_Scope void  SOMLINK _set_city(dLocation somSelf, Environment *ev,
		string city)
{
    dLocationData *somThis = dLocationGetData(somSelf);
    dLocationMethodDebug("dLocation","_set_city");

    if (_city) SOMFree(_city);
    _city = (string) SOMMalloc(strlen(city) + 1);
    strcpy(_city, city);
}

/*
 * Method from the IDL attribute statement:
 * "attribute string state"
 */

SOM_Scope void  SOMLINK _set_state(dLocation somSelf, Environment *ev,
		string state)
{
    dLocationData *somThis = dLocationGetData(somSelf);
    dLocationMethodDebug("dLocation","_set_state");

    if (_state) SOMFree(_state);
    _state = (string) SOMMalloc(strlen(state) + 1);
    strcpy(_state, state);
}
SOM_Scope void SOMLINK somDefaultInit(dLocation somSelf, somInitCtrl* ctrl)
{
    dLocationData *somThis; /* set in BeginInitializer */
    Environment *ev;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    dLocationMethodDebug("dLocation","somDefaultInit");
    dLocation_BeginInitializer_somDefaultInit;

    dLocation_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    ev = somGetGlobalEnvironment();

    _set_city(somSelf, ev, "unknown city");
    _set_state(somSelf, ev, "unknown state");
}

SOM_Scope void SOMLINK somDestruct(dLocation somSelf, octet doFree, somDestructCtrl* ctrl)
{
   dLocationData *somThis; /* set in BeginDestructor */
   somDestructCtrl globalCtrl;
   somBooleanVector myMask;
   dLocationMethodDebug("dLocation","somDestruct");
   dLocation_BeginDestructor;

   if (_city) SOMFree(_city);
   if (_state) SOMFree(_state);

   dLocation_EndDestructor;
}
