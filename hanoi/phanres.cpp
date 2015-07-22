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
// DESCRIPTION: The PersistentHanoiResource class is used for registering the 
//              involvement of a Hanoi instance in a top-level transaction and 
//              for handling transaction termination.  It may also store a 
//              before-image of a Hanoi instance.  It is a persistent object
//              and is effectively a log for the Hanoi class.
//==============================================================================

#ifndef SOM_Module_persistenthanoiresource_Source
#define SOM_Module_persistenthanoiresource_Source
#endif
#define PersistentHanoiResource_Class_Source

#include <somd.xh>           // DSOM interface
#include <somtran.xh>        // Transaction Service interface
#include <somuutil.h>        // SOM Utilities

#include "hancon.xh"         // HanoiConstants module
#include "hanres.xh"         // HanoiResource interface
#include "phanres.xih"       // PersistentHanoiResource implementation interface
#include "traninv.xh"        // TranInv interface
#include "hanoi.xh"          // Hanoi interface

//==============================================================================
// Function: error
//                                                                             
// Return whether or not the specified environment indicates an error occured in
// the previous SOM call.                                                       
//==============================================================================
inline int                         // whether or not error occured
error( const Environment*const ev  // SOM environment
     )
{ 
  return ev->_major != NO_EXCEPTION;
}

//==============================================================================
// Method: PersistentHanoiResource::somDefaultInit
//==============================================================================
SOM_Scope               void                              // No return value
SOMLINK somDefaultInit( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
                      , somInitCtrl*             ctrl
                      )
{
  PersistentHanoiResourceData *somThis; /* set in BeginInitializer */
  somInitCtrl globalCtrl;
  somBooleanVector myMask;
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","somDefaultInit");
  PersistentHanoiResource_BeginInitializer_somDefaultInit;

  PersistentHanoiResource_Init_HanoiResource_somDefaultInit(somSelf, ctrl);
  PersistentHanoiResource_Init_somPersistencePO_PO_somDefaultInit(somSelf, ctrl);

  somThis->prepared = FALSE;
}

//==============================================================================
// Method: PersistentHanoiResource::init_for_object_creation
//==============================================================================
SOM_Scope                         SOMObject*                        // Resultant object
SOMLINK init_for_object_creation( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
                                , Environment*             ev       // SOM environment 
                                )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","init_for_object_creation");

  somInitCtrl globalCtrl, *ctrl=0;
  somBooleanVector myMask;
  PersistentHanoiResource_BeginInitializer_somDefaultInit;

  SOMObject* result_obj = PersistentHanoiResource_parent_somPersistencePO_PO_init_for_object_creation(somSelf,ev);
  if (error(ev)) 
  {
    somPrintf("somPersistencePO::PO::init_for_object_creation failed\n");
    result_obj = 0;
  }
  else 
  {
    PersistentHanoiResource_Init_HanoiResource_somDefaultInit(somSelf, ctrl);
  
    somThis->prepared = FALSE;
  }

  return result_obj;
}

//==============================================================================
// Method: PersistentHanoiResource::init_for_object_reactivation
//==============================================================================
SOM_Scope                             SOMObject*                        // Resultant object                               
SOMLINK init_for_object_reactivation( PersistentHanoiResource* somSelf  // PersistentHanoiResource object 
                                    , Environment*             ev       // SOM environment                
                                    )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","init_for_object_reactivation");

  somInitCtrl globalCtrl, *ctrl=0;
  somBooleanVector myMask;
  PersistentHanoiResource_BeginInitializer_somDefaultInit;

  SOMObject* result_obj = PersistentHanoiResource_parent_somPersistencePO_PO_init_for_object_reactivation(somSelf,ev);
  if (error(ev)) 
  {
    somPrintf("somPersistencePO::PO::init_for_object_creation failed\n");
    result_obj = 0;
  }
  else 
  {
    PersistentHanoiResource_Init_HanoiResource_somDefaultInit(somSelf, ctrl);
  
    somThis->prepared = FALSE;
  }

  return result_obj;
}

