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
static char *sccsid = "@(#) 1.20 src/somples.os2/somd/cpp/peer/peer.cpp, somx.somd, som2.5, s259604a 1/12/96 10:29:41 [1/24/96 17:50:58]";
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
 * This is a simple event-driven DSOM program, which demonstrates the use
 * of EMan with DSOM.
 *
 * The program responds to two types of events:
 *
 *     1) interval timer events
 *     2) DSOM requests
 *
 * Upon each timer event, we notify a "peer" process that the timer popped,
 * via a remote method call.
 *
 * In a simple "peer-to-peer" configuration, Process A will notify Process B
 * about it's timer pops, and vice versa.
 *
 * The program has the following steps:
*
*     1) Initializes itself as a DSOM application, registering
*        a callback function with EMan for handling DSOM messages.
*
*     2) Prompts the user for a target server name, and gets the
*        corresponding server object.
*
*     3) Prompts the user for a time interval value, and sets up
*        timer event.
*
*     4) Waits for the user to say "go"(Press Enter key), and
*        registers the timer notification with Eman.
*
*     5) Turn control over to EMan.
*/


#define INCL_DOSPROCESS
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>

#include <somd.xh>
#include <eman.xh>
#include "peerserv.xh"     /* this is supplied by the application */

void cleanup(Environment *);
void serverExitHandler(void);

#ifdef __IBMC__
 #pragma linkage(SOMD_RegisterCallback, system)
 #pragma linkage(DSOMEventCallBack, system)
 #pragma linkage(TimerEventCallBack, system)
#endif

/* global variables */

PeerServer  *RemotePeer  = (PeerServer *)NULL;
SOMEEMan    *MyEMan      = (SOMEEMan *)NULL;
long         Interval    = 0;
char         peerName1[80];
char         peerName2[80];

/***********************************************************************/
/*
 *  This is the DSOM callback function to be registered with EMan.
 *
 *  Upon a DSOM request, EMan will call this function to handle any
 *  pending DSOM messages.  The SOMD_NO_WAIT flag tells the SOMOA
 *  to return as soon as all pending messages have been processed.
 */

void DSOMEventCallBack (SOMEEvent *event, void *eventData)
{
    Environment ev;

    SOM_InitEnvironment(&ev);

    /* init DSOM */
    SOMD_Init(&ev);

    /* ignore the parameters */
    (void) event;  (void) eventData;

    /* process pending DSOM messages */
    (void) SOMD_SOMOAObject->execute_request_loop (&ev, SOMD_NO_WAIT);
}

/***********************************************************************/
/*
 *  This is the Timer callback function to be registered with EMan.
 *
 *  Upon a Timer "pop", EMan will call this function to notify the
 *  peer program that the event occurred.
 */

void TimerEventCallBack (SOMEEvent *event, void *eventData)
{
    Environment ev;

    SOM_InitEnvironment(&ev);

    /* init DSOM */
    SOMD_Init(&ev);

    /* ignore the parameters */
    (void) event;  (void) eventData;

    /* call the peer, with a oneway message */
    RemotePeer->noteTimeout(&ev, peerName2, peerName1);

   /* Exit if an exception is detected */
   if (ev._major != NO_EXCEPTION)
   {
       somPrintf("Exception detected during call on RemotePeer: %s\n", somExceptionId(&ev));
       exit(1);
   }
}

/***********************************************************************/
/*
 *  Main program.
 */

