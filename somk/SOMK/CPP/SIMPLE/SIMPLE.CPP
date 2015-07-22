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
/* @(#) somx/simple.cpp 2.8 12/26/95 17:59:36 [1/24/96 17:31:23] */

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

#ifndef SOM_Module_simple_Source
#define SOM_Module_simple_Source
#endif
#define Simple_Class_Source
#define VARIABLE_MACROS
#include "simple.xih"



/*
 * an initializer for Simple. sets x and y.
 */
SOM_Scope void SOMLINK withXandY(Simple *somSelf, somInitCtrl* ctrl, 
                                 long xValue, long yValue)
{
    SimpleData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    SimpleMethodDebug("Simple","withXandY");
    Simple_BeginInitializer_withXandY;

    Simple_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /* 
     * local Simple initialization code added by programmer 
     */
    _x = xValue; _y = yValue;
}



/*
 * an initializer for Simple. sets y = x+1.
 */
SOM_Scope void SOMLINK withX(Simple *somSelf, somInitCtrl* ctrl, 
                             long xValue)
{
    SimpleData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    SimpleMethodDebug("Simple","withX");
    Simple_BeginInitializer_withX;

    Simple_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /* 
     * local Simple initialization code added by programmer 
     */
     somThis->x = xValue; somThis->y = _x + 1;
}



/*
 * sets x = y = 42;
 */
SOM_Scope void SOMLINK somDefaultInit(Simple *somSelf, som3InitCtrl* ctrl)
{
    SimpleData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    SimpleMethodDebug("Simple","somDefaultInit");
    Simple_BeginInitializer_somDefaultInit;

    Simple_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /* 
     * local Simple initialization code added by programmer 
     */
    _x = _y = 42;
}



/*
 * for efficiency
 */
SOM_Scope void SOMLINK somDestruct(Simple *somSelf, octet doFree, 
                                   som3DestructCtrl* ctrl)
{
    SimpleData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    SimpleMethodDebug("Simple","somDestruct");
    Simple_BeginDestructor;

    /* 
     * local Simple deinitialization code added by programmer 
     */ 
    /* 
     * nothing to do, but this is a lot faster than not
     * overriding somDestruct and letting the generic support
     * run.
     */

    Simple_EndDestructor;
}


