// 3/8/96 09:36:52
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
#ifndef SOM_Module_apptbook_Source
#define SOM_Module_apptbook_Source
#endif
#define AppointmentBook_Class_Source

#include "appt.xh"                       //Appointment class include file
#include "mtg.xh"                        //Meeting class include file
#include "ccall.xh"                      //ConferenceCall class include file
#include "apptbook.xih"
#include "samptype.h"                    //sample constants. For exceptions.

#include <tss.xh>                        //TSortedSequence collection class include
#include <tssitr.xh>                     //TSortedSequence iterator class include
#include <key.xh>                        //Key class for the sequence
#include <tassoc.xh>                     //somf_TAssoc used to pair objects to a key
#include <tdict.xh>                      //somf_TDictionary used for mapping using the keys.


/*
 * Add an appointment to the book Returns an appointment number.
 */

SOM_Scope Appointment_IDType  SOMLINK addAppointment(AppointmentBook *somSelf,
                                                      Environment *ev,
                                                     Appointment* newAppointment)
{
    AppointmentBookData *somThis = AppointmentBookGetData(somSelf);
    Appt_Key *key;
    Appointment *appt;
    Appointment_IDType apptId=0;
    long i;
    _IDL_SEQUENCE_Appointment apptList;
    AppointmentBookMethodDebug("AppointmentBook","addAppointment");


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
      apptId =  somSelf->_get_apptId(ev);
      somSelf->_set_apptId(ev, ++apptId);
      newAppointment->_set_apptId(ev, apptId);

    // Add the object to the (sorted sequence) deque collection represented
    // by the receiving object
      (somThis->appointments)->somfAdd(ev,  newAppointment);

    // Add the key (apptId) and value (new appt *) to the dictionary
      key->_set_objectKey( ev, apptId);
      (somThis->appointmentsDictionary)->somfAddKeyValuePairMM(ev, key, newAppointment);
    }
    return(apptId);

}
/*
 * Return a sequence of the given day's appointments.  Freeing the sequence
 * is the callers responsibility
 */

SOM_Scope _IDL_SEQUENCE_Appointment  SOMLINK getDaysAppointments(AppointmentBook *somSelf,
                                                                  Environment *ev,
                                                                 short year,
                                                                 short month,
                                                                 short day)
{
    AppointmentBookData *somThis = AppointmentBookGetData(somSelf);

    _IDL_SEQUENCE_Appointment daysAppointments = {0,0,(Appointment**)NULL};
    Appointment *appt;
    long count;
    somf_TSortedSequenceIterator *itr;

    AppointmentBookMethodDebug("AppointmentBook","getDaysAppointments");

    if (somThis->appointments) {  // If there are any appointments
      // Determine the number of objects in the sequence
      count = (somThis->appointments)->somfCount(ev);
      if (count) {
        // Initialize the sequence
        daysAppointments._buffer = (Appointment **)SOMMalloc(sizeof(Appointment *) * count);
        sequenceMaximum(daysAppointments) =  count;
        sequenceLength(daysAppointments) = 0;

        // Create an iterator suitable for iterating over the appointment objects
        itr = (somf_TSortedSequenceIterator *)
                    (somThis->appointments)->somfCreateIterator(ev);
        // Determine the first object in the sorted sequence
        appt =  (Appointment *)itr->somfFirst(ev);
        while (appt != NULL) {
          if ((appt->_get_year(ev) == year) &&
              (appt->_get_month(ev) == month) &&
              (appt->_get_day(ev) == day)) {
             sequenceElement(daysAppointments,
                             sequenceLength(daysAppointments)) = appt;
             ++ sequenceLength(daysAppointments);
          }
          appt = (Appointment *)itr->somfNext(ev);
        }
        itr->somFree();
      } /* endif */
    } /* endif */
    return(daysAppointments);
}

/*
 * Delete the given appointment from the book.
 */

SOM_Scope void  SOMLINK deleteAppointment(AppointmentBook *somSelf,
                                           Environment *ev, Appointment_IDType appointmentID)
{
    AppointmentBookData *somThis = AppointmentBookGetData(somSelf);

    Appointment *appt;
    Appt_Key *key;

    AppointmentBookMethodDebug("AppointmentBook","deleteAppointment");

    key = new Appt_Key;
    key->_set_objectKey(ev,  appointmentID);
      // Delete the specified key from the dictionary
    appt = (Appointment *)(somThis->appointmentsDictionary)->somfDeleteKey( ev, key);
    if(appt){
         // Remove the appointment from the sequence
       (somThis->appointments)->somfRemove(ev, appt);
         // Free the appointment, somfRemove just removes the entry
       appt->somFree();
    }
    key->somFree();

}

/*
 * Return a sequence of the appointments. Freeing the sequece is the
 * callers responsibility
 */

SOM_Scope _IDL_SEQUENCE_Appointment  SOMLINK getAllAppointments(AppointmentBook *somSelf,
                                                                 Environment *ev)
{
    AppointmentBookData *somThis = AppointmentBookGetData(somSelf);

    _IDL_SEQUENCE_Appointment allAppointments;
    Appointment *appt;
    long count;
    somf_TSortedSequenceIterator *itr;

    AppointmentBookMethodDebug("AppointmentBook","getAllAppointments");

    if (somThis->appointments) {  // If there are any appointments
             // Get count of the appointments
      count = (somThis->appointments)->somfCount(ev);
             // Initialize the return sequence
      allAppointments._buffer =  (Appointment **)SOMMalloc(sizeof(Appointment *) * count);
      sequenceMaximum(allAppointments) =  count;
      sequenceLength(allAppointments) = 0;

      itr = (somf_TSortedSequenceIterator *)
            (somThis->appointments)->somfCreateIterator(ev);
      appt = (Appointment *)itr->somfFirst(ev);
                           // Complete the sequence
      while (appt) {
        sequenceElement(allAppointments,sequenceLength(allAppointments)) =
          appt;
        ++ sequenceLength(allAppointments);
        appt = (Appointment *)itr->somfNext(ev);
      }
      itr->somFree();
    }
    return(allAppointments);
}

/*
 * Return a new uninitialized Meeting
 */

SOM_Scope void SOMLINK somDefaultInit(AppointmentBook *somSelf,
                                      som3InitCtrl* ctrl)
{
    AppointmentBookData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    Environment ev;
    AppointmentBookMethodDebug("AppointmentBook","somDefaultInit");
    AppointmentBook_BeginInitializer_somDefaultInit;

    AppointmentBook_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /*
     * local AppointmentBook initialization code added by programmer
     */
    somThis->appointments = new somf_TSortedSequence;
    somThis->appointmentsDictionary = new somf_TDictionary;
    somSelf->_set_apptId(&ev, 0);
}


SOM_Scope void SOMLINK somDestruct(AppointmentBook *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    AppointmentBookData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentBookMethodDebug("AppointmentBook","somDestruct");
    AppointmentBook_BeginDestructor;

    /*
     * local AppointmentBook deinitialization code added by programmer
     */

    (somThis->appointments)->somfRemoveAll(somGetGlobalEnvironment());
    (somThis->appointments)->somFree();
    (somThis->appointmentsDictionary)->somFree();

    AppointmentBook_EndDestructor;
}
