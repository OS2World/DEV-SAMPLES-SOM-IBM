// 3/8/96 15:42:14
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

#include <somd.xh>	//DSOM include
#include <somdcprx.xh>	//DSOM create proxy include

#include "appt.xh"		//Appointment class include
#include "mtg.xh"		//Meeting class include
#include "ccall.xh"		//ConferenceCall class include
#include "apptbook.xh"	//AppointmentBook class include
#include "key.xh"		//Key class include
#include "fapptbk.xh"	//AppointmentBookFactory include
#include "dapptbk.xh"	//DistributedAppointmentBook include

#include <stdio.h>		//Standard IO include
#include "samptype.h"	//sample constants include file

void initClasses(void);
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
  IDType id;
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

  ExtendedNaming_ExtendedNamingContext *enc=NULL;
  AppointmentBookFactory  *factory=NULL;
  int initOK = 0;

// ********** Initialize the environment
  initClasses();              // Initialize the application classes
                              // Initialize DSOM, required for SOMD_ORBObject
  SOMD_Init(ev);              // global and others.
  if (!checkEnv(ev)) {
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
      factory = (AppointmentBookFactory *)
             enc->find_any(ev, "class == 'DistributedAppointmentBook'", 0);
      if (checkEnv(ev)) {
                  // Clean up after error
        factory = (AppointmentBookFactory *)OBJECT_NIL;
        ((SOMDClientProxy *)enc)->somdProxyFree(ev);
        enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
      } else {
                  // Create the "AppointmentBooks" name context
        factory->createBooksContext(ev);
        if (checkEnv(ev)){
                  // Clean up after error
          ((SOMDClientProxy *)factory)->somdProxyFree(ev);
          ((SOMDClientProxy *)enc)->somdProxyFree(ev);
          factory = (AppointmentBookFactory *)OBJECT_NIL;
          enc = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
        } else {
          initOK = 1;
        }
        factory->createAppointmentContext(ev);
         if (checkEnv(ev)){
        // Clean up after error
          ((SOMDClientProxy *)factory)->somdProxyFree(ev);
          ((SOMDClientProxy *)enc)->somdProxyFree(ev);
          factory = (AppointmentBookFactory *)OBJECT_NIL;
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
        SOMD_Uninit(ev);         // Free the DSOM Environment
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
            setUpAppointment((Appointment *)meet);
            getLine(input, "Enter location          > ");
            meet->_set_location(ev, input);
            id = apptBook->addAppointment(ev, (Appointment *)meet);
            if (!checkEnv(ev)) {
              somPrintf(
                "Appointment id for this meeting is : %d \n", id);
              }
            factory->deleteMeeting(ev,meet);   //delete the meeting to reduce user count	
          break;

/*        Set up a conference call.
          ------------------------ */
          case 'c':
            ccall = factory->createConferenceCall(ev);
            setUpAppointment((Appointment *)ccall);
            getLine(input, "Enter phone number      > ");
            ccall->_set_phoneNumber(ev, input);
            id = apptBook->addAppointment(ev, (Appointment *)ccall);
            if (!checkEnv(ev)) {
              somPrintf(
               "Appointment id for this conference call is : %d \n", id);
          }
           factory->deleteConferenceCall(ev,ccall);   //delete the Conference call to reduce the user count
         break;

/*        Remove an appointment.
          ---------------------- */
          case 'r':
            getLine(input, "Enter appointment id > ");
            id = atoi(input);
            apptBook->deleteAppointment(ev, id);
            checkEnv(ev);
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
            apptList = apptBook->getDaysAppointments(ev, year, month, day);
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
          break;

/*        Clean up and exit.
          ------------------ */
          case 'q':
            factory->deleteAppointmentBook(ev, apptBook);
            checkEnv(ev);
            ((SOMDClientProxy *)apptBook)->somdProxyFree(ev);
            ((SOMDClientProxy *)factory)->somdProxyFree(ev);
            ((SOMDClientProxy *)enc)->somdProxyFree(ev);
            SOMD_Uninit(ev);         // Free the DSOM Environment
          break;

        }
        fflush(NULL); // Flush all open streams
      } while (menu[0] != 'q');
    } else {               // if get appointment book failed.
      if (factory)
        ((SOMDClientProxy *)factory)->somdProxyFree(ev);
      if (enc)
        ((SOMDClientProxy *)enc)->somdProxyFree(ev);
      SOMD_Uninit(ev);
    }

  }  // Initialize of the environment failed
  return 0;
}
/********************************************************************************/
void initClasses()
{
AppointmentNewClass(0,0);
AppointmentBookNewClass(0,0);
ConferenceCallNewClass(0,0);
Appt_KeyNewClass(0,0);
MeetingNewClass(0,0);
DistributedAppointmentBookNewClass(0,0);
AppointmentBookFactoryNewClass(0,0);
}

