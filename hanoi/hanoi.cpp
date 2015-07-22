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
// DESCRIPTION: The Hanoi class implements a recoverable Transaction Service
//              server for the Tower of Hanoi puzzle.
//==============================================================================

#ifndef SOM_Module_hanoi_Source
#define SOM_Module_hanoi_Source
#endif
#define Hanoi_Class_Source

#include <stexcep.xh>        // Standard exceptions
#include <somd.xh>           // DSOM interface
#include <somlock.xh>        // Concurrency Service interface

#include "hancon.xh"         // HanoiConstants module
#include "hanres.xh"         // HanoiResource interface
#include "phanres.xh"        // PersistentHanoiResource interface
#include "traninv.xh"        // TranInv interface
#include "hanoi.xih"         // Hanoi implementation interface

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
// Method: Hanoi::somDefaultInit                                                       
//                                                                             
// Override the default initialisation for the Hanoi class.
//==============================================================================
SOM_Scope               void                  // no return value
SOMLINK somDefaultInit( Hanoi*       somSelf  // Hanoi object
                      , somInitCtrl* ctrl     // 
                      )
{
  HanoiData *somThis; /* set in BeginInitializer */
  somInitCtrl globalCtrl;
  somBooleanVector myMask;
  HanoiMethodDebug("Hanoi","somDefaultInit");
  Hanoi_BeginInitializer_somDefaultInit;
  
  Hanoi_Init_CosTransactions_TransactionalObject_somDefaultInit(somSelf, ctrl);
  Hanoi_Init_somPersistence_IRPO_somDefaultInit(somSelf, ctrl);
  Hanoi_Init_CosStream_Streamable_somDefaultInit(somSelf, ctrl);
  
  Environment e;          // SOM environment
  Environment *const ev = &e;
  
  SOM_InitEnvironment(ev);
   
  // Create a lockset
  {
    CosConcurrencyControl_LockSetFactory*const lockset_factory = 
                         new CosConcurrencyControl_LockSetFactory;
    somThis->lockset = lockset_factory->create(ev);
    delete lockset_factory;
  }
  
  // Clear the spindles
  for (long i = 0; i < 3; i++)
    for (long j = 0; j < HanoiConstants_DISKS; j++)
      somThis->spindle[i][j] = 0;
  
  // Set up the disks on the first spindle in decreasing order of size
  for (long k = 0; k < HanoiConstants_DISKS; k++ )
    somThis->spindle[0][k] = HanoiConstants_DISKS - k;
  
  // Note that the object is not involved with any transaction
  somThis->tran_inv = new TranInv;
  somThis->pres = 0;
  somThis->name = 0;

  // Cache a current object in the Hanoi instance for future use
  somThis->current = new CosTransactions_Current;
  if (!somIsObj(somThis->current)) 
    somPrintf("CosTransactions::Current creation failed\n");
  
  SOM_UninitEnvironment(ev);
}

//==============================================================================
// Method: Hanoi::init_for_object_creation
//                                                                             
// Override the creation initialisation for the Hanoi class.
//==============================================================================
SOM_Scope                         SOMObject*            // Resultant object
SOMLINK init_for_object_creation( Hanoi*       somSelf  // Hanoi object
                                , Environment* ev       // SOM environment
                                )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","init_for_object_creation");

  somInitCtrl globalCtrl, *ctrl = 0;
  somBooleanVector myMask;
  Hanoi_BeginInitializer_somDefaultInit;
  
  SOMObject* result_obj = Hanoi_parent_somPersistence_IRPO_init_for_object_creation(somSelf,ev);
  if (error(ev)) 
  {
    somPrintf("somPersistence::IRPO::init_for_object_creation failed\n");
    result_obj = 0;
  }
  else 
  {
    Hanoi_Init_CosTransactions_TransactionalObject_somDefaultInit(somSelf,ctrl);
    Hanoi_Init_CosStream_Streamable_somDefaultInit(somSelf,ctrl);
    
    // Create a lockset
    {
      CosConcurrencyControl_LockSetFactory*const lockset_factory = 
                           new CosConcurrencyControl_LockSetFactory;
      somThis->lockset = lockset_factory->create(ev);
      delete lockset_factory;
    }
    
    // Clear the spindles
    for (long i = 0; i < 3; i++)
      for (long j = 0; j < HanoiConstants_DISKS; j++)
        somThis->spindle[i][j] = 0;
    
    // Set up the disks on the first spindle in decreasing order of size
    for (long k = 0; k < HanoiConstants_DISKS; k++ )
      somThis->spindle[0][k] = HanoiConstants_DISKS - k;
    
    // Note that the object is not involved with any transaction
    somThis->tran_inv = new TranInv;
    somThis->pres = 0;
    somThis->name = 0;

    // Cache a current object in the Hanoi instance for future use
    somThis->current = new CosTransactions_Current;
    if (!somIsObj(somThis->current)) 
      somPrintf("CosTransactions::Current creation failed\n");
  } 
  
  return result_obj;
}

