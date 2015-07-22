
/*
 *
 *
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
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



#ifndef SOM_Module_MemoryManagement_Source
#define SOM_Module_MemoryManagement_Source
#endif
#define MemoryManagement_Class_Source

#include "memman.xih"
#include <somtc.xh>

#include <stdio.h>



SOM_Scope long  SOMLINK longMethod(MemoryManagement *somSelf,  Environment *ev,
                                   long inParameter, long* outParameter,
                                   long* inOutParameter)
{
    /* MemoryManagementData *somThis = MemoryManagementGetData(somSelf); */
    MemoryManagementMethodDebug("MemoryManagement","longMethod");
#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
  /* There is no special processing required for scalars if object owns
     or dual owned */
#endif

    return (*inOutParameter = *outParameter = inParameter + *inOutParameter);
}



SOM_Scope string  SOMLINK stringMethod(MemoryManagement *somSelf,  Environment *ev,
                                        string inParameter, string* outParameter,
                                        string* inOutParameter)
{
    MemoryManagementData *somThis = MemoryManagementGetData(somSelf);
    MemoryManagementMethodDebug("MemoryManagement","stringMethod");

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
   /* Check to see if any of the parameters are currently allocated, in some
      instances, the server code may want to reuse the storage, but since this is a
      sample, the memeory will simply be freed and realloced */
      if (somThis->saveReturnString != NULL) SOMFree(somThis->saveReturnString);
      if (somThis->saveInString != NULL) SOMFree(somThis->saveInString);
      if (somThis->saveOutString != NULL) SOMFree(somThis->saveOutString);
      if (somThis->saveInOutString != NULL) SOMFree(somThis->saveInOutString);
#endif

    string retVal = (char *)SOMMalloc(strlen(inParameter) +
                              strlen(*inOutParameter) +
                              1);


    strcpy(retVal, inParameter);
    strcat(retVal, *inOutParameter);

/***************************************************************************
 * NOTE: When an unbounded string is passed as the value in an inout method parameter
 * the returned value is constrained to be no longer than the input value.
 **************************************************************************/
    strupr(*inOutParameter);

    string tempString = (char *)SOMMalloc(strlen(inParameter) + 1);

    strcpy(tempString, inParameter);
    strrev(tempString); // Reverse the string
    *outParameter = tempString;

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
   /* Save pointers to the storage so that they can be freed or reused later */
   somThis->saveReturnString = retVal;
   somThis->saveInString = inParameter;
   somThis->saveOutString = *outParameter;
   somThis->saveInOutString = *inOutParameter;
#endif

    return(retVal);

}


SOM_Scope _IDL_SEQUENCE_long*  SOMLINK sequenceMethod(MemoryManagement *somSelf,
                                                       Environment *ev,
                                                      _IDL_SEQUENCE_long* inParameter,
                                                      _IDL_SEQUENCE_long** outParameter,
                                                      _IDL_SEQUENCE_long** inOutParameter)
{
    MemoryManagementData *somThis = MemoryManagementGetData(somSelf);
    MemoryManagementMethodDebug("MemoryManagement","sequenceMethod");

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
 /* Check to see if any of the parameters are currently allocated, in some
    instances, the server code may want to reuse the storage, but since this is a
    sample, the memeory will simply be freed and realloced */
    if (somThis->saveReturnSequenceLong != NULL) {
       SOMFree(somThis->saveReturnSequenceLong->_buffer);
       SOMFree(somThis->saveReturnSequenceLong);
       }
    if (somThis->saveInSequenceLong != NULL) {
       SOMFree(somThis->saveInSequenceLong->_buffer);
       SOMFree(somThis->saveInSequenceLong);
       }
    if (somThis->saveOutSequenceLong != NULL) {
       SOMFree(somThis->saveOutSequenceLong->_buffer);
       SOMFree(somThis->saveOutSequenceLong);
       }
    if (somThis->saveInOutSequenceLong != NULL) {
       SOMFree(somThis->saveInOutSequenceLong->_buffer);
       SOMFree(somThis->saveInOutSequenceLong);
       }
#endif

    _IDL_SEQUENCE_long * retVal = (_IDL_SEQUENCE_long *)SOMMalloc(sizeof(_IDL_SEQUENCE_long));
    retVal->_maximum = 10;
    retVal->_buffer = (long *)SOMMalloc(retVal->_maximum * sizeof(long));
    retVal->_length = 0;
    for (int i=0; i<inParameter->_length; ++i, retVal->_length ++)
       retVal->_buffer[i] = inParameter->_buffer[i] + (*inOutParameter)->_buffer[i];

    *outParameter = (_IDL_SEQUENCE_long *)SOMMalloc(sizeof(_IDL_SEQUENCE_long));
    (*outParameter)->_maximum = 10;
    (*outParameter)->_buffer = (long *)SOMMalloc((*outParameter)->_maximum * sizeof(long));
    (*outParameter)->_length = 0;
    for (i=0; i<inParameter->_length; ++i, (*outParameter)->_length ++) {
       (*outParameter)->_buffer[i] = inParameter->_buffer[i];
       (*outParameter)->_buffer[i] *= 2;
       }

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
    /* Save pointers to the storage so that they can be freed or reused later */
    somThis->saveReturnSequenceLong = retVal;
    somThis->saveInSequenceLong = inParameter;
    somThis->saveOutSequenceLong = *outParameter;
    somThis->saveInOutSequenceLong = *inOutParameter;
#endif

    return (retVal);

}


