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

#ifndef lint
static char *sccsid = "%Z% %I% %W% %G% %U% [%H% %T%]";
#endif

/*
 * CLIENT1.CPP
 *
 *  NOTE
 *     Please read the README file before inspecting this code. It provides
 *  a context to enhance your understanding of how the Externalization
 *  Service can be used to your advantage.
 *
 *  DESCRIPTION:
 *     Client1.exe is a sample that uses the Externalization Service
 *  interface as defined by OMG.  The next sample (Client2) demonstrates
 *  IBM extensions to that interface to achieve better performance by
 *  eliminating the multiple remote dispatches that occur in step 3 below.
 *
 *  CLIENT1 ALGORITHM -- main()
 *     1. Create remote object
 *     2. Create local stream
 *     3. Externalize remote object into a stream on the local
 *        server thread.
 *     4. Internalize the local stream into a local object
 *     5. Process the local object.
 *     end algorithm
 */


#include <iostream.h>
#include <somd.xh>

#include <somestrm.xh>  // Usage bindings for Externalization
#include <somestio.xh>

#include "util.hpp"     // Utilities for the samples

#include "samobj.xh"    // Usage bindings of the sample object


/*
   This sample uses streams identified by the following string. Streams
of this type support the streaming of stringified object references
to the streamIO buffer of the stream. (shallow copy)
*/
#define STRM_CLASS  ((string) "somExternalization::OSStream")

/*
   Streams always contain a streamIO object.  Choosing the streamIO type
determines how object state values are represented in the streamIO buffer
when an object is externalized.  The  string, "somStream::StringStreamIO",
is used to choose a string representation for object state values wrtten to
the streamIO buffer.
*/
#define STRM_IO_CLASS = ((string)"somStream::StringStreamIO")

void main(int argc, char **argv) {
  Environment                      *ev = somGetGlobalEnvironment();
  somExternalization_StreamFactory *strmFactory, *tmpStrmFactory;
  somLifeCycle_FactoryFinder       *ff;
  somExternalization_OSStream      *localStrm;
  OESSample_Department             *remoteObject,
                                   *localObject;

  SOM_InitEnvironment(ev);                   // Setup for processing
  SOMD_Init(ev);
  startServerThread();

// Create the stream in this process
  strmFactory  = (somExternalization_StreamFactory *) (void *)
     new somExternalization_StreamFactory;
  checkException(ev, "Client1 -- Creating the stream factory");

  localStrm =  (somExternalization_OSStream *) (void *)
     strmFactory->create_with_types(ev,
                                    ((string) "somExternalization::OSStream"),
                                    ((string)"somStream::StringStreamIO"));
  checkException(ev, "Client1 -- Creating the local stream");



// Get a reference to the remote object.
  remoteObject = getRemoteObjectReference();


// Stream the remote object's state to the streamIO buffer of the
// stream object on the server thread of the local process.
  localStrm->externalize(ev,remoteObject);
  checkException(ev, "Client1 -- Externalizing the remote object");

// Create a local object for processing. The created factory finder (ff)
// causes the object to be created on the local server thread via the
// internalize call.
  ff =  createMyFF();
  localObject = (OESSample_Department *) localStrm->internalize(ev,ff);
  checkException(ev, "Client1 -- Internalizing into the local object");

// Calculate the aritmetic mean using the local object.
  OESSample_SalesValues   *sales  = localObject->_get_sales(ev);
  OESSample_CompanyCounts *counts = localObject->_get_counts(ev);
  long                    count   = counts->getCount(ev,DEPT45);
  float                   sum     = 0.0;
  long                    c;

  for(c = 0; c < count; ++c)
     sum += sales->getSale(ev);


  cout << "\n\tThe mean of the sales is " << sum/(float)count << endl;


  // Perform cleanup
  ORBfree(localStrm);
  checkException(ev, "Client1 -- Freeing the local stream");

  ((SOMDObject *)remoteObject)->release(ev);
  checkException(ev, "Client1 -- Release the romote object");

  localObject->uninit_for_object_destruction(ev);
  checkException(ev, "Client1 -- Unititializing the local object");
  ORBfree(localObject);
  checkException(ev, "Client1 -- Freeing the local object");

  ((somLifeCycle_ServerSetLocation *)ff->get_location_scope(ev))
     ->uninit_for_object_destruction(ev);
     checkException(ev, "Client1 -- Uninitializing the location object");
  ORBfree(ff->get_location_scope(ev));
  checkException(ev, "Client1 -- Freeing the location object\n");

  ff->uninit_for_object_destruction(ev);
  checkException(ev, "Client1 -- Uninitializing the factory finder object");
  ORBfree(ff);
  checkException(ev, "Client1 -- Freeing the factory finder object");


  cout << "\n\n\n******* RUN COMPLETE ********\n\n\n" << endl;

  SOMD_Uninit(ev);
}

