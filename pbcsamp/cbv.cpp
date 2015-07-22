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
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *     SOM incremental update: 2.45
 */

#ifndef SOM_Module_cbv_Source
#define SOM_Module_cbv_Source
#endif
#define CBVClass_Class_Source

#include "cbv.xih"              // CBVClass include file
#include "samp.xh"            // SampleClass include file
#include <stdio.h>              // print function
#include "cont.xh"             // ContainedClass include file

const long MAX_STRING_SIZE = 1024;

SOM_Scope void  SOMLINK SetCBVmember(CBVClass *somSelf,  Environment *ev,
                                     SampleClass* cbv_object)
{
    CBVClassData *somThis = CBVClassGetData(somSelf);
    CBVClassMethodDebug("CBVClass","SetCBVmember");

    // since parameter object is owned by server, a simple assignment will do
    somThis->cbv_member = cbv_object;

    //  do not SOMFree any name;
    //  The object_owns_parameters implies memory is taken care of by called object !!!
    //  actually do not do ANYTHING to memory that is of type object_owns_parameters
    //       after it has been passed to the method call

}


SOM_Scope void  SOMLINK SetCBVmemberNoPBV(CBVClass *somSelf,  Environment *ev,
                                     SampleClass* cbvNoPBV_object)
{
    char buf[MAX_STRING_SIZE];
    char *name1;
    ContainedClass* tempcc;

    CBVClassData *somThis = CBVClassGetData(somSelf);
    CBVClassMethodDebug("CBVClass","SetCBVmemberNoPBV");

    // in this case, a remote object was passed as a parameter so we can
    // just assign cbv_member to the object passed to us
    somThis->cbv_member = cbvNoPBV_object;

 }

SOM_Scope void  SOMLINK SetCBVmemberNoOwn(CBVClass *somSelf,  Environment *ev,
                                     SampleClass* cbvNoOwn_object)
{
    char buf[MAX_STRING_SIZE];
    char * name1;
    char * name2;
    ContainedClass* tempcc;

    CBVClassData *somThis = CBVClassGetData(somSelf);
    CBVClassMethodDebug("CBVClass","SetCBVmemberNoOwn");

    // since server does not own the parameter, we have to create the object
    // and then copy the name values
    somThis->cbv_member = new SampleClass;
    name1 = (string) SOMMalloc(strlen(cbvNoOwn_object->_get_name(ev)));
    strcpy(name1,cbvNoOwn_object->_get_name(ev));
    (somThis->cbv_member)->_set_name(ev, name1);
    tempcc = new ContainedClass;
    name2 = (string) SOMMalloc(strlen(cbvNoOwn_object->_get_cc(ev)->_get_name(ev)));
    strcpy(name2,cbvNoOwn_object->_get_cc(ev)->_get_name(ev));
    somThis->cbv_member->_set_cc(ev,tempcc);
    (somThis->cbv_member->_get_cc(ev))->_set_name(ev, name2);

}


SOM_Scope SampleClass*  SOMLINK GetCBVmember(CBVClass *somSelf,
                                              Environment *ev)
{
    CBVClassData *somThis = CBVClassGetData(somSelf);
    CBVClassMethodDebug("CBVClass","GetCBVmember");

    return somThis->cbv_member;
}

SOM_Scope SampleClass*  SOMLINK GetCBVmemberNoPBV(CBVClass *somSelf,
                                              Environment *ev)
{
    CBVClassData *somThis = CBVClassGetData(somSelf);
    CBVClassMethodDebug("CBVClass","GetCBVmember");

    return somThis->cbv_member;
}



/*
 * Default object initializer
 */
SOM_Scope void SOMLINK somDefaultInit(CBVClass *somSelf, som3InitCtrl* ctrl)
{
    CBVClassData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    CBVClassMethodDebug("CBVClass","somDefaultInit");
    CBVClass_BeginInitializer_somDefaultInit;

    CBVClass_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /*
     * local CBVClass initialization code added by programmer
     */
    somThis->cbv_member = (SampleClass*)NULL;
 }

/*
 * Default object uninitializer
 */
SOM_Scope void SOMLINK somDestruct(CBVClass *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    CBVClassData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    CBVClassMethodDebug("CBVClass","somDestruct");
    CBVClass_BeginDestructor;

    /*
     * local CBVClass deinitialization code added by programmer
     */

    delete somThis->cbv_member;

    CBVClass_EndDestructor;
}


