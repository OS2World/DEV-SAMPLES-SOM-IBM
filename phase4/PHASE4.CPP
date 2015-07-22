// 3/8/96 15:52:37
/*
#
#
#    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1996
#   All Rights Reserved
#   Licensed Materials - Property of IBM
#   US Government Users Restricted Rights - Use, duplication or
#   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
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

#include <stdio.h>
#include <somd.xh>         // global C++ include file for DSOM definitions
#include <somdcprx.xh>   // SOMDClientProxy class

#include "pappt.xh"         // PersistentAppointment class
#include "pmtg.xh"          // PersistentMeeting class
#include "pccall.xh"         // PersistentConferenceCall class
#include "key.xh"            // Appt_Key class
#include "dapptbk.xh"      // DistributedAppointmentBook class
#include "pdapptbk.xh"    // PersistentDistributedApointmentBook class

#include "fapptbk.xh"      // AppointmentBookFactory class
#include "pfapptbk.xh"    // PersistentAppointmentBookFactory class

#include "testutil.h"
#include "samptype.h"

void initClasses(void);
int initSOMenv(Environment *ev,
  ExtendedNaming_ExtendedNamingContext *enc,
  PersistentAppointmentBookFactory  *factory);
/*     Prints a message and reads a line into buffer.
       --------------------------------------------- */
static void getLine(char *, char *);

static void getLine(char *buffer, char *msg)
{
  int i,c;
  somPrintf(msg);
  for (i=0; i<(MAX_LENGTH-1) && (c=getchar()) != EOF && c!= '\n'; ++i)
    buffer[i] = c;
  buffer[i] = '\0';
}

void setUpAppointment(Appointment *appt)
{
   char input[MAX_LENGTH];
   Environment *ev = somGetGlobalEnvironment();

   getLine(input, "Enter month             > ");
   appt->_set_month(ev, atoi(input));

   getLine(input, "Enter day               > ");
   appt->_set_day( ev, atoi(input));

   getLine(input, "Enter year              > ");
   appt->_set_year(ev, atoi(input));

   getLine(input, "Enter starting time     > ");
   appt->_set_start(ev, atoi(input));

   getLine(input, "Enter ending time       > ");
   appt->_set_end(ev, atoi(input));

   getLine(input, "Enter subject           > ");
   appt->_set_subject(ev, input);
}

