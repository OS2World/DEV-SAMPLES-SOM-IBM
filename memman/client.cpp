/**********************************************************************
 *
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
 *
 *
 * name : client.cpp
 *
 * function: demonstate memory management and the IDL modifiers in,
 *           out and inout on the diffent IDL types.
 *
 * There are 3 basic type of memory memory management demponstrated:
 *   caller owned -- This is the default memory management policy, this
 *       can also be identified by using the modifier:
 *             memory_management = corba;
 *       using the memory_management modifier applies to all the
 *       attributes and returned values from an object, where as the
 *       modifier caller_owns_result and caller_owns_parameters can be
 *       specified on a method by method basis. For this memory policy
 *       the caller is responsible for freeing all the memory which is
 *       allocated by the object and the calling program.
 *
 *   object owned -- This memory management policy is a modifier that
 *       must be specified at the method level. This identifies the
 *       parameters and return values which the object ownes, the
 *       caller should not modify or destroy any such values, the
 *       object is responsible for destroying these values. When using
 *       object owned and DSOM, storage for out, inout and returned values
 *       is created in the proxy, this storage is held until the proxy is
 *       destoryed or the caller invokes the somdReleaseResources on the
 *       proxy. This will free all the storage which the proxy has
 *       accumulated on previous calls.
 *
 *    dual owned -- This is a combination of caller owns and object owns
 *       and only applies when the objects are distributed (DSOM). In
 *       this policy the caller is responsible for destorying for all
 *       out, inout and result storage. On the server side, the
 *       implementation of the policy is the same as the object owned
 *       policy.
 *
 *   For more information on the memory management policy please refer
 *   to the SOM user guide.
 *
 *   The running of the program is of little interest, since the
 *   output appears the same no matter which memory policy is used.
 *   the most interesting aspect is the examination of the code and
 *   the different ways inwhich the memory is managed depending
 *   upon the memory management policy which was selected.
 *
 ***********************************************************************/


// Standard Includes
#include <iostream.h>

#ifdef __DSOM__
  #include <somd.xh> // Header file for DSOM application
#else
  #include <som.xh> // Header file for SOM application
#endif
#include <somtc.xh> // Header file for som any type support

// Header Include
#include "memman.xh"

#include <stdio.h>

/* Inline method to print the connents of an array of longs */
inline string printArray(long array[10])
{ char * buffer = new char [100];
  buffer[0]='\0';
  for(int k=0; k<10;++k) sprintf(buffer+strlen(buffer)," %d ", array[k]);
  return (buffer); }

inline void  printSequence(_IDL_SEQUENCE_long * sequenceOf)
       {for (int z=0; z<sequenceOf->_length; ++z) cout << sequenceOf->_buffer[z] << ' ';}


/* incline method to check the return code from SOM */
void _checkEnv(Environment *ev); // Prototype for functionto call on error
inline void checkEnv(Environment *ev) { if (ev->_major != NO_EXCEPTION) _checkEnv(ev); }

