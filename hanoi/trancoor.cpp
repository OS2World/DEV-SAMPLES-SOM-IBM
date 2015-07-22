//
// COMPONENT_NAME: somtr::SAMPLES
//
// ORIGINS: 27
//
// Licensed Materials - Property of IBM                                     
//                                                                          
// 25H7-912, 5697-197 (EMEA), 17H5-843 (JAPAN)                              
//                                                                          
// (c) Copyright IBM Corp. 1995                                             
//
// DISCLAIMER OF WARRANTIES.
// The following [enclosed] code is sample code created by IBM
// Corporation. This sample code is not part of any standard or IBM
// product and is provided to you solely for the purpose of assisting
// you in the development of your applications.  The code is provided
// "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
// THIS CODE.  IBM shall not be liable for any damages arising out of
// your use of the sample code, even if they have been advised of the
// possibility of such damages.
//
// DISTRIBUTION.
// This sample code can be freely distributed, copied, altered, and
// incorporated into other software, provided that it bears the above
// Copyright notice and DISCLAIMER intact.
// 

//==============================================================================
// DESCRIPTION: The TranCoord class makes it possible to use a transaction
//              coordinator as the key of a hash table.
//==============================================================================

#ifndef SOM_Module_trancoor_Source
#define SOM_Module_trancoor_Source
#endif
#define TranCoord_Class_Source

#include <somtran.xh>        // Transaction Service interface

#include "trancoor.xih"      // TranCoord implementation interface

//============================================================================
// Method: TranCoord:create
//                                                                             
// Initialise a TranCoord instance to point at a transaction coordinator.
//============================================================================
SOM_Scope       void                                  // No return value
SOMLINK create( TranCoord*                   somSelf  // TranCoord object
              , Environment*                 ev       // SOM environment
              , somInitCtrl*                 ctrl     // 
              , CosTransactions_Coordinator* coord    // Coordinator
              )
{
  TranCoordData *somThis; /* set in BeginInitializer */
  somInitCtrl globalCtrl;
  somBooleanVector myMask;
  TranCoordMethodDebug("TranCoord","create");
  TranCoord_BeginInitializer_create;

  TranCoord_Init_somf_MCollectible_somDefaultInit(somSelf,ctrl);

  somThis->coord = coord;
}

//============================================================================
// Method: TranCoord:somfHash
//                                                                             
// Return a hash value for the transaction coordinator.
//============================================================================
SOM_Scope         long                  // Hash value
SOMLINK somfHash( TranCoord*   somSelf  // TranCoord object
                , Environment* ev       // SOM environment
                )
{
  TranCoordData *somThis = TranCoordGetData(somSelf);
  TranCoordMethodDebug("TranCoord","somfHash");

  return (long)(somThis->coord->hash_transaction(ev));
}

//============================================================================
// Method: TranCoord:somfIsEqual
//                                                                             
// Compare two transaction coordinators for representing the same transaction.
//============================================================================
SOM_Scope            boolean                     // Result of comparison
SOMLINK somfIsEqual( TranCoord*         somSelf  // TranCoord object
                   , Environment*       ev       // SOM environment
                   , somf_MCollectible* obj      // TranCoord for comparison
                   )
{
  TranCoordData *somThis = TranCoordGetData(somSelf);
  TranCoordMethodDebug("TranCoord","somfIsEqual");

  return somThis->coord->is_same_transaction(ev,TranCoordGetData((TranCoord*)obj)->coord);
}
