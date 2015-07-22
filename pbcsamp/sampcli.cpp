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
 */

/*
 * Module name: main.cpp
 *
 * Description: This is a sample program that demonstrates how DSOM
 *                 pass by value works. Please reference the SOM User's
 *                 Guide, Passing Parameters By Copying section and
 *                 Passing Object by Copying section.
 *
 *                 The CBVClass is used in this sample program. It
 *                 contains an instance variable, cbvmember, which is a
 *                 SampleClass object. The SampleClass contains an
 *                 attribute which is a ContainedClass object.
 *
 *                 The CBVClass has methods to set/get the instance
 *                 variable, cbvmember, in many different ways. These
 *                 methods are:
 *                 SetCBVmember         - pass_by_copy_parameters
 *                                                object_owns_parameters
 *                 SetCBVmemberNoPBV - normal remote method without
 *                                                 pass by value parameters
 *                 SetCBVmemberNoOwn - pass_by_copy_parameters
 *                                                 but not owned by server
 *                 GetCBVmember         - pass_by_copy_result
 *                 GetCBVmemberNoPBV - normal remote method without
 *                                                 pass by copy
 *
 *                 The first section of the program will call
 *                 SetCBVmember to set the cbvmember and then call
 *                 GetCBVmember to get the object back.
 *
 *                 The second section of the program will call
 *                 SetCBVmemberNoPBV to set the cbvmember and then
 *                 call GetCBVmemberNoPBV to get the object back. This
 *                 is the normal remote method call which does not use
 *                 the pass by value parameters. The purpose of the
 *                 section is for user to compare the differences between
 *                 this and using the pass by value parameters.
 *
 *                 The third section of the program will call
 *                 SetCBVmemberNoOwn to set the cbvmember and then
 *                 call GetCBVmember to get the object back. Since the
 *                 server does not owned the parameter, the
 *                 implementation code for SetCBVmemberNoOwn is a
 *                 little different then the other set methods.
 *
 */

#include <somd.xh>                // DSOM include file
#include "samp.xh"                // SampleClass include file
#include "cbv.xh"                  // CBVClass include file
#include "cont.xh"                 // ContainedClass include file
#include <stdio.h>                  // print function
#include <iostream.h>             // cout

SampleClass * getLocalSampleClassObj(Environment *ev,char*, char*);
void printNames(Environment *ev, SampleClass*);
int checkEV(Environment *ev);
void clean_up(CBVClass * cbvp, SampleClass * sampp1, SampleClass * sampp2);

int main (int argc, char *argv[])
{
    SampleClass *snd_obj = NULL;
    SampleClass *localNoOwn_obj = NULL;
    SampleClass *tempsamp = NULL;

    ContainedClass *cc_obj = NULL;
    ContainedClass *cc_objNoOwn = NULL;
    ContainedClass *tempcc;

    SampleClass *rcv_obj = NULL;
    SampleClass *rcv_objNoPBV = NULL;
    SampleClass *rcv_obj2 = NULL;

    CBVClass *cbv = NULL;
    CBVClass *cbvNoPBV = NULL;
    CBVClass *cbvNoOwn = NULL;

    ExtendedNaming_ExtendedNamingContext *enc;
    SOMObject *sampfac;
    SOMObject *cbvfac;
    SOMObject *ccfac;

    Environment *ev;

    int status = 0;


    //--------------------------------------------------------------
    // initialization
    //--------------------------------------------------------------
    ev = SOM_CreateLocalEnvironment();
    SOMD_Init(ev);

    //--------------------------------------------------------------
    // find ENC
    //--------------------------------------------------------------
    enc = (ExtendedNaming_ExtendedNamingContext *)
          SOMD_ORBObject->resolve_initial_references(ev, "FactoryService");

    //--------------------------------------------------------------
    // find factory
    //--------------------------------------------------------------
    cbvfac = enc->find_any(ev, "class == 'CBVClass' and alias == 'sampServer'", 0);
    sampfac = enc->find_any(ev, "class == 'SampleClass' and alias == 'sampServer'", 0);
    ccfac = enc->find_any(ev, "class == 'ContainedClass' and alias == 'sampServer'", 0);
    if (checkEV(ev)) return(-1);

    //************************************************************
    // PBV and Owned by server
    //************************************************************

    //------------------------------------------------------------
    // Create a remote CBVClass object
    //------------------------------------------------------------
    cbv = (CBVClass *)(void *)((SOMClass *)(void *)cbvfac)->somNew();

    //------------------------------------------------------------
    // create local objects to use as the pass by value parameter
    //------------------------------------------------------------
    snd_obj = getLocalSampleClassObj(ev,"samp name","cc name");

    //------------------------------------------------------------
    // Call SetCBVmember which has pass by copy parameter and the
    // parameter will be owned by the server
    //------------------------------------------------------------
    cbv->SetCBVmember(ev, snd_obj);
    if (checkEV(ev)) {
        clean_up(cbv, rcv_obj, NULL);
        return(-1);
    }

    //-------------------------------------------------------
    // Call GetCBVmember which will return a local obj
    // - pass by value result
    //-------------------------------------------------------
    rcv_obj = cbv->GetCBVmember(ev);
    if (checkEV(ev)) {
        clean_up(cbv, rcv_obj, NULL);
        return(-1);
    }
    else {
        if (rcv_obj) printNames(ev,rcv_obj);
        clean_up(cbv, rcv_obj, NULL);
     }

     //************************************************************
     // Not pass by value
     //************************************************************

     //------------------------------------------------------------
     // Create a remote CBVClass object
     //------------------------------------------------------------
     cbvNoPBV = (CBVClass *)(void *)((SOMClass *)(void *)cbvfac)->somNew();

     //------------------------------------------------------------
     // Create a remote SampleClass object
     //------------------------------------------------------------
     tempsamp =
            (SampleClass *)(void *)((SOMClass *)(void *)sampfac)->somNew();

     tempsamp->_set_name(ev,"no pbv samp name ");

     //------------------------------------------------------------
     // Create a remote ContainedClass object
     //------------------------------------------------------------
     tempcc =
            (ContainedClass *)(void *)((SOMClass *)(void *)ccfac)->somNew();
     tempsamp->_set_cc(ev,tempcc);
     tempsamp->_get_cc(ev)->_set_name(ev, "no pbv cc name ");

     //------------------------------------------------------------
     // Call SetCBVmemberNoPBV which pass an object reference instead
     //------------------------------------------------------------
     cbvNoPBV->SetCBVmemberNoPBV(ev, tempsamp);
     if (checkEV(ev)) {
        clean_up(cbvNoPBV, NULL, NULL);
        return(-1);
    }

     //------------------------------------------------------------
     // Call GetCBVmember which will return a local result
     // - pass by value result
     //------------------------------------------------------------
     rcv_objNoPBV = cbvNoPBV->GetCBVmemberNoPBV(ev);
     if (checkEV(ev)) {
         clean_up(cbvNoPBV, rcv_objNoPBV, NULL);
         return(-1);
     }
    else {
        if (rcv_objNoPBV) printNames(ev,rcv_objNoPBV);
        clean_up(cbvNoPBV, rcv_objNoPBV, NULL);
     }

     //*************************************************************
     // PBV but Not Owned by server
     //*************************************************************
     //-------------------------------------------------------------
     // Create a remote CBVClass object
     //-------------------------------------------------------------
     cbvNoOwn = (CBVClass *)(void *)((SOMClass *)(void *)cbvfac)->somNew();

     //-------------------------------------------------------------
     // since snd_obj already belong to the server, we have to create
     // another local obj
     // create local objects
     //-------------------------------------------------------------
     localNoOwn_obj = getLocalSampleClassObj(ev, "no own samp name ",
                                                                 "No own cc name");

     //-------------------------------------------------------------
     // Call Set CBVmemberNoOwn which has pass by copy parameter but
     // parameter will not be owned by the server
     //-------------------------------------------------------------
     cbvNoOwn->SetCBVmemberNoOwn(ev, localNoOwn_obj);
     if (checkEV(ev)) {
         clean_up(cbvNoOwn, localNoOwn_obj, NULL);
         return(-1);
     }

     //------------------------------------------------------------
     // Call GetCBVmember which will return a local result
     //------------------------------------------------------------
     rcv_obj2 = cbvNoOwn->GetCBVmember(ev);
     if (checkEV(ev)) {
         clean_up(cbvNoOwn, localNoOwn_obj, NULL);
         return(-1);
     }
    else {
        if (rcv_obj2) printNames(ev,rcv_obj2);
        clean_up(cbvNoOwn, localNoOwn_obj, rcv_obj2);
     }

    SOM_DestroyLocalEnvironment(ev);
    return (status);
}


