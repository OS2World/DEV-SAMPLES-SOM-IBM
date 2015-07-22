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
static char *sccsid = "@(#) 1.7 src/somples.os2/somd/cpp/event/eventsvr.cpp, somx.somd, som2.5, s259604a 12/26/95 20:31:21 [1/24/96 17:50:57]";
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

#ifndef _ALL_SOURCE
#define _ALL_SOURCE
#endif

#define INCL_DOSPROCESS
#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <somd.xh>
#include <orb.xh>

#include <impldef.xh>
#include <implrep.xh>

#include <intfacdf.xh>
#include <somdom.xh>

#include "cell.xh"
#include "eventcom.xh"
#include "eventch.xh"

extern SOMOA                     *SOMD_SOMOAObject;
extern ORB                       *SOMD_ORBObject;
extern ImplementationDef         *SOMD_ImplDefObject;
extern SOMDObjectMgr             *SOMD_ObjectMgr;
extern int SOM_TraceLevel;
extern int somdCommDebugFlag;

Environment       ev;

/* function prototypes */
void exitUsage(char *prog);
int  write_obj_ref(SOMDObject *objref, char *file);
void cleanup(Environment *);
void serverExitHandler(void);
#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)


int main(int argc, char **argv)
{
    char              *penv;
    char              *alias;
    int               rc;
    Cell              *cellObj;
    SOMDObject        *cellObj_proxy;
    SOMDObject        *evchObj_proxy;

    EventChannelAdmin_EventChannel   *evchObj;
    EventChannelAdmin_SupplierAdmin  *supp_admin;
    EventComm_EventConnection        *evcon_if;
    EventComm_Consumer               *cons_if;

    SOM_InitEnvironment(&ev);
    SOMD_Init(&ev);

    /* Parse arguments */
    switch (argc)
    {
      case 2:
	alias = argv[1];
	break;

      default:
        somPrintf("\n Invalid number of arguments");
        cleanup(&ev);
	exitUsage(argv[0]);
    }

    SOMD_ImplDefObject = SOMD_ImplRepObject->find_impldef_by_alias(&ev, alias);

    if (checkEv(&ev))
    {
      somPrintf("find_impldef_by_alias failed, exception = %s\n",
		 somExceptionId(&ev));
      cleanup(&ev);
      exit(1);
    }

    SOMD_SOMOAObject = new SOMOA;

    if ((SOMD_ImplDefObject == NULL) ||         /* The initialization failed */
        (SOMD_SOMOAObject   == NULL) ||
	(SOMD_ORBObject     == NULL))
    {
       somPrintf("Event Server Initialization Failed\n");
       cleanup(&ev);
       exit(1);
    }

    SOMD_SOMOAObject->impl_is_ready(&ev, SOMD_ImplDefObject);

    if (checkEv(&ev))
       {
	 somPrintf("impl_is_ready failed, exception = %s\n",
		    somExceptionId(&ev));
	 SOMD_ImplDefObject->somFree();
	 cleanup(&ev);
	 exit(1);
       }

       /* register exit handler */
       rc = DosExitList(EXLST_ADD | 0x0100, (PFNEXITLIST) serverExitHandler);
       if (rc != 0)
       {
	 somPrintf("DosExitList failed, rc = %d\n", rc);
	 SOMD_ImplDefObject->somFree();
	 cleanup(&ev);
	 exit(1);
       }


       /* Create the cell object  */
       cellObj = new Cell;
       if (cellObj == NULL) {
          somPrintf("Error creating cell object\n");
          cleanup(&ev);
          exit(1);
       }

       /*  Create a DSOM object reference for the cellObj  */
       cellObj_proxy = SOMD_SOMOAObject->create_SOM_ref(&ev,
							cellObj,
							SOMD_ImplDefObject);
       if (checkEv(&ev)) {
          somPrintf("Error creating cell object proxy\n");
          cellObj->somFree();
          cleanup(&ev);
          exit(1);
       }

       /*  Create EventChannel object  */
       evchObj = new EventChannelAdmin_EventChannel;
       if (evchObj == NULL) {
          somPrintf("Error creating event channel object\n");
          cellObj->somFree();
          cellObj_proxy->somFree();
          cleanup(&ev);
          exit(1);
       }

       /*  Create a DSOM object reference for evch  */
       evchObj_proxy = SOMD_SOMOAObject->create_SOM_ref(&ev,
							evchObj,
							SOMD_ImplDefObject);
       if (checkEv(&ev)) {

          somPrintf("Error creating DSOM cell object proxy, exception = %s\n",
                    somExceptionId(&ev));
          cellObj->somFree();
          cellObj_proxy->somFree();
          evchObj->somFree();
          cleanup(&ev);
          exit(1);
       }

       /*
	* Get a supplier admin obj and add a push style supplier onto the
	* event channel using this admin interface.
	* Returns a consumer interface which will be used by the supplier
	*/
       supp_admin = evchObj->for_suppliers(&ev);
       evcon_if = new EventComm_EventConnection;
       cons_if = supp_admin->add_push_supplier(&ev, evcon_if);

       /* let the cell object know what the consumer_if is */
       cellObj->setConsumerIf(&ev, cons_if);

       /* Write out the string representations of the object references */
       rc = write_obj_ref(cellObj_proxy, "cell.rep");
       if (rc) {
          somPrintf("Error writing out cell object reference to cell.rep\n");
          evcon_if->somFree();
          cellObj->somFree();
          cellObj_proxy->somFree();
          evchObj->somFree();
          evchObj_proxy->somFree();
          cleanup(&ev);
          exit(1);
       }
       rc = write_obj_ref(evchObj_proxy, "event.rep");
       if (rc) {
          somPrintf("Error writing out event channel object reference to event.rep\n");
          evcon_if->somFree();
          cellObj->somFree();
          cellObj_proxy->somFree();
          evchObj->somFree();
          evchObj_proxy->somFree();
          cleanup(&ev);
          exit(1);
       }

       /* Now call loop until deactivation */
       somPrintf("Event Server going into the execute_request_loop\n");
       SOMD_SOMOAObject->execute_request_loop(&ev, SOMD_WAIT);
    if (checkEv(&ev)) {
	somPrintf("execute_request_loop failed, exception = %s\n",
		  somExceptionId(&ev));
          cellObj->somFree();
          cellObj_proxy->somFree();
          evchObj->somFree();
          evchObj_proxy->somFree();
          cleanup(&ev);
          exit(1);
       }

       cellObj->somFree();
       cellObj_proxy->somFree();
       evchObj->somFree();
       evchObj_proxy->somFree();

       SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
       cleanup(&ev);

        return 0;
}


