// 3/8/96 15:41:52
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


#ifndef SOM_Module_fapptbk_Source
#define SOM_Module_fapptbk_Source
#endif
#define AppointmentBookFactory_Class_Source


#include <time.h>                        //time include to generate ids for appointments

#include <somd.xh>                       //dsom include
#include <somnm.xh>                      //som naming include
#include <orb.xh>                        //object request broker include for default context

#include "appt.xh"                       //Appointment class include
#include "mtg.xh"                        //Meeting class include
#include "ccall.xh"                      //ConferenceCall class include
#include "dappt.xh"                      //DistributeAppointment class include
#include "dmtg.xh"                       //DistributedMeeting class include
#include "dccall.xh"                     //DistributedConferenceCall class include
#include "apptbook.xh"                   //AppointmentBook class include
#include "key.xh"                        //Key for appointment collection.

#include "dapptbk.xh"                    //DistributedAppointmentBook class include
#include "fapptbk.xih"

#include "samptype.h"                    //sample constants. For exception generation.

void initClasses();

/*
 * This method creates a new appointment book using the given name.
 * If an appointment book with the given name already exists, an
 * exception is returned.
 */

SOM_Scope AppointmentBook*  SOMLINK createAppointmentBook(AppointmentBookFactory *somSelf,
                                                           Environment *ev,
                                                          string name)
{

    CosNaming_NamingContext      *BookContext;
    DistributedAppointmentBook    *DistApptBook=NULL;
    LName                                  *bookContextname;
    LNameComponent                   *lnc;
    CosNaming_Name                     Name;
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","createAppointmentBook");


    initClasses();             // Initialize the appointment book classes
    SOMD_Init(ev);           // Initialize DSOM
    if (!(somThis->nameContext)) {  // If the nameContext has not been set then error
       // raise the exception
       NoABContext *nocontext = (NoABContext *)SOMMalloc(sizeof(NoABContext));
       nocontext->ErrCode = NOABCONTEXT;
       strcpy(nocontext->Reason, "AppointmentBooks context not initialized");
       somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoABContext",
                          (void *)nocontext);
    } else {
      // Set up the context name for the book
      lnc = create_lname_component();
      lnc->set_id(ev, name);
      lnc->set_kind(ev, "");
      bookContextname = create_lname();
      bookContextname->insert_component(ev, 0, lnc);

      Name = bookContextname->to_idl_form(ev);
      // Check to see if one already registered in name server if so error
      DistApptBook = (DistributedAppointmentBook *)
                      (somThis->nameContext)->resolve(ev, &Name);
      if ( (ev->_major == NO_EXCEPTION)  || // Want name not found error
           (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) != 0) )
      {
        // raise the exception
        BookExists *bookexists = (BookExists *)SOMMalloc(sizeof(BookExists));
        bookexists->ErrCode = BOOKEXISTS;
        strcpy(bookexists->Reason, "A book by this name already exists.");
        somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::BookExists",
                          (void *)bookexists);
        DistApptBook = (DistributedAppointmentBook *)OBJECT_NIL;
      }
      else
      {         // if the book not bound to the name service
         if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) == 0){
           somExceptionFree(ev);
              // This Server can Create a distributed book note: regimpl -l for BookServer classes
              // Create a distributed book
           DistApptBook = (DistributedAppointmentBook *)somSelf->getNewAppointmentBook(ev,name);
           DistApptBook->_set_bookName(ev, name);
           DistApptBook->incrementUsers(ev);    // Increment the number of book users

           // Register the Distributed Appointment Book in the Name server
           (somThis->nameContext)->bind(ev, &Name, DistApptBook);
           // The client is expected to check the environment for errors
         }
      }
    }
    return((AppointmentBook *)DistApptBook);
}

/*
 * This method deletes the given appointment book
 * The book will not be deleted if there is more than one user of the book,
 * checkInAppointmentBook is used by this method to check-in the book
 * If the given appointment book does not exist, an exception is returned.
 */