//****************************************************************
// getLocalSampleClassObj will create a local SampleClass object and
// set name attribute to sampname.
// Then create a ContainedClass object and point to it from the
// SampleClass object.
// Set name attribute for the ContainedClass object to ccname.
//****************************************************************
SampleClass* getLocalSampleClassObj(Environment *ev,char* sampname,
                                                   char* ccname)
{

    //create local objects
    SampleClass * temp_samp = new SampleClass;
    ContainedClass * temp_cc  = new ContainedClass;

    //call method on local object (cc_obj) to set name
    temp_cc->_set_name( ev, ccname );

    //set reference of snd_obj to point to cc_obj
    temp_samp->_set_cc( ev, temp_cc );

    temp_samp->_set_name(ev, sampname);
    return temp_samp;
}


//*****************************************************************
// print out the value of the name attribute for SampleClass
// and the ContainedClass
//*****************************************************************
void printNames(Environment *ev, SampleClass* sampobj)
{
    somPrintf("%s\n", sampobj->_get_name(ev));
    somPrintf("%s\n", sampobj->_get_cc( ev )->_get_name( ev ));
}


//******************************************************************
// For use after calling a SOM routine that takes an ev parameter
// After the SOM call, do: checkEV( ev );
// ev defined as Environment*
// This routine halts the program if a System or User Exception occurs
//******************************************************************
void clean_up(CBVClass * cbvp, SampleClass * sampp1, SampleClass * sampp2) {
     if (sampp1 != NULL) delete sampp1;
     if (sampp2 != NULL) delete sampp2;
     if (cbvp != NULL) SOMFree(cbvp);
}


//******************************************************************
// For use after calling a SOM routine that takes an ev parameter
// After the SOM call, do: checkEV( ev );
// ev defined as Environment*
// This routine halts the program if a System or User Exception occurs
//******************************************************************
int checkEV( Environment* ev ) {
   int ret=0;
   switch (ev->_major) {
      case SYSTEM_EXCEPTION:
          cout << "System Exception" << endl;
          cout << "Exception ID: " << somExceptionId( ev ) << endl;
          cout << "Minor Code: " << ((StExcep*)somExceptionValue( ev ))->minor << endl;
          cout << "Status: " << ((StExcep*)somExceptionValue( ev ))->completed << endl;
          somdExceptionFree( ev );
          ret = 1;
          break;
      case USER_EXCEPTION:
          cout << "User Exception" << endl;
          cout << "Exception ID: " << somExceptionId( ev ) << endl;
          somdExceptionFree( ev );
          ret = 1;
          break;
      case NO_EXCEPTION:
          break;
  }
  return ret;
}

