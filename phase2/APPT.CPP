// 3/8/96 09:36:23
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

#ifndef SOM_Module_appt_Source
#define SOM_Module_appt_Source
#endif

#define Appointment_Class_Source

#include "appt.xih"

#include <stdio.h>
#include "samptype.h"  // Application include file


/*
 * This method returns the external representation of the
 * appointment as a string.  The caller should free the
 * returned string with SOMFree.
 * Attributes:
 */

SOM_Scope string  SOMLINK bufferize(Appointment *somSelf,  Environment *ev)
{
    AppointmentData *somThis = AppointmentGetData(somSelf);
    AppointmentMethodDebug("Appointment","bufferize");

    if (somThis->bp != NULL) {
      SOMFree(somThis->bp);
    }
    somThis->bp = (string)SOMMalloc(BUF_SIZE);
    sprintf(somThis->bp, "(%d) Date: %d/%d/%d Start: %d End: %d\nSubject: %s\n",
      somThis->apptId, somThis->month, somThis->day, somThis->year,
       somThis->start, somThis->end, (somThis->subject)?somThis->subject:"");

    return(somThis->bp);
}

/*
 * The appointment subject
 */

SOM_Scope void  SOMLINK _set_subject(Appointment *somSelf,  Environment *ev,
                                     string subject)
{
    AppointmentData *somThis = AppointmentGetData(somSelf);
    AppointmentMethodDebug("Appointment","_set_subject");

    if (somThis->subject != NULL) {
      SOMFree(somThis->subject);
      somThis->subject = NULL;
    }
    if(subject == NULL) return;   // Passed in string empty
    somThis->subject = (string)SOMMalloc(strlen(subject)+1);
    strcpy(somThis->subject, subject);
}



SOM_Scope void SOMLINK somDefaultInit(Appointment *somSelf, som3InitCtrl* ctrl)
{
    AppointmentData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentMethodDebug("Appointment","somDefaultInit");
    Appointment_BeginInitializer_somDefaultInit;

    Appointment_Init_somf_MOrderableCollectible_somDefaultInit(somSelf, ctrl);

    /*
     * local Appointment initialization code added by programmer
     */
    somThis->year = 0;
    somThis->month = 0;
    somThis->day = 0;
    somThis->start = 0;
    somThis->end = 0;
    somThis->subject = NULL;
    somThis->apptId = 0;
    somThis->bp = NULL;
}


SOM_Scope void SOMLINK somDestruct(Appointment *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    AppointmentData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentMethodDebug("Appointment","somDestruct");
    Appointment_BeginDestructor;

    /*
     * local Appointment deinitialization code added by programmer
     */
    somThis->year = 0;
    somThis->month = 0;
    somThis->day = 0;
    somThis->start = 0;
    somThis->end = 0;
    if (somThis->subject != NULL) {
      SOMFree(somThis->subject);
      somThis->subject = NULL;
    }
    if (somThis->bp != NULL) {
      SOMFree(somThis->bp);
      somThis->bp = NULL;
    }

    Appointment_EndDestructor;
}


SOM_Scope boolean  SOMLINK somfIsEqual(Appointment *somSelf,
                                        Environment *ev, somf_MCollectible* obj)
{
    AppointmentData *somThis = AppointmentGetData(somSelf);
    AppointmentMethodDebug("Appointment","somfIsEqual");

    if (somThis->apptId == ((Appointment *)obj)->_get_apptId(ev)) return(TRUE);
    return(FALSE);
}
/*
 * Required by somf_TDictionary
 */

/*
 * Override required by somf_TDictionary to
 * return an entry look-up value
 * These overrides are required by somf_TSortedSequece so that
 * the elements can be inserted in the list with the correct relationship
 */

SOM_Scope long  SOMLINK somfHash(Appointment *somSelf,  Environment *ev)
{
    AppointmentData *somThis = AppointmentGetData(somSelf);
    AppointmentMethodDebug("Appointment","somfHash");

    return (somThis->apptId);
}


SOM_Scope boolean  SOMLINK somfIsLessThan(Appointment *somSelf,
                                           Environment *ev, somf_MOrderableCollectible* obj)
{
    AppointmentData *somThis = AppointmentGetData(somSelf);
    AppointmentMethodDebug("Appointment","somfIsLessThan");

    if (((Appointment *)obj)->_get_year(ev) < somThis->year) return(TRUE);
    else if (((Appointment *)obj)->_get_year(ev) == somThis->year){
       if (((Appointment *)obj)->_get_month(ev) < somThis->month) return(TRUE);
       else if (((Appointment *)obj)->_get_month(ev) == somThis->month) {
          if (((Appointment *)obj)->_get_day(ev) < somThis->day) return(TRUE);
          else if (((Appointment *)obj)->_get_day(ev) == somThis->day){
             if (((Appointment *)obj)->_get_start(ev) < somThis->start) return(TRUE);
             else if (((Appointment *)obj)->_get_start(ev) == somThis->start) {
               if (((Appointment *)obj)->_get_end(ev) < somThis->end) return(TRUE);
             }
          }
       }
    }
    return(FALSE);
}


/*
 * Attribute modifiers
 */

SOM_Scope boolean  SOMLINK somfIsGreaterThan(Appointment *somSelf,
                                              Environment *ev,
                                             somf_MOrderableCollectible* obj)
{
    AppointmentData *somThis = AppointmentGetData(somSelf);
    AppointmentMethodDebug("Appointment","somfIsGreaterThan");

    if (((Appointment *)obj)->_get_year(ev) > somThis->year) return(TRUE);
    else if (((Appointment *)obj)->_get_year(ev) == somThis->year){
       if (((Appointment *)obj)->_get_month(ev) > somThis->month) return(TRUE);
       else if (((Appointment *)obj)->_get_month(ev) == somThis->month) {
          if (((Appointment *)obj)->_get_day(ev) > somThis->day) return(TRUE);
          else if (((Appointment *)obj)->_get_day(ev) == somThis->day){
             if (((Appointment *)obj)->_get_start(ev) > somThis->start) return(TRUE);
             else if (((Appointment *)obj)->_get_start(ev) == somThis->start) {
               if (((Appointment *)obj)->_get_end(ev) > somThis->end) return(TRUE);
             }
          }
       }
    }
    return(FALSE);
}