int main (int argc, char *argv[])
{
    /* Get the SOM environment variable */
    Environment* ev = somGetGlobalEnvironment();


#ifdef __DSOM__
    /* Initialize the DSOM runtime environment */
    SOMD_Init(ev);
#endif

    MemoryManagement *myObject =
#ifdef __DSOM__
    /* Create the DSOM object */
                   (MemoryManagement *)somdCreate(ev, "MemoryManagement", FALSE);
    checkEnv(ev);
#else
    /* Create the SOM object */
                   new MemoryManagement;
#endif


// Output information about how the program was compiler for the users
cout << "The client is compiled using : " <<
#ifdef __DSOM__
"DSOM. " <<
#else
"SOM. " <<
#endif
endl << "And memory management policy of : " <<
#ifdef __MEMORY_MGMT_DUAL_OWNED__
"memory management is dual owned" <<
#else
#ifdef __MEMORY_MGMT_OBJECT_OWNED__
"memory management is object owned" <<
#else
"Default memory management policy (corba) " <<
#endif
#endif
endl;


/*********************************************************************
 * Begin the sample code to show memory management and in, out, inout
 * modifiers
 ********************************************************************/


/*********************************************************************
 * Long/scalar types
 ********************************************************************/
{
// Allocate memory
   long inParameter=12,
        outParameter=0,
        inOutParameter=31,
        returnValue=0;

// Display variable values before call to method
   cout << "\n\nBefore call : " << endl;
   cout << "inParameter = " << inParameter << endl ;
   cout << "outParameter = " << outParameter << endl;
   cout << "inOutParameter = " << inOutParameter << endl;
   cout << "returnValue = " << returnValue << endl;

// Call the method
   returnValue = myObject->longMethod(ev, inParameter, &outParameter, &inOutParameter);

// Display variable values after call to the method
   cout << "\nAfter call : " << endl;
   cout << "inParameter = " << inParameter << endl ;
   cout << "outParameter = " << outParameter << endl;
   cout << "inOutParameter = " << inOutParameter << endl;
   cout << "returnValue = " << returnValue << endl;

// Delete the appropriate items here
   /* with scalar types there is need to free the memory,
    the memory will be free when the variables pass out of scope */

}

/*********************************************************************
 * Strings
 ********************************************************************/
{
// Allocate memory
   string inParameter = (char *)SOMMalloc(sizeof(char)*100),
          outParameter = NULL,
          inOutParameter = (char *)SOMMalloc(sizeof(char)*100),
          returnValue = NULL;

  strcpy(inParameter,"eulav retemaraPtuo ehT\0");
  strcpy(inOutParameter,"The inOutParameter value");

// Display variable values before call to method
  cout << "\n\nBefore call : (strings)" << endl;
  cout << "inParameter = " << inParameter << endl;
  cout << "outParameter = " << outParameter << endl;
  cout << "inOutParameter = " << inOutParameter << endl;
  cout << "returnValue = " << returnValue << endl;

// Call the method
  returnValue = myObject->stringMethod(ev, inParameter, &outParameter, &inOutParameter);
  checkEnv(ev);

// Display variable values after call to the method
  cout << "\nAfter call : " << endl;
  cout << "inParameter = " << inParameter << endl;
  cout << "outParameter = " << outParameter << endl;
  cout << "inOutParameter = " << inOutParameter << endl;
  cout << "returnValue = " << returnValue << endl;

// Delete the appropriate items here
#if (defined(__MEMORY_MGMT_DEFAULT__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
  #ifdef __DSOM__
  SOMFree(inParameter); inParameter = NULL;
/****************************************************************************
 * ORBfree -- for some out parameters and returned values, corba specifies the use of
 * ORBfree to free DSOM allocated storage, for DSOM may use special memory
 * management techniques to allocate memory. Storage so allocated must be treated spcially
 * by the user; specifically, pointers within it may not modified, not can they be freed using
 * SOMFree and must be freed using ORBfree.
 **************************************************************************/
  ORBfree(outParameter); outParameter = NULL;
  SOMFree(inOutParameter); inOutParameter = NULL;
  ORBfree(returnValue); returnValue = NULL;
  #else
  SOMFree(inParameter); inParameter = NULL;
  SOMFree(outParameter); outParameter = NULL;
  SOMFree(inOutParameter); inOutParameter = NULL;
  SOMFree(returnValue); returnValue = NULL;
  #endif

#else
// There is no memory to free in the case of object owned
#endif

}

/*********************************************************************
 * Sequence (_buffer)
 ********************************************************************/
{
// Allocate memory
   _IDL_SEQUENCE_long * inParameter = (_IDL_SEQUENCE_long *) SOMMalloc(sizeof( _IDL_SEQUENCE_long)),
                      * outParameter = NULL,
                      * inOutParameter = (_IDL_SEQUENCE_long *) SOMMalloc(sizeof( _IDL_SEQUENCE_long)),
                      * returnValue;

   inParameter->_maximum = 10;
   inParameter->_buffer = (long *)SOMMalloc(inParameter->_maximum * sizeof(long));
   inParameter->_length = 0;
   for (int i=0; i<inParameter->_maximum; ++i, inParameter->_length ++)
      inParameter->_buffer[i] = i;

   inOutParameter->_maximum = 10;
   inOutParameter->_buffer = (long *)SOMMalloc(inOutParameter->_maximum * sizeof(long));
   inOutParameter->_length = 0;
   for (i=0; i<inOutParameter->_maximum; ++i, inOutParameter->_length ++)
      inOutParameter->_buffer[i] = inOutParameter->_maximum - i - 1;

// Display variable values before call to method
   cout << "\n\nBefore call : (sequences)" << endl;
   cout << "inParameter : "; printSequence(inParameter); cout << endl;
   cout << "inOutParameter : "; printSequence(inOutParameter); cout << endl;

// Call the method
   returnValue = myObject->sequenceMethod(ev, inParameter, &outParameter, &inOutParameter);
   checkEnv(ev);

// Display variable values after call to the method
   cout << "\nAfter call : " << endl;
   cout << "inParameter : "; printSequence(inParameter); cout << endl;
   cout << "inOutParameter : "; printSequence(inOutParameter); cout << endl;
   cout << "outParameter : "; printSequence(outParameter); cout << endl;
   cout << "returnValue : "; printSequence(returnValue); cout << endl;

// Delete the appropriate items here
#if (defined(__MEMORY_MGMT_DEFAULT__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
  #ifdef __DSOM__
  SOMFree(inParameter->_buffer); inParameter->_buffer = NULL;
  /* The ORBfree for the outParameter will also free the _buffer */
  SOMFree(inOutParameter->_buffer); inOutParameter->_buffer = NULL;
  /*********************************************************************
   * In the case of freeing a sequence, the buffer is passed as a parameter to ORBfree
   *********************************************************************/
  ORBfree(returnValue->_buffer); returnValue = NULL;

  SOMFree(inParameter); inParameter = NULL;
  ORBfree(outParameter); outParameter = NULL;
  SOMFree(inOutParameter); inOutParameter = NULL;
  SOMFree(returnValue); returnValue = NULL;
  #else
  SOMFree(inParameter->_buffer); inParameter->_buffer = NULL;
  SOMFree(outParameter->_buffer); outParameter->_buffer = NULL;
  SOMFree(inOutParameter->_buffer); inOutParameter->_buffer = NULL;
  SOMFree(returnValue->_buffer); returnValue->_buffer = NULL;

  SOMFree(inParameter); inParameter = NULL;
  SOMFree(outParameter); outParameter = NULL;
  SOMFree(inOutParameter); inOutParameter = NULL;
  SOMFree(returnValue); returnValue = NULL;
  #endif

#else
// There is no memory to free in the case of object owned
#endif
}

#if (defined(__MEMORY_MGMT_OBJECT_OWNED__) && defined(__DSOM__))
/***************************************************************
 * In the case of  using DSOM and the object owns policy, the proxy object
 * stores the memory associated with all returned and out values until the
 * proxy is released. To prevent the memory from accumulating in the proxy,
 * invoke somdReleaseResources as shown below.
 ***************************************************************/
      ((SOMDClientProxy *)myObject)->somdReleaseResources(ev);
#else
// There is no need to free resources from the proxy for other memory management policies
#endif


/*********************************************************************
 * Objects
 ********************************************************************/
{
// Allocate memory
   SupportObject * inParameter =
#ifdef __DSOM__
                          (SupportObject *)somdCreate(ev, "SupportObject", FALSE),
#else
                          new SupportObject,
#endif
                 * outParameter = NULL,
                 * inOutParameter =
#ifdef __DSOM__
                          (SupportObject *)somdCreate(ev, "SupportObject", FALSE),
#else
                          new SupportObject,
#endif
                 * returnValue = NULL;

  checkEnv(ev);


   inParameter->setValues(ev, 10, "inParameter string", 'b');
   inOutParameter->setValues(ev, 4, "inOutParameter string", 'x');

// Display variable values before call to method
   cout << "\n\nBefore call : (objects)" << endl;
   cout << "inParameter : " << inParameter->returnValues(ev) << endl;
   cout << "inOutParameter : " << inOutParameter->returnValues(ev) << endl;

// Call the method
   returnValue = myObject->objectMethod(ev, inParameter, &outParameter, &inOutParameter);
   checkEnv(ev);

// Display variable values after call to the method
   cout << "\nAfter call :" << endl;
   cout << "inParameter : " << inParameter->returnValues(ev) << endl;
   cout << "outParameter : " << outParameter->returnValues(ev) << endl;
   cout << "inOutParameter : " << inOutParameter->returnValues(ev) << endl;
   cout << "returnValue : " << returnValue->returnValues(ev) << endl;

// Delete the appropriate items here
#if (defined(__MEMORY_MGMT_DUAL_OWNED__) || defined(__DSOM__))
  /******************************************************************
  * When the dual owned policiy is used (with DSOM), the caller is responsible for
  * deleting the proxies to the object, this can be done by the following:
  ******************************************************************/
   ((SOMDObject *)inParameter)->release(ev); inParameter = NULL;
   ((SOMDObject *)outParameter)->release(ev); outParameter = NULL;
   ((SOMDObject *)inOutParameter)->release(ev); inOutParameter = NULL;
   ((SOMDObject *)returnValue)->release(ev); returnValue = NULL;
#else
#ifdef __MEMORY_MGMT_DEFAULT__
  /*******************************************************************
  * When memory management is corba (the default) or caller owns the delete		
  * operator in C++ can be used. somFree can also be used to free the objects in
  * this case also.
  *******************************************************************/
  delete inParameter; inParameter = NULL;
  delete outParameter; outParameter = NULL;
  delete inOutParameter; inOutParameter = NULL;
  delete returnValue; returnValue = NULL;
#else
 // No cleanup for object owns policy
#endif
#endif

}

/*********************************************************************
 * octet
 ********************************************************************/
{
// Allocate memory
   octet inParameter[100],
        outParameter[100],
        inOutParameter[100];

   strcpy((string)inParameter, "This is the inParameter");
   strcpy((string)inOutParameter, "This is the outParameter");

// Display variable values before call to method
   cout << "\n\nBefore call : (memory allocation)" << endl;
   cout << "inParameter = " << (string)inParameter << endl;
   cout << "inOutParameter = " << (string)inOutParameter << endl;

// Call the method
    myObject->octetMethod(ev, inParameter, outParameter, inOutParameter);
    checkEnv(ev);

// Display variable values after call to the method
   cout << "\nAfter call :: " << endl;
   cout << "inParameter = " << (string)inParameter << endl;
   cout << "outParameter = " << (string)outParameter << endl;
   cout << "inOutParameter = " << (string)inOutParameter << endl;

// Delete the appropriate items here
#if (defined(__MEMORY_MGMT_DEFAULT__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
  #ifdef __DSOM__
  // There is no memory to free in this case
  #else
  // There is no memory to free in this case
  #endif

#else
// There is no memory to free in the case of object owned
#endif


}

/*********************************************************************
 * arrays
 ********************************************************************/
{
// Allocate memory
   long inParameter[10],
        outParameter[10], /* Note: In this example the memory for the output parameter is
                             allocated here in the client program */
        inOutParameter[10];

   for (int i=0; i<10; ++i) {
      inParameter[i]=i;
      inOutParameter[i] = 9-i;
      }

// Display variable values before call to method
   cout << "\n\nBefore call : (array)" << endl;
   cout << "inParameter = " << printArray(inParameter) << endl;
   cout << "inOutParameter = " << printArray(inOutParameter) << endl;

// Call the method
   myObject->arrayMethod(ev, inParameter, outParameter, inOutParameter);
   checkEnv(ev);

// Display variable values after call to the method
   cout << "\nAfter call : ";
   cout << "inParameter = " << printArray(inParameter) << endl;
   cout << "outParameter = " << printArray(outParameter) << endl;
   cout << "inOutParameter = " << printArray(inOutParameter) << endl;
// Delete the appropriate items here
#if (defined(__MEMORY_MGMT_DEFAULT__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
  #ifdef __DSOM__
  // There is no memory to free in this case
  #else
  // There is no memory to free in this case
  #endif

#else
// There is no memory to free in the case of object owned
#endif
}

/*********************************************************************
 * any types (_value)
 ********************************************************************/
{
// Allocate memory
   any   inParameter,
       * outParameter,
       * inOutParameter = (any *)SOMMalloc(sizeof(any)),
       * returnValue = NULL;

// I am not sure how to set the type code here!!!

   inParameter._type = tcNew(tk_long);
   long * tempLong = (long *)SOMMalloc(sizeof(long));
   *tempLong = 99;
   inParameter._value = tempLong;
   //inParameter._value) = (string)SOMMalloc(100);

   inOutParameter->_type = tcNew(tk_long);
   tempLong = (long *)SOMMalloc(sizeof(long));
   *tempLong = 6;
   inOutParameter->_value = tempLong;
   //inOutParameter->_value = (char *)SOMMalloc(100);
//   (long)*inOutParameter->_value = 6;

// Display variable values before call to method
   cout << "\n\nBefore call : (any type)" << endl;
   cout << "inParameter = " << *((long *)inParameter._value) << endl;
   cout << "inOutParameter = " << *((long*)inOutParameter->_value) << endl;

// Call the method
   returnValue = myObject->anyMethod(ev, &inParameter, &outParameter, &inOutParameter);
   checkEnv(ev);

// Display variable values after call to the method
   cout << "\nAfter call : " << endl;
   cout << "inParameter = " << *((long *)inParameter._value) << endl;
   cout << "outParameter = " << *((long *)outParameter->_value)  << endl;
   cout << "inOutParameter = " << *((long *)inOutParameter->_value) << endl;
   cout << "returnValue = " << *((long *)returnValue->_value) << endl;

// Delete the appropriate items here
#if (defined(__MEMORY_MGMT_DEFAULT__) || defined(__MEMORY_MGMT_DUAL_OWNED__))
  #ifdef __DSOM__
   /* Delete the _value which was allocated for the type */
   SOMFree(inParameter._value); inParameter._value = NULL;
   SOMFree(outParameter->_value); outParameter->_value = NULL;
   SOMFree(inOutParameter->_value); inOutParameter->_value = NULL;
   SOMFree(returnValue->_value); returnValue->_value = NULL;

  /* Delete the _type which was created with tcNew */
   TypeCode_free(inParameter._type, ev);
   TypeCode_free(outParameter->_type, ev);
   TypeCode_free(inOutParameter->_type, ev);
   TypeCode_free(returnValue->_type, ev);

   /* Delete the any variables */
   /* inParameter is declared locally and will be freed when it passes out of
      scope */
   SOMFree(outParameter); outParameter = NULL;
   SOMFree(inOutParameter); inOutParameter = NULL;
   SOMFree(returnValue); returnValue = NULL;

  #else
   /* Delete the _value which was allocated for the type */
   SOMFree(inParameter._value); inParameter._value = NULL;
   SOMFree(outParameter->_value); outParameter->_value = NULL;
   SOMFree(inOutParameter->_value); inOutParameter->_value = NULL;
   SOMFree(returnValue->_value); returnValue->_value = NULL;

   /* Delete the any variables */
   /* inParameter is declared locally and will be freed when it passes out of
      scope */
   SOMFree(outParameter); outParameter = NULL;
   SOMFree(inOutParameter); inOutParameter = NULL;
   SOMFree(returnValue); returnValue = NULL;

  #endif

#else
// There is no memory to free in the case of object owned
#endif
}

/* Delete the actual object -- In the instance where the memory is owned by the object
   the memory will be free by the code which the creator of the object has placed in
   somDestruct method -- This is automatically called via the SOMFree */

#ifdef __DSOM__
   /* In SOM 3.0 SOMFree will destroy both the proxy and the target object,
      This is a change from 2.x where only the proxt would be destroyed */
   SOMFree(myObject);
   /* the delete operator in C++ will have the same affect as the previous
      statement */

   /* In some instances the application may want to only delete the proxy
      and keep the target on the server, in that case the client would
      use: */
   // release(myObject, ev); /* to release the proxy */

   /* to destroy the target object */
   // somdTargetFree(myObject, ev);  /* would be used */
#else
   /* To delete the SOM object */
   SOMFree(myObject); myObject = NULL;
   /* For C++, delete myObject can also be use */
#endif

return 0;
}


void _checkEnv(Environment * ev) {
   switch (ev->_major) {
      case SYSTEM_EXCEPTION:
         cout << "System Exception\n";
         cout << "Excpetion ID: " << somExceptionId(ev) << '\n';
         cout << "Minor Code: " << ((StExcep*)somExceptionValue(ev))->minor << '\n';
         cout << "Status: " << ((StExcep*)somExceptionValue(ev))->completed << endl;
         break;
      case USER_EXCEPTION:
         cout << "User Exception\n";
         cout << "Excpetion ID: " << somExceptionId(ev) << endl;
         break;
      case NO_EXCEPTION:
         break; // Should never get here if the inline function is used to make call
      default:
        cout << "Unknown exception" << endl;
     }
#ifdef __DSOM
  somdExceptionFree(ev);
#else
  somExceptionFree(ev);
#endif
  exit (1);
}

