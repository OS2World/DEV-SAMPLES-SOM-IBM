/*
 *
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
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
 */


#ifndef lint
static char *sccsid = "@(#) 1.8 src/sompos/testpds/src/Sampledir1/maindb2.cpp, sompos, som2.5 12/19/95 17:36:30 [1/3/96 11:02:24]";
#endif

#include <somd.xh>
#include <sompdb2.xh>
#include <iostream.h>
#include "phone.xh"

boolean checkError(Environment *ev);

int main ( int argc, char* argv[] )
{

  // declare variables
  Environment ev;
  somPersistenceDB2_PID_DB2 *pid;
  phoneEntry *PhoneEntry, *TempPhoneEntry;
  string phone_number;
  string name;
  short office;

  // initialize environment
  SOM_InitEnvironment( &ev );
  SOMD_Init( &ev );

    // instantiate and setup the pid
    pid=(somPersistenceDB2_PID_DB2 *) (void *) somdCreate(&ev,"somPersistenceDB2::PID_DB2", TRUE);
    pid->_set_dbalias( &ev, "TESTDB" );
    pid->_set_schema_mapper( &ev, "smap");
    pid->write_short( &ev, 1234 );

    // instantiate and setup the phone entry and get persistent ref from os::server and release old ref
    TempPhoneEntry=(phoneEntry *) (void*) somdCreate(&ev,"phoneEntry", FALSE);
    PhoneEntry=(phoneEntry *) ((void*)TempPhoneEntry->init_for_object_creation(&ev));
    ((SOMDObject *) (void*) TempPhoneEntry)->release(&ev);

    // initialize data in PhoneEntry object
    PhoneEntry->_set_name( &ev, "Jane Smith" );
    PhoneEntry->_set_phone_number( &ev, "555-555-0000" );
    PhoneEntry->_set_office( &ev, 42 );

    // print name, phone number, and office settings to screen
    name = PhoneEntry->_get_name( &ev );
    phone_number = PhoneEntry->_get_phone_number( &ev );
    office = PhoneEntry->_get_office( &ev );

    cout << "1st Entry (persistent)" << endl;
    cout << "Name: " << name << endl;
    cout << "Phone Number: " << phone_number << endl;
    cout << "Office: " << office << endl << endl;

    // Store object data to DB2 datastore and check for any errors
    cout << "Storing data..." << endl;
    PhoneEntry->_set_p( &ev, pid );
    PhoneEntry->store( &ev, pid);

    // check for errors on store, proceed only if no error
    if (checkError(&ev) == FALSE)
     {

      // overwrite name, phone number, and office settings in the object but not the persistent datastore
      PhoneEntry->_set_name( &ev, "John Doe");
      PhoneEntry->_set_phone_number( &ev, "555-555-1111");
      PhoneEntry->_set_office( &ev, 61 );

      // print new name, phone number, and office settings to screen
      name = PhoneEntry->_get_name( &ev );
      phone_number = PhoneEntry->_get_phone_number( &ev );
      office = PhoneEntry->_get_office( &ev );

      cout << "2nd Entry (transient)" << endl;
      cout << "Name: " << name << endl;
      cout << "Phone Number: " << phone_number << endl;
      cout << "Office: " << office << endl << endl;

      // Restore data from DB2 datastore and check for any errors
      cout << "Restoring data..." << endl;
      PhoneEntry->restore( &ev, pid);

      // check for errors on restore, proceed only if no error
      if (checkError(&ev) == FALSE)
	{

           // print the restored object data to the screen
           name = PhoneEntry->_get_name( &ev );
           phone_number = PhoneEntry->_get_phone_number( &ev );
           office = PhoneEntry->_get_office( &ev );

           cout << "1st Entry (restored)" << endl;
           cout << "Name: " << name << endl;
           cout << "Phone Number: " << phone_number << endl;
           cout << "Office: " << office << endl;

         }

      // delete the persistent object from the DB2 datastore
      PhoneEntry->uninit_for_object_destruction(&ev);

      PhoneEntry->Delete(&ev,pid);
      checkError(&ev);

      // free the object memory
      PhoneEntry->somFree();

      // delete the pid
      pid->somFree();

      // cleanup the environment
      SOMD_Uninit( &ev );
      SOM_UninitEnvironment( &ev );
     }
  return 0;
}


// check for Errors
boolean checkError(Environment *ev)
{
    char *exID;

    // check for any exception
    if (ev->_major != NO_EXCEPTION)
    {
           exID = somExceptionId(ev);
           cout << "Error Occurred - Exception Id = " << exID << endl;
           somdExceptionFree(ev);
           return TRUE;
     }
     else
	return FALSE;
	
}


