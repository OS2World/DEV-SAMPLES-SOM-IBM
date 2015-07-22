// 3/8/96 16:13:06
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



#ifndef SOM_Module_events_Source
#define SOM_Module_events_Source
#endif
#define ApptBookEventService_Class_Source
#include <somnm.xh>      // Naming Service
#include "apevents.xih"
#include "eventh.xh"

#include <es_impl.xh>   // Event Service

#include <orb.xh>       // Object Request Broker
#include "samptype.h"
#include <somd.xh>      // DSOM



/*
 * This method is called to initialize the objects needed to transport events from the
 * appointment book to the client. If it does not exist, an event channel is created
 * and a reference to it is stored in the name service.
 */

SOM_Scope void  SOMLINK initEventService(ApptBookEventService *somSelf,
                                          Environment *ev)
{

  ApptBookEventServiceMethodDebug("ApptBookEventService","initEventService");
  ApptBookEventServiceData *somThis = ApptBookEventServiceGetData(somSelf);


  CosNaming_NamingContext      *rootContext;
  LName                        *eventContextName;
  LNameComponent               *lnc;                     // Library Name Component
  CosNaming_Name               Name;
  CosNaming_NamingContext      *nameContext=NULL;
  SomEventService_EventChannel *eventChannel = NULL;
  char                         *exId;
  AppointmentEventHandler *myAppointmentEventHandler;


  // Check to see if the event channel exists. If not, create it and
  //Register it in the name service
  eventChannel = (SomEventService_EventChannel *) somSelf->getEventChannelRef(ev);
  if(ev->_major != NO_EXCEPTION)
  {
   somExceptionFree(ev);
   rootContext = (CosNaming_NamingContext *)SOMD_ORBObject->
                   resolve_initial_references(ev, "NameService");

   if (ev->_major != NO_EXCEPTION) {
      somPrintf("Failed to get the root Naming Context");
   } else { // Root Context OK

     // Set up the context name for the Event Service
     lnc = create_lname_component();
     lnc->set_id(ev, "EventService");
     lnc->set_kind(ev, "");
     eventContextName = create_lname();
     eventContextName->insert_component(ev, 0, lnc);
     Name = eventContextName->to_idl_form(ev);
     nameContext = rootContext->bind_new_context(ev, &Name);
     exId = somExceptionId(ev);
     if (exId != NULL &&
         strcmp(exId,ex_CosNaming_NamingContext_AlreadyBound) == 0) {
       somExceptionFree(ev);
       nameContext = (CosNaming_NamingContext *) rootContext->resolve(ev, &Name);
    }
     if (ev->_major != NO_EXCEPTION) {
        somPrintf("Failed to get the Event Service Naming Context");
     } else {  // Event Service Name Context OK

       // Set up the context name for the Event Channel
       lnc = create_lname_component();
       lnc->set_id(ev, "EventChannel");
       lnc->set_kind(ev, "");
       eventContextName = create_lname();
       eventContextName->insert_component(ev, 0, lnc);
       Name = eventContextName->to_idl_form(ev);
       if (ev->_major != NO_EXCEPTION)
       { // Error resolving Event Channel

         somPrintf("Failed to get the existing Event Channel.\n");
       }
       else
       { // Event channel does not exist - Create it

         // Create an Event Channel and bind a name to it
         somPrintf("Creating Event channel\n");
         eventChannel =
           (SomEventService_EventChannel *) (void *)
              somdCreate(ev, "SomEventService::EventChannel", TRUE);
         checkEnv(ev);

         //register the event channel with the name service
         nameContext->unbind(ev, &Name);
         somExceptionFree(ev);

         nameContext->bind(ev, &Name, (SOMObject *)(void *) eventChannel);

         if (ev->_major != NO_EXCEPTION)
         { // Unable to bind the name to an Event Channel
           somPrintf("Unable to bind Event Channel to name.\n");
         }
       } // End - Event channel Name does not exist - Create it
     } // End - Event Service Name Context OK
   } // End - Root context OK

 }
  // Create the event handler object and connect to event channel
  myAppointmentEventHandler  = new AppointmentEventHandler;
  myAppointmentEventHandler->eventChannelConnect(ev);

  //Save the name of the event channel
  somThis->ECNameContext = nameContext;
  somThis->ECName = Name;

  return;

}