SOM_Scope void  SOMLINK deleteAppointmentBook(AppointmentBookFactory *somSelf,
                                               Environment *ev,
                                              AppointmentBook* apptbook)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","deleteAppointmentBook");

                                                     // If this is the last user of the book
    if (((DistributedAppointmentBook *)apptbook)->_get_numberOfUsers(ev) == 1) {
         somSelf->deleteBook(ev, apptbook);  // Use Customizable method for deletion

    }
    // Check in the book which frees the in memory object and unbinds object from namespace
    somSelf->checkInAppointmentBook(ev, apptbook);
}

/*
 * This method locates and returns the appointment book with the given name.
 * If the appointment book doesn't exist, this method returns an exception.
 */

SOM_Scope AppointmentBook*  SOMLINK checkOutAppointmentBook(AppointmentBookFactory *somSelf,
                                                             Environment *ev,
                                                            string name)
{
    DistributedAppointmentBook   *DistApptBook=NULL;
    LName                        *bookContextname;
    LNameComponent               *lnc;
    CosNaming_Name                Name;
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","checkOutAppointmentBook");

    if (!(somThis->nameContext)) {  // If the nameContext is not set then error
       // raise the exception
       NoABContext *nocontext = (NoABContext *)SOMMalloc(sizeof(NoABContext));
       nocontext->ErrCode = NOABCONTEXT;
       strcpy(nocontext->Reason, "AppointmentBooks context not initialized");
       somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoABContext",
                          (void *)nocontext);
    } else {
      // Set up the context name for the book
      lnc = create_lname_component();
      lnc->set_id(ev, name);
      lnc->set_kind(ev, "");
      bookContextname = create_lname();
      bookContextname->insert_component(ev, 0, lnc);

      Name = bookContextname->to_idl_form(ev);
      // Check to see if one already registered in name server
      DistApptBook = (DistributedAppointmentBook *)
                      (somThis->nameContext)->resolve(ev, &Name);
      if (ev->_major == NO_EXCEPTION)  // If found increment users
      {
        DistApptBook->incrementUsers(ev);    // Increment the number of book users
      }
      else
      {
         // Clear the exception if the name was not found, and look elsewhere for book
         if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) == 0){
           somExceptionFree(ev);
           DistApptBook = (DistributedAppointmentBook *)somSelf->
                                   getExistingAppointmentBook(ev, name);  // use custom get existing method
         } else
           DistApptBook = (DistributedAppointmentBook *)OBJECT_NIL;

      }
    }
    return((AppointmentBook *)DistApptBook);

}

/*
 * When a client program no longer needs access to an appointment book,
 * the book will be freed from memory if this is the last user of the book
 * this method is invoked.
 */

SOM_Scope void  SOMLINK checkInAppointmentBook(AppointmentBookFactory *somSelf,
                                                Environment *ev,
                                               AppointmentBook* apptbook)
{
    DistributedAppointmentBook   *DistApptBook;
    LName                        *bookContextname;
    LNameComponent               *lnc;
    CosNaming_Name                Name;
    DistributedAppointmentBook   *DistAppointmentBook;
    long                                count=0;
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","checkInAppointmentBook");

    // Check for the root naming context
    if (!(somThis->nameContext)) {
       // raise the exception
       NoABContext *nocontext = (NoABContext *)SOMMalloc(sizeof(NoABContext));
       nocontext->ErrCode = NOABCONTEXT;
       strcpy(nocontext->Reason, "AppointmentBooks context not initialized");
       somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoABContext",
                          (void *)nocontext);
    } else {
                     // Decrement the number of users of this book
      ((DistributedAppointmentBook *)apptbook)->decrementUsers(ev);
                     // Check to see if this was the last user
      count = ((DistributedAppointmentBook *)apptbook)->_get_numberOfUsers(ev);
      if (!count)
      {
                     // set up the name context for the book
        lnc = create_lname_component();
        lnc->set_id(ev,((DistributedAppointmentBook *)apptbook)->_get_bookName(ev));
        lnc->set_kind(ev, "");
        bookContextname = create_lname();
        bookContextname->insert_component(ev, 0, lnc);
        Name = bookContextname->to_idl_form(ev);
                     // unbind the name from the name context
        (somThis->nameContext)->unbind(ev, &Name);
                     // the appointmentbook up to date here so free the object
        ((DistributedAppointmentBook *)apptbook)->somFree();
        apptbook == (AppointmentBook *)OBJECT_NIL;
      }
    }
}



