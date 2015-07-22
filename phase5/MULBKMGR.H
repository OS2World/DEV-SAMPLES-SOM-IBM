// 3/8/96 16:13:42
/*
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
#
*/
/*
 *
 *
 * DISCLAIMER OF WARRANTIES.
 * The following [enclosed] code is sample code created by IBM
 * Corporation. This sample code is not part of any standard or IBM
 * product and is provided to you solely for the purpose of assisting
 * you in the development of your applications.  The code is provided
 * "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 * THIS CODE.  IBM shall not be liable for any damages arising out of
 * your use of the sample code, even if they have been advised of the
 * possibility of such damages.
 *
 * DISTRIBUTION.
 * This sample code can be freely distributed, copied, altered, and
 * incorporated into other software, provided that it bears the above
 * Copyright notice and DISCLAIMER intact.
 */

#ifndef mulbkmgr_h
#define mulbkmgr_h
#include <pappt.xh>
#include <apptbook.xh>
#include "pappt.xh"
#include "pmtg.xh"
#include "pccall.xh"
#include "key.xh"
#include "dapptbk.xh"
#include "pdapptbk.xh"

#include "pfapptbk.xh"
#include "samptype.h"

#ifndef MAX_APPT_BOOKS
//The maximum number of books added. The arrays could be dynamically allocated but a fixed
//number was chosen to simplify this sample.
#define MAX_APPT_BOOKS 10
#endif
//class PersistentAppointmentBookFactory;

class MultiBookManager
{
   public:
//Constructor for the multiple book manager. This will initialize the arrays,
//the number of books and the factory used to create the books.
      MultiBookManager(PersistentAppointmentBookFactory  *factory);
//The AddMultiAppt method adds an appointment to multiple books. The pointer to the
//appointment is passed in the variable pApptIn. A pointer to the present appointment
//book is passed in the variable book. This method calls the DeleteBooks method to clear
//any existing books. It will also call the InputUserIds method to prompt for the names of
//the appointment books that the user wants to add the appointment to. This meethod will
//then use the factory to create the books add the appointments and check for errors.If
//there is an error during the add then the method will rollback the changes.
      void AddMultiAppt(Appointment * pApptIn,
                        AppointmentBook* book);
//Destructor for the multiple appointment book manager.
      ~MultiBookManager();

   private:
      //Factory for creating, checking out and deleting appointment books.
      PersistentAppointmentBookFactory  * apptFactory;
      //An array of appointment book user ids.
      char apptUserIds[MAX_APPT_BOOKS][MAX_LENGTH];
      //An array of pointers to appointment books.
      AppointmentBook * pApptBkArray[MAX_APPT_BOOKS];
      //The count of appointment books that will have an appointment added.
      int numberOfBooks;

//The InputUserIds method will prompt the user to enter the user ids of the appointment
//books that need to have the appointment added. The user is only allowed to add up to the
//maximum nuumber of appointment books - 1. One location in the appointment book array is
//needed for the users appointment book.
      void DeleteBooks();
//The DeleteBooks method deletes all the appointment books that the multiple book manager
//created. It also sets the array values to NULL.
      void InputUserIds();

};


#endif /* mulbkmgr_h */

