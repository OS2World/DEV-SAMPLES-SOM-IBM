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
 * COMPANY.CPP
 *
 *  NOTE
 *     Please read the README file before inspecting this code. It provides
 *  a context to enhance your understanding of how the Externalization
 *  Service can be used to your advantage.
 *
 *  DESCRIPTION:
 *
 *     This program creates a company object on an object server named
 *  objServer.  The object is created their because it is the only
 *  server that has the VeryBigCo interface registered with it.  This
 *  company object is processed by the clients (client1 and client2)
 *  packaged with this sample.
 *
 *  COMPANY ALGORITHM -- main()
 *     1. Create remote object
 *     2. Bind the object to a name.  This allows clients to use the
 *        Name Service to resolve a reference to the object by using the name.
 *     end algorithm
 */


#include <iostream.h>
#include <somd.xh>
#include <somnm.xh>     // Usage binding for Name Service
#include "util.hpp"     // Utilities for the samples
#include "samobj.xh"    // Usage bindings of the sample object


void main(int argc, char **argv) {
  Environment *ev = somGetGlobalEnvironment();
  OESSample_VeryBigCo * companyObj, *tmpCo;

  SOM_InitEnvironment(ev);                   // Setup for processing
  SOMD_Init(ev);

   tmpCo =  (OESSample_VeryBigCo *)  (void *)
      somdCreate(ev, "OESSample::VeryBigCo", FALSE);
   checkException(ev, "Company -- Creating the company object");

   companyObj =  (OESSample_VeryBigCo *) (void *)
      tmpCo->init_for_object_creation(ev);
   checkException(ev, "Company -- Initializing the company object");

   ((SOMDObject *)tmpCo)->release(ev);
   checkException(ev,
      "Company -- Dis-associating pointer from company object storage");

   companyObj->populate(ev);

   somPrintf("*******     REMOTE OBJECT CREATED ON THE OBJSERVER      ********\n");

   ExtendedNaming_ExtendedNamingContext *nameContext;
   char *kind = "object interface";                     // Define the name
   char *id   = "Dept45";                               // for the Department
   CosNaming_NameComponent nameComponent = {id, kind};  // 45 object that is
   CosNaming_Name  name = { 1, 1, &nameComponent };     // embedded in companyObj

   nameContext =  (ExtendedNaming_ExtendedNamingContext*)
      SOMD_ORBObject->resolve_initial_references(ev, "NameService");
   checkException(ev, "Company -- Fetch reference to root naming context");

// Register the Department 45 object with the Naming Server.
   nameContext->bind(ev, &name, (SOMObject*) ((void *) companyObj->_get_dept45(ev)) );
   if (ev->_major == USER_EXCEPTION) {
      if(strcmp(ev->exception._exception_name, "::CosNaming::NamingContext::AlreadyBound") == 0) {
         somExceptionFree(ev);
         nameContext->unbind(ev, &name);
         nameContext->bind(ev, &name, (SOMObject*) ((void *) companyObj->_get_dept45(ev)) );
      }
      else {
         somPrintf("Department 45 object bind failed\n");
        exit(1);
      }
   }

   somPrintf("******* REMOTE OBJECT REGISTERED WITH THE NAME SERVER   ********\n");
   somPrintf("*******    You may now run client1 and/or client2       ********\n");

}

