// 3/8/96 15:41:47
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

#ifndef SOM_Module_dapptbk_Source
#define SOM_Module_dapptbk_Source
#endif
#define DistributedAppointmentBook_Class_Source

#include <somd.xh>                       //DSOM include file
#include <somdcprx.xh>                   //DSOM create proxy include for factory proxy.

#include "appt.xh"                       //Appointment class include file
#include "mtg.xh"                        //Meeting class include file
#include "ccall.xh"                      //ConferenceCall class include file
#include "samptype.h"                    //sample constants. For exceptions.
#include "dappt.xh"                      //DistributeAppointment class include file.
#include "dapptbk.xih"

#include <tss.xh>                        //TSortedSequence collection class include
#include <tssitr.xh>                     //TSortedSequence iterator class include
#include <key.xh>                        //Key class for the sequence
#include <tassoc.xh>                     //somf_TAssoc used to pair objects to a key
#include <tdict.xh>                      //somf_TDictionary used for mapping using the keys.
#include "fapptbk.xh"                    //AppointmentBookFactory class include for deleting appts in the book.


/*
 * This attribute maintains the name of the appointment book.
 */

SOM_Scope void  SOMLINK _set_bookName(DistributedAppointmentBook *somSelf,
                                       Environment *ev, string bookName)
{
    DistributedAppointmentBookData *somThis = DistributedAppointmentBookGetData(somSelf);
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","_set_bookName");

    if (somThis->bookName != NULL) {
      SOMFree(somThis->bookName);
      somThis->bookName = NULL;
    }
    if (bookName == NULL) return;   // Passed in string empty
    somThis->bookName = (string)SOMMalloc(strlen(bookName)+1);
    strcpy(somThis->bookName, bookName);
}

/*
 * The userid of the owner of this appointment book.
 */

SOM_Scope void  SOMLINK _set_bookOwner(DistributedAppointmentBook *somSelf,
                                        Environment *ev, string bookOwner)
{
    DistributedAppointmentBookData *somThis = DistributedAppointmentBookGetData(somSelf);
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","_set_bookOwner");

    if (somThis->bookOwner != NULL) {
      SOMFree(somThis->bookOwner);
      somThis->bookOwner = NULL;
    }
    if (bookOwner == NULL) return;   // Passed in string empty
    somThis->bookOwner = (string)SOMMalloc(strlen(bookOwner)+1);
    strcpy(somThis->bookOwner, bookOwner);
}

SOM_Scope void  SOMLINK incrementUsers(DistributedAppointmentBook *somSelf,
                                        Environment *ev)
{
    DistributedAppointmentBookData *somThis = DistributedAppointmentBookGetData(somSelf);
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","incrementUsers");

    somThis->numberOfUsers++;
}

SOM_Scope void  SOMLINK decrementUsers(DistributedAppointmentBook *somSelf,
                                        Environment *ev)
{
    DistributedAppointmentBookData *somThis = DistributedAppointmentBookGetData(somSelf);
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","decrementUsers");

    somThis->numberOfUsers--;
}

SOM_Scope void SOMLINK somDefaultInit(DistributedAppointmentBook *somSelf,
                                      som3InitCtrl* ctrl)
{
    DistributedAppointmentBookData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","somDefaultInit");
    DistributedAppointmentBook_BeginInitializer_somDefaultInit;

    DistributedAppointmentBook_Init_AppointmentBook_somDefaultInit(somSelf, ctrl);

    /*
     * local DistributedAppointmentBook initialization code added by programmer
     */
     somThis->numberOfUsers = 0;
}


/*
 * Attribute modifiers
 */
SOM_Scope void SOMLINK somDestruct(DistributedAppointmentBook *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    DistributedAppointmentBookData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","somDestruct");
    DistributedAppointmentBook_BeginDestructor;

    /*
     * local DistributedAppointmentBook deinitialization code added by programmer
     */
    if (somThis->bookName != NULL) {
      SOMFree(somThis->bookName);
      somThis->bookName = NULL;
    }

    if (somThis->bookOwner != NULL) {
      SOMFree(somThis->bookOwner);
      somThis->bookOwner = NULL;
    }
    DistributedAppointmentBook_EndDestructor;
}