SOM_Scope SupportObject*  SOMLINK objectMethod(MemoryManagement *somSelf,
                                                Environment *ev,
                                               SupportObject* inParameter,
                                               SupportObject** outParameter,
                                               SupportObject** inOutParameter)
{

    MemoryManagementData *somThis = MemoryManagementGetData(somSelf);
    MemoryManagementMethodDebug("MemoryManagement","objectMethod");

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
 /* Check to see if any of the parameters are currently allocated, in some
    instances, the server code may want to reuse the storage, but since this is a
    sample, the memeory will simply be freed and realloced */
    if (somThis->saveReturnSupportObject != NULL) SOMFree(somThis->saveReturnSupportObject);
    if (somThis->saveInSupportObject != NULL) SOMFree(somThis->saveInSupportObject);
    if (somThis->saveOutSupportObject != NULL) SOMFree(somThis->saveOutSupportObject);
    if (somThis->saveInOutSupportObject != NULL) SOMFree(somThis->saveInOutSupportObject);
#endif


    *outParameter = new SupportObject;


    (*outParameter)->setValues(ev, inParameter->_get_a(ev), inParameter->_get_b(ev), inParameter->_get_c(ev));

    (*inOutParameter)->setValues(ev, 2*(*inOutParameter)->_get_a(ev),
                                 strupr((*inOutParameter)->_get_b(ev)),
                                 (*inOutParameter)->_get_c(ev) + ' ');


    /* Return statement to be customized: */
    SupportObject* retVal = NULL;
    retVal = new SupportObject;
    retVal->setValues(ev, 3, "Returned value", 'p');

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
    /* Save pointers to the storage so that they can be freed or reused later */
    somThis->saveReturnSupportObject = retVal;
    somThis->saveInSupportObject = inParameter;
    somThis->saveOutSupportObject = *outParameter;
    somThis->saveInOutSupportObject = *inOutParameter;
#endif

    return (retVal);
}



SOM_Scope void  SOMLINK octetMethod(MemoryManagement *somSelf,  Environment *ev,
                                     octet inParameter[100],
                                     octet outParameter[100],
                                     octet inOutParameter[100])
{
    /* MemoryManagementData *somThis = MemoryManagementGetData(somSelf); */
    MemoryManagementMethodDebug("MemoryManagement","memoryMethod");

    strupr((string)inOutParameter);

    strcpy((string)outParameter, "This is the outParameter return from the method");

}


SOM_Scope void  SOMLINK arrayMethod(MemoryManagement *somSelf,  Environment *ev,
                                    long inParameter[10], long outParameter[10],
                                    long inOutParameter[10])
{
    MemoryManagementData *somThis = MemoryManagementGetData(somSelf);
    MemoryManagementMethodDebug("MemoryManagement","arrayMethod");

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
 /* No memory to save with arrays */
#endif

    for (int i = 0; i<10; ++i)
       (outParameter)[i] = inParameter[10-i-1];
    for ( i=0; i<10; ++i)
       (inOutParameter)[i] += 10;

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
    /* Save pointers to the storage so that they can be freed or reused later */
#endif

}



