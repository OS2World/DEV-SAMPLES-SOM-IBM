/*
 *
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 *
 *
 *  DISCLAIMER OF WARRANTIES.
 *  The following [enclosed] code is sample code created by IBM
 *  Corporation. This sample code is not part of any standard or IBM
 *  product and is provided to you solely for the purpose of assisting
 *  you in the development of your applications.  The code is provided
 *  "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 *  NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 *  THIS CODE.  IBM shall not be liable for any damages arising out of
 *  your use of the sample code, even if they have been advised of the
 *  possibility of such damages.
 *
 *  DISTRIBUTION.
 *  This sample code can be freely distributed, copied, altered, and
 *  incorporated into other software, provided that it bears the above
 *  Copyright notice and DISCLAIMER intact.
 *
 */

/*******************************************************************
   util.cpp

   This source contains routines that create a factory finder and a
server thread needed by the Externalization Service given the
constraints assumed by the Externalization Samples.
*******************************************************************/
#include "util.hpp"


/*******************************************************************
  exception()

   Routine used by functions in this source file to handle excep-
tions prior to process termination due to events that will not allow
the sample to perform as intended.
*******************************************************************/
void checkException(Environment *ev, char *message) {
   if (ev->_major != NO_EXCEPTION) {
      somPrintf("\n***** %s FAILED *****\n", message);
      somExceptionFree(ev);
      exit(1);
   }
}


/*******************************************************************
   createMyFF()

   This routine creates a factory finder and returns a pointer to
that factory finder to the caller.  Factory finders are Life Cycle
objects.  The factory finder created below is used to determine the
server on which an internalized object is placed.

The factory finder and its location object are not heavily used by
our sample clients so we allow DSOM to create those objects on any
server it chooses.  This allow us to use the simplest creation process.
*******************************************************************/
somLifeCycle_FactoryFinder *createMyFF() {
   Environment *ev = somGetGlobalEnvironment();

// Create a factory finder and initialize it
   somLifeCycle_FactoryFinder * myFF =
      (somLifeCycle_FactoryFinder *)(void *)
         somdCreate(ev, "somLifeCycle::FactoryFinder", FALSE);
   checkException(ev, "createMyFF() -- Factory Finder Creation");
   myFF->init_for_object_creation(ev);
   checkException(ev, "createMyFF() -- Factory Finder initialization");

// Create Location object to direct myFF's search for factories.
   somLifeCycle_ServerSetLocation * svrSetLoc =
       (somLifeCycle_ServerSetLocation *) ((void *)
          somdCreate(ev,"somLifeCycle::ServerSetLocation",FALSE) );
   checkException(ev, "createMyFF() -- ServerSetLocation Creation");
   svrSetLoc->init_for_object_creation(ev);
   checkException(ev, "createMyFF() -- ServerSetLocation initialization");

//Get impldefinition of the server that the Department object is built on.
   SOMD_ImplDefObject =
      SOMD_ImplRepObject->find_impldef_by_alias(ev,CLIENT_SERVER_THREAD);
   checkException(ev, "createMyFF() -- Fetching client server thread implementation definition");

// Get the servers identifier string
   string impldefId = SOMD_ImplDefObject->_get_impl_id(ev);
   checkException(ev, "createMyFF() -- Fetching client server thread id");

// Constrain factory search to the server that the Department object is built on.
   svrSetLoc->add_server_id(ev, impldefId);
   checkException(ev, "createMyFF() -- Adding client server thread id to location object");
   myFF->set_location_scope(ev,  svrSetLoc);
   checkException(ev, "createMyFF() -- Adding location object to the factory finder");
   return(myFF);
}


/*******************************************************************
   getRemoteObjectReference()

      This routine returns a reference to the object that contains
data for Department 45 (recall it contains two objects).  It
contains a reference to counts which is owned by the company and
sales which is owned by dept45.

   Recall that the program company.exe was run prior to invoking
this function from client1 and client2.  The company.exe created the
object on a server and registered the object with the naming service.
This function gets a reference to the object by using the Name Service.
A constraint is pass to the Name Service and a object reference is
returned.  The server that the actual object is on is not specified in
the code for company.exe, client1.exe, or client2.exe (object adminis-
tration has been separated from object use.).

Refer to the Naming Service Chapter of the Object Service User's
guide for more details on using the Naming Service.
*******************************************************************/

OESSample_Department *getRemoteObjectReference() {
   Environment                          *ev = somGetGlobalEnvironment();
   OESSample_Department                 *dept45;
   ExtendedNaming_ExtendedNamingContext *nameContext;

// Construct the search criteria to be passed to the Naming Service.
   char                                 *kind = "object interface";
   char                                 *id   = "Dept45";
   CosNaming_NameComponent              nameComponent = {id, kind};
   CosNaming_Name                       name = { 1, 1, &nameComponent };

// Retrieve a reference to the root naming context.
   nameContext =  (ExtendedNaming_ExtendedNamingContext*)
      SOMD_ORBObject->resolve_initial_references(ev, "NameService");
   checkException(ev, "getRemoteObjectReference() -- Fetching naming context");

   dept45 = ( (OESSample_Department *) (void *)
       nameContext->resolve(ev, &name) );
   checkException(ev, "getRemoteObjectReference() -- Fetching reference to dept45");

   return(dept45);
}


/*******************************************************************
   threadProc()

   The entry point for this routine is passed as an argument to
startSvrThread() below. It starts the event loop of the server.
*******************************************************************/
ULONG  threadProc() {
  Environment *ev = somGetGlobalEnvironment();

  SOMD_SOMOAObject->execute_request_loop(ev,SOMD_WAIT);
  checkException(ev, "Starting server thread's request loop");
  return(NULL);
}


/*******************************************************************
   startServerThread()

   This routine is used to start a server thread in the client
processes of the Externalization Samples.  Remote objects are
internalized into this thread's address space so that client
processing can be performed using local address accesses.
*******************************************************************/
void startServerThread() {
  TID taskID;
  Environment *ev = somGetGlobalEnvironment();

  SOMD_SOMOAObject   = new SOMOA;
  SOMD_ImplDefObject = SOMD_ImplRepObject->find_impldef_by_alias(ev,CLIENT_SERVER_THREAD);
  checkException(ev, "Fetching implementation of the server thread");

  somos_init_services(TRUE);
  SOMD_SOMOAObject->impl_is_ready(ev,SOMD_ImplDefObject);
  somos_init_services_afterimpl(TRUE);

  if( DosCreateThread( (PTID)       &taskID,
                       (PFNTHREAD) threadProc,  // Func to execute when thread starts
                       (ULONG)     0,           // No parameters passed
                       (ULONG)     0,           // Start thread in ready state
                       (ULONG)     STACK_SIZE
                     ) != 0 ) {
     somPrintf("\n%s  failed/n", CLIENT_SERVER_THREAD);
     exit(1);
  }
}


 //  if(SOMStartThread( &threadToken, (somTD_SOMThreadProc *)threadProc, NULL, 0, STACK_SIZE)) {

