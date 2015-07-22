// 3/8/96 15:42:30
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

#ifndef SOM_Module_dccall_Source
#define SOM_Module_dccall_Source
#endif
#define DistributedConferenceCall_Class_Source

#include "dccall.xih"


SOM_Scope void SOMLINK somDefaultInit(DistributedConferenceCall *somSelf,
                                      som3InitCtrl* ctrl)
{
    DistributedConferenceCallData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    DistributedConferenceCallMethodDebug("DistributedConferenceCall","somDefaultInit");
    DistributedConferenceCall_BeginInitializer_somDefaultInit;

    DistributedConferenceCall_Init_ConferenceCall_somDefaultInit(somSelf, ctrl);
    DistributedConferenceCall_Init_DistributeAppointment_somDefaultInit(somSelf, ctrl);

    /*
     * local DistributedConferenceCall initialization code added by programmer
     */
}


SOM_Scope void SOMLINK somDestruct(DistributedConferenceCall *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    DistributedConferenceCallData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    DistributedConferenceCallMethodDebug("DistributedConferenceCall","somDestruct");
    DistributedConferenceCall_BeginDestructor;

    /*
     * local DistributedConferenceCall deinitialization code added by programmer
     */

    DistributedConferenceCall_EndDestructor;
}