//==============================================================================
// Method: Hanoi::init_for_object_reactivation
//                                                                             
// Override the reactivation initialisation for the Hanoi class.
//
// N.B. The data member pres will be set up during internalize_from_stream as
//      will the other data members.
//==============================================================================
SOM_Scope                             SOMObject*            // Resultant object
SOMLINK init_for_object_reactivation( Hanoi*       somSelf  // Hanoi object
                                    , Environment* ev       // SOM environment
                                    )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","init_for_object_reactivation");

  somInitCtrl globalCtrl, *ctrl = 0;
  somBooleanVector myMask;
  Hanoi_BeginInitializer_somDefaultInit;
  
  SOMObject* result_obj = Hanoi_parent_somPersistence_IRPO_init_for_object_reactivation(somSelf,ev);
  if (error(ev)) 
  {
    somPrintf("somPersistence::IRPO::init_for_object_reactivation failed\n");
    result_obj = 0;
  }
  else 
  {
    Hanoi_Init_CosTransactions_TransactionalObject_somDefaultInit(somSelf,ctrl);
    Hanoi_Init_CosStream_Streamable_somDefaultInit(somSelf,ctrl);
    
    // Create a lockset
    {
      CosConcurrencyControl_LockSetFactory*const lockset_factory = 
                           new CosConcurrencyControl_LockSetFactory;
      somThis->lockset = lockset_factory->create(ev);
      delete lockset_factory;
    }
    
    // Clear the spindles - not essential prior to reactivation
    for (long i = 0; i < 3; i++)
      for (long j = 0; j < HanoiConstants_DISKS; j++)
        somThis->spindle[i][j] = 0;
    
    // Note that the object is not involved with any transaction
    somThis->tran_inv = new TranInv;
    somThis->pres = 0;
    somThis->name = 0;

    // Cache a current object in the Hanoi instance for future use
    somThis->current = new CosTransactions_Current;
    if (!somIsObj(somThis->current)) 
      somPrintf("CosTransactions::Current creation failed\n");
  }
 
  return result_obj;
}

//==============================================================================
// Method: Hanoi::somDestruct
//                                                                             
// Override the default uninitialisation for the Hanoi class.
//==============================================================================
SOM_Scope            void                      // No return value
SOMLINK somDestruct( Hanoi*           somSelf  // Hanoi object
                   , octet            doFree   //
                   , somDestructCtrl* ctrl     //
                   )
{
  HanoiData *somThis; /* set in BeginDestructor */
  somDestructCtrl globalCtrl;
  somBooleanVector myMask;
  HanoiMethodDebug("Hanoi","somDestruct");
  Hanoi_BeginDestructor;

  delete somThis->tran_inv;

  delete somThis->lockset;

  delete somThis->pres;

  if (somThis->name != 0)
    SOMFree(somThis->name);

  delete somThis->current;

  Hanoi_EndDestructor;
}

//==============================================================================
// Method: Hanoi::uninit_for_object_destruction
//                                                                             
// Override the destruction uninitialisation for the Hanoi class.
//==============================================================================
SOM_Scope                              void                  // No return value
SOMLINK uninit_for_object_destruction( Hanoi*       somSelf  // Hanoi object
                                     , Environment* ev       // SOM environment
                                     )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","uninit_for_object_destruction");

  if (somThis->pres != 0) 
    somThis->pres->Delete(ev,0);

  Hanoi_parent_somPersistence_IRPO_uninit_for_object_destruction(somSelf,ev);
}

