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
// DESCRIPTION: DLL initialisation function for the Hanoi sample program.
//==============================================================================

#include <somtran.xh>        // Transaction Service interface 

#include "hancon.xh"         // HanoiConstants module
#include "hanres.xh"         // HanoiResource interface
#include "phanres.xh"        // PersistentHanoiResource interface
#include "trancoor.xh"       // TranCoord interface
#include "traninv.xh"        // TranInv interface
#include "hanoi.xh"          // Hanoi interface

//==============================================================================
// Function: SOMInitModule
//                                                                             
// Creates the classes during class library initialisation.
//==============================================================================
#ifdef __IBMC__
  #pragma linkage (SOMInitModule, system)
#endif

SOMEXTERN              void                 // no return value
SOMLINK SOMInitModule( long   majorVersion  // major class version number
                     , long   minorVersion  // minor class version number
                     , string className     // name of class
                     )
{
  SOM_IgnoreWarning(majorVersion);  // ignore parameter
  SOM_IgnoreWarning(minorVersion);  // ignore parameter
  SOM_IgnoreWarning(className);     // ignore parameter
  
  HanoiNewClass(Hanoi_MajorVersion, Hanoi_MinorVersion);
  TranInvNewClass(TranInv_MajorVersion, TranInv_MinorVersion);
  HanoiResourceNewClass(HanoiResource_MajorVersion, HanoiResource_MinorVersion);
  PersistentHanoiResourceNewClass(PersistentHanoiResource_MajorVersion, PersistentHanoiResource_MinorVersion);
  TranCoordNewClass(TranCoord_MajorVersion, TranCoord_MinorVersion);
}
