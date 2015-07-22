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
static char *sccsid = "@(#) 2.18 src/somples.os2/somd/c/event/consumer.c, somx.somd, som2.5, s259604a 12/26/95 17:41:41 [1/24/96 17:31:03]";
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

#ifdef __OS2__
#define INCL_DOSPROCESS
#include <os2.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>

#include <somd.h>
#include <orb.h>

/* Local includes follow */
#include "eventcom.h"
#include "eventch.h"


boolean  ExceptionCheck(Environment *ev);

Environment ev;
Disconnected                         *discon;
char                                 *exId;
StExcep                              *stExval;

int main(int argc, char *argv[])
{
    char                                 answer[10];
    EventChannelAdmin_EventChannel       evchObj;
    EventComm_Supplier                   supp_if;
    EventChannelAdmin_ConsumerAdmin      cons_admin;
    EventComm_EventConnection            evcon_if;
    int                                  rc;
    boolean                              has_event = FALSE;
    any                                  notify;
    FILE                                 *fd;
    string                               objRefStr;
#ifdef __OS2__
    APIRET os2rc;
#endif

    SOM_InitEnvironment(&ev);
    SOMD_Init(&ev);

    if ((fd = fopen ("event.rep", "r")) == (FILE *) NULL)
    {
        somPrintf ("Unable to find object ref string in %s\n", "event.rep");
        SOMD_Uninit(&ev);
        SOM_UninitEnvironment(&ev);
        exit(1);
    }
    else
    {
	objRefStr = (char *) SOMMalloc (1024);
        fscanf(fd, "%s", objRefStr);
        evchObj =(EventChannelAdmin_EventChannel) _string_to_object(SOMD_ORBObject, &ev, objRefStr);
	if (evchObj == NULL)
	{
            somPrintf ("Unable to get objref from string %s.\n", objRefStr);
            SOMFree (objRefStr);
            fclose (fd);
            SOMD_Uninit(&ev);
            SOM_UninitEnvironment(&ev);
            exit(1);
	}

	SOMFree (objRefStr);
        fclose (fd);
    }

    cons_admin = _for_consumers(evchObj, &ev);

    evcon_if   = somdCreate(&ev, "EventComm::EventConnection", TRUE);
    if (ExceptionCheck(&ev))
    {
       somPrintf ("Unable to create Event Connection\n");
       _release(evchObj,&ev);
       SOMD_Uninit(&ev);
       SOM_UninitEnvironment(&ev);
       exit(1);
    }

    supp_if = _add_pull_consumer(cons_admin, &ev, evcon_if);
    somPrintf("Poll the event channel? <return to continue or Q/q to quit>\n");
    fflush(stdout);
    gets(answer);

    while  (strcmp(answer,"q") != 0 && strcmp(answer, "Q") != 0)
    {
	has_event = _try_pull(supp_if, &ev, &notify);

	if (ExceptionCheck(&ev))
	{
          somPrintf("%s", discon->Reason);
          _release(evchObj,&ev);
          _somFree(evcon_if);
          SOMD_Uninit(&ev);
          SOM_UninitEnvironment(&ev);
          exit (1);
	}

	while(!has_event)
	{
          somPrintf("Event Channel yet to be notified\n");
          fflush(stdout);
          has_event = _try_pull(supp_if, &ev, &notify);

          if (ExceptionCheck(&ev))
          {
             somPrintf("%s", discon->Reason);
             _release(evchObj,&ev);
             _somFree(evcon_if);
             SOMD_Uninit(&ev);
             SOM_UninitEnvironment(&ev);
             exit (1);
	    }

	    if (has_event) break;

	    #ifdef __OS2__
	    os2rc = DosSleep((ULONG)1000);
	    #else
	    sleep(1);
	    #endif
	}

       somPrintf("\n\nTHE EVENT CHANNEL HAS BEEN NOTIFIED\nThe notification obtained was:\n");
       somPrintf("The new cell value is: %ld\n\n",  *((long *)notify._value));
	TypeCode_free(notify._type, &ev);
       somPrintf("Poll the event channel? <return to continue or Q/q to quit>\n");
       gets(answer);
    }

    /* release the objects reference */
    _release(evchObj,&ev);
    _somFree(evcon_if);
    somdExceptionFree(&ev);

    SOMD_Uninit(&ev);
    SOM_UninitEnvironment(&ev);

    return 0;
}


boolean  ExceptionCheck(Environment *ev)
{
    switch(ev->_major)
    {
      case SYSTEM_EXCEPTION:
	exId = somExceptionId(ev);
	stExval = somExceptionValue(ev);
	break;

      case USER_EXCEPTION:
	exId = somExceptionId(ev);
	if (strcmp(exId, ex_Disconnected) == 0)
	{
	    discon = (Disconnected *) somExceptionValue(ev);
	}
	break;

      case NO_EXCEPTION:
	break;
    }

    return (ev->_major == SYSTEM_EXCEPTION || ev->_major == USER_EXCEPTION)
	    ? TRUE : FALSE;
}

