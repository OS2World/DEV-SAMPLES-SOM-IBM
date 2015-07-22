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
static char *sccsid = "@(#) 2.10 src/somples.os2/somd/cpp/animal/testsvr.cpp, somx.somd, som2.5, s259604a 12/26/95 17:47:15 [1/24/96 17:31:09]";
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
 *  This is a generic server which can use the shared server or persistent
 *  server activation policy.
 *
 *  Syntax:  testsvr <implid> | -a <alias>
 */

#include <stdlib.h>
#include <string.h>
#include <somd.xh>
#include <somoa.xh>
#include <implrep.xh>

void cleanup(Environment *);
#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)

int main(int argc, char **argv)
{
    Environment ev;

    /* local and DSOM initialization */
    SOM_InitEnvironment(&ev);
    SOMD_Init(&ev);
    SOMD_SOMOAObject = new SOMOA;

    /* parse arguments */
    switch (argc)
    {
    /* implementation id passed */
    case 2:
        /* get ImplementationDef from Implementation Repository */
        SOMD_ImplDefObject = SOMD_ImplRepObject->find_impldef(
                                                 &ev, (ImplId) argv[1]);
        if (checkEv(&ev))
        {
          somPrintf("find_impldef failed, exception = %s\n",
                     somExceptionId(&ev));
          cleanup(&ev);
          exit(1);
        }
        break;

    /* alias passed */
    case 3:
        if ((argv[1][0] == '-') &&
            ((argv[1][1] == 'a') || (argv[1][1] == 'A')))
        {
          /* get ImplementationDef from input alias */
          SOMD_ImplDefObject = SOMD_ImplRepObject->find_impldef_by_alias(
                                                   &ev, (ImplId) argv[2]);
          if (checkEv(&ev))
          {
            somPrintf("find_impldef_by_alias failed, exception = %s\n",
                       somExceptionId(&ev));
            cleanup(&ev);
            exit(1);
          }
        }
        else
        {
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
    SOMD_SOMOAObject->impl_is_ready(&ev, SOMD_ImplDefObject);

    if (checkEv(&ev))
    {
      somPrintf("impl_is_ready failed, exception = %s\n",
                 somExceptionId(&ev));
      cleanup(&ev);
      exit(1);
    }

    /* should register an exit handler which calls deactivate_impl here */

    /* process requests until deactivation */
    (void) SOMD_SOMOAObject->execute_request_loop(&ev, SOMD_WAIT);

    if (checkEv(&ev))
    {
      somPrintf("execute_request_loop failed, exception = %s\n",
                 somExceptionId(&ev));
      SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
      cleanup(&ev);
      exit(1);
    }

    SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
    cleanup(&ev);
    return 0;
}

/*
 *  Frees local and global DSOM objects.
 */

void cleanup(Environment *ev)
{
    if (SOMD_SOMOAObject != NULL) delete SOMD_SOMOAObject;
    if (SOMD_ImplDefObject != NULL) delete SOMD_ImplDefObject;
    SOMD_Uninit(ev);
    SOM_UninitEnvironment(ev);
    return;
}
