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
// DESCRIPTION: The TranInv class records which transactions have accessed a 
//              Hanoi instance and the corresponding resource objects which 
//              are registered with the Transaction service.
//==============================================================================

#ifndef SOM_Module_traninv_Source
#define SOM_Module_traninv_Source
#endif
#define TranInv_Class_Source

#include <somd.xh>           // DSOM interface
#include <sompposx.xh>       // POSIX PDS interface
#include <thash.xh>          // somf_THashTable interface
#include <somlock.xh>        // Concurrency Control interface

#include "hancon.xh"         // HanoiConstants module
#include "hanres.xh"         // HanoiResource interface
#include "phanres.xh"        // PersistentHanoiResource interface
#include "trancoor.xh"       // TranCoord interface
#include "traninv.xih"       // TranInv implementation interface
#include "hanoi.xh"          // Hanoi interface

static CosPersistencePID_PID* create_pid(Environment* ev,Hanoi* tower);

//==============================================================================
// Function: error
//                                                                             
// Return whether or not the specified environment indicates an error occured in
// the previous SOM call.                                                       
//==============================================================================
inline int                         // Whether or not error occured
error( const Environment*const ev  // SOM environment
     )
{ 
  return ev->_major != NO_EXCEPTION;
}

//==============================================================================
// Method: TranInv::somDefaultInit                                                       
//                                                                             
// Override the default initialisation for the TranInv class.
//==============================================================================
SOM_Scope               void                  // no return value
SOMLINK somDefaultInit( TranInv*     somSelf  // TranInv object
                      , somInitCtrl* ctrl     // 
                      )
{
  TranInvData *somThis; /* set in BeginInitializer */
  somInitCtrl globalCtrl;
  somBooleanVector myMask;
  TranInvMethodDebug("TranInv","somDefaultInit");
  TranInv_BeginInitializer_somDefaultInit;
  
  Environment e;          // SOM environment
  Environment *const ev = &e;
  
  SOM_InitEnvironment(ev);
  if (error(ev))
  {
    somPrintf("SOM_InitEnvironment failed\n");
    somdExceptionFree(ev);
  }
   
  // Create a lockset
  {
    CosConcurrencyControl_LockSetFactory* lockset_factory = 
                         new CosConcurrencyControl_LockSetFactory;
    somThis->latch = lockset_factory->create(ev);
    if (error(ev))
    {
      somPrintf("CosConcurrencyControl::LockSetFactory::create failed\n");
      somdExceptionFree(ev);
    }
    delete lockset_factory;
  }

  // Initialise the hash table to use somfIsEqual for comparing TranCoords.
  somThis->resmap = new somf_THashTable;

  somThis->resmap->somfTHashTableInitFL( ev
                                       , somf_MCollectibleClassData.somfIsEqual
                                       , 10
                                       );
  if (error(ev))
  {
    somPrintf("somf_THashTable::somfTHashTableInitFL failed\n");
    somdExceptionFree(ev);
  }

  // Cache a current object in the TranInv instance for future use
  somThis->current = new CosTransactions_Current;
  if (!somIsObj(somThis->current)) 
    somPrintf("CosTransactions::Current creation failed\n");
  
  SOM_UninitEnvironment(ev);
}

//==============================================================================
// Method: TranInv::somDestruct
//                                                                             
// Override the default uninitialisation for the TranInv class.
//==============================================================================
SOM_Scope            void                      // No return value
SOMLINK somDestruct( TranInv*         somSelf  // TranInv object
                   , octet            doFree   //
                   , somDestructCtrl* ctrl     //
                   )
{
    TranInvData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    TranInvMethodDebug("TranInv","somDestruct");
    TranInv_BeginDestructor;

    delete somThis->resmap;
    delete somThis->latch;
    delete somThis->current;

    TranInv_EndDestructor;
}