main (int argc, char **argv)
{
  unsigned long id = 0;
  short i;
  string bp;
  short day, month, year;
  Meeting *meet;
  ConferenceCall *ccall;
  Appointment *appt;
  AppointmentBook *apptBook;
  _IDL_SEQUENCE_Appointment apptList;
  char menu[MAX_LENGTH];
  char input[MAX_LENGTH];
  char buffer[MAX_BUFFER_SIZE];
  string menuLine =
    "\nm(meeting) c(conference) r(remove) d(print day) a(print all) q(quit) > ";
  Environment *ev = somGetGlobalEnvironment();
  CosTransactions_Current *Trans;
  ExtendedNaming_ExtendedNamingContext *enc=NULL;
  PersistentAppointmentBookFactory  *factory=NULL;
  int initOK = 0;

// ********** Initialize the environment
  initClasses();              // Initialize the application classes
  SOMD_Init(ev);           // Initialize DSOM, required for SOMD_ORBObject
                                 // global and others.

  DSOM_thread(argc, argv, ev);

  if (!checkEnv(ev)) {

    Trans = new CosTransactions_Current;

     // Get the context where factory objects are stored
     // enc is a proxy to the Name Service context
     // where object factories are stored
     enc = (ExtendedNaming_ExtendedNamingContext *)
              SOMD_ORBObject->resolve_initial_references(ev, "FactoryService");
     if (checkEnv(ev)) {
           enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
     } else {
           // query the name service for a factory
           // that creates an Appointment Book object
           factory = (PersistentAppointmentBookFactory *)
                         enc->find_any(ev, "class == 'PersistentDistributedAppointmentBook'", 0);

           if (checkEnv(ev)) {
              // Clean up after error
              factory = (PersistentAppointmentBookFactory *)OBJECT_NIL;
              ((SOMDClientProxy *)enc)->somdProxyFree(ev);
               enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
           } else {
                // Create the "AppointmentBooks" name context
                factory->createBooksContext(ev);
                if (checkEnv(ev)){
                     // Clean up after error
                     ((SOMDClientProxy *)factory)->somdProxyFree(ev);
                     ((SOMDClientProxy *)enc)->somdProxyFree(ev);
                     factory = (PersistentAppointmentBookFactory *)OBJECT_NIL;
                     enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
                } else {
                     initOK = 1;
            }
                factory->createAppointmentContext(ev);
                if (checkEnv(ev)){
                     // Clean up after error
                     ((SOMDClientProxy *)factory)->somdProxyFree(ev);
                     ((SOMDClientProxy *)enc)->somdProxyFree(ev);
                     factory = (PersistentAppointmentBookFactory *)OBJECT_NIL;
                     enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
                } else {
                     initOK = 1;
            }
         }
     }
  }
// ********** Done Initialize the environment
  if (initOK) {
    getLine(input, "Enter your choice \n n(Get new appointment book) \n e(Get existing book) \n > ");
    switch (input[0]) {
      case 'n':
          getLine(input,"Enter the user name > \n");
                                  // Call the factory to create an appointment book
          apptBook = factory->createAppointmentBook (ev, input);
        break;
      case 'e':
          getLine(input,"Enter the user name > \n");
                                  // Call the factory to check out an existing book
          apptBook = factory->checkOutAppointmentBook (ev, input);
        break;
      default:
        return 1;
        break;
    } // end switch
    if (!checkEnv(ev)) {

      do{

/*      Display the menu options and read the operation.
        ------------------------------------------------ */
        getLine(menu, menuLine);

        switch(menu[0]){

/*        Set up a meeting.
          ----------------- */
          case 'm':

            meet = factory->createMeeting(ev);

            Trans->begin(ev);
            setUpAppointment((Appointment *)meet);
            getLine(input, "Enter location          > ");
            meet->_set_location(ev, input);
            Trans->commit(ev,FALSE);

            Trans->begin(ev);
            id = apptBook->addAppointment(ev, (Appointment *)meet);
            if (checkEnv(ev)) {
                 somPrintf("Could not add this meeting to the book.\n");
                 Trans->rollback(ev);
                 if (checkEnv(ev)) {
                      somPrintf("Rollback failed!!\n");
                 }
            }
            else {
                 Trans->commit(ev,FALSE);
                 somPrintf("Appointment id for this meeting is : %d \n", id);
            }
            Trans->begin(ev);
            factory->deleteMeeting(ev,meet);
            Trans->commit(ev,FALSE);
          break;

/*        Set up a conference call.
          ------------------------ */
          case 'c':

            ccall = factory->createConferenceCall(ev);

            Trans->begin(ev);
            setUpAppointment((Appointment *)ccall);
            getLine(input, "Enter phone number      > ");
            ccall->_set_phoneNumber(ev, input);
            Trans->commit(ev,FALSE);

            Trans->begin(ev);
            id = apptBook->addAppointment(ev, (Appointment *)ccall);
            if (checkEnv(ev)) {
                 somPrintf("Could not add this call to the book.\n");
                 Trans->rollback(ev);
                 if (checkEnv(ev)) {
                     somPrintf("Rollback failed!! \n");
                 }
            }
            else {
                 Trans->commit(ev,FALSE);
                 somPrintf("Appointment id for this conference call is : %d \n", id);
            }
            Trans->begin(ev);
            factory->deleteConferenceCall(ev,ccall);
            Trans->commit(ev,FALSE);
          break;

/*        Remove an appointment.
          ---------------------- */
          case 'r':
            getLine(input, "Enter appointment id > ");
            id = atoi(input);

            Trans->begin(ev);
            apptBook->deleteAppointment(ev, id);
            Trans->commit(ev,FALSE);

            break;

/*        Display the appointments for a given day.
          ----------------------------------------- */
          case 'd':
            getLine(input, "Enter appointment month > ");
            month = atoi(input);
            getLine(input, "Enter appointment day   > ");
            day = atoi(input);
            getLine(input, "Enter appointment year  > ");
            year = atoi(input);
                           /* Get the day's appointments in a sequence */

            Trans->begin(ev);
            apptList = apptBook->getDaysAppointments(ev, year, month, day);
            Trans->commit(ev,FALSE);

            if (!checkEnv(ev)) {
              for (i=0; i< sequenceLength(apptList); i++) {
                appt = sequenceElement(apptList,i);
                bp = appt->bufferize(ev);
                somPrintf("\n%s",bp);
                                  // Free the storage from bufferize
                ORBfree(bp);
              }

         // free objects in the buffer
              ORBfree(apptList._buffer);
            }
          break;

/*        Display all the appointments.
          ----------------------------- */
          case 'a':

            Trans->begin(ev);
            apptList = apptBook->getAllAppointments(ev);

            if (!checkEnv(ev)) {
              for (i=0; i< sequenceLength(apptList); i++) {
                  appt = sequenceElement(apptList,i);
                  bp = appt->bufferize(ev);
                  somPrintf("\n%s",bp);
                ORBfree(bp);
              }
              ORBfree(apptList._buffer);
            }
            Trans->commit(ev,FALSE);

          break;

/*        Clean up and exit.
          ------------------ */
          case 'q':

            Trans->begin(ev);
            factory->deleteAppointmentBook(ev, apptBook);
            Trans->commit(ev,FALSE);

            checkEnv(ev);
            ((SOMDClientProxy *)apptBook)->somdProxyFree(ev);
            ((SOMDClientProxy *)factory)->somdProxyFree(ev);
            ((SOMDClientProxy *)enc)->somdProxyFree(ev);

            Trans->somFree();
            DSOM_term(ev);
            SOM_UninitEnvironment(ev);
          break;

        }
        fflush(NULL); // Flush all open streams
      } while (menu[0] != 'q');
    } else {               // if get appointment book failed.
      if (factory)
        ((SOMDClientProxy *)factory)->somdProxyFree(ev);
      if (enc)
        ((SOMDClientProxy *)enc)->somdProxyFree(ev);

      Trans->somFree();
      DSOM_term(ev);
      SOM_UninitEnvironment(ev);

    }

  }  // Initialize of the environment failed
  return 0;
}
/********************************************************************************/
void initClasses()
{
    PersistentAppointmentNewClass(0,0);
    PersistentConferenceCallNewClass(0,0);
    Appt_KeyNewClass(0,0);
    PersistentMeetingNewClass(0,0);
    PersistentDistributedAppointmentBookNewClass(0,0);
    PersistentAppointmentBookFactoryNewClass(0,0);
}