/*
 * Create the AppointmentBooks context off of the root naming context
 */

SOM_Scope void  SOMLINK createBooksContext(AppointmentBookFactory *somSelf,
                                            Environment *ev)
{
    CosNaming_NamingContext      *rootContext=NULL;
    LName                        *bookContextname;
    LNameComponent               *lnc;
    CosNaming_Name                Name;
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","createBooksContext");

    SOMD_Init(ev);
    // Get the root naming context
    rootContext = (CosNaming_NamingContext *)SOMD_ORBObject->
                   resolve_initial_references(ev, "NameService");
    if (ev->_major == NO_EXCEPTION) {
    // Set up the context name for the book
      lnc = create_lname_component();
      lnc->set_id(ev, "AppointmentBooks");
      lnc->set_kind(ev, "");
      bookContextname = create_lname();
      bookContextname->insert_component(ev, 0, lnc);

      Name = bookContextname->to_idl_form(ev);
      somThis->nameContext = rootContext->bind_new_context(ev, &Name);
      if (ev->_major != NO_EXCEPTION) {
        // Check if already bound error, if so reset and resolve name
        if (strcmp(somExceptionId(ev),
                        ex_CosNaming_NamingContext_AlreadyBound) == 0) {
          somExceptionFree(ev);
          somThis->nameContext = (CosNaming_NamingContext *)rootContext->resolve(ev, &Name);
        }
        if (ev->_major != NO_EXCEPTION) {
          // Failed to get the Book Naming Context raise the exception
          NoABContext *nocontext = (NoABContext *)SOMMalloc(sizeof(NoABContext));
          nocontext->ErrCode = NOABCONTEXT;
          strcpy(nocontext->Reason, "AppointmentBooks context not initialized");
          somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoABContext",
                          (void *)nocontext);
          somThis->nameContext = (CosNaming_NamingContext *)OBJECT_NIL;
        }
      }
    }
}

/*
 * This method is used by createAppointmentBook, to customize the NEW operation for the
 * book override this method
 */

SOM_Scope AppointmentBook*  SOMLINK getNewAppointmentBook(AppointmentBookFactory *somSelf,
                                                           Environment *ev,
                                                          string name)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","getNewAppointmentBook");


    return((AppointmentBook *)new DistributedAppointmentBook);
}

/*
 * This method is used by and checkOutAppointmentBook
 *  to customize the get existing book function override this method
 */

SOM_Scope AppointmentBook*  SOMLINK getExistingAppointmentBook(AppointmentBookFactory *somSelf,
                                                                Environment *ev,
                                                               string name)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","getExistingAppointmentBook");

    // raise the exception
    BookNOExist *booknoexist = (BookNOExist *)SOMMalloc(sizeof(BookNOExist));
    booknoexist->ErrCode = BOOKNOEXIST;
    strcpy(booknoexist->Reason, "A book by this name does not exist");
    somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::BookNOExist",
                          (void *)booknoexist);
     return((AppointmentBook *)OBJECT_NIL);

}

/*
 * This method is used by deleteAppointmentBook
 * to customize the delete of an appointment book override this method
 */

SOM_Scope void  SOMLINK deleteBook(AppointmentBookFactory *somSelf,
                                    Environment *ev, AppointmentBook* apptbook)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","deleteBook");

}

/*
 * This method creates a new meeting. The method will generate a unique name.
 * If a meeting with the given name already exists, an
 * exception is returned.
 */

