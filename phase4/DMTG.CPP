// 3/8/96 15:42:37
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

#ifndef SOM_Module_dmtg_Source
#define SOM_Module_dmtg_Source
#endif
#define DistributedMeeting_Class_Source

#include "dmtg.xih"


SOM_Scope void SOMLINK somDefaultInit(DistributedMeeting *somSelf,
                                      som3InitCtrl* ctrl)
{
    DistributedMeetingData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    DistributedMeetingMethodDebug("DistributedMeeting","somDefaultInit");
    DistributedMeeting_BeginInitializer_somDefaultInit;

    DistributedMeeting_Init_Meeting_somDefaultInit(somSelf, ctrl);
    DistributedMeeting_Init_DistributeAppointment_somDefaultInit(somSelf, ctrl);

    /*
     * local DistributedMeeting initialization code added by programmer
     */
}


SOM_Scope void SOMLINK somDestruct(DistributedMeeting *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    DistributedMeetingData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    DistributedMeetingMethodDebug("DistributedMeeting","somDestruct");
    DistributedMeeting_BeginDestructor;

    /*
     * local DistributedMeeting deinitialization code added by programmer
     */

    DistributedMeeting_EndDestructor;
}


