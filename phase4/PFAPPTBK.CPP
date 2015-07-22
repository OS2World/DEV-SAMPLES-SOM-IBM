// 3/8/96 15:52:32
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

#ifndef SOM_Module_pfapptbk_Source
#define SOM_Module_pfapptbk_Source
#endif
#define PersistentAppointmentBookFactory_Class_Source

#include "pappt.xh"           // PersistentAppointment class
#include "pmtg.xh"            // PersistentMeeting class
#include "pccall.xh"          // PersistentConferenceCall class
#include "key.xh"             // Appt_key class
#include "pdapptbk.xh"        // PersistentAppointmentBook class
#include <sompposx.xh>        // somPersistencePOSIX class
#include "pfapptbk.xih"
void initClass();


SOM_Scope void SOMLINK somDefaultInit(PersistentAppointmentBookFactory *somSelf,
                                      som3InitCtrl* ctrl)
{
    PersistentAppointmentBookFactoryData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","somDefaultInit");
    PersistentAppointmentBookFactory_BeginInitializer_somDefaultInit;

    PersistentAppointmentBookFactory_Init_AppointmentBookFactory_somDefaultInit(somSelf, ctrl);

    /*
     * local PersistentAppointmentBookFactory initialization code added by programmer
     */
}


/*
 * Factory customization methods
 */
SOM_Scope void SOMLINK somDestruct(PersistentAppointmentBookFactory *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    PersistentAppointmentBookFactoryData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","somDestruct");
    PersistentAppointmentBookFactory_BeginDestructor;

    /*
     * local PersistentAppointmentBookFactory deinitialization code added by programmer
     */

    PersistentAppointmentBookFactory_EndDestructor;
}


/*
 * create a new IPO Meeting
 */

SOM_Scope Meeting*  SOMLINK getNewMeeting(PersistentAppointmentBookFactory *somSelf,
                                           Environment *ev, string name)
{
    PersistentMeeting                               *PstMeeting=NULL;
    somPersistencePOSIX_PID_POSIX           *pidMeeting;  // Persistent ID for POSIX datastore

    /* PersistentAppointmentBookFactoryData *somThis = PersistentAppointmentBookFactoryGetData(somSelf); */
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","getNewMeeting");

    initClass();

    // Prepare pid. It will be freed by IPO when the object is somFreed
    pidMeeting = new somPersistencePOSIX_PID_POSIX;
    pidMeeting->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    PstMeeting = (PersistentMeeting *)(((somPersistence_IPOFactory *)
                       _PersistentMeeting)->createIPO(ev,pidMeeting));

    // Exception checking is omitted here for the clarity purpose

    return((Meeting *)PstMeeting);
 }


/*
 * create a new IPO ConferenceCall
 */

SOM_Scope ConferenceCall*  SOMLINK getNewConferenceCall(PersistentAppointmentBookFactory *somSelf,
                                                         Environment *ev, string name)
{
    PersistentConferenceCall                    *PstConfCall = NULL;
    somPersistencePOSIX_PID_POSIX           *pidCall;  // Persistent ID for POSIX datastore

    /* PersistentAppointmentBookFactoryData *somThis = PersistentAppointmentBookFactoryGetData(somSelf); */
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","getNewConferenceCall");

    initClass();

    // Prepare pid. It will be freed by IPO when the object is somFreed
    pidCall = new somPersistencePOSIX_PID_POSIX;
    pidCall->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    PstConfCall = (PersistentConferenceCall *)(((somPersistence_IPOFactory *)
                  _PersistentConferenceCall)->createIPO(ev, pidCall));

    // Exception checking is omitted here for the clarity purpose

    return((ConferenceCall *)PstConfCall);
}

/*
 * restore an existing Appointment
 */

SOM_Scope Appointment*  SOMLINK getExistingAppointment(PersistentAppointmentBookFactory *somSelf,
                                                        Environment *ev,
                                                       string name)
{
    PersistentAppointment                        *PstAppt = NULL;
    somPersistencePOSIX_PID_POSIX           *pidAppt;  // Persistent ID for POSIX datastore

    /* PersistentAppointmentBookFactoryData *somThis = PersistentAppointmentBookFactoryGetData(somSelf); */
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","getExistingAppointment");

    initClass();

    // Prepare pid. It will be freed by IPO when the object is somFreed
    pidAppt = new somPersistencePOSIX_PID_POSIX;
    pidAppt->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    PstAppt = (PersistentAppointment *)(((somPersistence_IPOFactory *)
                  _PersistentAppointment)->createIPO(ev, pidAppt));

    return((Appointment *)PstAppt);

}


/*
 * creates a new IPO PersistentAppointmentBook
 */

SOM_Scope AppointmentBook*  SOMLINK getNewAppointmentBook(PersistentAppointmentBookFactory *somSelf,
                                                           Environment *ev,
                                                          string name)
{
    PersistentDistributedAppointmentBook      *PstDistApptBook=NULL;
    somPersistencePOSIX_PID_POSIX              *pidBook;  // Persistent ID for POSIX datastore
    CosTransactions_Current                        *Trans;

    /* PersistentAppointmentBookFactoryData *somThis = PersistentAppointmentBookFactoryGetData(somSelf); */
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","getNewAppointmentBook");

    initClass();             // Initialize the appointment book classes
    Trans = new CosTransactions_Current;

    // Prepare pid. It will be freed by IPO when the object is somFreed
    pidBook = new somPersistencePOSIX_PID_POSIX;
    pidBook->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    PstDistApptBook = (PersistentDistributedAppointmentBook *)(((somPersistence_IPOFactory *)
                               _PersistentDistributedAppointmentBook)->createIPO(ev,pidBook));

    // Exception checking is omitted here for the clarity purpose

    // Start transaction
    Trans->begin(ev);

    PstDistApptBook->_set_numberOfAppts(ev, 0);
    PstDistApptBook->_set_bookName(ev,name);

    // End transaction
    Trans->commit(ev,FALSE);
    Trans->somFree();

    return((AppointmentBook *)PstDistApptBook);
 }



/*
 * restore an existing PersistentAppointmentBook
 */

SOM_Scope AppointmentBook*  SOMLINK getExistingAppointmentBook(PersistentAppointmentBookFactory *somSelf,
                                                                Environment *ev,
                                                               string name)
{
    PersistentDistributedAppointmentBook      *PstDistApptBook=NULL;
    somPersistencePOSIX_PID_POSIX              *pidBook;  // Persistent ID for POSIX datastore

    /* PersistentAppointmentBookFactoryData *somThis = PersistentAppointmentBookFactoryGetData(somSelf); */
    PersistentAppointmentBookFactoryMethodDebug("PersistentAppointmentBookFactory","getExistingAppointmentBook");

    initClass();             // Initialize the appointment book classes

    // Prepare pid. It will be freed by IPO when the object is somFreed
    pidBook = new somPersistencePOSIX_PID_POSIX;
    pidBook->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    PstDistApptBook = (PersistentDistributedAppointmentBook *)(((somPersistence_IPOFactory *)
                               _PersistentDistributedAppointmentBook)->createIPO(ev,pidBook));

    // Exception checking is omitted here for the clarity purpose

    return((AppointmentBook *)PstDistApptBook);

}

void initClass()
{
     PersistentAppointmentNewClass(0,0);
     PersistentConferenceCallNewClass(0,0);
     PersistentMeetingNewClass(0,0);
     PersistentDistributedAppointmentBookNewClass(0,0);
     PersistentAppointmentBookFactoryNewClass(0,0);
}

