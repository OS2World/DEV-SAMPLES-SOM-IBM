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

#ifndef lint
static char *sccsid = "@(#) 1.10 src/sompos/testpds/src/SampleDir/phonmain.cpp, sompos, som2.5 1/5/96 09:57:27 [2/13/96 13:51:51]";
#endif

#include <somd.xh>
#include <sompposx.xh>
#include <iostream.h>
#include "phone.xh"

boolean checkError(Environment *ev);

int main (int argc, char *argv[])
{

    // declare variables
    Environment ev;
    somPersistencePOSIX_PID_POSIX *pid;
    phoneEntry *PhoneEntry, *TempPhoneEntry;
    string phone_number;
    string name;
    short office;

    // initialize environment
    SOM_InitEnvironment (&ev);
    SOMD_Init(&ev);

    // instantiate and set up the pid
   pid=(somPersistencePOSIX_PID_POSIX *) (void *) somdCreate(&ev,"somPersistencePOSIX::PID_POSIX", TRUE);

   pid->_set_pathName(&ev, "phone.dat");

    // instantiate and set up the phone entry
    TempPhoneEntry=(phoneEntry *) (void*) somdCreate(&ev,"phoneEntry", FALSE);
    PhoneEntry=(phoneEntry *) ((void*)TempPhoneEntry->init_for_object_creation(&ev));
    ((SOMDObject *) (void*) TempPhoneEntry)->release(&ev);

    PhoneEntry->_set_name(&ev,"John Smith");
    PhoneEntry->_set_phone_number(&ev,"507-253-0000");
    PhoneEntry->_set_office(&ev,42);
    PhoneEntry->_set_p(&ev,pid);
    PhoneEntry->store(&ev,pid);
    checkError(&ev);

    // this entry will overwrite the "John Smith" entry
    PhoneEntry->_set_name(&ev,"John Doe");
    PhoneEntry->_set_phone_number(&ev,"555-555-1111");
    PhoneEntry->_set_office(&ev,61);
    PhoneEntry->store(&ev,NULL);
    checkError(&ev);

    // restore the entry
    PhoneEntry->restore(&ev,pid);
    if (checkError(&ev) == FALSE)
	{
                 name = PhoneEntry->_get_name(&ev);
                 phone_number = PhoneEntry->_get_phone_number(&ev);
                 office = PhoneEntry->_get_office(&ev);

                 // print the restored phone entry
                 cout << "Name: " << name << endl;
                 cout << "Phone Number: " << phone_number << endl;
                 cout << "Office: " << office << endl;
            }

    // delete the persistent object
    PhoneEntry->uninit_for_object_destruction(&ev);

    PhoneEntry->Delete(&ev,pid);
    checkError(&ev);

    PhoneEntry->somFree();

    // delete the pid
    pid->somFree();

   // cleanup the environment
    SOMD_Uninit(&ev);
    SOM_UninitEnvironment (&ev);

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