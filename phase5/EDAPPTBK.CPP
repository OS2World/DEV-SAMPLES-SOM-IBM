//  3/8/96 16:13:12
/*
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
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


#ifndef SOM_Module_edapptbk_Source
#define SOM_Module_edapptbk_Source
#endif
#define EventPersDistAppointmentBook_Class_Source



#include <es_impl.xh>   // Event Service
#include <somnm.xh>     // Naming Service
#include <orb.xh>       // Object Request Broker
#include <tss.xh>       // Sorted Sequence Collection Class
#include <tssitr.xh>    // Sorted Sequence Iterator
#include <somd.xh>      // DSOM
#include <tdict.xh>     // Dictionary Collection Class
#include "pccall.xh"
#include "pmtg.xh"
#include "pappt.xh"
#include "appt.xh"
#include "mtg.xh"
#include "ccall.xh"
#include "apptbook.xh"
#include "dapptbk.xh"
#include "pdapptbk.xh"
#include "samptype.h"
#include "edapptbk.xih"
#include "apevents.xh"
#include "efapptbk.xh"
#include "key.xh"

#include <stdio.h>



//This method connects the appointment book to the event channel.

/*
 *This method connects the appointment book to the event channel.
 */

SOM_Scope void  SOMLINK eventChannelConnect(EventPersDistAppointmentBook *somSelf,
                                             Environment *ev)
{
    EventPersDistAppointmentBookData *somThis = EventPersDistAppointmentBookGetData(somSelf);
    EventPersDistAppointmentBookMethodDebug("EventPersDistAppointmentBook","eventChannelConnect");

    SomEventService_EventChannel *eventChannel = NULL;
    ApptBookEventService *apptBookEventService = new ApptBookEventService;

    // Get reference to event service
    eventChannel = (SomEventService_EventChannel *)
        apptBookEventService->getEventChannelRef(ev);
        checkEnv(ev);
    delete apptBookEventService;

    //Obtain SupplierAdmin object from event channel
    CosEventChannelAdmin_SupplierAdmin *supplierAdmin =
      eventChannel->for_suppliers(ev);
    checkEnv(ev);
    somPrintf("have supplierAdmin\n");

    //Obtain proxy Push Consumer object from the supplier admin object
    CosEventChannelAdmin_ProxyPushConsumer *aProxyPushConsumer =
      supplierAdmin->obtain_push_consumer(ev);
    checkEnv(ev);
    somPrintf("have ProxyPushConsumer\n");

    //connect push supplier to the consumers via the event channel's proxyPushConsummer
    aProxyPushConsumer->connect_push_supplier(ev,somSelf);
    checkEnv(ev);
    somPrintf("Event supplier connected\n");

    somPrintf("aProxyPushConsumer: %8x \n", aProxyPushConsumer);
    // Save reference to proxy push consumer in the Address book
    somSelf->_set_myProxyPushConsumer(ev,((unsigned long) aProxyPushConsumer));
    checkEnv(ev);
    somPrintf("done\n");

}

SOM_Scope Appointment_IDType  SOMLINK addAppointment(EventPersDistAppointmentBook *somSelf,
                                                      Environment *ev,
                                                     Appointment* newAppointment)
{
    EventPersDistAppointmentBookData *somThis = EventPersDistAppointmentBookGetData(somSelf);
    EventPersDistAppointmentBookMethodDebug("EventPersDistAppointmentBook","addAppointment");


    // Send an event to the event channel
    CosEventChannelAdmin_ProxyPushConsumer *ppc =
         (CosEventChannelAdmin_ProxyPushConsumer*) somSelf->_get_myProxyPushConsumer(ev);



    any myany;

    char str[20];
    char *buf;
    buf = str;

    buf = somSelf->_get_bookName(ev);
    myany._type = TypeCodeNew(tk_string, 20);
    myany._value = &buf;


    somPrintf("pushing event\n");
    checkEnv(ev);
    ppc->push(ev, &myany);
    checkEnv(ev);




    return (EventPersDistAppointmentBook_parent_PersistentDistributedAppointmentBook_addAppointment(somSelf,
                                                                                                    ev,
                                                                                                    newAppointment));
}




SOM_Scope void  SOMLINK deleteAppointment(EventPersDistAppointmentBook *somSelf,
                                           Environment *ev, Appointment_IDType appointmentID)
{
    EventPersDistAppointmentBookData *somThis = EventPersDistAppointmentBookGetData(somSelf);
    EventPersDistAppointmentBookMethodDebug("EventPersDistAppointmentBook","deleteAppointment");

    PersistentAppointment *pAppt=NULL;
    Appt_Key *key;

    key = new Appt_Key;
    key->_set_objectKey(ev,  appointmentID);

    // Delete the specified key from the dictionary
    pAppt = (PersistentAppointment *)(somSelf->_get_appointmentsDictionary(ev))->somfDeleteKey( ev, key);
    if(pAppt){

        // Remove the appointment from the sequence
       (somSelf->_get_appointments(ev))->somfRemove(ev, pAppt);

        // Free the appointment, somfRemove just removes the entry
        EventPersDistAppointmentBookFactory  *factory = NULL;
        ExtendedNaming_ExtendedNamingContext *enc = NULL;

        enc = (ExtendedNaming_ExtendedNamingContext *)
                 SOMD_ORBObject->resolve_initial_references(ev, "FactoryService");

        if (ev->_major != NO_EXCEPTION) {
             enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
        } else {
            // query the name service for an appointment book factory to create an Appointment
            factory = (EventPersDistAppointmentBookFactory *)
                          enc->find_any(ev, "class == 'EventPersDistAppointmentBook'", 0);
            if (ev->_major != NO_EXCEPTION) {
                 // Clean up after error
                 factory = (EventPersDistAppointmentBookFactory *)OBJECT_NIL;
                 ((SOMDClientProxy *)enc)->somdProxyFree(ev);
                 enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
            } else {
                 factory->deleteAppointment(ev,pAppt);

                 // Decrement the number of Appointments
                 somSelf->_set_numberOfAppts(ev, (somSelf->_get_appointments(ev))->somfCount(ev) );
            };
       };
    };
    key->somFree();


}

