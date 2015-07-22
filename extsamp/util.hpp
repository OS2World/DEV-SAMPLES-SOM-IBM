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
   util.hpp

   This header documents the interface shared by the the two clients
   and the utility source (util.cpp)
*******************************************************************/

#ifndef UTIL_HPP
#define UTIL_HPP

#include <somd.xh>

#include <somlc.xh>               // Support factory finder creation
#include <impldef.xh>
#include <somnm.xh>
extern somLifeCycle_FactoryFinder *createMyFF();

#include "samobj.xh"

#define INCL_DOSPROCESS
#include <os2.h>               // Support server thread creation
#include <somosutl.h>
#define STACK_SIZE  65536
#define CLIENT_SERVER_THREAD "clientSvrThread"
#define OBJ_SERVER_ALIAS     "objServer"

extern ULONG  threadProc();
extern void startServerThread();

extern OESSample_Department *getRemoteObjectReference();
extern void checkException(Environment*, char*);

#endif