//==============================================================================
// Method: Hanoi::uninit_for_object_passivation
//                                                                             
// Override the passivation uninitialisation for the Hanoi class.
//==============================================================================
SOM_Scope                              void                  // No return value
SOMLINK uninit_for_object_passivation( Hanoi*       somSelf  // Hanoi object
                                     , Environment* ev       // SOM environment
                                     )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","uninit_for_object_passivation");

  if (somThis->pres != 0) 
    somThis->pres->disconnect(ev,0);

  Hanoi_parent_somPersistence_IRPO_uninit_for_object_passivation(somSelf,ev);
}

//==============================================================================
// Method: Hanoi::display
//                                                                             
// Return a string portraying the current state.
//==============================================================================
SOM_Scope        string  
SOMLINK display( Hanoi*       somSelf  // Hanoi object
               , Environment* ev       // SOM environment
               )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","display");
  string result = "";

  // Read-lock the object 
  somThis->lockset->lock(ev,CosConcurrencyControl_read);

  if(!error(ev))
  {
    // Register a resource in the current transaction.  This returns 0 if 
    // there is no current transaction.
    HanoiResource*const resource = somSelf->register_in_transaction(ev,0);

    result = (string)SOMMalloc(4096);
    
    *result = '\0';

    strcat(result,"\n\n"); // lines before display
  
    // Display contents of spindles one level at a time
    for (long i = HanoiConstants_DISKS - 1; i >= 0; i--)
    {
      strcat(result,"\n"); // start new level
  
      // Display contents at ith level one spindle at a time
      for (long j = 0; j < 3; j++)
      {
        // Display contents of ith level of jth spindle
        long diskno = somThis->spindle[j][i];
  
        if (diskno == 0) 
        {
          for (long k = HanoiConstants_DISKS - 1; k > 0; k--)
            strcat(result," ");
  
          strcat(result,".");
  
          for (k = HanoiConstants_DISKS - 1; k > 0; k--)
            strcat(result," ");
        } 
        else 
        {
          for (long k = HanoiConstants_DISKS - diskno; k > 0; k--)
            strcat(result," ");
  
          for (k = 2 * diskno - 1; k > 0; k--)
            strcat(result,"=");
  
          for (k = HanoiConstants_DISKS - diskno; k > 0; k--)
            strcat(result," ");
        };
  
        strcat(result,"  "); // spaces between spindles
      }
    }
  
    strcat(result,"\n"); // line after display

    // If there is no current transaction, release the lock.
    if (resource == 0)
      somThis->lockset->unlock(ev,CosConcurrencyControl_read);
  }

  return result;
}

//==============================================================================
// Method: Hanoi::get_full_spindle
//
// Determine which spindle is holding all the disks.  Return -1 if no spindle
// holds all the disks.
//==============================================================================
SOM_Scope                 long                  // full spindle
SOMLINK get_full_spindle( Hanoi*       somSelf  // Hanoi object
                        , Environment* ev       // SOM environment
                        )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","get_full_spindle");

  long full_spindle = -1;

  // Read-lock the object 
  somThis->lockset->lock(ev,CosConcurrencyControl_read);

  if(!error(ev))
  {
    // Register a resource in the current transaction.  This returns 0 if 
    // there is no current transaction.
    HanoiResource*const resource = somSelf->register_in_transaction(ev,0);
  
    for (long i = 0; i < 3; i++) 
      if (somThis->spindle[i][HanoiConstants_DISKS - 1] != 0)
      {
        full_spindle = i;
        break;
      }
  
    // If there is no current transaction, release the lock.
    if (resource == 0)
      somThis->lockset->unlock(ev,CosConcurrencyControl_read);
  }

  return full_spindle;
}