SOM_Scope Meeting*  SOMLINK createMeeting(AppointmentBookFactory *somSelf,
                                           Environment *ev)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","createMeeting");

    /* Return statement to be customized: */
    CosNaming_NamingContext      *apptContext;
    DistributedMeeting           *distMeeting=NULL;
    LName                        *appointmentContextname;
    LNameComponent               *lnc;
    CosNaming_Name               Name;
    char                         idBuffer[33];
    char                         name[13];
    const char            *namePrefix = "M";
    int                            idTime;
    int                            idDay;
    time_t                       t;
    struct                       tm today;

    initClasses();             // Initialize the appointment book classes
    SOMD_Init(ev);           // Initialize DSOM
    time(&t);
    today = *gmtime(&t);
    idDay = today.tm_yday;
    idTime = (today.tm_hour * 3600) + (today.tm_min * 60) + today.tm_sec;
    strcpy(name,namePrefix);
    itoa(idTime,idBuffer,10);
    strcat(name, idBuffer);
    strcat(name,".");
    itoa(idDay,idBuffer,10);
    strcat(name,idBuffer);
        // If the AppointmentNameContext has not been set then error
    if (!(somThis->AppointmentNameContext)) {
       // raise the exception
       NoApptContext *nocontext = (NoApptContext *)SOMMalloc(sizeof(NoApptContext));
       nocontext->ErrCode = NOAPPTCONTEXT;
       strcpy(nocontext->Reason, "Appointments context not initialized");
       somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoApptContext",
                          (void *)nocontext);
    } else {
      // Set up the context name for the appointment
      lnc = create_lname_component();
      lnc->set_id(ev, name);
      lnc->set_kind(ev, "");
      appointmentContextname = create_lname();
      appointmentContextname->insert_component(ev, 0, lnc);

      Name = appointmentContextname->to_idl_form(ev);
      // Check to see if one already registered in name server if so error
      distMeeting = (DistributedMeeting *)(void *)(somThis->AppointmentNameContext)->resolve(ev, &Name);
      if ( (ev->_major == NO_EXCEPTION)  || // Want name not found error
           (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) != 0) )
      {
        // raise the exception
        AppointmentExists *apptExists = (AppointmentExists *)SOMMalloc(sizeof(AppointmentExists));
        apptExists->ErrCode = APPOINTMENTEXISTS;
        strcpy(apptExists->Reason, "A book by this name already exists.");
        somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::AppointmentExists",
                          (void *)apptExists);
        distMeeting = (DistributedMeeting *)(void *)OBJECT_NIL;
      }
      else
      {         // if the book not bound to the name service
         if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) == 0){
           somExceptionFree(ev);
              // This Server can Create a distributed book note: regimpl -l for BookServer classes
              // Create a distributed book
           distMeeting = (DistributedMeeting*)somSelf->getNewMeeting(ev,name);
           distMeeting->_set_appointmentName(ev, name);
           distMeeting->incrementUsers(ev);    // Increment the number of book users
                                               // Set the Appointment ID for the book
           distMeeting->_set_apptId(ev, (idTime*1000+idDay));
           // Register the Distributed Appointment Book in the Name server
           (somThis->AppointmentNameContext)->bind(ev, &Name,(SOMObject *)(void *) distMeeting);
           // The client is expected to check the environment for errors
         }
      }
    }
    return((Meeting *)distMeeting);
}

/*
 * This method deletes the given meeting
 * The meeting will not be deleted if there is more than one user of the meeting,
 * checkInAppointment is used by this method to check-in the meeting
 * If the given meeting does not exist, an exception is returned.
 */

SOM_Scope void  SOMLINK deleteMeeting(AppointmentBookFactory *somSelf,
                                       Environment *ev, Meeting* meet)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","deleteMeeting");

    if (((DistributedMeeting *)meet)->_get_numberOfUsers(ev) == 1) {
         somSelf->deleteAppointment(ev, meet);  // Use Customizable method for deletion
    }
    else {
      // Check in the book which frees the in memory object and unbinds object from namespace
      somSelf->checkInAppointment(ev, meet);
    }
}

/*
 * This method creates a new meeting using the given name.
 * If a meeting with the given name already exists, an
 * exception is returned.
 */