//==============================================================================
// Method: PersistentHanoiResource::somDestruct
//==============================================================================
SOM_Scope            void                              // No return value
SOMLINK somDestruct( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
                   , octet                    doFree   //
                   , somDestructCtrl*         ctrl     //
                   )
{
  PersistentHanoiResourceData *somThis; /* set in BeginDestructor */
  somDestructCtrl globalCtrl;
  somBooleanVector myMask;
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","somDestruct");
  PersistentHanoiResource_BeginDestructor;

  PersistentHanoiResource_EndDestructor;
}

//==============================================================================
// Method: PersistentHanoiResource::uninit_for_object_destruction
//==============================================================================
SOM_Scope                              void                              // No return value                
SOMLINK uninit_for_object_destruction( PersistentHanoiResource* somSelf  // PersistentHanoiResource object 
                                     , Environment*             ev       // SOM environment                
                                     )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","uninit_for_object_destruction");

  PersistentHanoiResource_parent_somPersistencePO_PO_uninit_for_object_destruction(somSelf,ev);
}

//==============================================================================
// Method: PersistentHanoiResource::uninit_for_object_passivation
//==============================================================================
SOM_Scope                              void                              // No return value                
SOMLINK uninit_for_object_passivation( PersistentHanoiResource* somSelf  // PersistentHanoiResource object 
                                     , Environment*             ev       // SOM environment                
                                     )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","uninit_for_object_passivation");

  PersistentHanoiResource_parent_somPersistencePO_PO_uninit_for_object_passivation(somSelf,ev);
}

//==============================================================================
// Method: PersistentHanoiResource::register_resource
//
// N.B. This method is only ever called to register with a top-level 
//      transaction.
//==============================================================================
SOM_Scope                  void                                  // No return value
SOMLINK register_resource( PersistentHanoiResource*     somSelf  // PersistentHanoiResource object
                         , Environment*                 ev       // SOM environment
                         , CosTransactions_Coordinator* coord    // Transaction coordinator
                         , Hanoi*                       tower    // Hanoi object
                         )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","register_resource");

  if (somSelf->_get_registered(ev) == FALSE)
  {
    somSelf->_set_coord(ev,coord);
    somSelf->_set_tower(ev,tower);
    somThis->recovery_coord = coord->register_resource(ev,somSelf);
    somSelf->_set_registered(ev,TRUE);
  }
}

//==============================================================================
// Method: PersistentHanoiResource::prepare
//                                                                             
// Get the changes to a Hanoi object ready to be committed or rolled back.  Vote
// read only if no update has been performed.  If an update has been performed, 
// vote commit if all the disks are on one spindle.  Otherwise roll back and
// vote rollback.
//==============================================================================
SOM_Scope        CosTransactions_Vote              // Vote from prepare
SOMLINK prepare( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
               , Environment*             ev       // SOM environment
               )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","prepare");

  Hanoi*const tower = somSelf->_get_tower(ev);

  if (somSelf->_get_before_image_exists(ev) == FALSE)
  {
    somSelf->self_destruct(ev);

    return CosTransactions_VoteReadOnly;
  }
  else
  {
    CosTransactions_Vote vote = CosTransactions_VoteRollback;

    somSelf->store(ev,0); 

    if (error(ev)) 
    {
      somPrintf("PersistentHanoiResource::store failed\n");
      somdExceptionFree(ev);
    } 
    else
      vote = tower->prepare(ev,somSelf);
  
    if (vote == CosTransactions_VoteRollback) 
      somSelf->rollback(ev);
    else
      somThis->prepared = TRUE;

    return vote;
  }
}

