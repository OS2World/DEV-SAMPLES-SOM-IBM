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
static char *sccsid = "@(#) 1.6 src/somples.os2/somd/cpp/proxy/bar.cpp, somx.somd, som2.5, s259604a 12/26/95 20:33:00 [1/24/96 17:50:59]";
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

#define Bar_Class_Source
#include <somdtype.xh>
#include "bar.xih"


SOM_Scope string  SOMLINK method1(Bar *somSelf,
                                  Environment *ev,
                                  string* arg1,
                                  long* arg2,
                                  ReferenceData* arg3)
{

    BarData *somThis = BarGetData(somSelf);
    BarMethodDebug("Bar","method1");

    /* set return value for arg1 */

    *arg1 = "abc";

    /* set return value for arg2 */

    somPrintf("Bar: arg2 input value  = %d\n", *arg2);
    somPrintf("Bar: arg3 maximum      = %d\n", arg3->_maximum);
    somPrintf("Bar: arg3 length       = %d\n", arg3->_length);

    if (arg3->_buffer)
    {
       somPrintf("Bar: arg3 buffer       = %s\n", arg3->_buffer);
    }

    *arg2 =  arg3->_maximum + *arg2;
    somPrintf("Bar: arg2 output value = %d\n", *arg2);

    /* return results */

    return "method1 completed";
}

SOM_Scope void SOMLINK somDefaultInit(Bar *somSelf, somInitCtrl* ctrl)
{
    BarData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    BarMethodDebug("Bar","somDefaultInit");
    Bar_BeginInitializer_somDefaultInit;

    Bar_Init_SOMObject_somDefaultInit(somSelf, ctrl);
    somThis->attribute_long = 9999;
}