/*
 * SOM_Scope Meeting*  SOMLINK createConferenceCall(AppointmentBookFactory *somSelf,
 *                                                   Environment *ev)
 */

/*
 * The prototype for createConferenceCall was replaced by the following prototype:
 */
SOM_Scope ConferenceCall*  SOMLINK createConferenceCall(AppointmentBookFactory *somSelf,
                                                         Environment *ev)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","createConferenceCall");

    CosNaming_NamingContext      *apptContext;
    DistributedConferenceCall    *distCCall=NULL;
    LName                        *appointmentContextname;
    LNameComponent               *lnc;
    CosNaming_Name               Name;
    char                         idBuffer[33];
    char                         name[13];
    const char            *namePrefix = "C";
    int                            idTime;
    int                            idDay;
    time_t                       t;
    struct                       tm today;

    initClasses();             // Initialize the appointment book classes
    SOMD_Init(ev);           // Initialize DSOM
    time(&t);
    today = *gmtime(&t);
    idDay = today.tm_yday;
    idTime = (today.tm_hour * 3600) + (today.tm_min * 60) + today.tm_sec;
    strcpy(name,namePrefix);
    itoa(idTime,idBuffer,10);
    strcat(name, idBuffer);
    strcat(name,".");
    itoa(idDay,idBuffer,10);
    strcat(name,idBuffer);
 // If the AppointmentNameContext has not been set then error
    if (!(somThis->AppointmentNameContext)) {
       // raise the exception
       NoApptContext *nocontext = (NoApptContext *)SOMMalloc(sizeof(NoApptContext));
       nocontext->ErrCode = NOAPPTCONTEXT;
       strcpy(nocontext->Reason, "Appointments context not initialized");
       somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoApptContext",
                          (void *)nocontext);
    } else {
      // Set up the context name for the book
      lnc = create_lname_component();
      lnc->set_id(ev, name);
      lnc->set_kind(ev, "");
      appointmentContextname = create_lname();
      appointmentContextname->insert_component(ev, 0, lnc);

      Name = appointmentContextname->to_idl_form(ev);
      // Check to see if one already registered in name server if so error
      distCCall = (DistributedConferenceCall *)(void *)(somThis->AppointmentNameContext)->resolve(ev, &Name);
      if ( (ev->_major == NO_EXCEPTION)  || // Want name not found error
           (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) != 0) )
      {
        // raise the exception
        AppointmentExists *apptExists = (AppointmentExists *)SOMMalloc(sizeof(AppointmentExists));
        apptExists->ErrCode = APPOINTMENTEXISTS;
        strcpy(apptExists->Reason, "A book by this name already exists.");
        somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::AppointmentExists",
                          (void *)apptExists);
        distCCall = (DistributedConferenceCall *)(void *)OBJECT_NIL;
      }
      else
      {         // if the book not bound to the name service
         if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) == 0){
           somExceptionFree(ev);
              // This Server can Create a distributed appointment note: regimpl -l for BookServer classes
              // Create a distributed appointment
           distCCall = (DistributedConferenceCall*)somSelf->getNewConferenceCall(ev,name);
           distCCall->_set_appointmentName(ev, name);
           distCCall->incrementUsers(ev);    // Increment the number of book users
                                               // Set the Appointment ID for the book
           distCCall->_set_apptId(ev, (idTime*1000 + idDay));
           // Register the Distributed Appointment Book in the Name server
           (somThis->AppointmentNameContext)->bind(ev, &Name,(SOMObject *)(void *) distCCall);
           // The client is expected to check the environment for errors
         }
      }
    }
    return((ConferenceCall *)distCCall);
}

/*
 * This method deletes the given meeting
 * The meeting will not be deleted if there is more than one user of the meeting,
 * checkInAppointment is used by this method to check-in the meeting
 * If the given meeting does not exist, an exception is returned.
 */

