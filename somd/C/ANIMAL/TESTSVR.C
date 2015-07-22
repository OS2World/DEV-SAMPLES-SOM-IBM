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
/* @(#) somx/testsvr.c 2.8 12/26/95 17:39:27 [1/24/96 17:31:01] */

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
 *  This is a generic server which can use the shared server or persistent
 *  server activation policy.
 * 
 *  Syntax:  testsvr <implid> | -a <alias>
 */

#define INCL_DOSPROCESS
#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include <somd.h>
#include <somoa.h>
#include <implrep.h>

void cleanup(Environment *);
void serverExitHandler(void);
#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)

int main(int argc, char **argv)
{
    Environment ev;
    APIRET rc;

    /* local and DSOM initialization */
    SOM_InitEnvironment(&ev);
    SOMD_Init(&ev);

    SOMD_SOMOAObject = (SOMOA) SOMOANew();
    if (SOMD_SOMOAObject == NULL) {
      somPrintf("failed creating SOMOA object\n");
      cleanup(&ev);
      exit(1);
    }

    /* parse arguments */
    switch (argc) {

    /* implementation id passed */
    case 2:
        /* get ImplementationDef from Implementation Repository */
        SOMD_ImplDefObject = _find_impldef(SOMD_ImplRepObject, &ev,
			                   (ImplId) argv[1]);
	if (checkEv(&ev)) {
          somPrintf("find_impldef failed, exception = %s\n",
		     somExceptionId(&ev));
          cleanup(&ev);
          exit(1);
        }
        break;

    /* alias passed */
    case 3:
        if ((argv[1][0] == '-') &&
	    ((argv[1][1] == 'a') || (argv[1][1] == 'A'))) {
          /* get ImplementationDef from input alias */
          SOMD_ImplDefObject = _find_impldef_by_alias(SOMD_ImplRepObject, &ev,
						      (ImplId) argv[2]);
	  if (checkEv(&ev)) {
            somPrintf("find_impldef_by_alias failed, exception = %s\n",
		       somExceptionId(&ev));
            cleanup(&ev);
            exit(1);
          }
        }
        else {
          somPrintf("invalid arguments\n");
          cleanup(&ev);
          exit(1);
        }
        break;

    /* invalid number of arguments */
    default:
        somPrintf("invalid number of arguments\n");
        cleanup(&ev);
        exit(1);
        break;
    } 

    /* implementation now ready to process requests */
    _impl_is_ready(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);
    if (checkEv(&ev)) {
      somPrintf("impl_is_ready failed, exception = %s\n",
		 somExceptionId(&ev));
      cleanup(&ev);
      exit(1);
    }
    
    /* register exit handler */
    rc = DosExitList(EXLST_ADD | 0x0100, (PFNEXITLIST) serverExitHandler);
    if (rc != 0) {
      somPrintf("DosExitList failed, rc = %d\n", rc); 
      cleanup(&ev);
      exit(1);
    }

    /* process requests until deactivation */
    (void) _execute_request_loop(SOMD_SOMOAObject, &ev, SOMD_WAIT);
    if (checkEv(&ev)) {
      somPrintf("execute_request_loop failed, exception = %s\n",
		 somExceptionId(&ev));
      _deactivate_impl(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);
      cleanup(&ev);
      exit(1);
    }

    _deactivate_impl(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);
    cleanup(&ev);
    return 0;
}

/*
 *  Frees local and global DSOM objects.  
 */

void cleanup(Environment *ev)
{
    if (SOMD_SOMOAObject != NULL) _somFree(SOMD_SOMOAObject);
    if (SOMD_ImplDefObject != NULL) _somFree(SOMD_ImplDefObject);
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
    _deactivate_impl(SOMD_SOMOAObject, &ev, SOMD_ImplDefObject);
    cleanup(&ev);
    (void) DosExitList(EXLST_EXIT, 0);
    return;
}
