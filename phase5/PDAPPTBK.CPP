// 3/8/96 15:52:25
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

#ifndef SOM_Module_pdapptbk_Source
#define SOM_Module_pdapptbk_Source
#endif
#define PersistentDistributedAppointmentBook_Class_Source

#include <orb.xh>             // CORBA ORB object
#include <somd.xh>          // global C++ include file for DSOM definitions
#include <tss.xh>             // somf_TSortedSequence class
#include <tdict.xh>           // somf_TDictionary class
#include <tssitr.xh>          // somf_TSortedSequenceIterator class
#include <pappt.xh>         // PersistentAppointment class
#include <pmtg.xh>          // PersistentMeeting class
#include <pccall.xh>         // PersistentConferenceCall class
#include <key.xh>            // Appt_Key class
#include <dapptbk.xh>      // DistributedAppointmentBook class
#include <pfapptbk.xh>     // PersistentAppointmentBookFactory
#include "pdapptbk.xih"


/*
 *restore an appointment to the book
 */

SOM_Scope void  SOMLINK addToSequence(PersistentDistributedAppointmentBook *somSelf,
                                            Environment *ev,
                                           Appointment* oldAppointment)
{
    Appt_Key *key;
    Appointment_IDType apptId=0;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","addToSequence");

    key = new Appt_Key;
    apptId =  oldAppointment->_get_apptId(ev);

    // Add the object to the (sorted sequence) deque collection represented
    // by the receiving object
      (somSelf->_get_appointments(ev))->somfAdd(ev,  oldAppointment);

    // Add the key (apptId) and value (new appt *) to the dictionary
      key->_set_objectKey(ev, apptId);
      (somSelf->_get_appointmentsDictionary(ev))->somfAddKeyValuePairMM(ev, key, oldAppointment);
}


/*
 * The prototype for somDefaultInit was replaced by the following prototype:
 */
SOM_Scope void SOMLINK somDefaultInit(PersistentDistributedAppointmentBook *somSelf,
                                      som3InitCtrl* ctrl)
{
    PersistentDistributedAppointmentBookData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","somDefaultInit");
    PersistentDistributedAppointmentBook_BeginInitializer_somDefaultInit;

    PersistentDistributedAppointmentBook_Init_DistributedAppointmentBook_somDefaultInit(somSelf, ctrl);
    PersistentDistributedAppointmentBook_Init_somPersistence_IPO_somDefaultInit(somSelf, ctrl);
    PersistentDistributedAppointmentBook_Init_somStream_Streamable_somDefaultInit(somSelf, ctrl);

    /*
     * local PersistentDistributedAppointmentBook initialization code added by programmer
     */
}


/*
 * called for IPO object initialization, MUST override it
 */
/*
 * Be aware of:
 * One of the parent, DistributedAppointmentBook, doestn't inherit from somOS::ServiceBase,
 * the default emitted code for init_for_object_creation and init_for_object_reactivation
 * MUST be altered to get the object initialized and reactivated correctly
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_creation(PersistentDistributedAppointmentBook *somSelf,
                                                        Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","init_for_object_creation");
    PersistentDistributedAppointmentBook_BeginInitializer_somDefaultInit;
    // Initialize somPersistence_IPO
    newSelf = PersistentDistributedAppointmentBook_parent_somPersistence_IPO_init_for_object_creation(somSelf,
                                                                                         ev);
    // Initialize Appointment and Streamable
    PersistentDistributedAppointmentBook_Init_DistributedAppointmentBook_somDefaultInit(newSelf, ctrl);
    PersistentDistributedAppointmentBook_Init_somStream_Streamable_somDefaultInit(newSelf, ctrl);

    return newSelf;
 }


/*
 * Attribute modifiers
 */
/*
 * Be aware of:
 * One of the parent, DistributedAppointmentBook, doestn't inherit from the somOS::
 * ServiceBase class, the default emitted code for init_for_object_creation and init_for_
 * object_reactivation MUST be altered to get the object initialized and reactivated
 * correctly.
 */
SOM_Scope void SOMLINK somDestruct(PersistentDistributedAppointmentBook *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    PersistentDistributedAppointmentBookData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","somDestruct");
    PersistentDistributedAppointmentBook_BeginDestructor;

    /*
     * local PersistentDistributedAppointmentBook deinitialization code added by programmer
     */

    PersistentDistributedAppointmentBook_EndDestructor;
}


/*
 * called by OS Server, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_reactivation(PersistentDistributedAppointmentBook *somSelf,
                                                            Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","init_for_object_reactivation");
    PersistentDistributedAppointmentBook_BeginInitializer_somDefaultInit;

    // Initialize somPersistence_IPO
    newSelf = PersistentDistributedAppointmentBook_parent_somPersistence_IPO_init_for_object_reactivation(somSelf,
                                                                                      ev);
    // Initialize Appointment and Streamable
    PersistentDistributedAppointmentBook_Init_DistributedAppointmentBook_somDefaultInit(newSelf, ctrl);
    PersistentDistributedAppointmentBook_Init_somStream_Streamable_somDefaultInit(newSelf, ctrl);

    return newSelf;
}


/*
 * increment number of appointments
 */