//==============================================================================
// Method: Hanoi::get_empty_spindle
//
// Determine a spindle which is empty.  Return -1 if no spindle is empty.
//==============================================================================
SOM_Scope                  long                  // empty spindle
SOMLINK get_empty_spindle( Hanoi*       somSelf  // Hanoi object
                         , Environment* ev       // SOM environment
                         )
{
  HanoiData *somThis = HanoiGetData(somSelf); 
  HanoiMethodDebug("Hanoi","get_empty_spindle");

  long to_spindle = -1;

  // Read-lock the object
  somThis->lockset->lock(ev,CosConcurrencyControl_read);

  if(!error(ev))
  {
    // Register a resource in the current transaction.  This returns 0 if 
    // there is no current transaction.
    HanoiResource*const resource = somSelf->register_in_transaction(ev,0);

    for (long i = 0; i < 3; i++) 
      if (somThis->spindle[i][0] == 0)
      {
        to_spindle = i;
        break;
      }

    // If there is no current transaction release the lock.
    if (resource == 0)
      somThis->lockset->unlock(ev,CosConcurrencyControl_read);
  }

  return to_spindle;
}

//==============================================================================
// Method: Hanoi::move_single_disk
//
// Move a disk from one spindle to another.
//==============================================================================
SOM_Scope                 void                                 
SOMLINK move_single_disk( Hanoi*       somSelf        // Hanoi object
                        , Environment* ev             // SOM environment
                        , long         from_spindle   // source spindle
                        , long         to_spindle     // target spindle
                        )
{
  HanoiData *somThis = HanoiGetData(somSelf); 
  HanoiMethodDebug("Hanoi","move_single_disk");
  long from_index, to_index;
  
  // Write-lock the object 
  somThis->lockset->lock(ev,CosConcurrencyControl_write);
  
  if(!error(ev))
  {
    // Register a resource in the current transaction.  This returns 0 if 
    // there is no current transaction.
    HanoiResource*const resource = somSelf->register_in_transaction(ev,0);
  
    if (resource == 0)
    {
      somThis->lockset->unlock(ev,CosConcurrencyControl_write);

      StExcep_TransactionRequired* exc = (StExcep_TransactionRequired*)
                                           SOMMalloc(sizeof(StExcep_TransactionRequired));
      if (exc != 0) 
      {
        exc->minor = 0;
        exc->completed = NO;
      } 

      somSetException(ev,SYSTEM_EXCEPTION,ex_TransactionRequired,exc);
    }
    else
    {
      resource->save_before_image(ev,somThis->spindle);

      // Move a disk by locating the position of the top disk on the from spindle,  locating the
      // position just above the top disk on the to spindle, and then copying the disk from one
      // spindle to the other and deleting it from the original spindle.
      
      for (long i = HanoiConstants_DISKS - 1; i >= 0; i--)
        if (somThis->spindle[from_spindle][i] != 0)
        {
           from_index = i;
           break;
        };
      
      // if i < 0 the  source spindle is empty so raise an exception
      if (i < 0) 
        somSetException(ev,USER_EXCEPTION,ex_SPINDLE_EMPTY,0);
      else
      {
        for (long j = 0; j < HanoiConstants_DISKS; j++)
           if (somThis->spindle[to_spindle][j] == 0)
           {
              to_index = j;
              break;
           };
      
        // If there is a smaller disk on the top of the target spindle then 
        // a disk is being moved incorrectly so raise an exception
        if (to_index > 0 && somThis->spindle[to_spindle][to_index-1] <= 
                              somThis->spindle[from_spindle][from_index])
          somSetException(ev,USER_EXCEPTION,ex_DISK_ORDER,0);
        else
        {
          somThis->spindle[to_spindle][to_index] = somThis->spindle[from_spindle][from_index];
          somThis->spindle[from_spindle][from_index] = 0;
        }
      }
    }
  }
}

