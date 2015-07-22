/*
 *   COMPONENT_NAME: somx
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
#ifndef lint
static char *sccsid = "@(#) 1.6 src/somples.os2/somd/c/peerthrd/peer.c, somx.somd, som2.5, s259604a 1/12/96 12:44:33 [1/24/96 18:29:40]";
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
 * This is a simple DSOM server application program. It demonstrates how
 * a thread can be spawned to service requests so that the application
 * is free to behave as a client.
 *
 * The application prompts for it's own alias, the alias of the peer server
 * it should connect to, and a timer interval.
 *
 * After getting it's own alias this application registers it's implementation
 * with the SOMD_SOMOAObject and spawns a thread to service requests.
 * After getting the alias of the remote server to which it will send
 * messages, the application uses the Factory Extended Naming Context to
 * find a factory and create the target server.
 * After getting the timer interval the application alternately sleeps for
 * the specified time interval and sends a one way message to the target.
 *
 * In this simple "peer-to-peer" configuration, Process A will notify Process B
 * about it's time interval expirations, and vice versa.
 *
 */


#define INCL_DOSPROCESS
#include <os2.h>

#include <stdio.h>
#include <stdlib.h>

#include <somd.h>
#include "peerserv.h"     /* this is supplied by the application */

void cleanup(Environment *);
void serverExitHandler(void);

#ifdef __IBMC__
 #pragma linkage(service_thread, system)
#endif

/* global variables */

ExtendedNaming_ExtendedNamingContext FENCObject = NULL;
PeerServer       RemoteEvent = NULL;
SOMClass         RemotePeer  = NULL;
long             Interval    = 0;
char             peerName1[80];
char             peerName2[80];
char             constraint[512];
TID service_tid=0U;
void service_thread(unsigned long);
unsigned long thread_stack_size=1024L*32L;
unsigned long dummy=0;

/***********************************************************************/
/*
 *  Main program.
 */

int main(int argc, char **argv)
{
    Environment ev;
    char inbuf[256];
    long regId;
    int  rc;

    /* Basic initialization */


    SOM_InitEnvironment(&ev);

    /*************************************************************
     * Initializes itself as a DSOM application
     *************************************************************/

    /* init DSOM */
    SOMD_Init(&ev);


    /* create the SOMOA object for this server */
    SOMD_SOMOAObject = (SOMOA) SOMOANew();

    if (SOMD_SOMOAObject == NULL)
    {
        somPrintf("SOMD_SOMOAObject == NULL\n");
        cleanup(&ev);
        exit(1);
    }

    /* get and create this server's ImplementationDef */
    somPrintf("What is the alias for this server? ");
    gets(peerName1);

    SOMD_ImplDefObject = _find_impldef_by_alias(SOMD_ImplRepObject,
                                                &ev, peerName1);
    if ((SOMD_ImplDefObject == NULL) || (ev._major != NO_EXCEPTION))
    {
        somPrintf("Could not find alias %s in Implementation Repository\n",
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

    /* indicate that we are ready to process messages */
    _impl_is_ready(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);
    if (ev._major != NO_EXCEPTION)
    {
        somPrintf("SOMOA exception: %s\n", somExceptionId(&ev));
        exit(1);
    }


    /* start a thread to service requests */
    if ( DosCreateThread(&service_tid, (PFNTHREAD) service_thread,
                         dummy, 0, thread_stack_size) !=0)
    {
        somPrintf("Unable to start service_thread() \n");
        exit(1);
    }



    /*************************************************************
     * Prompts the user for a target server name, and gets the   *
     * corresponding server object.                              *
     *************************************************************/

    somPrintf("What is the alias for the target server? ");
    gets(peerName2);

    /*************************************************************
     * Get the lifecycle factory services context object so we   *
     * search for the appropriate peer
     *************************************************************/

    FENCObject = (ExtendedNaming_ExtendedNamingContext)
        _resolve_initial_references(SOMD_ORBObject, &ev, "FactoryService");

    if (ev._major != NO_EXCEPTION || FENCObject == NULL)
    {
        somPrintf("Could not resolve Factory Services context object\n");
        exit(1);
    }

    /*************************************************************
     * Build a conditional constraint to ensure that DSOM does   *
     * not create a local object                                 *
     *************************************************************/

    sprintf(constraint, "alias == '%s' and class == 'PeerServer'", peerName2);
    RemotePeer = (SOMClass)_find_any(FENCObject, &ev, constraint, 0);
    if (ev._major != NO_EXCEPTION || RemotePeer == NULL)
    {
        somPrintf("Couldn't find factory service for RemotePeer %s\n",
                   peerName2);
        exit(1);
    }

    /*************************************************************
     * Now create an instance of PeerServer in the remote peer   *
     *************************************************************/

    RemoteEvent = _somNew(RemotePeer);
    if (RemoteEvent == NULL)
    {
        somPrintf("Could not create instance of PeerServer in remote peer\n");
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


    /*************************************************************
     * Waits for the user to say "go"
     *************************************************************/

    somPrintf("Type <Enter> key when ready to go: ");
    gets(inbuf);


    /*************************************************************
     * Start client processing tasks.                            *
     *************************************************************/

    /* do client loop */
    while(ev._major == NO_EXCEPTION){
        DosSleep(Interval);
        /* call the remote peer, with a oneway message */
        _noteTimeout(RemoteEvent, &ev, peerName2, peerName1);
    }
    somPrintf("Exception detected from call on RemotePeer",
                   peerName2);

    DosKillThread(service_tid);

    /*************************************************************
     * shut down DSOM process (if ever get here )                *
     *************************************************************/

    _deactivate_impl(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);

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
        _somFree(SOMD_SOMOAObject);
        SOMD_SOMOAObject = NULL;
    }
    if (SOMD_ImplDefObject != NULL){
        _somFree(SOMD_ImplDefObject);
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
         _deactivate_impl(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);
    cleanup(&ev);
    (void) DosExitList(EXLST_EXIT, 0);
    return;
}


void service_thread(unsigned long dummy)
{
     Environment ev;

     SOM_InitEnvironment(&ev);

     /* pass control to SOMOA to service requests */
    (void) _execute_request_loop (SOMD_SOMOAObject, &ev, SOMD_WAIT);

}