//==============================================================================
// Method: TranInv::involve
//                                                                             
// Record a transaction's involvement with a recoverable object and return
// a Hanoi resource object suitable for registration with the transaction.
//
// N.B. See the Persistence Service chapter of the SOMobjects Developer's 
//      Toolkit User's Guide for information about the persistence classes
//      used here.
//==============================================================================
SOM_Scope        HanoiResource*                        // Resource 
SOMLINK involve( TranInv*                     somSelf  // TranInv object
               , Environment*                 ev       // SOM environment
               , CosTransactions_Coordinator* coord    // Transaction coordinator
               , Hanoi*                       tower    // Hanoi instance
               )
{
  TranInvData *somThis = TranInvGetData(somSelf);
  TranInvMethodDebug("TranInv","involve");

  HanoiResource* res = 0;

  // Suspend the current transaction so locks are owned by the current thread.
  CosTransactions_Control*const control = somSelf->suspend(ev);
  if(!error(ev))
  {
    somThis->latch->lock(ev,CosConcurrencyControl_upgrade);
    if (error(ev))
    {
      somPrintf("CosConcurrencyControl::LockSet::lock failed\n");
      somdExceptionFree(ev);
    }
    else
    {
      // Register a resource with the transaction if we are not yet involved
      // in the transaction.
      if ((res = somSelf->get_resource(ev,coord)) == 0) 
      {
        const boolean top_level_tran = coord->is_top_level_transaction(ev);
        if (error(ev))
        {
          somPrintf("CosTransactions::Coordinator::is_top_level_transaction failed\n");
          somdExceptionFree(ev);
        }
        else
          if (top_level_tran == TRUE) 
          {
            { 
              PersistentHanoiResource* temp_res = (PersistentHanoiResource*)(void*)
                                                     somdCreate( ev
                                                               , "PersistentHanoiResource"
                                                               , FALSE
                                                               );
              if (error(ev))
              {
                somPrintf("somdCreate failed\n");
                somdExceptionFree(ev);
              }
              else
              {
                res = (PersistentHanoiResource*)(void*)temp_res->init_for_object_creation(ev); 
                if (error(ev))
                {
                  somPrintf("PersistentHanoiResource::init_for_object_creation failed\n");
                  somdExceptionFree(ev);
                }
                else
                {
                  ((SOMDObject*)(void*)temp_res)->release(ev);
                  if (error(ev))
                  {
                    somPrintf("PersistentHanoiResource::release failed\n");
                    somdExceptionFree(ev);
                  }
                  CosPersistencePID_PID* pid = create_pid(ev,tower);
                  ((PersistentHanoiResource*)res)->_set_p(ev,pid);
                }
              }
            }
          }
          else 
            res = new HanoiResource;
    
        somThis->latch->change_mode( ev
                                   , CosConcurrencyControl_upgrade
                                   , CosConcurrencyControl_write
                                   );
        if (error(ev))
        {
          somPrintf("CosConcurrencyControl::LockSet::change_mode failed\n");
          somdExceptionFree(ev);
          somThis->latch->unlock(ev,CosConcurrencyControl_upgrade);
        }
        else
        {
          TranCoord*const tc = new TranCoord(ev,coord);
          if (error(ev))
          {
            somPrintf("new TranCoord failed\n");
            somdExceptionFree(ev);
          }
          else
          {
            (void)(somThis->resmap->somfAddMM(ev,tc,res));
            if (error(ev))
            {
              somPrintf("somf_THashTable::somfAddMM failed\n");
              somdExceptionFree(ev);
            }
          }
      
          somThis->latch->unlock(ev,CosConcurrencyControl_write);
        }
      }
      else
        somThis->latch->unlock(ev,CosConcurrencyControl_upgrade);
  
      if (error(ev))
      {
        somPrintf("CosConcurrencyControl::LockSet::unlock failed\n");
        somdExceptionFree(ev);
      }
    }
  
    somSelf->resume(ev,control);
  }

  return res;
}

//============================================================================
// Method: TranInv::uninvolve
//                                                                             
// Delete a transaction's involvement with a recoverable object.
//============================================================================
SOM_Scope          void                                  // No return value
SOMLINK uninvolve( TranInv*                     somSelf  // TranInv instance
                 , Environment*                 ev       // SOM environment
                 , CosTransactions_Coordinator* coord    // Transaction coordinator
                 )
{
  TranInvData *somThis = TranInvGetData(somSelf); 
  TranInvMethodDebug("TranInv","uninvolve");

  TranCoord*const tc = new TranCoord(ev,coord);
  if (error(ev))
  {
    somPrintf("new TranCoord failed\n");
    somdExceptionFree(ev);
  }
  else
  {
    // Suspend the current transaction so locks are owned by the current thread.
    CosTransactions_Control*const control = somSelf->suspend(ev);
    if (!error(ev)) 
    {
      somThis->latch->lock(ev,CosConcurrencyControl_write);
      if (error(ev))
      {
        somPrintf("CosConcurrencyControl::LockSet::lock failed\n");
        somdExceptionFree(ev);
      }
      else
      {
        // Destroy the TranCoord key and remove it and its resource from the hash 
        // table (but do not destroy the resource).
        (void)(somThis->resmap->somfDelete(ev,tc));
        if (error(ev))
        {
          somPrintf("somf_THashTable::somfDelete failed\n");
          somdExceptionFree(ev);
        }
      
        somThis->latch->unlock(ev,CosConcurrencyControl_write); 
        if (error(ev))
        {
          somPrintf("CosConcurrencyControl::LockSet::unlock failed\n");
          somdExceptionFree(ev);
        }
      }
    
      somSelf->resume(ev,control);
    } 
    
    delete tc;
  }
}