//==============================================================================
// Method: Hanoi::register_in_transaction
//                                                                             
// Register the Hanoi object in the specified transaction if necessary and        
// remember that this has been done.  Store the address of the transaction      
// Coordinator for future use. 
//
// If a null transaction coordinator is specified, the current transaction is 
// assumed.
//
// If there is no current transaction or if we fail to register with a current
// transaction, 0 is returned.  The caller will then behave as if there
// is no current transaction.
//==============================================================================
SOM_Scope                        HanoiResource*                        // Resource for transaction
SOMLINK register_in_transaction( Hanoi*                       somSelf  // Hanoi object
                               , Environment*                 ev       // SOM environment
                               , CosTransactions_Coordinator* coord    // Transaction coordinator
                               )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","register_in_transaction");

  HanoiResource* resource = 0;

  CosTransactions_Coordinator* coordinator = coord == 0 ? somSelf->get_transaction_coordinator(ev) : coord;

  if (coordinator != 0) 
  {
    resource = somThis->tran_inv->involve(ev,coordinator,somSelf);

    if (error(ev)) 
      somPrintf("TranInv::involve failed\n");
    else
      // Request the resource to register itself appropriately depending on whether
      // the transaction is a top-level transaction or a subtransaction.
      resource->register_resource(ev,coordinator,somSelf);
      if (error(ev)) 
        somPrintf("HanoiResource::register_resource failed\n");
  }

  return resource;
}

//============================================================================
// Method: Hanoi::get_tran_inv
//                                                                             
// Get the address of the transaction involvement object for a Hanoi instance.
//============================================================================
SOM_Scope             TranInv*         // Returned transaction involvement
SOMLINK get_tran_inv( Hanoi* somSelf   // Hanoi object
                    , Environment* ev  // SOM environment
                    )
{
    HanoiData *somThis = HanoiGetData(somSelf);
    HanoiMethodDebug("Hanoi","get_tran_inv");

    return somThis->tran_inv;
}

//==============================================================================
// Method: Hanoi::prepare
//                                                                             
// Get the changes to a Hanoi object ready to be committed or rolled back.  
// If an update has been performed, vote commit if all the disks are on one 
// spindle and vote backout otherwise.  
// Store a persistent reference to the PersistentHanoiResource input parameter
// for use during recovery.
//==============================================================================
SOM_Scope        CosTransactions_Vote              // vote from prepare
SOMLINK prepare( Hanoi*                   somSelf  // Hanoi object
               , Environment*             ev       // SOM environment
               , PersistentHanoiResource* pres     // PersistentHanoiResource instance
               )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","prepare");

  // Check to see if all the disks are on a single spindle.  
  // Note: We cannot call get_full_spindle() as this would attempt to
  //       obtain a read lock on behalf of the current thread since the
  //       current transaction is not available during prepare processing.
  //       This would deadlock since the current transaction holds an 
  //       update lock.
  long full_spindle = -1;

  for (long i = 0; i < 3; i++) 
    if (somThis->spindle[i][HanoiConstants_DISKS - 1] != 0)
    {
      full_spindle = i;
      break;
    }

  if (full_spindle >= 0)
  {
    // Store the persistent resource so that its reference can be
    // stored for use during recovery
    somThis->pres = pres;

    // Store the state persistently so it can either be backed out or 
    // committed depending on the outcome of the syncpoint.
    somSelf->store(ev); 

    return CosTransactions_VoteCommit;
  }
  else
    return CosTransactions_VoteRollback;
}

//==============================================================================
// Method: Hanoi::rollback
//                                                                             
// Roll back the changes to a Hanoi object.
//==============================================================================
SOM_Scope         void                                    // No return value
SOMLINK rollback( Hanoi*       somSelf                    // Hanoi object
                , Environment* ev                         // SOM environment
                , long         s[3][HanoiConstants_DISKS] // before-image
                )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","rollback");

  // Back out the changes using the before-image of the state at the start 
  // of the transaction 
  for (long i = 0; i < 3; i++)
    for (long j = 0; j < HanoiConstants_DISKS; j++)
      somThis->spindle[i][j] = s[i][j];

  // Store the object state persistently
  somSelf->store(ev); 
}