//==============================================================================
// Method: PersistentHanoiResource::commit
//                                                                             
// Commit the changes to a prepared Hanoi object or raise an exception if the
// object has not been prepared.
//==============================================================================
SOM_Scope       void                              // No return value
SOMLINK commit( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
              , Environment*             ev       // SOM environment
              )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","commit");

  if (somThis->prepared == TRUE) 
    somSelf->self_destruct(ev);
  else 
    somSetException(ev,USER_EXCEPTION,ex_NotPrepared,0);
}

//==============================================================================
// Method: PersistentHanoiResource::rollback
//                                                                             
// Roll back the changes to a Hanoi object.
//==============================================================================
SOM_Scope         void                              // No return value
SOMLINK rollback( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
                , Environment*             ev       // SOM environment
                )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","rollback");

  somSelf->apply_before_image(ev);

  somSelf->self_destruct(ev);
}

//==============================================================================
// Method: PersistentHanoiResource::recover
//                                                                             
// Perform recovery on behalf of a transaction.
//==============================================================================
SOM_Scope        void                              // No return value
SOMLINK recover( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
               , Environment*             ev       // SOM environment
               )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","recover");
  
  // Drive the recovery coordinator to replay the completion of the transaction.
  // If the recovery coordinator says the transaction was not prepared, roll 
  // back.  If the recovery coordinator cannot be contacted at all, then take 
  // a heuristic decision.  If the recovery coordinator could not be contacted 
  // temporarily or if it returns a status of 'unknown', remain in doubt about 
  // the outcome of the transaction and retry.

  CosTransactions_Status status = somThis->recovery_coord->replay_completion(ev,somSelf);

  while ((error(ev) && (strcmp(somExceptionId(ev),ex_COMM_FAILURE) == 0 ||
                        strcmp(somExceptionId(ev),ex_TRANSIENT) == 0 )) ||
         status == CosTransactions_StatusUnknown
        )
  {
    if (error(ev)) 
      somdExceptionFree(ev);
    // preferably sleep for a while - omitted for simplicity
    somPrintf("Replay completion being retried ...\n");
    status = somThis->recovery_coord->replay_completion(ev,somSelf);
  }

  // If the transaction was not prepared, roll it back.  Otherwise the
  // recovery coordinator could not be contacted and we take a heuristic
  // decision based on the contents of somenv.ini.
  if (error(ev))
  {
    boolean commit_transaction = FALSE;

    if (strcmp(somExceptionId(ev),ex_NotPrepared) != 0) 
    {
      string heuristic_var = somutgetenv("HEURISTICDIRECTION","[SOM_TRANSACTION]");
  
      if(heuristic_var != 0) 
        commit_transaction = (*heuristic_var == '1');
    } 

    somdExceptionFree(ev);

    if (commit_transaction == TRUE)
    {
      somSelf->commit(ev);
      if (error(ev)) 
        somPrintf("PersistentHanoiResource::commit failed\n");
    }
    else
    {
      somSelf->rollback(ev);
      if (error(ev)) 
        somPrintf("PersistentHanoiResource::rollback failed\n");
    }
  }
}

//==============================================================================
// Method: PersistentHanoiResource::self_destruct
//                                                                             
// Uninitialise and free the HanoiResource instance.  Although it is unusual
// for a method to free the instance it was called with, this is the only 
// simple way to clean up resource objects.
//==============================================================================
SOM_Scope              void                              // No return value
SOMLINK self_destruct( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
                     , Environment*             ev       // SOM environment
                     )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","self_destruct");

  // Delete persistent state using the default PID associated with the PO.
  // Since the object may not have been stored if the transaction rolled back
  // without preparing, exceptions may be raised.  These are ignored.
  somSelf->Delete(ev,0);

  if (error(ev)) 
    somdExceptionFree(ev);

  // Delete any recovery coordinator including its persistent state
  if (somThis->recovery_coord != 0) 
    delete somThis->recovery_coord;

  // Break the relationship between the Hanoi instance and the 
  // PersistentHanoiResource if the transaction updated the Hanoi instance
  if (somSelf->_get_before_image_exists(ev) == TRUE)
  {
    Hanoi*const tower = somSelf->_get_tower(ev);
  
    tower->delete_log(ev);
    if (error(ev)) 
    {
      somPrintf("Hanoi::delete_log failed\n");
      somdExceptionFree(ev);
    }
  }

  PersistentHanoiResource_parent_HanoiResource_self_destruct(somSelf,ev);
  if (error(ev)) 
    somPrintf("HanoiResource::self_destruct failed\n");
}