SOM_Scope void  SOMLINK deleteConferenceCall(AppointmentBookFactory *somSelf,
                                              Environment *ev,
                                             ConferenceCall* ccall)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","deleteConferenceCall");

    if (((DistributedConferenceCall *)ccall)->_get_numberOfUsers(ev) == 1) {
         somSelf->deleteAppointment(ev, ccall);  // Use Customizable method for deletion

    }
    else {
      // Check in the book which frees the in memory object and unbinds object from namespace
      somSelf->checkInAppointment(ev, ccall);
    }
}

/*
 * This method locates and returns the appointment with the given name.
 * If the appointment book doesn't exist, this method returns an exception.
 */

SOM_Scope Appointment*  SOMLINK checkOutAppointment(AppointmentBookFactory *somSelf,
                                                     Environment *ev,
                                                     string name)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","checkOutAppointment");

    /* Return statement to be customized: */
    { Appointment* retVal;  return (retVal); }
}

/*
 * When a client program no longer needs access to an appointment,
 * the book will be freed from memory if this is the last user of the book
 * this method is invoked.
 */

SOM_Scope void  SOMLINK checkInAppointment(AppointmentBookFactory *somSelf,
                                            Environment *ev,
                                           Appointment* appt)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","checkInAppointment");

    LName                        *apptContextname;
    LNameComponent               *lnc;
    CosNaming_Name               Name;
    long                                count=0;

    // Check for the root naming context
    if (!(somThis->AppointmentNameContext)) {
       // raise the exception
       NoApptContext *nocontext = (NoApptContext *)SOMMalloc(sizeof(NoApptContext));
       nocontext->ErrCode = NOAPPTCONTEXT;
       strcpy(nocontext->Reason, "Appointments context not initialized");
       somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoApptContext",
                          (void *)nocontext);
    } else {
                     // Decrement the number of users of this book
      ((DistributeAppointment *)appt)->decrementUsers(ev);
                     // Check to see if this was the last user
      count = ((DistributeAppointment *)appt)->_get_numberOfUsers(ev);
      if (!count)
      {
                     // set up the name context for the book
        lnc = create_lname_component();
        lnc->set_id(ev,((DistributeAppointment *)appt)->_get_appointmentName(ev));
        lnc->set_kind(ev, "");
        apptContextname = create_lname();
        apptContextname->insert_component(ev, 0, lnc);
        Name = apptContextname->to_idl_form(ev);
                     // unbind the name from the name context
        (somThis->AppointmentNameContext)->unbind(ev, &Name);
                     // the appointmentbook up to date here so free the object
        ((DistributeAppointment *)appt)->somFree();
        appt == (Appointment *)OBJECT_NIL;
      }
    }
}

/*
 * Create the Appointment context off of the root naming context
 */

SOM_Scope void  SOMLINK createAppointmentContext(AppointmentBookFactory *somSelf,
                                                  Environment *ev)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","createAppointmentContext");
    CosNaming_NamingContext      *rootContext=NULL;
    LName                        *appointmentContextname;
    LNameComponent               *lnc;
    CosNaming_Name                Name;
    SOMD_Init(ev);
    // Get the root naming context
    rootContext = (CosNaming_NamingContext *)SOMD_ORBObject->
                   resolve_initial_references(ev, "NameService");
    if (ev->_major == NO_EXCEPTION) {
    // Set up the context name for the book
      lnc = create_lname_component();
      lnc->set_id(ev, "Appointments");
      lnc->set_kind(ev, "");
      appointmentContextname = create_lname();
      appointmentContextname->insert_component(ev, 0, lnc);

      Name = appointmentContextname->to_idl_form(ev);
      somThis->AppointmentNameContext = rootContext->bind_new_context(ev, &Name);
      if (ev->_major != NO_EXCEPTION) {
        // Check if already bound error, if so reset and resolve name
        if (strcmp(somExceptionId(ev),
                        ex_CosNaming_NamingContext_AlreadyBound) == 0) {
          somExceptionFree(ev);
          somThis->AppointmentNameContext = (CosNaming_NamingContext *)rootContext->resolve(ev, &Name);
        }
        if (ev->_major != NO_EXCEPTION) {
          // Failed to get the Book Naming Context raise the exception
          NoApptContext *nocontext = (NoApptContext *)SOMMalloc(sizeof(NoApptContext));
          nocontext->ErrCode = NOAPPTCONTEXT;
          strcpy(nocontext->Reason, "Appointments context not initialized");
          somSetException(ev, USER_EXCEPTION, "AppointmentBookFactory::NoApptContext",
                          (void *)nocontext);
          somThis->AppointmentNameContext = (CosNaming_NamingContext *)OBJECT_NIL;
        }
      }
    }

}


