//
//   COMPONENT_NAME: somx
//
//   ORIGINS: 27
//
//
//    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//
#ifndef lint
static char *sccsid = "@(#) 1.5 src/somples.os2/somd/cpp/animal/dloc.cpp, somx.somd, som2.5, s259604a 12/26/95 20:29:01 [1/24/96 17:50:55]";
#endif

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
 *
 *  DISCLAIMER OF WARRANTIES:  The following [enclosed] code is sample
 *  code created by IBM Corporation.  This sample code is not part of any
 *  standard or IBM product and is provided to you solely for the purpose
 *  of assisting you in the development of your applications.  This code
 *  is provided "AS IS", without warranty of any kind.  IBM shall not be
 *  liable for any damages arising out of your use of the sample code,
 *  even if they have been advised of the possibility of such damages.
 */

#define dLocation_Class_Source

#include <string.h>
#include "dloc.xih"


/*
 * Method from the IDL attribute statement:
 * "attribute string city"
 */

/*
 * City where animal was born.
 */

SOM_Scope void  SOMLINK _set_city(dLocation *somSelf, Environment *ev,
		string city)
{
    dLocationData *somThis = dLocationGetData(somSelf);
    dLocationMethodDebug("dLocation","_set_city");

    if (somThis->city) SOMFree(somThis->city);
    somThis->city = (string) SOMMalloc(strlen(city) + 1);
    strcpy(somThis->city, city);
}


/*
 * Method from the IDL attribute statement:
 * "attribute string state"
 */

/*
 * State where animal was born.
 */

SOM_Scope void  SOMLINK _set_state(dLocation *somSelf, Environment *ev,
		string state)
{
    dLocationData *somThis = dLocationGetData(somSelf);
    dLocationMethodDebug("dLocation","_set_state");

    if (somThis->state) SOMFree(somThis->state);
    somThis->state = (string) SOMMalloc(strlen(state) + 1);
    strcpy(somThis->state, state);
}
SOM_Scope void SOMLINK somDefaultInit(dLocation *somSelf, somInitCtrl* ctrl)
{
    dLocationData *somThis; /* set in BeginInitializer */
    Environment *ev;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    dLocationMethodDebug("dLocation","somDefaultInit");
    dLocation_BeginInitializer_somDefaultInit;

    dLocation_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    ev = somGetGlobalEnvironment();
    somSelf->_set_city(ev, "unknown city");
    somSelf->_set_state(ev, "unknown state");
}
SOM_Scope void SOMLINK somDestruct(dLocation *somSelf, octet doFree,
				   somDestructCtrl* ctrl)
{
   dLocationData *somThis; /* set in BeginDestructor */
   somDestructCtrl globalCtrl;
   somBooleanVector myMask;
   dLocationMethodDebug("dLocation","somDestruct");
   dLocation_BeginDestructor;

    if (somThis->city) SOMFree(somThis->city);
    if (somThis->state) SOMFree(somThis->state);

  dLocation_EndDestructor;
}
