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
// DESCRIPTION: The HanoiResource class is used for registering the involvement
//              of a Hanoi instance in a subtransaction and for handling
//              subtransaction termination.  It may also store a before-image of
//              a Hanoi instance.
//==============================================================================

#ifndef SOM_Module_hanres_Source
#define SOM_Module_hanres_Source
#endif
#define HanoiResource_Class_Source

#include <somtran.xh>        // Transaction Service interface
#include <stexcep.xh>        // Standard exceptions
#include <somd.xh>           // DSOM interface

#include "hancon.xh"         // HanoiConstants module
#include "hanres.xih"        // HanoiResource implementation interface
#include "phanres.xh"        // PersistentHanoiResource interface
#include "traninv.xh"        // TranInv interface 
#include "hanoi.xh"          // Hanoi interface

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
// Method: HanoiResource::somDefaultInit
//==============================================================================
SOM_Scope               void                    // No return value
SOMLINK somDefaultInit( HanoiResource* somSelf  // HanoiResource object
                      , somInitCtrl*   ctrl     // 
                      )
{
  HanoiResourceData *somThis; /* set in BeginInitializer */
  somInitCtrl globalCtrl;
  somBooleanVector myMask;
  HanoiResourceMethodDebug("HanoiResource","somDefaultInit");
  HanoiResource_BeginInitializer_somDefaultInit;
  
  HanoiResource_Init_CosTransactions_SubtransactionAwareResource_somDefaultInit(somSelf,ctrl);
  HanoiResource_Init_somf_MCollectible_somDefaultInit(somSelf,ctrl);
  HanoiResource_Init_CosStream_Streamable_somDefaultInit(somSelf,ctrl);
  
  somThis->registered = FALSE;
  somThis->tower = 0;
  somThis->coord = 0;
  somThis->before_image_exists = FALSE;
}

//==============================================================================
// Method: HanoiResource::register_resource
//                                                                             
// Register with the specified transaction and note the Hanoi instance that
// the resource is related to unless we have already registered in which case
// no action is required.
//
// N.B. This method is only ever called to register with a subtransaction.
//==============================================================================
SOM_Scope                  void                                  // No return value
SOMLINK register_resource( HanoiResource*               somSelf  // HanoiResource object
                         , Environment*                 ev       // SOM environment
                         , CosTransactions_Coordinator* coord    // Subtransaction coordinator
                         , Hanoi*                       tower    // Hanoi object
                         )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf);
  HanoiResourceMethodDebug("HanoiResource","register_resource");

  if (somThis->registered == FALSE)
  {
    somThis->coord = coord;
    somThis->tower = tower;
    coord->register_subtran_aware(ev,somSelf);
    if (error(ev)) 
    {
      somPrintf("CosTransactions::Coordinator::register_subtran_aware failed\n");
      somdExceptionFree(ev);
    }
    else
      somThis->registered = TRUE;
  }
}

//==============================================================================
// Method: HanoiResource::save_before_image
//                                                                             
// Store a before-image.
//==============================================================================
SOM_Scope                  void                                      // No return value
SOMLINK save_before_image( HanoiResource* somSelf                    // HanoiResource object
                         , Environment*   ev                         // SOM environment
                         , long           s[3][HanoiConstants_DISKS] // before-image
                         )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf);
  HanoiResourceMethodDebug("HanoiResource","save_before_image");

  if (somThis->before_image_exists == FALSE) 
  {
    for (long i = 0; i < 3; i++)
      for (long j = 0; j < HanoiConstants_DISKS; j++)
        somThis->spindle_before_image[i][j] = s[i][j];
  
    somThis->before_image_exists = TRUE;
  }
}

//==============================================================================
// Method: HanoiResource::apply_before_image
//                                                                             
// Roll back the changes to a Hanoi object.
//==============================================================================
SOM_Scope                   void                    // No return value
SOMLINK apply_before_image( HanoiResource* somSelf  // HanoiResource object
                          , Environment*   ev       // SOM environment
                          )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf);
  HanoiResourceMethodDebug("HanoiResource","apply_before_image");

  if (somThis->before_image_exists == TRUE)
    somThis->tower->rollback(ev,somThis->spindle_before_image);
}