/*
 *Returns reference to event channel
 * This method is called to get the reference to the event channel. It is called by both
 * the event handler in the client and the appointment book in the BookServer.
 */

SOM_Scope unsigned long  SOMLINK getEventChannelRef(ApptBookEventService *somSelf,
                                                     Environment *ev)
{

    /* ApptBookEventServiceData *somThis = ApptBookEventServiceGetData(somSelf); */
    ApptBookEventServiceMethodDebug("ApptBookEventService","getEventChannelRef");

    SomEventService_EventChannel *eventChannel = NULL;
    char                         *ECStringRef;   //Stringified name of event channel

    CosNaming_NamingContext      *rootContext;
    LName                        *eventContextName;
    LNameComponent               *lnc;                     // Library Name Component
    CosNaming_Name               Name;
    CosNaming_NamingContext      *nameContext = NULL;
    long                         error = 0;
    EventError                   *eventError;


    rootContext = (CosNaming_NamingContext *)SOMD_ORBObject->
                   resolve_initial_references(ev, "NameService");

    if (!(checkEnv(ev) || !rootContext))
    {
      // Set up the context name for the Event Service
      lnc = create_lname_component();
      lnc->set_id(ev, "EventService");
      lnc->set_kind(ev, "");
      eventContextName = create_lname();
      eventContextName->insert_component(ev, 0, lnc);
      Name = eventContextName->to_idl_form(ev);
      nameContext = (CosNaming_NamingContext *) rootContext->resolve(ev, &Name);
      if (ev->_major == NO_EXCEPTION)
      {


        // Set up the context name for the Event Channel
        lnc = create_lname_component();
        lnc->set_id(ev, "EventChannel");
        lnc->set_kind(ev, "");
        eventContextName = create_lname();
        eventContextName->insert_component(ev, 0, lnc);
        Name = eventContextName->to_idl_form(ev);

        // check to see if the event channel already registered in name server
        eventChannel =
            (SomEventService_EventChannel*)(void *) nameContext->resolve(ev, &Name);
        if(ev->_major == NO_EXCEPTION)
        {
           //Test for a valid reference
           CosEventChannelAdmin_ConsumerAdmin *consumerAdmin =
             eventChannel->for_consumers(ev);

           if(ev->_major != NO_EXCEPTION)
           {
              // Failed to get Event channel reference
              somExceptionFree(ev);
              error = ApptBookEventService_EventChannelReferenceFailure;
           }

        }
        else
        {
            // Failed to get Event channel reference
            somExceptionFree(ev);
            error = ApptBookEventService_EventChannelReferenceFailure;
        }

     }
     else
     {
       //Failed to get the Event Service Naming Context
       somExceptionFree(ev);
       error = ApptBookEventService_EventContextFailure;
     }
    }
    else
    {
      //Failed to get the root Naming Context
      error = ApptBookEventService_RootContextFailure;
    }

    // Build an exception if needed
    if (error != 0)
    {
     eventError = (EventError *)SOMMalloc(sizeof(EventError));
     eventError->errorCode = error;
     strcpy(eventError->reason, "Event Channel Reference Error");
     somSetException(ev,
                     USER_EXCEPTION,
                     ex_ApptBookEventService_EventError,
                     (void *) eventError);
                     eventChannel = NULL;

    }


    return (long) eventChannel;

}





/*
 * Cleans up the event objects
 */

SOM_Scope void  SOMLINK deleteEventChannel(ApptBookEventService *somSelf,
                                            Environment *ev)
{
    ApptBookEventServiceData *somThis = ApptBookEventServiceGetData(somSelf);
    ApptBookEventServiceMethodDebug("ApptBookEventService","deleteEventChannel");

    CosNaming_NamingContext      *nameContext=NULL;
    CosNaming_Name               Name;


    nameContext = somThis->ECNameContext;
    Name = somThis->ECName;
    if (nameContext != NULL)
    {
     nameContext->unbind(ev, &Name);
     checkEnv(ev);
    }
}

