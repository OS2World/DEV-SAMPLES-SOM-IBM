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
static char *sccsid = "@(#) 1.6 src/somples.os2/somd/cpp/animal/dbdog.cpp, somx.somd, som2.5, s259604a 12/26/95 20:28:25 [1/24/96 17:50:55]";
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


#define dBigDog_Class_Source

#include <stdio.h>
#include <string.h>
#include "dbdog.xih"


SOM_Scope string  SOMLINK display(dBigDog *somSelf, Environment *ev)
{
    string mybuff, bp;

    dBigDogData *somThis = dBigDogGetData(somSelf);
    dBigDogMethodDebug("dBigDog","display");

    mybuff = parent_display(somSelf, ev);
    bp = mybuff;
    bp += strlen(mybuff);
    sprintf(bp, "I weigh %.2f pounds.\n", somThis->BDweight);
    return mybuff;
}

SOM_Scope void SOMLINK somDefaultInit(dBigDog *somSelf, somInitCtrl* ctrl)
{
    dBigDogData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    dBigDogMethodDebug("dBigDog","somDefaultInit");
    dBigDog_BeginInitializer_somDefaultInit;

    dBigDog_Init_dDog_somDefaultInit(somSelf,ctrl);
    somSelf->_set_noise(somGetGlobalEnvironment(), "WOOF, WOOF");
}