SOM_Scope any*  SOMLINK anyMethod(MemoryManagement *somSelf,  Environment *ev,
                                  any* inParameter, any** outParameter,
                                  any** inOutParameter)
{
    MemoryManagementData *somThis = MemoryManagementGetData(somSelf);
    MemoryManagementMethodDebug("MemoryManagement","anyMethod");

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
 /* Check to see if any of the parameters are currently allocated, in some
    instances, the server code may want to reuse the storage, but since this is a
    sample, the memeory will simply be freed and realloced */
    if (somThis->saveReturnAny != NULL) {
       SOMFree(somThis->saveReturnAny->_value);
       SOMFree(somThis->saveInAny);
       }
    if (somThis->saveInAny != NULL) {
       SOMFree(somThis->saveInAny->_value);
       SOMFree(somThis->saveInAny);
       }
    if (somThis->saveOutAny != NULL) {
       SOMFree(somThis->saveOutAny->_value);
       SOMFree(somThis->saveOutAny);
       }
    if (somThis->saveInOutAny != NULL) {
       SOMFree(somThis->saveInOutAny->_value);
       SOMFree(somThis->saveInOutAny);
       }
#endif

    *outParameter = new any;
    (*outParameter)->_type = tcNew(tk_long);
    long * tempLong = (long *)SOMMalloc(sizeof(long));
    *tempLong = 100;
    (*outParameter)->_value = (void *)tempLong;

    tempLong = (long *)(*inOutParameter)->_value;
    (*tempLong) *= 4;

    any * retVal = new any;
    retVal->_type = TypeCodeNew(tk_long);
    tempLong = (long *)SOMMalloc(sizeof(long));
    *tempLong = 10;
    retVal->_value = (void *)tempLong;

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
    /* Save pointers to the storage so that they can be freed or reused later */
    somThis->saveReturnAny = retVal;
    somThis->saveInAny = inParameter;
    somThis->saveOutAny = *outParameter;
    somThis->saveInOutAny = *inOutParameter;
#endif



    return (retVal);
}


SOM_Scope void SOMLINK somDefaultInit(MemoryManagement *somSelf, som3InitCtrl* ctrl)
{
    MemoryManagementData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    MemoryManagementMethodDebug("MemoryManagement","somDefaultInit");
    MemoryManagement_BeginInitializer_somDefaultInit;

    MemoryManagement_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /*
     * local MemoryManagement initialization code added by programmer
     */
#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
    somThis->saveReturnString = NULL;
    somThis->saveInString = NULL;
    somThis->saveOutString = NULL;
    somThis->saveInOutString = NULL;
    somThis->saveReturnSequenceLong = NULL;
    somThis->saveInSequenceLong = NULL;
    somThis->saveOutSequenceLong = NULL;
    somThis->saveInOutSequenceLong = NULL;
    somThis->saveReturnSupportObject = NULL;
    somThis->saveInSupportObject = NULL;
    somThis->saveOutSupportObject = NULL;
    somThis->saveInOutSupportObject = NULL;
    somThis->saveReturnAny = NULL;
    somThis->saveInAny = NULL;
    somThis->saveOutAny = NULL;
    somThis->saveInOutAny = NULL;
#endif

}


SOM_Scope void SOMLINK somDestruct(MemoryManagement *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    MemoryManagementData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    MemoryManagementMethodDebug("MemoryManagement","somDestruct");
    MemoryManagement_BeginDestructor;

    /*
     * local MemoryManagement deinitialization code added by programmer
     */

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
    if (somThis->saveReturnString != NULL) SOMFree(somThis->saveReturnString);
    if (somThis->saveInString != NULL) SOMFree(somThis->saveInString);
    if (somThis->saveOutString != NULL) SOMFree(somThis->saveOutString);
    if (somThis->saveInOutString != NULL) SOMFree(somThis->saveInOutString);
    if (somThis->saveReturnSequenceLong != NULL) {
       SOMFree(somThis->saveReturnSequenceLong->_buffer);
       SOMFree(somThis->saveInSequenceLong);
       }
    if (somThis->saveInSequenceLong != NULL) {
       SOMFree(somThis->saveInSequenceLong->_buffer);
       SOMFree(somThis->saveInSequenceLong);
       }
    if (somThis->saveOutSequenceLong != NULL) {
       SOMFree(somThis->saveOutSequenceLong->_buffer);
       SOMFree(somThis->saveOutSequenceLong);
       }
    if (somThis->saveInOutSequenceLong != NULL) {
       SOMFree(somThis->saveInOutSequenceLong->_buffer);
       SOMFree(somThis->saveInOutSequenceLong);
       }
    if (somThis->saveReturnSupportObject != NULL) SOMFree(somThis->saveReturnSupportObject);
    if (somThis->saveInSupportObject != NULL) SOMFree(somThis->saveInSupportObject);
    if (somThis->saveOutSupportObject != NULL) SOMFree(somThis->saveOutSupportObject);
    if (somThis->saveInOutSupportObject != NULL) SOMFree(somThis->saveInOutSupportObject);
    if (somThis->saveReturnAny != NULL) {
       SOMFree(somThis->saveReturnAny->_value);
       SOMFree(somThis->saveInAny);
       }
    if (somThis->saveInAny != NULL) {
       SOMFree(somThis->saveInAny->_value);
       SOMFree(somThis->saveInAny);
       }
    if (somThis->saveOutAny != NULL) {
       SOMFree(somThis->saveOutAny->_value);
       SOMFree(somThis->saveOutAny);
       }
    if (somThis->saveInOutAny != NULL) {
       SOMFree(somThis->saveInOutAny->_value);
       SOMFree(somThis->saveInOutAny);
       }
#endif

    MemoryManagement_EndDestructor;
}