//==============================================================================
// Method: PersistentHanoiResource::externalize_to_stream
//                                                                             
// Write the persistent state of a PersistentHanoiResource object to a stream.
//==============================================================================
SOM_Scope                      void                              // No return value 
SOMLINK externalize_to_stream( PersistentHanoiResource* somSelf  // PersistentHanoiResource object
                             , Environment*             ev       // SOM environment
                             , CosStream_StreamIO*      stream   // Stream 
                             )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","externalize_to_stream");

  PersistentHanoiResource_parent_HanoiResource_externalize_to_stream(somSelf,ev,stream);

  stream->write_object(ev,somSelf->_get_tower(ev));

  if (error(ev)) 
  {
    somPrintf("CosStream_StreamIO::write_object failed\n");
    somdExceptionFree(ev);
  }
  else 
  {   
    string recovery_coord_string = SOMD_ORBObject->object_to_string(ev,somThis->recovery_coord);

    if (error(ev)) 
    {
      somPrintf("ORB::object_to_string failed\n");
      somdExceptionFree(ev);
    }
    else 
    {
      stream->write_string(ev,recovery_coord_string);
      ORBfree(recovery_coord_string);
    
      if (error(ev)) 
      {
        somPrintf("CosStream_StreamIO::write_string failed\n");
        somdExceptionFree(ev);
      }
    }
  }
}

//==============================================================================
// Method: PersistentHanoiResource::internalize_from_stream
//                                                                             
// Read the persistent state of a PersistentHanoiResource object from a stream.
//==============================================================================
SOM_Scope                        void                                 // No return value
SOMLINK internalize_from_stream( PersistentHanoiResource*    somSelf  // PersistentHanoiResource object
                               , Environment*                ev       // SOM environment
                               , CosStream_StreamIO*         stream   // Stream
                               , CosLifeCycle_FactoryFinder* ff       // Factory finder
                               )
{
  PersistentHanoiResourceData *somThis = PersistentHanoiResourceGetData(somSelf);
  PersistentHanoiResourceMethodDebug("PersistentHanoiResource","internalize_from_stream");
  
  PersistentHanoiResource_parent_HanoiResource_internalize_from_stream(somSelf,ev,stream,ff);

  Hanoi* tower = (Hanoi*)(void*)(stream->read_object(ev,ff,0));

  if (error(ev)) 
  {
    somPrintf("CosStream_StreamIO::read_object failed\n");
    somdExceptionFree(ev);
  }
  else 
  {
    somSelf->_set_tower(ev,tower);
    if (error(ev)) 
    {
      somPrintf("HanoiResource::_set_tower failed\n");
      somdExceptionFree(ev);
    }
    else 
    {
      string recovery_coord_string = stream->read_string(ev);
      if (error(ev)) 
      {
        somPrintf("CosStream_StreamIO::read_string failed\n");
        somdExceptionFree(ev);
      }
      else
      {
        somThis->recovery_coord = (CosTransactions_RecoveryCoordinator*)(void*)
                                    (SOMD_ORBObject->string_to_object(ev,recovery_coord_string));
        SOMFree(recovery_coord_string);

        if (error(ev)) 
        {
          somPrintf("ORB::string_to_object failed\n");
          somdExceptionFree(ev);
        }
      } 
    }
  }
}
