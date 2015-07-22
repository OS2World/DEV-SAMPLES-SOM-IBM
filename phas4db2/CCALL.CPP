// 3/8/96 09:36:46
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

#ifndef SOM_Module_ccall_Source
#define SOM_Module_ccall_Source
#endif
#define ConferenceCall_Class_Source

#include "ccall.xih"

#include "samptype.h" // Application include file
#include <stdio.h>

/*
 * The phoneNumber for the appointment.
 */

SOM_Scope void  SOMLINK _set_phoneNumber(ConferenceCall *somSelf,
                                          Environment *ev, string phoneNumber)
{
    ConferenceCallData *somThis = ConferenceCallGetData(somSelf);
    ConferenceCallMethodDebug("ConferenceCall","_set_phoneNumber");

    if (somThis->phoneNumber != NULL) {
      SOMFree(somThis->phoneNumber);
      somThis->phoneNumber = NULL;
    }
    if (phoneNumber == NULL) return;
    somThis->phoneNumber = (string)SOMMalloc(strlen(phoneNumber)+1);
    strcpy(somThis->phoneNumber, phoneNumber);
}


SOM_Scope void SOMLINK somDefaultInit(ConferenceCall *somSelf,
                                      som3InitCtrl* ctrl)
{
    ConferenceCallData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    ConferenceCallMethodDebug("ConferenceCall","somDefaultInit");
    ConferenceCall_BeginInitializer_somDefaultInit;

    ConferenceCall_Init_Appointment_somDefaultInit(somSelf, ctrl);

    /*
     * local ConferenceCall initialization code added by programmer
     */
     somThis->phoneNumber = NULL;
}


SOM_Scope void SOMLINK somDestruct(ConferenceCall *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    ConferenceCallData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    ConferenceCallMethodDebug("ConferenceCall","somDestruct");
    ConferenceCall_BeginDestructor;

    /*
     * local ConferenceCall deinitialization code added by programmer
     */
    if (somThis->phoneNumber != NULL) {
      SOMFree(somThis->phoneNumber);
      somThis->phoneNumber = NULL;
    }

    ConferenceCall_EndDestructor;
}


/*
 *   Attribute Modifiers
 */

SOM_Scope string  SOMLINK bufferize(ConferenceCall *somSelf,
                                     Environment *ev)
{
    string pbp;
    string bp;
    ConferenceCallData *somThis = ConferenceCallGetData(somSelf);
    ConferenceCallMethodDebug("ConferenceCall","bufferize");

// Call parent to fill buffer with parent information, ie. date,time etc.
    pbp = ConferenceCall_parent_Appointment_bufferize(somSelf, ev);
    bp = (string)SOMMalloc(BUF_SIZE + strlen(pbp));
    sprintf(bp, "%sphoneNumber: %s\n", pbp,
            (somThis->phoneNumber)?(somThis->phoneNumber):"");
    SOMFree(pbp);
    return(bp);

}