int main(int argc, char **argv)
{
    Environment        ev;
    Environment        peerEv;
    Environment        *pev;
    SOMEEMRegisterData *data;
    char inbuf[256];
    long regId;
    int  rc;

    /* Basic initialization */

    MyEMan = new SOMEEMan;

    SOM_InitEnvironment(&ev);
    SOM_InitEnvironment(&peerEv);

    /*************************************************************
     * Initializes itself as a DSOM application, registering     *
     * a callback function with EMan for handling DSOM messages. *
     *************************************************************/

    /* init DSOM */
    SOMD_Init(&ev);

    /* register event handler */
    SOMD_RegisterCallback (MyEMan, (SOMD_Callback *) DSOMEventCallBack);

    /* create the SOMOA object for this server */
    SOMD_SOMOAObject = new SOMOA;

    if (SOMD_SOMOAObject == NULL)
    {
        somPrintf("SOMD_SOMOAObject == NULL\n");
        cleanup(&ev);
        exit(1);
    }

    /* get and create this server's ImplementationDef */
    somPrintf("What is the alias for this server? ");
    gets(peerName1);

    SOMD_ImplDefObject =
	 SOMD_ImplRepObject->find_impldef_by_alias(&ev, peerName1);

    if (SOMD_ImplDefObject == NULL)
    {
       somPrintf("Could not find alias %s in Implemenation Repository\n",
                  peerName1);
       cleanup(&ev);
       exit(1);
    }

    /* register exit handler */
    rc = DosExitList(EXLST_ADD | 0x0100, (PFNEXITLIST) serverExitHandler);
    if (rc != 0)
    {
      somPrintf("DosExitList failed, rc = %d\n", rc);
      cleanup(&ev);
      exit(1);
    }


    /*************************************************************
     * Prompts the user for a target server name, and gets the   *
     * corresponding server object.                              *
     *************************************************************/

    somPrintf("What is the alias for the target server? ");
    gets(peerName2);

    RemotePeer =(PeerServer*)SOMD_ObjectMgr->somdFindServerByName(&ev, peerName2);
    if (ev._major != NO_EXCEPTION || RemotePeer == NULL)
    {
        somPrintf("Could not create instance of PeerServer in the remote peer\n");
        cleanup(&ev);
        exit(1);
    }

    /* indicate that we are ready to process messages */
    SOMD_SOMOAObject->impl_is_ready(&ev, SOMD_ImplDefObject);
    if (ev._major != NO_EXCEPTION)
    {
	somPrintf("SOMOA exception: %s\n", somExceptionId(&ev));
	exit(1);
    }

    /*************************************************************
     * Prompts the user for a time interval value, and sets up a *
     * timer event.                                              *
     *************************************************************/

    somPrintf("What is the timer interval, in seconds? ");
    gets(inbuf);

    Interval = atoi(inbuf);
    Interval *= 1000;

    data = new SOMEEMRegisterData;
    data->someClearRegData(&ev);
    data->someSetRegDataEventMask(&ev, EMTimerEvent, NULL);
    data->someSetRegDataTimerInterval(&ev, Interval);

    /*************************************************************
     * Waits for the user to say "go", and registers the timer   *
     * event with EMan.                                          *
     *************************************************************/

    somPrintf("Type <Enter> key when ready to go: ");
    gets(inbuf);

    regId = MyEMan->someRegisterProc(&ev, data,
				     (EMRegProc *) TimerEventCallBack,
				     NULL);

    /*************************************************************
     * Turn control over to EMan.                                *
     *************************************************************/

    MyEMan->someProcessEvents(&ev);

    /*************************************************************
     * shut down DSOM process (if ever get here )                *
     *************************************************************/

    SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);

    cleanup(&ev);

    return 0;
}


/*
 *  Frees local and global DSOM objects.
 */

void cleanup(Environment *ev)
{
    if (ev->_major != NO_EXCEPTION)
         somExceptionFree(ev);

    if (SOMD_SOMOAObject != NULL){
         SOMD_SOMOAObject->somFree();
         SOMD_SOMOAObject=NULL;
    }
    if (SOMD_ImplDefObject != NULL){
         SOMD_ImplDefObject->somFree();
         SOMD_ImplDefObject = NULL;
     }
    SOMD_Uninit(ev);
    SOM_UninitEnvironment(ev);
    return;
}


/*
 *  Called when server process terminates.  Deactivates the implementation.
 */

void serverExitHandler()
{
    Environment ev;

    SOM_InitEnvironment(&ev);
    if(SOMD_SOMOAObject)
        SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
    cleanup(&ev);
    (void) DosExitList(EXLST_EXIT, 0);
    return;
}

