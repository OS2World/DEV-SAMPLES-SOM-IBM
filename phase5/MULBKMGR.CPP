// 3/8/96 16:13:39
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

#include <somd.xh>
#include <somdcprx.xh>

#include "mulbkmgr.h"
#include <iostream.h>
//
//Constructor for the multiple book manager. This will initialize the arrays,
//the number of books and the factory used to create the books.
//
MultiBookManager::MultiBookManager(PersistentAppointmentBookFactory  * factory = NULL):
   numberOfBooks(0), apptFactory(factory)
{
   if (factory != NULL){
      int idCount;
      for(idCount = 0;idCount < MAX_APPT_BOOKS; idCount ++) {
         strcpy(apptUserIds[idCount],"");        //Clear the user id array
         pApptBkArray[idCount] = NULL;           //Clear the appointment book pointer array
      };
   };
};

//
//The AddMultiAppt method adds an appointment to multiple books. The pointer to the
//appointment is passed in the variable pApptIn. A pointer to the present appointment
//book is passed in the variable book. This method calls the DeleteBooks method to clear
//any existing books. It will also call the InputUserIds method to prompt for the names of
//the appointment books that the user wants to add the appointment to. This meethod will
//then use the factory to create the books add the appointments and check for errors.If
//there is an error during the add then the method will rollback the changes.
//

void MultiBookManager::AddMultiAppt(Appointment * pApptIn,
                                    AppointmentBook* book)
{
   int idCount;
   CosTransactions_Current  *const MultiTrans = new CosTransactions_Current;
   Environment *ev1 = somGetGlobalEnvironment();
   char ans[MAX_LENGTH];

   DeleteBooks();                                 //Delete any existing books
   InputUserIds();                                //Input the user ids for this add
   //Create or recreate the books using the names that were entered
   for(idCount = 0;idCount < numberOfBooks; idCount ++) {
      //Check for a null string. If it is null skip this entry.
      if(strcmp(apptUserIds[idCount],"") != 0) {
         //Prompt for new or existing appointment book
         somPrintf("Enter your choice \n n(%s",apptUserIds[idCount]);
         somPrintf(" is a new appointment book) \n e(%s",apptUserIds[idCount]);
         somPrintf(" is an existing book) \n > ");
         //Input the answer
         cin>>ans;
         //If the answer is 'n' or 'N' then create the new book. The factory is used to
         //create or check out the book.
         if(strcmpi(ans,"n")==0){
            pApptBkArray[idCount] = apptFactory->createAppointmentBook(ev1,apptUserIds[idCount]);
         }
         //else check out the existing book
         else {
            pApptBkArray[idCount] = apptFactory->checkOutAppointmentBook(ev1,apptUserIds[idCount]);
         }
         //If there is an error during the create return from the method
         if (checkEnv(ev1)) return;
      }
      //If the user id is null set this entry in the array to NULL
      else {
          pApptBkArray[idCount] = NULL;
      };
   };
   pApptBkArray[numberOfBooks] = book;            //Set the last entry to the book passed in.
   ++numberOfBooks;                               //Increment the number of books.
   // Start transaction
   MultiTrans->begin(ev1);
   //Add the appointment to the books
   for(idCount = 0;idCount < numberOfBooks; idCount ++) {
      if(pApptBkArray[idCount]!=NULL) {
         //Add the appointment to a single book at a time.
         pApptBkArray[idCount]->addAppointment(ev1, (Appointment *)pApptIn);
         if (checkEnv(ev1)) {
            // if one of the adds fail roll back transaction.
            somPrintf("Rollback all calendars");
            MultiTrans->rollback(ev1);
            return;
         }
      }
   }
   //If we made it this far the adds completed successfully
   // end Transaction
   somPrintf("Appointment id is : %d \n", pApptIn->_get_apptId(ev1));
   MultiTrans->commit(ev1,FALSE);                 //Commit the transaction
};

//
//The InputUserIds method will prompt the user to enter the user ids of the appointment
//books that need to have the appointment added. The user is only allowed to add up to the
//maximum nuumber of appointment books - 1. One location in the appointment book array is
//needed for the users appointment book.
//

void MultiBookManager::InputUserIds()
{
   char ans[MAX_LENGTH];                       //Answer string
   int idCount;                                //Loop counter

   //Clear the user id array.
   for(idCount = 0; idCount < MAX_APPT_BOOKS; idCount ++) {
      strcpy(apptUserIds[idCount],"");         //Set the entry to a null string
   }
   strcpy(ans,"y");                            //Init the answer.
   //Enter the book names. User is only allowed to add the max number of books - 1.
   //One location is needed for the users book.
   for(idCount = 0;(strcmpi(ans,"y")==0  && idCount < (MAX_APPT_BOOKS-1)); idCount ++) {
      somPrintf("Enter user name         > ");
      cin>>apptUserIds[idCount];               //Enter the book name.
      somPrintf("Another user name? Y/N    > ");
      cin>>ans;                                //Enter the answer.
   };
   numberOfBooks = idCount;
};

//
//The DeleteBooks method deletes all the appointment books that the multiple book manager
//created. It also sets the array values to NULL.
//

void MultiBookManager::DeleteBooks()
{
   Environment *ev1 = somGetGlobalEnvironment();  //Environment variable.
   int idCount;                                   //Loop counter

   //Loop through the array and delete the books
   for(idCount = 0;idCount < (numberOfBooks - 1); idCount ++) {
      //Check for NULL pointer
      if (pApptBkArray[idCount] != NULL) {
         //Use the factory to delete the book.
         apptFactory->deleteAppointmentBook(ev1, pApptBkArray[idCount]);
         //Set the pointer to NULL
         pApptBkArray[idCount] = NULL;
         if (checkEnv(ev1)) return;
      }
   };
   //Set the last pointer to NULL
   pApptBkArray[numberOfBooks - 1] = NULL;
   numberOfBooks = 0;                             //Zero out the counter
};

//
//Destructor for the multiple appointment book manager.
//

MultiBookManager::~MultiBookManager()
{
   DeleteBooks();                                 //Delete all the books
};