SOM_Scope Appointment_IDType  SOMLINK addAppointment(DistributedAppointmentBook *somSelf,
                                                      Environment *ev,
                                                     Appointment* newAppointment)
{
    DistributedAppointmentBookData *somThis = DistributedAppointmentBookGetData(somSelf);
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","addAppointment");

    Appt_Key *key;
    Appointment *appt;
    Appointment_IDType apptId=0;
    long i;
    _IDL_SEQUENCE_Appointment apptList;


    // Check to see if an appointment like this already exists.
    apptList = somSelf->getDaysAppointments(ev, newAppointment->_get_year(ev),
                                                newAppointment->_get_month(ev),
                                                newAppointment->_get_day(ev));
    for (i=0; i< sequenceLength(apptList) && ev->_major == NO_EXCEPTION; i++) {
       appt = sequenceElement(apptList,i);
       if ( (appt->_get_start(ev) == newAppointment->_get_start(ev)) &&
              (appt->_get_end(ev) == newAppointment->_get_end(ev)) ) {
         // raise the exception
         OverLap *overlap = (OverLap *)SOMMalloc(sizeof(OverLap));
         overlap->ErrCode = OVERLAP;
         strcpy(overlap->Reason, "Appointment already exists");
         somSetException(ev, USER_EXCEPTION, ex_AppointmentBook_OverLap,
                          (void *)overlap);
       }
    }
    SOMFree(apptList._buffer);

    if (ev->_major == NO_EXCEPTION) {
      key = new Appt_Key;
      apptId =  newAppointment->_get_apptId(ev);
      ((DistributeAppointment*)(void*)newAppointment)->incrementUsers(ev);
    // Add the object to the (sorted sequence) deque collection represented
    // by the receiving object
      (somSelf->_get_appointments(ev))->somfAdd(ev,  newAppointment);

    // Add the key (apptId) and value (new appt *) to the dictionary
      key->_set_objectKey( ev, apptId);
      (somSelf->_get_appointmentsDictionary(ev))->somfAddKeyValuePairMM(ev, key, newAppointment);
    }
    return(apptId);
}

/*
 * Attribute modifiers
 */

SOM_Scope void  SOMLINK deleteAppointment(DistributedAppointmentBook *somSelf,
                                           Environment *ev, Appointment_IDType appointmentID)
{
    DistributedAppointmentBookData *somThis = DistributedAppointmentBookGetData(somSelf);
    DistributedAppointmentBookMethodDebug("DistributedAppointmentBook","deleteAppointment");
    Appointment *appt;
    Appt_Key *key;

    key = new Appt_Key;
    key->_set_objectKey(ev,  appointmentID);
      // Delete the specified key from the dictionary
    appt = (Appointment *)(somSelf->_get_appointmentsDictionary(ev))->somfDeleteKey( ev, key);
    if(appt){
         // Remove the appointment from the sequence
       (somSelf->_get_appointments(ev))->somfRemove(ev, appt);
         // Free the appointment, somfRemove just removes the entry
       AppointmentBookFactory  *factory=NULL;
       ExtendedNaming_ExtendedNamingContext *enc=NULL;
       enc = (ExtendedNaming_ExtendedNamingContext *)
              SOMD_ORBObject->resolve_initial_references(ev, "FactoryService");
       if (ev->_major != NO_EXCEPTION) {
            enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
       } else {
                  // query the name service for a factory
                  // that creates an Appointment Book object
            factory = (AppointmentBookFactory *)
                   enc->find_any(ev, "class == 'DistributeAppointment'", 0);
            if (ev->_major != NO_EXCEPTION) {
                  // Clean up after error
              factory = (AppointmentBookFactory *)OBJECT_NIL;
              ((SOMDClientProxy *)enc)->somdProxyFree(ev);
              enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
            } else {
               factory->deleteAppointment(ev,appt);
            };
       };
    };
    key->somFree();
}


