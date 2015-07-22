// 3/8/96 09:37:11
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


#ifndef SOM_Module_mtg_Source
#define SOM_Module_mtg_Source
#endif
#define Meeting_Class_Source

#include "mtg.xih"

#include "samptype.h" // Application include file
#include <stdio.h>


/*
 * The location of the appointment.
 */

SOM_Scope void  SOMLINK _set_location(Meeting *somSelf,  Environment *ev,
                                      string location)
{
    MeetingData *somThis = MeetingGetData(somSelf);
    MeetingMethodDebug("Meeting","_set_location");

    if (somThis->location != NULL) {
      SOMFree(somThis->location);
      somThis->location = NULL;
    }
    if (location == NULL) return;
    somThis->location = (string)SOMMalloc(strlen(location)+1);
    strcpy(somThis->location, location);

}


SOM_Scope void SOMLINK somDefaultInit(Meeting *somSelf, som3InitCtrl* ctrl)
{
    MeetingData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    MeetingMethodDebug("Meeting","somDefaultInit");
    Meeting_BeginInitializer_somDefaultInit;

    Meeting_Init_Appointment_somDefaultInit(somSelf, ctrl);

    /*
     * local Meeting initialization code added by programmer
     */
    somThis->location = NULL;
}


SOM_Scope void SOMLINK somDestruct(Meeting *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    MeetingData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    MeetingMethodDebug("Meeting","somDestruct");
    Meeting_BeginDestructor;

    /*
     * local Meeting deinitialization code added by programmer
     */

    if (somThis->location != NULL) {
       SOMFree(somThis->location);
       somThis->location = NULL;
    }

    Meeting_EndDestructor;
}


/*
 *   Attribute Modifiers
 */

SOM_Scope string  SOMLINK bufferize(Meeting *somSelf,  Environment *ev)
{
    string pbp;
    string bp;
    MeetingData *somThis = MeetingGetData(somSelf);
    MeetingMethodDebug("Meeting","bufferize");

// Call parent to fill buffer with parent information, ie. date,time etc.
    pbp = Meeting_parent_Appointment_bufferize(somSelf, ev);
    bp = (string)SOMMalloc(BUF_SIZE + strlen(pbp));
    sprintf(bp, "%sLocation: %s\n", pbp,
            (somThis->location)?(somThis->location):"");
    SOMFree(pbp);
    return(bp);

}