int  write_obj_ref(SOMDObject *objref, char *file)
{
    FILE *fd;
    char *objRefStr;

    objRefStr = SOMD_ORBObject->object_to_string (&ev, objref);

    if ((fd = fopen (file, "w")) == (FILE *) NULL)
    {
       somPrintf ("Unable to create file %s, errno %d\n", file, errno);
	return -1;
    }

    fwrite ((void *) objRefStr, sizeof (char), strlen (objRefStr), fd);

    ORBfree (objRefStr);
    fclose (fd);
    return (0);
}


void exitUsage(char *prog)
{
    somPrintf("\n\n Usage: %s <impl alias> \n\n", prog);
    exit(-1);
}


/*
 *  Frees local and global DSOM objects.
 */

void cleanup(Environment *ev)
{
    if (SOMD_SOMOAObject != NULL) SOMD_SOMOAObject->somFree();
    if (SOMD_ImplDefObject != NULL) SOMD_ImplDefObject->somFree();
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
    FILE *fd;

    SOM_InitEnvironment(&ev);
    SOMD_SOMOAObject->deactivate_impl(&ev, SOMD_ImplDefObject);
    if ((fd = fopen ("cell.rep", "w")) != (FILE *) NULL) fclose (fd);
    if ((fd = fopen ("event.rep", "w")) != (FILE *) NULL) fclose (fd);
    cleanup(&ev);
    (void) DosExitList(EXLST_EXIT, 0);
    return;
}