//==============================================================================
// Method: Hanoi::rollback_subtransaction
//                                                                             
// Roll back the changes to a Hanoi object.
//==============================================================================
SOM_Scope                        void                                    // no return value
SOMLINK rollback_subtransaction( Hanoi*       somSelf                    // Hanoi object
                               , Environment* ev                         // SOM environment
                               , long         s[3][HanoiConstants_DISKS] // before-image
                               )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","rollback_subtransaction");

  // Back out the changes using the before-image of the state at the start 
  // of the transaction 
  for (long i = 0; i < 3; i++)
    for (long j = 0; j < HanoiConstants_DISKS; j++)
      somThis->spindle[i][j] = s[i][j];
}

//============================================================================
// Method: Hanoi::delete_log
//                                                                             
// Zero the pointer to the PersistentHanoiResource representing the 
// involvement of the Hanoi instance in a top-level transaction.
// The persistent equivalent of this is the deletion of the 
// PersistentHanoiResource.  This ensures that the persistent object
// reference stored in the Hanoi instance's persistent data is invalid and
// will be converted to a zero pointer if the Hanoi instance is restored
// from its persistent data.
//============================================================================
SOM_Scope           void                 // No return value
SOMLINK delete_log( Hanoi*       somSelf // Hanoi object
                  , Environment* ev      // SOM environment 
                  )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","delete_log");
 
  somThis->pres = 0;
}

//============================================================================
// Method: Hanoi::uninvolve_in_transaction
//                                                                             
// Forget the involvement of the Hanoi instance in the specified transaction
// and drop any locks that were acquired on the Hanoi instance on behalf of
// the transaction.
//============================================================================
SOM_Scope                         void                                 // No return value
SOMLINK uninvolve_in_transaction( Hanoi*                       somSelf // Hanoi object
                                , Environment*                 ev      // SOM environment 
                                , CosTransactions_Coordinator* coord   // Transaction coordinator
                                )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","uninvolve_in_transaction");

  somThis->tran_inv->uninvolve(ev,coord);

  somSelf->drop_locks(ev,coord);
}

//==============================================================================
// Method: Hanoi::drop_locks
//                                                                             
// Drop any locks held on a Hanoi object on behalf of a particular transaction.
//==============================================================================
SOM_Scope           void                                 // No return value
SOMLINK drop_locks( Hanoi*                       somSelf // Hanoi object
                  , Environment*                 ev      // SOM environment 
                  , CosTransactions_Coordinator* coord   // Transaction coordinator
                  )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","drop_locks");

  // Use the transaction coordinator to get the lock coordinator in order 
  // to drop all the locks acquired by the transaction.
  CosConcurrencyControl_LockCoordinator*const lock_coord = somThis->lockset->get_coordinator(ev,coord);
  if (error(ev))
  {
    somPrintf("CosConcurrencyControl::LockSet::get_coordinator failed\n");
    somdExceptionFree(ev);
  }
  else 
  {
    lock_coord->drop_locks(ev);
    if (error(ev))
    {
      somPrintf("CosConcurrencyControl::LockCoordinator::drop_locks failed\n");
      somdExceptionFree(ev);
    }
  }
}

//==============================================================================
// Method: Hanoi::externalize_to_stream
//                                                                             
// Write the persistent state of a Hanoi object to a stream.
//==============================================================================
SOM_Scope                      void                          // no return value 
SOMLINK externalize_to_stream( Hanoi*              somSelf   // Hanoi object
                             , Environment*        ev        // SOM environment
                             , CosStream_StreamIO* stream    // Stream 
                             )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","externalize_to_stream");

  Hanoi_parent_CosStream_Streamable_externalize_to_stream(somSelf, ev, stream);

  stream->write_string(ev,somThis->name);
  if (error(ev)) 
  {
    somPrintf("CosStream_StreamIO::write_string failed\n");
    somdExceptionFree(ev);
  }
  else 
  {
    for (int i=0; i<3; i++) 
      for (int j=0; j<HanoiConstants_DISKS; j++) 
      {
        stream->write_long(ev, somThis->spindle[i][j]);
        if (error(ev)) 
        {
          somPrintf("CosStream_StreamIO::write_long failed\n");
          somdExceptionFree(ev);
        }
      }
  
    stream->write_object(ev,somThis->pres);
    if (error(ev)) 
    {
      somPrintf("CosStream_StreamIO::write_object failed\n");
      somdExceptionFree(ev);
    }
  } 
}

