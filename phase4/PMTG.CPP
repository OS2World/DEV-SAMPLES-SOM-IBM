// 3/8/96 15:52:41
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

#ifndef SOM_Module_pmtg_Source
#define SOM_Module_pmtg_Source
#endif
#define PersistentMeeting_Class_Source

#include "pmtg.xih"


SOM_Scope void SOMLINK somDefaultInit(PersistentMeeting *somSelf,
                                      som3InitCtrl* ctrl)
{
    PersistentMeetingData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentMeetingMethodDebug("PersistentMeeting","somDefaultInit");
    PersistentMeeting_BeginInitializer_somDefaultInit;

    PersistentMeeting_Init_DistributedMeeting_somDefaultInit(somSelf, ctrl);
    PersistentMeeting_Init_PersistentAppointment_somDefaultInit(somSelf, ctrl);

    /*
     * local PersistentMeeting initialization code added by programmer
     */
}


SOM_Scope void SOMLINK somDestruct(PersistentMeeting *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    PersistentMeetingData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentMeetingMethodDebug("PersistentMeeting","somDestruct");
    PersistentMeeting_BeginDestructor;

    /*
     * local PersistentMeeting deinitialization code added by programmer
     */

    PersistentMeeting_EndDestructor;
}



/*
 * called for IPO object initialization, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_creation(PersistentMeeting *somSelf,
                                                        Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    /* PersistentMeetingData *somThis = PersistentMeetingGetData(somSelf); */
    PersistentMeetingMethodDebug("PersistentMeeting","init_for_object_creation");
    PersistentMeeting_BeginInitializer_somDefaultInit;

    // Initialize PersistentAppointment
    newSelf =  PersistentMeeting_parent_PersistentAppointment_init_for_object_creation(somSelf,
                                                                                    ev);
    // Initialize DistributedMeeting
    PersistentMeeting_Init_DistributedMeeting_somDefaultInit(newSelf, ctrl);

    return newSelf;
}

/*
 * called only by OS Server, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_reactivation(PersistentMeeting *somSelf,
                                                            Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    /* PersistentMeetingData *somThis = PersistentMeetingGetData(somSelf); */
    PersistentMeetingMethodDebug("PersistentMeeting","init_for_object_reactivation");
    PersistentMeeting_BeginInitializer_somDefaultInit;

    // Initialize PersistentAppointment
    newSelf =  PersistentMeeting_parent_PersistentAppointment_init_for_object_reactivation(somSelf,
                                                                                                           ev);
    // Initialize DistributedMeeting
    PersistentMeeting_Init_DistributedMeeting_somDefaultInit(newSelf, ctrl);

    return newSelf;
}


SOM_Scope void  SOMLINK externalize_to_stream(PersistentMeeting *somSelf,
                                               Environment *ev,
                                              CosStream_StreamIO* stream)
{
    /* PersistentMeetingData *somThis = PersistentMeetingGetData(somSelf); */
    PersistentMeetingMethodDebug("PersistentMeeting","externalize_to_stream");

    PersistentMeeting_parent_PersistentAppointment_externalize_to_stream(somSelf,
                                                                           ev,
                                                                           stream);
    stream->write_string(ev, somSelf->Meeting::_get_location(ev));
    stream->write_string(ev, somSelf->DistributedMeeting::_get_appointmentName(ev));
    stream->write_long(ev, somSelf->DistributedMeeting::_get_numberOfUsers(ev));
}


SOM_Scope void  SOMLINK internalize_from_stream(PersistentMeeting *somSelf,
                                                 Environment *ev,
                                                CosStream_StreamIO* stream,
                                                CosLifeCycle_FactoryFinder* ff)
{
    /* PersistentMeetingData *somThis = PersistentMeetingGetData(somSelf); */
    PersistentMeetingMethodDebug("PersistentMeeting","internalize_from_stream");

    PersistentMeeting_parent_PersistentAppointment_internalize_from_stream(somSelf,
                                                                             ev,
                                                                             stream,
                                                                             ff);
    somSelf->Meeting::_set_location(ev, stream->read_string(ev));
    somSelf->DistributedMeeting::_set_appointmentName(ev, stream->read_string(ev));
    somSelf->DistributedMeeting::_set_numberOfUsers(ev, stream->read_long(ev));
}
