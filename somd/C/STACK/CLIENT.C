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
static char *sccsid = "@(#) 2.14 src/somples.os2/somd/c/stack/client.c, somx.somd, som2.5, s259604a 12/26/95 17:38:01 [1/24/96 17:31:00]";
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

#include <somd.h>
#include "stack.h"

boolean OperationOK(Environment *ev);

int main(int argc, char *argv[])
{
  Environment ev;
  Stack stk;
  long num = 100;

  SOM_InitEnvironment(&ev);
  SOMD_Init(&ev);

  stk = somdCreate(&ev, "Stack", TRUE);

  /* Verify successful object creation */
  if ( OperationOK(&ev) )
  {
     while ( !_full(stk, &ev) )
     {
	_push(stk, &ev, num);
	somPrintf("Top: %d\n", _top(stk, &ev));
	num += 100;
     }

     /* Test stack overflow exception */
     _push(stk, &ev, num);
     OperationOK(&ev);

     while ( !_empty(stk, &ev) )
     {
	somPrintf("Pop: %d\n", _pop(stk, &ev));
     }

     /* Test stack underflow exception */
     somPrintf("Top Underflow: %d\n", _top(stk, &ev));
     OperationOK(&ev);
     somPrintf("Pop Underflow: %d\n", _pop(stk, &ev));
     OperationOK(&ev);

     _push(stk, &ev, -10000);
     somPrintf("Top: %d\n", _top(stk, &ev));
     somPrintf("Pop: %d\n", _pop(stk, &ev));

     _somFree(stk);

     if ( OperationOK(&ev) )
     {
	somPrintf("Stack test successfully completed.\n");
     }
  }

  SOMD_Uninit(&ev);
  SOM_UninitEnvironment(&ev);

  return(0);
}


boolean OperationOK(Environment *ev)
{
   char *exID;

   switch (ev->_major)
   {
     case SYSTEM_EXCEPTION:
       exID = somExceptionId(ev);
       somPrintf("System exception: %s\n", exID);
       somdExceptionFree(ev);
       return (FALSE);

     case USER_EXCEPTION:
       exID = somExceptionId(ev);
       somPrintf("User exception: %s\n", exID);
       somdExceptionFree(ev);
       return (FALSE);

     case NO_EXCEPTION:
       return (TRUE);

     default:
       somPrintf("Invalid exception type in Environment.\n");
       somdExceptionFree(ev);
       return (FALSE);
   }
}