//==============================================================================
// Method: Hanoi::internalize_from_stream
//                                                                             
// Read the persistent state of a Hanoi object from a stream.
//==============================================================================
SOM_Scope                        void                                 // no return value
SOMLINK internalize_from_stream( Hanoi*                      somSelf  // Hanoi object
                               , Environment*                ev       // SOM environment
                               , CosStream_StreamIO*         stream   // Stream
                               , CosLifeCycle_FactoryFinder* ff       // Factory finder
                               )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","internalize_from_stream");

  Hanoi_parent_CosStream_Streamable_internalize_from_stream(somSelf, ev, stream, ff);

  somThis->name = stream->read_string(ev);
  if (error(ev)) 
  {
    somPrintf("CosStream_StreamIO::read_string failed\n");
    somdExceptionFree(ev);
  }
  else 
  {
    for (int i=0; i<3; i++) 
      for (int j=0; j<HanoiConstants_DISKS; j++) 
      {
        somThis->spindle[i][j] = stream->read_long(ev); 
        if (error(ev)) 
        {
          somPrintf("CosStream_StreamIO::read_long failed\n");
          somdExceptionFree(ev);
        }
      }
  
    somThis->pres = (PersistentHanoiResource*)(void*)(stream->read_object(ev,ff,0));

    // Ensure an invalid reference results in a null pointer.
    if (error(ev)) 
    {
      somPrintf("CosStream_StreamIO::read_object failed\n");
      somdExceptionFree(ev);
      somThis->pres = 0;
    }
    
    if (somThis->pres != 0) 
      somThis->pres->restore(ev,0);

    // Ensure an invalid reference results in a null pointer.
    if (error(ev))
    {
      somdExceptionFree(ev);
      somThis->pres = 0;
    }

    // Initiate recovery if there is a PersistentHanoiResource associated with
    // the Hanoi instance we are in the process of internalizing.  In this case
    // the Hanoi instance may have been updated by a transaction during prepare
    // with the possibility that the transaction did not complete.
    if (somThis->pres != 0) 
    {
      somThis->pres->recover(ev);
      if (error(ev))
      {
        somdExceptionFree(ev);
        somThis->pres = 0;
      }
    }
  } 
}

//==============================================================================
// Method: Hanoi::_set_name
//                                                                             
// Set the name of a Hanoi instance.  This is used for generating the name of
// a persistent resource related to the Hanoi instance.
//==============================================================================
SOM_Scope          void                  // No return value
SOMLINK _set_name( Hanoi*       somSelf  // Hanoi instance
                 , Environment* ev       // SOM environment
                 , string       name     // Name to be set
                 )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","_set_name");

  somThis->name = (char*)SOMMalloc(strlen(name) + 1);

  if (somThis->name != 0)
    strcpy(somThis->name,name);
}

//============================================================================
// Method: Hanoi::get_transaction_coordinator
//                                                                             
// Get the transaction coordinator of the current transaction using the 
// current object cached in the Hanoi instance.
//============================================================================
SOM_Scope                            CosTransactions_Coordinator*          // Current transaction coordinator
SOMLINK get_transaction_coordinator( Hanoi*                       somSelf  // Hanoi instance
                                   , Environment*                 ev       // SOM environment
                                   )
{
  HanoiData *somThis = HanoiGetData(somSelf);
  HanoiMethodDebug("Hanoi","get_transaction_coordinator");

  CosTransactions_Coordinator* coord = 0;

  // Get the control object, then the transaction coordinator.  
  CosTransactions_Control *const control = somThis->current->get_control(ev);
  if (error(ev))
  {
    somPrintf("CosTransactions::Current::get_control failed\n");
    somdExceptionFree(ev);
  }
  else
    if (control != 0) 
    {
      coord = control->get_coordinator(ev);
      if (error(ev))
      {
        somPrintf("CosTransactions::Control::get_coordinator failed\n");
        somdExceptionFree(ev);
        coord = 0;
      }
    }

  return coord;
}
