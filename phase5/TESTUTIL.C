/* 3/8/96 15:52:50
*/
/*
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*/
/*
 *
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

#ifndef lint
static char *sccsid = "@(#) 1.3 src/sompos/pof/utils.c, sompos, som3.0 4/5/95 10:02:10 [5/26/95 14:39:16]";
#endif


#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "testutil.h"
#include <som.h>
#include <somosutl.h>
#define INCL_DOS
#include <os2.h>

#define STACK_SIZE  65536

int getopt( int argc, char **argv);

/*=======================================================================
   Function: process_requests

   Starts the DSOM server request processing loop.
  =======================================================================*/
void process_requests( void* data )
{
    Environment e;
    Environment *ev = &e;
    string name;

    SOM_InitEnvironment(ev);

    name = __get_impl_alias(SOMD_ImplDefObject, ev);
    somPrintf(name);
    somPrintf( ": Client's server accepting requests.\n" );

    _execute_request_loop(SOMD_SOMOAObject, ev, SOMD_WAIT);

    if( CHECK_EV(ev) ) {
       somPrintf(name);
       somPrintf( ": Execute_request_loop failed, exception = %s\n", somExceptionId(ev) );
       DSOM_term(ev);
       exit(1);
    }
    somPrintf(name);
    somPrintf( ": Client's server terminating.\n" );

    return;
}

/*==========================================================================
routine: DSOM_thread
starts a DSOM server in another thread.
=========================================================================== */
void DSOM_thread(int argc, char **argv, Environment *ev) {
   int request_thread;
   string impl_alias = NULL;
   string impl_id = NULL;
   int c;
   boolean initialize_mode = FALSE;

   somPrintf("Setting up DSOM server.\n");

   /* Parse input arguments */
   while (--argc >0) {
     if ((*++argv)[0] == '-') {
       switch ((*argv)[1]) {
          case 'a':
          case 'A':
                      impl_alias = *++argv;
                      argc--;
                      break;
          case 'i':
          case 'I':
                      initialize_mode = TRUE;
                      break;
       }
     }
     else {
       impl_id = *argv;
     }
   }
    /* Setup SOMOS internals */
    somos_setup( );

    /* Create a SOM object adapter object*/
    SOMD_SOMOAObject = SOMOANew();

    /* Get the server's ImplementationDef from the Implementation Repository*/
    if (impl_alias)
       SOMD_ImplDefObject = _find_impldef_by_alias(SOMD_ImplRepObject, ev, impl_alias);
    else
       SOMD_ImplDefObject = _find_impldef(SOMD_ImplRepObject, ev, impl_id);

    /* Could not find implementation definition. Cannot continue! */
    if (CHECK_EV(ev))
        somos_exit(SOMOS_FIND_IMPLDEF_FAILED);

    /* Initialize any required object services - before impl_is_ready */
    somos_init_services(initialize_mode);

    /* Implementation now ready to process requests so tell the SOM object  */
    /* adapter we are ready                                                 */
    _impl_is_ready(SOMD_SOMOAObject, ev, SOMD_ImplDefObject);

    /* Initialize any required object services - after impl_is_ready */
    somos_init_services_afterimpl(initialize_mode);

    if (CHECK_EV(ev))
       somos_exit(SOMOS_IMPL_IS_READY_FAILED);

    /* Start the request processing thread passing no data. */
    request_thread = _beginthread(process_requests, NULL, STACK_SIZE, NULL);
    if (request_thread == -1)
      somPrintf("ClientServer: StartThread failed");

 }

 void DSOM_term( Environment *ev)
 {
     if (SOMD_ImplDefObject && SOMD_SOMOAObject) {
         SOMOA_deactivate_impl(SOMD_SOMOAObject, ev, SOMD_ImplDefObject);
     }

     SOMD_Uninit(ev);
 }

