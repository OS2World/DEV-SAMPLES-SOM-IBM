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
static char *sccsid = "@(#) 2.9 src/somples.os2/somd/c/event/eventch.c, somx.somd, som2.5, s259604a 12/26/95 17:42:24 [1/24/96 17:31:04]";
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



#define SOM_Module_eventch_Source
#include "eventcom.ih"
#include "eventch.ih"


SOM_Scope EventComm_EventConnection
    SOMLINK EventChannelAdmin_ConsumerAdminadd_push_consumer
				  (EventChannelAdmin_ConsumerAdmin somSelf,
				   Environment *ev,
				   EventComm_Consumer c)
{

    /* EventChannelAdmin_ConsumerAdminData *somThis = EventChannelAdmin_ConsumerAdminGetData(somSelf); */
    EventComm_EventConnection   evcon = EventComm_EventConnectionNew();

    EventChannelAdmin_ConsumerAdminMethodDebug("EventChannelAdmin_ConsumerAdmin","EventChannelAdmin_ConsumerAdminadd_push_consumer");


    EventComm_Consumer__set_ev_con(c, ev, evcon);

    /* Return statement to be customized: */
    return (evcon);

}

SOM_Scope EventComm_Supplier
	       SOMLINK EventChannelAdmin_ConsumerAdminadd_pull_consumer
			(EventChannelAdmin_ConsumerAdmin somSelf,
			 Environment *ev,
			 EventComm_EventConnection d)
{
    EventComm_Supplier  supp;
    /* EventChannelAdmin_ConsumerAdminData *somThis = EventChannelAdmin_ConsumerAdminGetData(somSelf); */
    EventChannelAdmin_ConsumerAdminMethodDebug("EventChannelAdmin_ConsumerAdmin","EventChannelAdmin_ConsumerAdminadd_pull_consumer");


    supp = EventComm_SupplierNew();
    EventComm_Supplier__set_ev_con(supp, ev, d);

    /* Return statement to be customized: */
    return (supp);

}

SOM_Scope EventComm_EventConnection
		  SOMLINK EventChannelAdmin_SupplierAdminadd_pull_supplier
		       (EventChannelAdmin_SupplierAdmin somSelf,
			Environment *ev,
			EventComm_Supplier s)
{

    /* EventChannelAdmin_SupplierAdminData *somThis = EventChannelAdmin_SupplierAdminGetData(somSelf); */
   EventComm_EventConnection   evcon = EventComm_EventConnectionNew();

   EventChannelAdmin_SupplierAdminMethodDebug("EventChannelAdmin_SupplierAdmin","EventChannelAdmin_SupplierAdminadd_pull_supplier");

    EventComm_Supplier__set_ev_con(s, ev, evcon);

    /* Return statement to be customized: */
    return (evcon);


}

SOM_Scope EventComm_Consumer
	  SOMLINK EventChannelAdmin_SupplierAdminadd_push_supplier
			(EventChannelAdmin_SupplierAdmin somSelf,
			 Environment *ev,
			 EventComm_EventConnection d)
{
    EventComm_Consumer cons;

    /* EventChannelAdmin_SupplierAdminData *somThis = EventChannelAdmin_SupplierAdminGetData(somSelf); */
    EventChannelAdmin_SupplierAdminMethodDebug("EventChannelAdmin_SupplierAdmin","EventChannelAdmin_SupplierAdminadd_push_supplier");


    cons = EventComm_ConsumerNew();
    EventComm_Consumer__set_ev_con(cons, ev, d);

    /* Return statement to be customized: */
    return (cons);
}


SOM_Scope EventChannelAdmin_ConsumerAdmin
	     SOMLINK EventChannelAdmin_EventChannelfor_consumers
		       (EventChannelAdmin_EventChannel somSelf,
			Environment *ev)
{
     EventChannelAdmin_EventChannelData *somThis = EventChannelAdmin_EventChannelGetData(somSelf);
    EventChannelAdmin_EventChannelMethodDebug("EventChannelAdmin_EventChannel","EventChannelAdmin_EventChannelfor_consumers");
    if (somThis->cons_admin == (EventChannelAdmin_ConsumerAdmin )0)
       somThis->cons_admin = EventChannelAdmin_ConsumerAdminNew();
    return somThis->cons_admin;

}

SOM_Scope EventChannelAdmin_SupplierAdmin
	       SOMLINK EventChannelAdmin_EventChannelfor_suppliers
		     (EventChannelAdmin_EventChannel somSelf,
		      Environment *ev)
{
     EventChannelAdmin_EventChannelData *somThis = EventChannelAdmin_EventChannelGetData(somSelf);
    EventChannelAdmin_EventChannelMethodDebug("EventChannelAdmin_EventChannel","EventChannelAdmin_EventChannelfor_suppliers");


    if (somThis->supp_admin == (EventChannelAdmin_SupplierAdmin )0)
       somThis->supp_admin = EventChannelAdmin_SupplierAdminNew();

    return somThis->supp_admin;
}

SOM_Scope void  SOMLINK EventChannelAdmin_EventChanneldestroy
	      (EventChannelAdmin_EventChannel somSelf,
	       Environment *ev)
{
     EventChannelAdmin_EventChannelData *somThis = EventChannelAdmin_EventChannelGetData(somSelf);
    EventChannelAdmin_EventChannelMethodDebug("EventChannelAdmin_EventChannel","EventChannelAdmin_EventChanneldestroy");

    _somFree(somThis->supp_admin);
    _somFree(somThis->cons_admin);
    _somFree(somSelf);
}



