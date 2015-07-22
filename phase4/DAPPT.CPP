// 3/8/96 15:42:23
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

#ifndef SOM_Module_dappt_Source
#define SOM_Module_dappt_Source
#endif
#define DistributeAppointment_Class_Source

#include "dappt.xih"


/*
 * This attribute maintains the name of the appointment book.
 */

SOM_Scope void  SOMLINK _set_appointmentName(DistributeAppointment *somSelf,
                                              Environment *ev,
                                             string appointmentName)
{
    DistributeAppointmentData *somThis = DistributeAppointmentGetData(somSelf);
    DistributeAppointmentMethodDebug("DistributeAppointment","_set_appointmentName");

    if (appointmentName == NULL) return;   // Passed in string empty
    somThis->appointmentName = (string)SOMMalloc(strlen(appointmentName)+1);
    strcpy(somThis->appointmentName, appointmentName);
}

SOM_Scope void  SOMLINK incrementUsers(DistributeAppointment *somSelf,
                                        Environment *ev)
{
    DistributeAppointmentData *somThis = DistributeAppointmentGetData(somSelf);
    DistributeAppointmentMethodDebug("DistributeAppointment","incrementUsers");

    somThis->numberOfUsers++;
}

SOM_Scope void  SOMLINK decrementUsers(DistributeAppointment *somSelf,
                                        Environment *ev)
{
    DistributeAppointmentData *somThis = DistributeAppointmentGetData(somSelf);
    DistributeAppointmentMethodDebug("DistributeAppointment","decrementUsers");

    somThis->numberOfUsers--;
}

SOM_Scope void SOMLINK somDefaultInit(DistributeAppointment *somSelf,
                                      som3InitCtrl* ctrl)
{
    DistributeAppointmentData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    DistributeAppointmentMethodDebug("DistributeAppointment","somDefaultInit");
    DistributeAppointment_BeginInitializer_somDefaultInit;

    DistributeAppointment_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /*
     * local DistributeAppointment initialization code added by programmer
     */
     somThis->numberOfUsers = 0;
}



/*
 * Attribute modifiers
 */
SOM_Scope void SOMLINK somDestruct(DistributeAppointment *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    DistributeAppointmentData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    DistributeAppointmentMethodDebug("DistributeAppointment","somDestruct");
    DistributeAppointment_BeginDestructor;

    /*
     * local DistributeAppointment deinitialization code added by programmer
     */

    if (somThis->appointmentName != NULL) {
      SOMFree(somThis->appointmentName);
      somThis->appointmentName = NULL;
    }

    DistributeAppointment_EndDestructor;
}


