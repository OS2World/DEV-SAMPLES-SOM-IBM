// 3/8/96 09:36:26
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

#include <appt.xh>    // Include the class definition headers.
#include <mtg.xh>
#include <ccall.xh>
#include <apptbook.xh>
#include <key.xh>

#include <stdio.h>
#include <samptype.h>

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

main ()
{
  short id;
  short i;
  string bp;
  short day, month, year;
  Meeting        *meet;
  ConferenceCall *ccall;
  Appointment    *appt;
  AppointmentBook *apptBook;
  _IDL_SEQUENCE_Appointment apptList;
  char menu[MAX_LENGTH];
  char input[MAX_LENGTH];
  char buffer[MAX_BUFFER_SIZE];
  string menuLine =
    "\nm(meeting) c(conference) r(remove) d(print day) a(print all) q(quit) > ";

  Environment *ev = somGetGlobalEnvironment(); // Get the global environment

  apptBook = new AppointmentBook;  // Create an instance of the book

  do{

/*     Display the menu options and read the operation.
       ------------------------------------------------ */
       getLine(menu, menuLine);

       switch(menu[0]){

/*       Set up a meeting.
         ----------------- */
         case 'm':
           meet = new Meeting;
           setUpAppointment((Appointment *)meet);
           getLine(input, "Enter location          > ");
           meet->_set_location(ev, input);
           id = apptBook->addAppointment(ev, (Appointment *)meet);
           if (ev->_major != NO_EXCEPTION) {
              somPrintf("\nEXCEPTION CAUGHT => ");
              somPrintf("Error on add ! \n << %s >> \n", somExceptionId(ev));
              somExceptionFree(ev);
              meet->somFree();
           } else
             somPrintf(
               "Appointment id for this meeting is : %d \n", id);
         break;

/*       Set up a conference call.
         ------------------------ */
         case 'c':
           ccall = new ConferenceCall;
           setUpAppointment((Appointment *)ccall);
           getLine(input, "Enter phone number      > ");
           ccall->_set_phoneNumber(ev, input);
           id = apptBook->addAppointment(ev, (Appointment *)ccall);
           if (ev->_major != NO_EXCEPTION) {
              somPrintf("\nEXCEPTION CAUGHT => ");
              somPrintf("Error on add ! \n << %s >> \n", somExceptionId(ev));
              somExceptionFree(ev);
              ccall->somFree();
           } else
             somPrintf(
               "Appointment id for this conference call is : %d \n",
             id);
         break;

/*       Remove an appointment.
         ---------------------- */
         case 'r':
           getLine(input, "Enter appointment id > ");
           id = atoi(input);
           apptBook->deleteAppointment(ev, id);
         break;

/*       Display the appointments for a given day.
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
           for (i=0; i< sequenceLength(apptList); i++) {
             appt = sequenceElement(apptList,i);
             bp = appt->bufferize(ev);
             somPrintf("\n%s",bp);
             SOMFree(bp);      // Free the storage from bufferize
           }
           SOMFree(apptList._buffer);
         break;

/*       Display all the appointments.
         ----------------------------- */
         case 'a':
           apptList = apptBook->getAllAppointments(ev);
           for (i=0; i< sequenceLength(apptList); i++) {
               appt = sequenceElement(apptList,i);
               bp = appt->bufferize(ev);
               somPrintf("\n%s",bp);
               SOMFree(bp);
           }
           SOMFree(apptList._buffer);
         break;

/*       Clean up and exit.
         ------------------ */
         case 'q':
         break;

     }
     fflush(NULL); // Flush all open streams
   } while (menu[0] != 'q');

   apptBook->somFree();   // Clean up the appointment book memory, this will
                          // free the list of appointments also.
}
