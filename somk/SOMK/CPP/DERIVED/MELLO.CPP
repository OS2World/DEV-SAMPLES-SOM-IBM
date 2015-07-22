//
//   COMPONENT_NAME: somx
//
//   ORIGINS: 27
//
//
//    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//
/* @(#) somx/mello.cpp 2.6 12/26/95 17:59:05 [1/24/96 17:31:22] */

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

#define Mello_Class_Source
#include "mello.xih"
#include <stdio.h>

/*
 * -- Original Instance Data (this is a comment -- actual defn in mello.xih)
 */
/*
typedef struct {
	char* msg;
} MelloData;
*/

/*
 * -- Method Procedures for Mello
 */

SOM_Scope string SOMLINK mello_(Mello *somSelf)
{
    MelloData *somThis = MelloGetData(somSelf);
    MelloMethodDebug("Mello","mello");

    return somSelf->_get_msg();

}

SOM_Scope void  SOMLINK somInit(Mello *somSelf)
{
    MelloData *somThis = MelloGetData(somSelf);
    MelloMethodDebug("Mello","somInit");

    somSelf->_set_msg("Mello World");
    parent_SOMObject_somInit(somSelf);
}

SOM_Scope Mello*  SOMLINK MelloCreate(M_Mello *somSelf,
		char* msg)
{
    /* M_MelloData *somThis = M_MelloGetData(somSelf); */
    M_MelloMethodDebug("M_Mello","MelloCreate");

    Mello *mello = somSelf->somNew();
    mello->_set_msg(msg);
    return (mello);

}

SOM_Scope void  SOMLINK somInitMIClass(M_Mello *somSelf,
		long inherit_vars,
		char* className,
		SOMClassSequence* parentClasses,
		long instanceSize, long dataAlignment,
		long maxStaticMethods,
		long majorVersion,
		long minorVersion)
{
    /* M_MelloData *somThis = M_MelloGetData(somSelf); */
    M_MelloMethodDebug("M_Mello","somInitMIClass");

    parent_SOMClass_somInitMIClass(somSelf,inherit_vars,className,parentClasses,instanceSize,0,maxStaticMethods,majorVersion,minorVersion);
}
