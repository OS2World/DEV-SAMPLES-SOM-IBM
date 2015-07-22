//  3/8/96 16:13:17
/*
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
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


#ifndef SOM_Module_efapptbk_Source
#define SOM_Module_efapptbk_Source
#endif
#define EventPersDistAppointmentBookFactory_Class_Source



#include <pmtg.xh>
#include <pccall.xh>
#include <edapptbk.xh>
#include <fapptbk.xh>
#include <pfapptbk.xh>
#include <sompposx.xh>    // POSIX Persistent Store
#include "efapptbk.xih"
#include "samptype.h"
void initEClass();



/*
 *prepares pid and gets a new persistent object
 */

SOM_Scope AppointmentBook*  SOMLINK getNewAppointmentBook(EventPersDistAppointmentBookFactory *somSelf,
                                                          Environment *ev,
                                                          string name)
{
    /* EventPersDistAppointmentBookFactoryData *somThis = EventPersDistAppointmentBookFactoryGetData(somSelf); */
    EventPersDistAppointmentBookFactoryMethodDebug("EventPersDistAppointmentBookFactory","getNewAppointmentBook");

    EventPersDistAppointmentBook             *EPstDistApptBook=NULL;
    somPersistencePOSIX_PID_POSIX            *pidBook;  // Persistent ID for POSIX datastore
    CosTransactions_Current                  *Trans;


    initEClass();             // Initialize the appointment book classes

    Trans = new CosTransactions_Current;

    // Prepare pid
    pidBook = new somPersistencePOSIX_PID_POSIX;
    pidBook->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    EPstDistApptBook = (EventPersDistAppointmentBook *)(((somPersistence_IPOFactory *)
                               _EventPersDistAppointmentBook)->createIPO(ev,pidBook));
    // Should check here to see if the book is created. If not, follow previous example
    // to raise an exception.

    // Connect Appointment book to event channel
    EPstDistApptBook->eventChannelConnect(ev);
    checkEnv(ev);

    // Start transaction
    Trans->begin(ev);

    EPstDistApptBook->_set_numberOfAppts(ev, 0);
    EPstDistApptBook->_set_bookName(ev, name);

    // End transaction
    Trans->commit(ev,FALSE);


    return((AppointmentBook *)EPstDistApptBook);


}

/*
 * locates and restores a persistent object
 * if not found an exception is returned
 */

SOM_Scope AppointmentBook*  SOMLINK getExistingAppointmentBook(EventPersDistAppointmentBookFactory *somSelf,
                                                                Environment *ev,
                                                               string name)
{
    /* EventPersDistAppointmentBookFactoryData *somThis = EventPersDistAppointmentBookFactoryGetData(somSelf); */
    EventPersDistAppointmentBookFactoryMethodDebug("EventPersDistAppointmentBookFactory","getExistingAppointmentBook");


    EventPersDistAppointmentBook               *EPstDistApptBook=NULL;
    somPersistencePOSIX_PID_POSIX              *pidBook;  // Persistent ID for POSIX datastore
    CosTransactions_Current                    *Trans;



    initEClass();             // Initialize the appointment book classes
    Trans = new CosTransactions_Current;

    // Prepare pid
    pidBook = new somPersistencePOSIX_PID_POSIX;
    pidBook->_set_pathName(ev,name);

    // Create a PersistentDistributedAppointmentBook
    EPstDistApptBook = (EventPersDistAppointmentBook *)(((somPersistence_IPOFactory *)
                               _EventPersDistAppointmentBook)->createIPO(ev,pidBook));
    // Should check here to see if the book is created. If not, follow previous example
    // to raise an exception.

    // Connect Appointment book to event channel
    EPstDistApptBook->eventChannelConnect(ev);
    checkEnv(ev);


     // Start transaction
    Trans->begin(ev);

    EPstDistApptBook->_set_bookName(ev, name);

    // End transaction
    Trans->commit(ev,FALSE);

    return((AppointmentBook *)EPstDistApptBook);



}



void initEClass()
{
     PersistentAppointmentNewClass(0,0);
     PersistentConferenceCallNewClass(0,0);
     PersistentMeetingNewClass(0,0);
     EventPersDistAppointmentBookNewClass(0,0);
     EventPersDistAppointmentBookFactoryNewClass(0,0);
}




