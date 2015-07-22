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
#ifndef lint
static char *sccsid = "@(#) 2.19 src/somples.os2/somd/c/proxy/tstproxy.c, somx.somd, som2.5, s259604a 12/26/95 17:45:48 [1/24/96 17:31:07]";
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


#define INCL_DOSPROCESS
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <somd.h>
#include <somtcnst.h>
#include "bar.h"

#define CheckEv(ev) (ev._major != NO_EXCEPTION)

main()
{
    FILE          *fileref;
    char          *exID;
    Environment   ev;
    Bar           barObj;
    long          test_long;
    long          count;
    string        stringref;
    string        return_string;
    string        arg1;
    long          arg2 = 210;
    ReferenceData arg3;

    SOM_InitEnvironment(&ev);
    SOMD_Init(&ev);

    /*
     *  test to see if the object exists
     */

    if ( !(fileref = fopen("fileref","r")) )
    {
        /*
	 *  if the object doesn't exist, create the object remotely  -
	 *  the local proxy is loaded by the runtime
	 */

	barObj = somdCreate(&ev, "Bar", TRUE);
	if ( CheckEv(ev) ) goto process_error;

	stringref = _object_to_string (SOMD_ORBObject,&ev,(SOMDObject) barObj);
	if ( CheckEv(ev) ) goto process_error;

	fileref = fopen("fileref", "w");
	fprintf(fileref, "%s", stringref);
	fclose(fileref);
	somPrintf("Created an object of class Bar\n");
    }
    else
    {
	/*
	 *  object does exist so use string_to_object to load the proxy
	 */

	stringref = (string) SOMMalloc(1024);
	fscanf(fileref, "%s", stringref);
	barObj = (Bar) _string_to_object(SOMD_ORBObject, &ev, stringref);
	if ( CheckEv(ev) ) goto process_error;
	somPrintf("Found reference to an object of class Bar\n");
    }

    test_long = Bar__get_attribute_long(barObj, &ev);
    if ( CheckEv(ev) ) goto process_error;
    somPrintf("Initial attribute_long = %d\n", test_long);

    /* set, get and print long attribute */
    Bar__set_attribute_long(barObj, &ev, 8888);
    test_long = Bar__get_attribute_long(barObj, &ev);
    if ( CheckEv(ev) ) goto process_error;
    somPrintf("Modified attribute_long = %d\n", test_long);

    arg3._maximum = 1024;
    arg3._length = sizeof("ReferenceData");
    arg3._buffer = (octet *) "ReferenceData";

    somPrintf("Invoking remote Bar method1\n");
    return_string = Bar_method1(barObj, &ev, &arg1, &arg2, &arg3);
    if ( CheckEv(ev) ) goto process_error;

    somPrintf("Should return: 'method1 completed', 'abc', 1234\n");
    somPrintf("return_string = %s\n", return_string);
    somPrintf("arg1 = %s\n", arg1);
    somPrintf("arg2 = %d\n\n", arg2);

    for (count = 0; count < 5; count++)
    {
	DosSleep(100);
	Bar__set_attribute_long(barObj, &ev, count);
	test_long = Bar__get_attribute_long(barObj, &ev);
	if ( CheckEv(ev) ) goto process_error;
	somPrintf("Incremented attribute_long = %d\n", test_long);
    };

    /* Release local barObj and exit */

    _release(barObj, &ev);
    SOMD_Uninit(&ev);
    SOM_UninitEnvironment(&ev);
    return(0);

process_error:
    exID = somExceptionId(&ev);

    if ( ev._major == SYSTEM_EXCEPTION )
    {
       somPrintf("System exception: %s\n", exID);

       if ( strcmp(ev.exception._exception_name, "INV_OBJREF") == 0 )
       {
	  somPrintf("Object reference no longer valid.\n");
	  somPrintf("Delete the file named fileref and rerun sample program.\n");
       }
    }
    else if ( ev._major == USER_EXCEPTION )
    {
       somPrintf("User exception: %s\n", exID);
    }

    somdExceptionFree(&ev);
    if (barObj != NULL) _release(barObj,&ev);
    SOMD_Uninit(&ev);
    SOM_UninitEnvironment(&ev);
    return(1);
}
