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
static char *sccsid = "@(#) 2.15 src/somples.os2/somd/c/dii/diitest.c, somx.somd, som2.5, s259604a 12/26/95 17:44:23 [1/24/96 17:31:06]";
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
 *  "diitest" is a sample program demonstrating the DSOM Dynamic
 *  Invocation Interface.
 */

#include <stdlib.h>
#include <somd.h>
#include <request.h>
#include <nvlist.h>
#include <repostry.h>
#include <operatdf.h>
#include "foo.h"

void printEv(Environment *);
void makeNVList(Environment *, char *, NVList *, NamedValue *);
void setNVListValue(Environment *, NVList, long, void *, long);
#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)

int main(int argc, char *argv[])
{
    Environment *ev;
    SOMDObject fooObj;
    NVList arglist;
    NamedValue result;
    Request reqObj;

    string        arg1 = NULL;  /* ARG_OUT */
    long          arg2 = 999;   /* ARG_INOUT */
    ReferenceData arg3;         /* ARG_IN */

    /* local and DSOM initialization */
    ev = SOM_CreateLocalEnvironment();
    SOMD_Init(ev);

    /* create a remote object */
    fooObj = somdCreate(ev, "Foo", TRUE);
    if (checkEv(ev))
    {
      somPrintf("Error creating remote object Foo\n");
      SOMD_Uninit(ev);
      SOM_DestroyLocalEnvironment(ev);
      exit(1);
    }

    /* create an NVList for the operation */
    makeNVList(ev, "::Foo::method1", &arglist, &result);
    if (checkEv(ev)) goto error;

    /* add parameters to the NVList */
    setNVListValue(ev, arglist, 0, (void *)&arg1, 0);
    if (checkEv(ev)) goto error;

    setNVListValue(ev, arglist, 1, (void *)&arg2, sizeof(arg2));
    if (checkEv(ev)) goto error;

    arg3._maximum = 1024;
    arg3._length = 0;
    arg3._buffer = NULL;

    setNVListValue(ev, arglist, 2, (void *)&arg3, 0);
    if (checkEv(ev)) goto error;

    /* create a request for the operation */
    (void) _create_request(fooObj, ev, (Context)NULL, "method1", arglist,
			   &result, &reqObj, (Flags)0);
    if (checkEv(ev)) goto error;

    /* invoke request on object */
    (void) _invoke(reqObj, ev, (Flags)0);
    if (checkEv(ev)) goto error;

    /* print result and out parameters */
    somPrintf("The result is: %s\n", *(string*)(result.argument._value));
    somPrintf("The value of arg1 is: %s\n", arg1);
    somPrintf("The value of arg2 is: %d\n", arg2);

    /* destroy local proxy object and remote object */
    _somFree(fooObj);

    SOMD_Uninit(ev);
    SOM_DestroyLocalEnvironment(ev);
    exit(0);

error:
    printEv(ev);
    if (fooObj != NULL) _somFree(fooObj);
    SOMD_Uninit(ev);
    SOM_DestroyLocalEnvironment(ev);
    exit(1);
}


/*
 *  Prints exception information.
 */

void printEv(Environment *ev)
{
  char *exId;
  StExcep *params;

  exId = somExceptionId(ev);
  params = somExceptionValue(ev);

  somPrintf("Error Occurred\n");
  somPrintf("exception string: %s\n", exId);
  somPrintf("minor error code: %u\n", params->minor);
  somPrintf(" completion code: %s\n", (params->completed == YES ? "YES" :
	  params->completed == NO ? "NO": "MAYBE"));
  somdExceptionFree(ev);
  return;
}


/*
 *  Sets the result type and allocates an NVList for the operation.
 */

void makeNVList(Environment *ev, char *methodid, NVList *arglist,
		NamedValue *result)
{

    OperationDef opdef=NULL;
    Description desc;
    OperationDescription *opdesc;

    /* get the OperationDef object from the Interface Repository */
    opdef = _lookup_id(SOM_InterfaceRepository, ev, methodid);
    if (opdef == NULL)
    {
        SOMD_Uninit(ev);
        SOM_DestroyLocalEnvironment(ev);
        exit(1);
    }

    if (ev->_major == NO_EXCEPTION)
    {
      /* get the operation description structure */
      desc = _describe(opdef, ev);

      if (ev->_major == NO_EXCEPTION)
      {
	opdesc = (OperationDescription *) desc.value._value;

	/* fill in the TypeCode field for result */
	result->argument._type = opdesc->result;

	/* create a NamedValue list for the operation */
	(void) _create_operation_list(SOMD_ORBObject, ev, opdef, arglist);
      }
    }

    return;
}


/*
 *  Adds parameters to the NVList.
 */

void setNVListValue(Environment *ev, NVList arglist, long pos, void *value,
		    long valsz)
{
    Identifier name;
    TypeCode tc;
    void *dummy;
    long dummylen;
    Flags flags;

    /* get initialized fields of item first */
    (void) _get_item(arglist, ev, pos, &name, &tc, &dummy, &dummylen, &flags);

    if (ev->_major == NO_EXCEPTION)
    {
      /*
       * IMPORTANT NOTE:
       * For scalar types, use the pointer to the variable.
       * For arguments represented by structures (including "union", "sequence",
       *     and "any"), use a pointer to the structure.
       * For arguments represented by pointers (incl. strings, arrays, and
       *     objrefs), use a pointer to the pointer.
       */

      /* reset the value and length fields of the item */
      (void) _set_item(arglist, ev, pos, name, tc, value, valsz, flags);
    }

    return;
}