//============================================================================
// Method: TranInv::get_resource
//                                                                             
// Return the address of the resource associated with a transaction.  If the 
// transaction is not known, return 0.
//============================================================================
SOM_Scope             HanoiResource*                       // Resource
SOMLINK get_resource( TranInv*                     somSelf // TranInv object
                    , Environment*                 ev      // SOM environment
                    , CosTransactions_Coordinator* coord   // Transaction coordinator
                    )
{
  TranInvData *somThis = TranInvGetData(somSelf); 
  TranInvMethodDebug("TranInv","get_resource");
  HanoiResource* result = 0;
  
  TranCoord*const tc = new TranCoord(ev,coord);
  if (error(ev))
  {
    somPrintf("new TranCoord failed\n");
    somdExceptionFree(ev);
  }
  else
  {
    // Suspend the current transaction so locks are owned by the current thread.
    CosTransactions_Control*const control = somSelf->suspend(ev);

    if(!error(ev))
    {
      somThis->latch->lock(ev,CosConcurrencyControl_read);
      if (error(ev))
      {
        somPrintf("CosConcurrencyControl::LockSet::lock failed\n");
        somdExceptionFree(ev);
      }
      else
      {
        result = (HanoiResource*)(somThis->resmap->somfRetrieve(ev,tc));
        if (error(ev))
        {
          somPrintf("somf_THashTable::somfRetrieve failed\n");
          somdExceptionFree(ev);
          result = 0;
        }
        else
          if (result == (HanoiResource*)SOMF_NIL) 
            result = 0;
        
        somThis->latch->unlock(ev,CosConcurrencyControl_read);  
        if (error(ev))
        {
          somPrintf("CosConcurrencyControl::LockSet::unlock failed\n");
          somdExceptionFree(ev);
        }
      }
      
      somSelf->resume(ev,control);
    }
  
    delete tc;
  }
  
  return result;
}

//============================================================================
// Method: suspend
//                                                                             
// Suspend any current transaction's involvement with the current thread.
//============================================================================
SOM_Scope        CosTransactions_Control*           // Suspend transaction
SOMLINK suspend( TranInv*                  somSelf  // TranInv instance
               , Environment*              ev       // SOM environment
               )
{
  TranInvData *somThis = TranInvGetData(somSelf);
  TranInvMethodDebug("TranInv","suspend");

  CosTransactions_Control* control = somThis->current->suspend(ev);
  if (error(ev))
  {
    somPrintf("CosTransactions::Current::suspend failed\n");
    control = 0;
  }

  return control;
}

//============================================================================
// Method: resume
//                                                                             
// Resume any transaction's involvement with the current thread.
//============================================================================
SOM_Scope       void                              // No return value
SOMLINK resume( TranInv*     somSelf              // TranInv instance
              , Environment* ev                   // SOM environment
              , CosTransactions_Control* control  // Suspended transaction
              )
{
  TranInvData *somThis = TranInvGetData(somSelf);
  TranInvMethodDebug("TranInv","resume");

  if (control != 0)
  {
    somThis->current->resume(ev,control);
    if (error(ev))
    {
      somPrintf("CosTransactions::Current::resume failed\n");
      somdExceptionFree(ev);
    }
  }
}

//==============================================================================
// Function: create_pid
//
// Create a persistent id. for the resource associated with the top-level
// transaction that is updating a Hanoi object.  Since there is at most one
// resource of this kind for each Hanoi object, the persistent id. name can be
// based on the name of the Hanoi object.  The POSIX PDS is used to avoid 
// unnecessary dependencies.
//
// N.B. See the Persistence Service chapter of the SOMobjects Developer's 
//      Toolkit User's Guide for information about the persistence class
//      used here.
//==============================================================================
static      CosPersistencePID_PID*  // Created PID 
create_pid( Environment* ev         // SOM environment
          , Hanoi*       tower      // Hanoi object
          )
{
  somPersistencePOSIX_PID_POSIX* res_pid = (somPersistencePOSIX_PID_POSIX*)(void*)
                                             somdCreate( ev 
                                                       , "somPersistencePOSIX::PID_POSIX"
                                                       , TRUE
                                                       );
  if (error(ev))
  {
    somPrintf("somdCreate failed\n");
    somdExceptionFree(ev);
  }
  else
  {
    string name = tower->_get_name(ev);
    if (error(ev))
    {
      somPrintf("Hanoi::get_name failed\n");
      somdExceptionFree(ev);
    }
    else
    {
      string res_path = (string)SOMMalloc(strlen(name) + 5);
      strcpy(res_path,name);
      strcat(res_path,".res");
    
      res_pid->_set_pathName(ev,res_path);
      if (error(ev))
      {
        somPrintf("somPersistencePOSIX::PID_POSIX::_set_pathName failed\n");
        somdExceptionFree(ev);
      }
      SOMFree(res_path);
    }
  }

  return res_pid;
}
