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
static char *sccsid = "@(#) 2.10 src/somples.os2/somd/c/event/eventcom.c, somx.somd, som2.5, s259604a 12/26/95 17:43:18 [1/24/96 17:31:05]";
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SOM_Module_eventcom_Source
#include <somd.h>
#include "eventcom.ih"

boolean   has_event = FALSE;
any       event_data = {NULL, (void *)0};

SOM_Scope void  SOMLINK EventComm_EventConnectiondisconnect(EventComm_EventConnection somSelf,
							     Environment *ev)
{
    /* EventComm_EventConnectionData *somThis = EventComm_EventConnectionGetData(somSelf); */

    EventComm_EventConnectionMethodDebug("EventComm_EventConnection","EventComm_EventConnectiondisconnect");

    _somFree(somSelf);

}


SOM_Scope void  SOMLINK EventComm_Consumerpush(EventComm_Consumer somSelf,
						Environment *ev,
						any* ev_data)
{
/*    EventComm_ConsumerData *somThis = EventComm_ConsumerGetData(somSelf); */
    Disconnected   *discon;

    EventComm_ConsumerMethodDebug("EventComm_Consumer","EventComm_Consumerpush");

    /*
     * Check to see whether the consumer_element struct containing "EventConnection"
     * interface exists for this Supplier
     */

    if (EventComm_Consumer__get_ev_con(somSelf, ev) != (EventComm_EventConnection)0) {
	TypeCode_free(event_data._type, ev);
	has_event = TRUE;
	event_data._type = ev_data->_type;
	event_data._value = ev_data->_value;
    }
    else { /* invoke exception handling  */
	discon = (Disconnected *)SOMMalloc(sizeof(Disconnected));
	discon->ErrCode = 9999;
	strcpy(discon->Reason, "Not connected to the event channel\n");
	somSetException(ev, USER_EXCEPTION, ex_Disconnected, (void *)discon);
    }
}

SOM_Scope void  SOMLINK EventComm_Consumerdisconnect(EventComm_Consumer somSelf,
						      Environment *ev)
{
/*    EventComm_ConsumerData *somThis = EventComm_ConsumerGetData(somSelf); */
    Disconnected   *discon;
    EventComm_EventConnection  evcon;

    EventComm_ConsumerMethodDebug("EventComm_Consumer","EventComm_Consumerdisconnect");
    evcon = EventComm_Supplier__get_ev_con(somSelf, ev);
    if (evcon != (EventComm_EventConnection)0) {
	EventComm_Consumer_parent_EventComm_EventConnection_disconnect(evcon, ev);
	EventComm_Supplier__set_ev_con(somSelf, ev, (EventComm_EventConnection)0);
    }
    else {
	discon = (Disconnected *)SOMMalloc(sizeof(Disconnected));
	discon->ErrCode = 9999;
	strcpy(discon->Reason, "Not connected to the event channel\n");
	somSetException(ev, USER_EXCEPTION, ex_Disconnected, (void *)discon);
    }
}


SOM_Scope boolean SOMLINK EventComm_Suppliertry_pull(EventComm_Supplier somSelf,
						   Environment *ev,
						   any* event)
{
/*    EventComm_SupplierData *somThis = EventComm_SupplierGetData(somSelf); */
    Disconnected   *discon;
    boolean        have_event = FALSE ;
    EventComm_SupplierMethodDebug("EventComm_Supplier","EventComm_Suppliertry_pull");

    /*
     * Check to see whether the supplier_element struct containing "EventConnection"
     * interface exists for this Consumer
     */
    if (EventComm_Supplier__get_ev_con(somSelf, ev) != (EventComm_EventConnection)0) {
	    if (has_event) {
		have_event = TRUE;
		has_event = FALSE;
	    }
	    else
		if (event_data._type == NULL) {
		    event_data._type = TC_long;
		    event_data._value = (long *) 0;
		}
	}
    else { /* eventconnection missing; invoke exception handling  */
	discon = (Disconnected *)SOMMalloc(sizeof(Disconnected));
	discon->ErrCode = 9999;
	strcpy(discon->Reason, "Not connected to the event channel\n");
	somSetException(ev, USER_EXCEPTION, ex_Disconnected, (void *)discon);
    }
    *event = event_data ;
    return (have_event);

}

SOM_Scope void  SOMLINK EventComm_Supplierdisconnect(EventComm_Supplier somSelf,
						      Environment *ev)
{
/*    EventComm_SupplierData *somThis = EventComm_SupplierGetData(somSelf); */
    Disconnected   *discon;
    EventComm_EventConnection  evcon;

    EventComm_SupplierMethodDebug("EventComm_Supplier","EventComm_Supplierdisconnect");
    evcon = EventComm_Supplier__get_ev_con(somSelf, ev);
    if (evcon != (EventComm_EventConnection)0) {
	EventComm_Consumer_parent_EventComm_EventConnection_disconnect(evcon, ev);
	EventComm_Supplier__set_ev_con(somSelf, ev, (EventComm_EventConnection)0);
    }
    else {
	discon = (Disconnected *)SOMMalloc(sizeof(Disconnected));
	discon->ErrCode = 9999;
	strcpy(discon->Reason, "Not connected to the event channel\n");
	somSetException(ev, USER_EXCEPTION, ex_Disconnected, (void *)discon);
    }
}