//==============================================================================
// Method: HanoiResource::self_destruct
//                                                                             
// Uninitialise and free the HanoiResource instance.  Although it is unusual
// for a method to free the instance it was called with, this is the only 
// simple way to clean up resource objects.  Part of uninitialising the 
// HanoiResource is to call the Hanoi instance to forget the involvement in 
// the current transaction and to drop the locks that the transaction 
// acquired.
//==============================================================================
SOM_Scope              void                    // No return value
SOMLINK self_destruct( HanoiResource* somSelf  // HanoiResource object
                     , Environment*   ev       // SOM environment
                     )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf);
  HanoiResourceMethodDebug("HanoiResource","self_destruct");

  somThis->tower->uninvolve_in_transaction(ev,somThis->coord);

  delete somSelf;
}

//==============================================================================
// Method: HanoiResource::commit_subtransaction
//
// Promote any before-image to the parent and then forget our involvement 
// with the transaction.
//==============================================================================
SOM_Scope void                                  // No return value
SOMLINK commit_subtransaction
        ( HanoiResource*               somSelf  // HanoiResource object 
        , Environment*                 ev       // SOM environment
        , CosTransactions_Coordinator* parent   // Coordinator of parent transaction
        )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf); 
  HanoiResourceMethodDebug("HanoiResource","commit_subtransaction");

  HanoiResource*const parent_res = somThis->tower->register_in_transaction(ev,parent);

  // If this is an update, ensure a before-image has been stored.  This
  // will no-op if the parent resource already has a before-image.
  if (somThis->before_image_exists == TRUE) 
    parent_res->save_before_image(ev,somThis->spindle_before_image);

  somSelf->self_destruct(ev);
}

//==============================================================================
// Method: HanoiResource::rollback_subtransaction
//
// Restore the state to that in any before-image and then forget our
// involvement in the transaction and drop any locks acquired.
//==============================================================================
SOM_Scope                        void                    // No return value
SOMLINK rollback_subtransaction( HanoiResource* somSelf  // HanoiResource object 
                               , Environment*   ev       // SOM environment
                               )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf); 
  HanoiResourceMethodDebug("HanoiResource","rollback_subtransaction");

  if (somThis->before_image_exists == TRUE)
  {
    // Back out the changes using the before-image of the state at the start 
    // of the transaction 
    somThis->tower->rollback_subtransaction(ev,somThis->spindle_before_image);
  }
  
  somSelf->self_destruct(ev);
}

//==============================================================================
// Method: HanoiResource::externalize_to_stream
//                                                                             
// Write the persistent state of a HanoiResource object to a stream.
//==============================================================================
SOM_Scope                      void                         // No return value 
SOMLINK externalize_to_stream( HanoiResource*      somSelf  // HanoiResource object
                             , Environment*        ev       // SOM environment
                             , CosStream_StreamIO* stream   // Stream 
                             )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf);
  HanoiResourceMethodDebug("HanoiResource","externalize_to_stream");

  HanoiResource_parent_CosStream_Streamable_externalize_to_stream(somSelf,ev,stream);

  stream->write_boolean(ev,somThis->before_image_exists);

  if (somThis->before_image_exists == TRUE)
    for (int i=0; i<3; i++) 
      for (int j=0; j<HanoiConstants_DISKS; j++) 
        stream->write_long(ev,somThis->spindle_before_image[i][j]);
}

//==============================================================================
// Method: HanoiResource::internalize_from_stream
//                                                                             
// Read the persistent state of a HanoiResource object from a stream.
//==============================================================================
SOM_Scope                        void                                 // No return value
SOMLINK internalize_from_stream( HanoiResource*              somSelf  // HanoiResource object
                               , Environment*                ev       // SOM environment
                               , CosStream_StreamIO*         stream   // Stream
                               , CosLifeCycle_FactoryFinder* ff       // Factory finder
                               )
{
  HanoiResourceData *somThis = HanoiResourceGetData(somSelf);
  HanoiResourceMethodDebug("HanoiResource","internalize_from_stream");

  HanoiResource_parent_CosStream_Streamable_internalize_from_stream(somSelf,ev,stream,ff);

  if (TRUE == (somThis->before_image_exists = stream->read_boolean(ev)))
    for (int i=0; i<3; i++) 
      for (int j=0; j<HanoiConstants_DISKS; j++) 
        somThis->spindle_before_image[i][j] = stream->read_long(ev); 

  somThis->registered = TRUE;
  somThis->tower = 0;
  somThis->coord = 0;
}
