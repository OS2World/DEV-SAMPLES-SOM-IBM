// 3/8/96 15:52:12
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

#ifndef SOM_Module_pappt_Source
#define SOM_Module_pappt_Source
#endif
#define PersistentAppointment_Class_Source

#include "pappt.xih"


SOM_Scope void SOMLINK somDefaultInit(PersistentAppointment *somSelf,
                                      som3InitCtrl* ctrl)
{
    PersistentAppointmentData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentAppointmentMethodDebug("PersistentAppointment","somDefaultInit");
    PersistentAppointment_BeginInitializer_somDefaultInit;

    PersistentAppointment_Init_Appointment_somDefaultInit(somSelf, ctrl);
    PersistentAppointment_Init_somStream_Streamable_somDefaultInit(somSelf, ctrl);

    /*
     * local PersistentAppointment initialization code added by programmer
     */
}


/*
 * Be aware of:
 * One of the parent, Appointment, doestn't inherit from the somOS::ServiceBase class,
 * so the default emitted code for init_for_object_creation and init_for_object_reactivation
 * MUST be altered to have the object initialized and reactivated correctly.
 */
SOM_Scope void SOMLINK somDestruct(PersistentAppointment *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    PersistentAppointmentData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentAppointmentMethodDebug("PersistentAppointment","somDestruct");
    PersistentAppointment_BeginDestructor;

    /*
     * local PersistentAppointment deinitialization code added by programmer
     */

    PersistentAppointment_EndDestructor;
}


/*
 * called for IPO object initialization, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_creation(PersistentAppointment *somSelf,
                                                        Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    /* PersistentAppointmentData *somThis = PersistentAppointmentGetData(somSelf); */
    PersistentAppointmentMethodDebug("PersistentAppointment","init_for_object_creation");
    PersistentAppointment_BeginInitializer_somDefaultInit;

    // Initialize somPersistence_IPO
    newSelf = PersistentAppointment_parent_somPersistence_IPO_init_for_object_creation(somSelf,
                                                                             ev);
    // Initialize Appointment and Streamable
    PersistentAppointment_Init_Appointment_somDefaultInit(newSelf, ctrl);
    PersistentAppointment_Init_somStream_Streamable_somDefaultInit(newSelf, ctrl);

    return newSelf;

}


/*
 * called by OS Server, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_reactivation(PersistentAppointment *somSelf,
                                                            Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    /* PersistentAppointmentData *somThis = PersistentAppointmentGetData(somSelf); */
    PersistentAppointmentMethodDebug("PersistentAppointment","init_for_object_reactivation");
    PersistentAppointment_BeginInitializer_somDefaultInit;

    // Initialize somPersistence_IPO
    newSelf = PersistentAppointment_parent_somPersistence_IPO_init_for_object_reactivation(somSelf,
                                                                                 ev);
    // Initialize Appointment and Streamable
    PersistentAppointment_Init_Appointment_somDefaultInit(newSelf, ctrl);
    PersistentAppointment_Init_somStream_Streamable_somDefaultInit(newSelf, ctrl);

    return newSelf;
}

SOM_Scope void  SOMLINK externalize_to_stream(PersistentAppointment *somSelf,
                                               Environment *ev,
                                              CosStream_StreamIO* stream)
{
    /* PersistentAppointmentData *somThis = PersistentAppointmentGetData(somSelf); */
    PersistentAppointmentMethodDebug("PersistentAppointment","externalize_to_stream");

    PersistentAppointment_parent_somStream_Streamable_externalize_to_stream(somSelf,
                                                                             ev,
                                                                             stream);

    stream->write_short(ev, somSelf->Appointment::_get_year(ev));
    stream->write_short(ev, somSelf->Appointment::_get_month(ev));
    stream->write_short(ev, somSelf->Appointment::_get_day(ev));
    stream->write_unsigned_long(ev, somSelf->Appointment::_get_start(ev));
    stream->write_unsigned_long(ev, somSelf->Appointment::_get_end(ev));
    stream->write_string(ev,somSelf->Appointment::_get_subject(ev));
    stream->write_unsigned_long(ev, somSelf->Appointment::_get_apptId(ev));
}


SOM_Scope void  SOMLINK internalize_from_stream(PersistentAppointment *somSelf,
                                                 Environment *ev,
                                                CosStream_StreamIO* stream,
                                                CosLifeCycle_FactoryFinder* ff)
{
    /* PersistentAppointmentData *somThis = PersistentAppointmentGetData(somSelf); */
    PersistentAppointmentMethodDebug("PersistentAppointment","internalize_from_stream");

    PersistentAppointment_parent_somStream_Streamable_internalize_from_stream(somSelf,
                                                                               ev,
                                                                               stream,
                                                                               ff);
    // Have to retrieve info in the same order as they were written into the stream
    somSelf->Appointment::_set_year(ev, stream->read_short(ev));
    somSelf->Appointment::_set_month(ev, stream->read_short(ev));
    somSelf->Appointment::_set_day(ev, stream->read_short(ev));
    somSelf->Appointment::_set_start(ev, stream->read_unsigned_long(ev));
    somSelf->Appointment::_set_end(ev, stream->read_unsigned_long(ev));
    somSelf->Appointment::_set_subject(ev, stream->read_string(ev));
    somSelf->Appointment::_set_apptId(ev, stream->read_unsigned_long(ev));
}