/*
 * SOM_Scope Meeting*  SOMLINK getNewMeeting(AppointmentBookFactory *somSelf,
 *                                            Environment *ev)
 */

SOM_Scope Meeting*  SOMLINK getNewMeeting(AppointmentBookFactory *somSelf,
                                           Environment *ev, string name)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","getNewMeeting");

    /* Return statement to be customized: */
    return (Meeting*)new DistributedMeeting;
}

/*
 * SOM_Scope ConferenceCall*  SOMLINK getNewConferenceCall(AppointmentBookFactory *somSelf,
 *                                                          Environment *ev,
 *                                                         string name)
 */


SOM_Scope ConferenceCall*  SOMLINK getNewConferenceCall(AppointmentBookFactory *somSelf,
                                                         Environment *ev,
                                                        string name)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","getNewConferenceCall");

    /* Return statement to be customized: */
    return (ConferenceCall*)new DistributedConferenceCall;
}

/*
 * This method is used by and checkOutAppointmentBook
 *  to customize the get existing book function override this method
 */

SOM_Scope Appointment*  SOMLINK getExistingAppointment(AppointmentBookFactory *somSelf,
                                                        Environment *ev,
                                                       string name)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","getExistingAppointment");

    /* Return statement to be customized: */
    { Appointment* retVal;  return (retVal); }
}

/*
 * This method is used by deleteAppointmentBook
 * to customize the delete of an appointment book override this method
 */

SOM_Scope void  SOMLINK deleteAppointment(AppointmentBookFactory *somSelf,
                                           Environment *ev, Appointment* appt)
{
    AppointmentBookFactoryData *somThis = AppointmentBookFactoryGetData(somSelf);
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","deleteAppointment");

    somSelf->checkInAppointment(ev, appt);
}

SOM_Scope void SOMLINK somDefaultInit(AppointmentBookFactory *somSelf,
                                      som3InitCtrl* ctrl)
{
    AppointmentBookFactoryData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","somDefaultInit");
    AppointmentBookFactory_BeginInitializer_somDefaultInit;

    AppointmentBookFactory_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /*
     * local AppointmentBookFactory initialization code added by programmer
     */
    somThis->nameContext = (CosNaming_NamingContext *)OBJECT_NIL;
    somThis->AppointmentNameContext = (CosNaming_NamingContext *)OBJECT_NIL;
}


SOM_Scope void SOMLINK somDestruct(AppointmentBookFactory *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    Environment *ev = somGetGlobalEnvironment();
    AppointmentBookFactoryData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentBookFactoryMethodDebug("AppointmentBookFactory","somDestruct");
    AppointmentBookFactory_BeginDestructor;

    /*
     * local AppointmentBookFactory deinitialization code added by programmer
     */
    (somThis->nameContext)->destroy(ev);
    somThis->nameContext=(CosNaming_NamingContext *)OBJECT_NIL;
    (somThis->AppointmentNameContext)->destroy(ev);
    somThis->AppointmentNameContext=(CosNaming_NamingContext *)OBJECT_NIL;

    AppointmentBookFactory_EndDestructor;
}


void initClasses()
{
AppointmentNewClass(0,0);
AppointmentBookNewClass(0,0);
ConferenceCallNewClass(0,0);
Appt_KeyNewClass(0,0);
MeetingNewClass(0,0);
DistributedAppointmentBookNewClass(0,0);
AppointmentBookFactoryNewClass(0,0);
DistributedMeetingNewClass(0,0);
DistributedConferenceCallNewClass(0,0);
DistributeAppointmentNewClass(0,0);
}
