// 3/8/96 16:13:23
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

#ifndef SOM_Module_eventh_Source
#define SOM_Module_eventh_Source
#endif
#define AppointmentEventHandler_Class_Source

#include "eventh.xih"
#include "apevents.xh"
#include "samptype.h"

#include <es_impl.xh>  // Event Service
#include <somnm.xh>    // Naming Service
#include <orb.xh>      // Object Request Broker
#include <somd.xh>     // DSOM

int getEventChannel(void *);


SOM_Scope void  SOMLINK eventChannelConnect(AppointmentEventHandler *somSelf,
                                             Environment *ev)
{
    AppointmentEventHandlerData *somThis = AppointmentEventHandlerGetData(somSelf);
    AppointmentEventHandlerMethodDebug("AppointmentEventHandler","eventChannelConnect");

    SomEventService_EventChannel *eventChannel = NULL;
    ApptBookEventService *apptBookEventService = new ApptBookEventService;

    // Get reference to event service
    eventChannel = (SomEventService_EventChannel *)
        apptBookEventService->getEventChannelRef(ev);
    delete apptBookEventService;


    //Obtain SupplierAdmin object from event channel
    CosEventChannelAdmin_ConsumerAdmin *consumerAdmin =
      eventChannel->for_consumers(ev);
    checkEnv(ev);



    //Obtain proxy Push Consumer object from the supplier admin object
    CosEventChannelAdmin_ProxyPushSupplier *proxyPushSupplier =
      consumerAdmin->obtain_push_supplier(ev);
    checkEnv(ev);
    //connect push supplier to the consumers via the event channel's proxyPushConsummer
    proxyPushSupplier->connect_push_consumer(ev,somSelf);
    somPrintf("Event Handler Connected to Event Channel\n");
    checkEnv(ev);
    // Save reference to proxy push consumer in the Address book
    somSelf->_set_myProxyPushSupplier(ev,(unsigned long)proxyPushSupplier);
    checkEnv(ev);

}



SOM_Scope void  SOMLINK push(AppointmentEventHandler *somSelf,
                              Environment *ev, any* data)
{

    /* AppointmentEventHandlerData *somThis = AppointmentEventHandlerGetData(somSelf); */
    AppointmentEventHandlerMethodDebug("AppointmentEventHandler","push");

    AppointmentEventHandler_parent_CosEventComm_PushConsumer_push(somSelf,
                                                                  ev,
                                                                  data);

   // The client is a multithreaded SOMOS::Server. Since this method runs in it's own
   // thread, it can handle the event while the main thread is running the user interface.

   somPrintf("!!! EVENT !!!\n");

   //Notify the application of the event
   char **str;
   int foo;

   foo = (int) data->_value;

   str = (char**) foo;

   somPrintf("Address book for %s has been updated.\n", *str);

}

SOM_Scope void SOMLINK somDefaultInit(AppointmentEventHandler *somSelf,
                                      som3InitCtrl* ctrl)
{
    AppointmentEventHandlerData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentEventHandlerMethodDebug("AppointmentEventHandler","somDefaultInit");
    AppointmentEventHandler_BeginInitializer_somDefaultInit;

    AppointmentEventHandler_Init_CosEventComm_PushConsumer_somDefaultInit(somSelf, ctrl);


}


SOM_Scope void SOMLINK somDestruct(AppointmentEventHandler *somSelf,
                                   octet doFree, som3DestructCtrl* ctrl)
{
    AppointmentEventHandlerData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    AppointmentEventHandlerMethodDebug("AppointmentEventHandler","somDestruct");
    AppointmentEventHandler_BeginDestructor;

    /*
     * local AppointmentEventHandler deinitialization code added by programmer
     */

   Environment        *ev = somGetGlobalEnvironment();
   // Disconnect from the event channel
   CosEventChannelAdmin_ProxyPushSupplier *proxyPushSupplier =
    (CosEventChannelAdmin_ProxyPushSupplier *)
      ((unsigned long) somSelf->_get_myProxyPushSupplier(ev));
    proxyPushSupplier->disconnect_push_supplier(ev);


    AppointmentEventHandler_EndDestructor;
}



