// 3/8/96 15:52:18
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

#ifndef SOM_Module_pccall_Source
#define SOM_Module_pccall_Source
#endif
#define PersistentConferenceCall_Class_Source

#include "pccall.xih"


SOM_Scope void SOMLINK somDefaultInit(PersistentConferenceCall *somSelf,
                                      som3InitCtrl* ctrl)
{
    PersistentConferenceCallData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentConferenceCallMethodDebug("PersistentConferenceCall","somDefaultInit");
    PersistentConferenceCall_BeginInitializer_somDefaultInit;

    PersistentConferenceCall_Init_DistributedConferenceCall_somDefaultInit(somSelf, ctrl);
    PersistentConferenceCall_Init_PersistentAppointment_somDefaultInit(somSelf, ctrl);

    /*
     * local PersistentConferenceCall initialization code added by programmer
     */
}


SOM_Scope void SOMLINK somDestruct(PersistentConferenceCall *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    PersistentConferenceCallData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentConferenceCallMethodDebug("PersistentConferenceCall","somDestruct");
    PersistentConferenceCall_BeginDestructor;

    /*
     * local PersistentConferenceCall deinitialization code added by programmer
     */

    PersistentConferenceCall_EndDestructor;
}


/*
 * called for IPO object initialization, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_creation(PersistentConferenceCall *somSelf,
                                                        Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    /* PersistentConferenceCallData *somThis = PersistentConferenceCallGetData(somSelf); */
    PersistentConferenceCallMethodDebug("PersistentConferenceCall","init_for_object_creation");
    PersistentConferenceCall_BeginInitializer_somDefaultInit;
    // Initialize PersistentAppointment
    newSelf =PersistentConferenceCall_parent_PersistentAppointment_init_for_object_creation(somSelf,
                                                                                           ev);
    // Initialize DistributedConferenceCall
    PersistentConferenceCall_Init_DistributedConferenceCall_somDefaultInit(somSelf, ctrl);

    return newSelf;
}

/*
 * called by OS Server, MUST override it
 */

/*
 * called only by OS Server, MUST override it
 */

SOM_Scope SOMObject*  SOMLINK init_for_object_reactivation(PersistentConferenceCall *somSelf,
                                                            Environment *ev)
{
    SOMObject *newSelf;
    som3InitCtrl* ctrl = NULL;
    somInitCtrl globalCtrl;
    somBooleanVector myMask;

    /* PersistentConferenceCallData *somThis = PersistentConferenceCallGetData(somSelf); */
    PersistentConferenceCallMethodDebug("PersistentConferenceCall","init_for_object_reactivation");
    PersistentConferenceCall_BeginInitializer_somDefaultInit;

    // Initialize PersistentAppointment
    newSelf =PersistentConferenceCall_parent_PersistentAppointment_init_for_object_reactivation(somSelf,
                                                                                                                   ev);
    // Initialize DistributedConferenceCall
    PersistentConferenceCall_Init_DistributedConferenceCall_somDefaultInit(newSelf, ctrl);

    return newSelf;
}


SOM_Scope void  SOMLINK externalize_to_stream(PersistentConferenceCall *somSelf,
                                               Environment *ev,
                                              CosStream_StreamIO* stream)
{
    /* PersistentConferenceCallData *somThis = PersistentConferenceCallGetData(somSelf); */
    PersistentConferenceCallMethodDebug("PersistentConferenceCall","externalize_to_stream");

    PersistentConferenceCall_parent_PersistentAppointment_externalize_to_stream(somSelf,
                                                                                  ev,
                                                                                   stream);
    stream->write_string(ev, somSelf->ConferenceCall::_get_phoneNumber(ev));
    stream->write_string(ev, somSelf->DistributedConferenceCall::_get_appointmentName(ev));
    stream->write_long(ev, somSelf->DistributedConferenceCall::_get_numberOfUsers(ev));
}


SOM_Scope void  SOMLINK internalize_from_stream(PersistentConferenceCall *somSelf,
                                                 Environment *ev,
                                                CosStream_StreamIO* stream,
                                                CosLifeCycle_FactoryFinder* ff)
{
    /* PersistentConferenceCallData *somThis = PersistentConferenceCallGetData(somSelf); */
    PersistentConferenceCallMethodDebug("PersistentConferenceCall","internalize_from_stream");

    PersistentConferenceCall_parent_PersistentAppointment_internalize_from_stream(somSelf,
                                                                                    ev,
                                                                                    stream,
                                                                                    ff);
    somSelf->ConferenceCall::_set_phoneNumber(ev, stream->read_string(ev));
    somSelf->DistributedConferenceCall::_set_appointmentName(ev, stream->read_string(ev));
    somSelf->DistributedConferenceCall::_set_numberOfUsers(ev, stream->read_long(ev));
}