SOM_Scope Appointment_IDType  SOMLINK addAppointment(PersistentDistributedAppointmentBook *somSelf,
                                                      Environment *ev,
                                                     Appointment* newAppointment)
{
    Appointment_IDType id;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","addAppointment");

    id =PersistentDistributedAppointmentBook_parent_DistributedAppointmentBook_addAppointment(somSelf,
                                                                                                  ev,
                                                                                                  newAppointment);
    // Increment the number of appointments
    if (ev->_major == NO_EXCEPTION) {
       somThis->numberOfAppts = (somSelf->_get_appointments(ev))->somfCount(ev);
    }

    return(id);
}


/*
 * delete appointment from the book
 */

SOM_Scope void  SOMLINK deleteAppointment(PersistentDistributedAppointmentBook *somSelf,
                                           Environment *ev, Appointment_IDType appointmentID)
{
    PersistentAppointment *pAppt=NULL;
    Appt_Key *key;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","deleteAppointment");

    key = new Appt_Key;
    key->_set_objectKey(ev,  appointmentID);

    // Delete the specified key from the dictionary
    pAppt = (PersistentAppointment *)(somSelf->_get_appointmentsDictionary(ev))->somfDeleteKey( ev, key);
    if(pAppt){

        // Remove the appointment from the sequence
       (somSelf->_get_appointments(ev))->somfRemove(ev, pAppt);

        // Free the appointment, somfRemove just removes the entry
        PersistentAppointmentBookFactory  *factory = NULL;
        ExtendedNaming_ExtendedNamingContext *enc = NULL;

        enc = (ExtendedNaming_ExtendedNamingContext *)
                 SOMD_ORBObject->resolve_initial_references(ev, "FactoryService");

        if (ev->_major != NO_EXCEPTION) {
             enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
        } else {
            // query the name service for a factory that creates a Appointment
            factory = (PersistentAppointmentBookFactory *)
                          enc->find_any(ev, "class == 'PersistentAppointment'", 0);
            if (ev->_major != NO_EXCEPTION) {
                 // Clean up after error
                 factory = (PersistentAppointmentBookFactory *)OBJECT_NIL;
                 ((SOMDClientProxy *)enc)->somdProxyFree(ev);
                 enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
            } else {
                 factory->deleteAppointment(ev,pAppt);

                 // Decrement the number of Appointments
                 somThis->numberOfAppts = (somSelf->_get_appointments(ev))->somfCount(ev);
            };
       };
    };
    key->somFree();

}


SOM_Scope void  SOMLINK externalize_to_stream(PersistentDistributedAppointmentBook *somSelf,
                                               Environment *ev,
                                              CosStream_StreamIO* stream)
{
    _IDL_SEQUENCE_Appointment allAppointments;
    PersistentAppointment     *Appt;
    unsigned long i;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","externalize_to_stream");

    PersistentDistributedAppointmentBook_parent_somStream_Streamable_externalize_to_stream(somSelf,
                                                                                 ev,
                                                                                 stream);

    stream->write_string(ev, somSelf->_get_bookName(ev));  // Write the appointment book user name
    stream->write_unsigned_long(ev, somSelf->_get_numberOfAppts(ev));  // Write number of appts
    allAppointments = somSelf->getAllAppointments(ev);

    for (i=0; i< sequenceLength(allAppointments); i++) {
       Appt = (PersistentAppointment *)sequenceElement(allAppointments,i);
       stream->write_object(ev, Appt);    // Only store the persistent reference, not the entire object
    }
}


SOM_Scope void  SOMLINK internalize_from_stream(PersistentDistributedAppointmentBook *somSelf,
                                                 Environment *ev,
                                                CosStream_StreamIO* stream,
                                                CosLifeCycle_FactoryFinder* ff)
{
    unsigned long numAppts, numUsers;
    PersistentAppointment     *Appt;
    somf_TSortedSequenceIterator *itr;

    PersistentDistributedAppointmentBookData *somThis = PersistentDistributedAppointmentBookGetData(somSelf);
    PersistentDistributedAppointmentBookMethodDebug("PersistentDistributedAppointmentBook","internalize_from_stream");

    PersistentDistributedAppointmentBook_parent_somStream_Streamable_internalize_from_stream(somSelf,
                                                                                   ev,
                                                                                   stream,
                                                                                   ff);
    // Release all objects in the old collection
    itr = (somf_TSortedSequenceIterator *)
                (somSelf->_get_appointments(ev))->somfCreateIterator(ev);
    // Determine the first object in the sorted sequence
    Appt =  (PersistentAppointment *)itr->somfFirst(ev);
    while (Appt != NULL) {
        if (somIsObj(Appt)) ((SOMDObject*)Appt)->release(ev);
        Appt = (PersistentAppointment *)itr->somfNext(ev);
    }
    itr->somFree();
    (somSelf->_get_appointments(ev))->somfRemoveAll(ev);
    (somSelf->_get_appointmentsDictionary(ev))->somfRemoveAll(ev);

    // Have to retrieve info in the same order as they were written into the stream
    string tempName = stream->read_string(ev);
    somSelf->_set_bookName(ev, tempName);     // Read the book name
    SOMFree(tempName);

    numAppts = stream->read_unsigned_long(ev);   // Read number of appts
    somSelf->_set_numberOfAppts(ev, numAppts);

    for (unsigned short i=0; i < numAppts; i++) {

       Appt = (PersistentAppointment *) (stream->read_object(ev, ff, NULL));

       // Restore the appointment to the book sequence
       somSelf->addToSequence(ev, Appt);
    }
}
