// 3/8/96 15:42:17
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
#ifndef _IBASE_
#include <ibase.hpp>
#endif
#include <iapp.hpp>                    // IApplication class
#include <ifont.hpp>                   // IFont class
#include <istring.hpp>                 // IString class
#include <isplitcv.hpp>                // Split canvas class
#include <itime.hpp>                   // ITime class
#ifdef __OS2__
#include <strstrea.h>                  // String stream
#else
#include <strstream.h>                 // String stream
#endif

#include <somd.xh>    // Include DSOM class definition headers
#include <somdcprx.xh>
                      // Include the base class definition headers.
#include "appt.xh"
#include "mtg.xh"
#include "ccall.xh"
#include "apptbook.xh"
#include "key.xh"
#include "samptype.h"
                      // Include the GUI class definition headers.
#include "somsamp.h"
#include "somsamp.hpp"
#include "mdialog.hpp"                 // ApptDialog Class
#include "ndialog.hpp"                 // NameDialog Class
#include "udialog.hpp"                 // UpdlDialog Class
#include "msgbox.hpp"                  // Message Box Class
                      // Include the distributed class definition headers.
#include "dapptbk.xh"
#include "fapptbk.xh"

#define MAX_ROW     8
#define MAX_COL     7

void initClasses();
//*************************************************************************
// main  - Application entry point                                        *
//*************************************************************************
void main()
{
  SOMSampWindow mainWindow (WND_APPTBK);
  mainWindow.show();
  IApplication::current().run();
} /* end main */


//**************************************************************************
// SOMSampWindow :: SOMSampWindow - Constructor for our main window        *
//**************************************************************************
SOMSampWindow :: SOMSampWindow(unsigned long windowId)
  : IFrameWindow(windowId,
                 desktopWindow(), // parent
                 desktopWindow(), // owner
                 IRectangle(),
                 IFrameWindow::defaultStyle() |
                 IFrameWindow::minimizedIcon  |
                 IFrameWindow::accelerator)
   ,menuBar(windowId, this)
   ,statusLine(WND_STATUS, this, this)
   ,monthLine(WND_MONTH, &statusLine, &statusLine)
   ,userLine(WND_USER, &statusLine, &statusLine)
   ,buttons(WND_BUTTONS, this, this)
   ,leftButton(MI_VIEWP, &buttons, &buttons)
   ,rightButton(MI_VIEWN, &buttons, &buttons)
   ,infoArea(this)
   ,splitCanvas (WND_SPLTCANVAS, this, this)
   ,daysCanvas(WND_MCCANVAS, &splitCanvas, &splitCanvas,
               IRectangle(),
               IMultiCellCanvas::defaultStyle())
   ,mleAppointments(WND_MCCANVAS, &splitCanvas, &splitCanvas,
                    IRectangle(),
                    IMultiLineEdit::defaultStyle() |
                    IMultiLineEdit::readOnly       |
                    IMultiLineEdit::verticalScroll |
                    IMultiLineEdit::ignoreTab)
{
  int i, row, col;
  long w,h;

  setClient(&splitCanvas);
  splitCanvas.setOrientation(ISplitCanvas::verticalSplit);

  statusLine.setOrientation(ISplitCanvas::verticalSplit);
  addExtension(&statusLine, IFrameWindow::aboveClient,
               IFont(&statusLine).maxCharHeight());

  infoArea.setInactiveText(STR_INFO);

  leftButton.enableTabStop();
  buttons.setMargin(ISize());
  buttons.setPad(ISize());
  unsigned long bh = (unsigned long)buttons.minimumSize().height();
  addExtension(&buttons, IFrameWindow::belowClient,
               (unsigned long)buttons.minimumSize().height()+10);

  splitCanvas.setSplitWindowPercentage(&daysCanvas, 68);
  splitCanvas.setSplitWindowPercentage(&mleAppointments, 32);
  todaysDate = new IDate();
  prevDate = (IDate*)NULL;
  nextDate = (IDate*)NULL;
  // Set up text windows for the days of the week
  for (i = 0; i<7; i++)
  {
     weekDays[i] = new IStaticText(WND_WEEKDAYS+i, &daysCanvas, &daysCanvas);
     weekDays[i]->setAlignment(IStaticText::centerCenter);
     daysCanvas.addToCell(weekDays[i],
                          i+1,         // column (1 to 7)
                          2);          // row
  }
  weekDays[0]->setText("Sun");
  weekDays[1]->setText("Mon");
  weekDays[2]->setText("Tue");
  weekDays[3]->setText("Wed");
  weekDays[4]->setText("Thu");
  weekDays[5]->setText("Fri");
  weekDays[6]->setText("Sat");
  // Set up the Push Button windows for the days of the Month
  for (row=3, i=0; row<9; row++)
     for (col=1; col<8; col++, i++) {
        monthDays[i] = new IPushButton(WND_DAYS+i,
                                       &daysCanvas,
                                       &daysCanvas,
                                       IRectangle(),
                                       IPushButton::defaultStyle()
                                       );
        daysCanvas.addToCell(monthDays[i], col, row);
     }
  sizeTo(ISize(600,   // width
               475)); // height
  setupCanvas(*todaysDate);
  setupButtonText(*todaysDate);

  ICommandHandler::handleEventsFor(this);
  IKeyboardHandler::handleEventsFor(this);

  apptBook = (AppointmentBook *)OBJECT_NIL;
  enc    = (ExtendedNaming_ExtendedNamingContext *)OBJECT_NIL;
  factory= (AppointmentBookFactory *)OBJECT_NIL;
  initOK = FALSE;
}

SOMSampWindow :: ~SOMSampWindow()
{
  ICommandHandler::stopHandlingEventsFor(this);
}

//**************************************************************************
// SOMSampWindow :: setupCanvas                                            *
//                                                                         *
//                                                 MAX_COL                 *
// Col->  1     2      3      4      5      6      7                       *
// Row                                                                     *
//                                                                         *
//  1    Sun    Mon    Tue    Wed    Thu    Fri    Sat                     *
//                                                                         *
//  2                                                                      *
//                                                                         *
//  3   ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿                    *
//      ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ                    *
//  4   ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿                    *
//      ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ                    *
//  5   ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿                    *
//      ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ                    *
//  6   ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿                    *
//      ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ                    *
//  7   ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿                    *
//      ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ                    *
//  8   ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿  ÚÄÄÄ¿                    *
//      ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ  ÀÄÄÄÙ                    *
//  MAX_ROW                                                                *
//                                                                         *
//                                                                         *
//                                                                         *
//**************************************************************************
SOMSampWindow :: setupCanvas(IDate adate)
{
  static int colMap[7] = { 2, 3, 4, 5, 6, 7, 1 };
  char buffer[3];
  IDate *firstDayOtheMonth;
  int initCol, i, col, day, daysInMonth;

  daysInMonth = adate.daysInMonth(adate.monthOfYear(),
                                        adate.year());
  firstDayOtheMonth = new IDate(adate.monthOfYear(),
                                1,
                                adate.year());
  // this is the index of the first day of the month
  initCol = colMap[(int)(firstDayOtheMonth->dayOfWeek())];
  firstDayOfMonth = monthDay = initCol - 1;

  /* Do first row */
  for (col=1, i=0, day=1; col<MAX_COL+1; col++, i++)
     if (col<initCol)
        monthDays[i]->hide();
     else {
        sprintf(buffer, "%d", day++);
        monthDays[i]->setText(buffer);
        if (!monthDays[i]->isVisible())
           monthDays[i]->show();
     }
  /* Do for the remainder of the month */
  for (; day<=daysInMonth; i++) {
     sprintf(buffer, "%d", day++);
     monthDays[i]->setText(buffer);
     if (!monthDays[i]->isVisible())
        monthDays[i]->show();

  }
  /* Do for whatevers left */
  for (; i<MAXDAYS; i++)
     monthDays[i]->hide();

  if (adate.monthOfYear() == IDate::today().monthOfYear()) {
     highlightDay(monthDay+IDate::today().dayOfMonth()-1);
  } else {
     highlightDay(monthDay);
  }

  delete firstDayOtheMonth;
  return(0);
}

//**************************************************************************
// SOMSampWindow :: highlightDay                                           *
//**************************************************************************
void SOMSampWindow :: highlightDay(int dayButton)
{
   IDate *newDate;
   monthDays[monthDay]->unhighlight();
   monthDays[monthDay]->disableDefault();

   monthDays[dayButton]->show();
   monthDays[dayButton]->setFocus();
   monthDays[dayButton]->highlight();
   monthDays[dayButton]->enableDefault();
   monthDay = dayButton;

   newDate = new IDate(todaysDate->monthOfYear(),
                       dayButton           // index of selected button
                       -firstDayOfMonth    // index of first day
                       +1,
                       todaysDate->year());
   delete todaysDate;
   todaysDate = newDate;
}

//**************************************************************************
// SOMSampWindow :: fillApptlist                                                *
//**************************************************************************
void SOMSampWindow :: fillApptlist(IDate adate)
{
   string bp;
   strstream ss;
   Appointment *appt;
   _IDL_SEQUENCE_Appointment apptList;
   Environment *ev = somGetGlobalEnvironment();

   if (apptBook) {
     apptList = apptBook->getDaysAppointments(ev,
                                              adate.year(),
                                              adate.monthOfYear(),
                                              adate.dayOfMonth());

     mleAppointments.removeAll();
     mleAppointments.addAsLast(adate.asString());
     if (sequenceLength(apptList)>0) {
        for (int i=sequenceLength(apptList)-1; i>=0; i--) {
           appt = sequenceElement(apptList,i);
           ss << endl;
           ss << "\nStart: " << ITime(appt->_get_start(ev)) << endl;
           ss << "End: " << ITime(appt->_get_end(ev)) << endl;
           if (appt->somIsA(_ConferenceCall))
              ss << "Phone: " << ((ConferenceCall *)appt)->_get_phoneNumber(ev) << endl;
           else
              ss << "Location: " << ((Meeting *)appt)->_get_location(ev) << endl;
           ss << "Subject: " << appt->_get_subject(ev) << endl;
           ss << "___________" << ends;
           mleAppointments.addAsLast(bp=ss.str());
           ss.rdbuf()->seekoff(0, ios::beg, ios::in|ios::out);
        }
        ORBfree(apptList._buffer); // Free buffer DSOM will free buffer elements
        ss.rdbuf()->freeze(0);     // release string buffer
     }
   } /* endif */
}

//**************************************************************************
// SOMSampWindow :: setupButtonText                                        *
//**************************************************************************
SOMSampWindow :: setupButtonText(IDate adate)
{
  static char statusBuf[80];
#define Jan 1
#define Dec 12
  int prev, next;
  sprintf(statusBuf, "%s %d", adate.monthName(), adate.year());
  monthLine.setText(statusBuf);
  prev = ((int)adate.monthOfYear()-1) % (Dec+1);
  next = ((int)adate.monthOfYear()+1) % (Dec+1);
  if (prevDate)
     delete prevDate;
  if (nextDate)
     delete nextDate;
  if (prev) {
     prevDate = new IDate((IDate::Month)prev, 1, adate.year());
  } else {
     prevDate = new IDate((IDate::Month)Dec, 1, adate.year()-1);
  }
  if (next) {
     nextDate = new IDate((IDate::Month)next, 1, adate.year());
  } else {
     nextDate = new IDate((IDate::Month)Jan, 1, adate.year()+1);
  }


  // Note: the following doesn't appear to make any difference on AIX!!!???
  // Need better solution there
  leftButton.sizeTo(ISize(10*IFont(&leftButton).avgCharWidth(),    // width
                          leftButton.size().height()));            // height
  leftButton.sizeTo(ISize(10*IFont(&rightButton).avgCharWidth(),    // width
                          rightButton.size().height()));            // height

  leftButton.setText(prevDate->monthName());
  rightButton.setText(nextDate->monthName());
  return(0);
}

//*************************************************************************
// Boolean SOMSampWindow :: command(ICommandEvent &cmdEvent)
// SOMSampWindow :: command - handle window commands                      *
//************************************************************************/
Boolean SOMSampWindow :: key(IKeyboardEvent &keyEvent)
{
  Boolean eventProcessed=false;
  IKeyboardEvent::VirtualKey vk;

  if (keyEvent.isVirtual()&&keyEvent.isUpTransition()) {
     vk = keyEvent.virtualKey();
     switch (vk) {
        case IKeyboardEvent::pageUp:
           setupCanvas(*prevDate);
           setupButtonText(*prevDate);  // modifies prevDate, nextDate
           fillApptlist(*todaysDate);   // Fill the MLE area with days appts
           break;
        case IKeyboardEvent::pageDown:
           setupCanvas(*nextDate);
           setupButtonText(*nextDate);  // modifies nextDate, prevDate
           fillApptlist(*todaysDate);   // Fill the MLE area with days appts
           break;
        default:
         eventProcessed=false;
     } /* end switch */
  }
  return(eventProcessed);
}

//*************************************************************************
// SOMSampWindow :: command - handle window commands                      *
//************************************************************************/
Boolean SOMSampWindow :: command(ICommandEvent &cmdEvent)
{
  Boolean eventProcessed(true);         // Assume event will be processed
  Environment *ev = somGetGlobalEnvironment();
  unsigned short value;                 //Return value from dialog
  NameDialog *nameDialog;
  UpdlDialog *updlDialog;
  Appointment *appt, *appt_tmp;

  unsigned long cmd;
  IDType id;
  _IDL_SEQUENCE_Appointment apptList;
  static char statusBuf[80];
  long list_index;

  cmd = cmdEvent.commandId();
  if (cmd >= WND_DAYS && cmd <= WND_DAYSLAST) {
     highlightDay(cmd-WND_DAYS);           // Select new day
     fillApptlist(*todaysDate);            // Fill the MLE area with days appts
  } else {
     switch (cmd) {
       case MI_VIEWP:                      // Code to Process Left Command Item
         setupCanvas(*prevDate);           // modifies todaysDate to prev month
         setupButtonText(*prevDate);       // modifies prevDate, nextDate
         fillApptlist(*todaysDate);        // Fill the MLE area with days appts
         break;

       case MI_VIEWN:                      // Code to Process Right Command Item
         setupCanvas(*nextDate);           // modifies todaysDate to next month
         setupButtonText(*nextDate);       // modifies nextDate, prevDate
         fillApptlist(*todaysDate);        // Fill the MLE area with days appts
         break;

       case MI_APPTBK_FILE_NEW:   // "New" menu item
       case MI_APPTBK_FILE_OPEN:  // "Open"
       {
         IString Name;
         Name = getenv("USER");
         if (cmd == MI_APPTBK_FILE_NEW)
            nameDialog=new NameDialog(Name,
                                       WND_NEWAPPTDIALOG, this);
         else
            nameDialog=new NameDialog(Name,
                                       WND_OPENAPPTDIALOG, this);

         nameDialog->showModally();      // Show this Text Dialog as Modal

         value=nameDialog->result();     // Get result (eg OK or Cancel)
         if (value != DID_CANCEL) {      // Set new string if not canceled
           if (!initOK) initSOMenv(ev);  // Init environment if not set
           if (initOK) {                 // If everything initialized continue
             if (cmd == MI_APPTBK_FILE_NEW)
                            // create an AppointmentBook object
               apptBook = factory->createAppointmentBook (ev, Name.asString());
             else
                            // check out an existing AppointmentBook object
               apptBook = factory->checkOutAppointmentBook (ev, Name.asString());
             if (!checkEnv(ev)) {
                                 // Add the book name to the dialog status line
               sprintf(statusBuf, "%s %s", "Calendar for: ", Name.asString());
               userLine.setText(statusBuf);

               if (cmd == MI_APPTBK_FILE_NEW)
                 ((DistributedAppointmentBook *)apptBook)->_set_bookName(ev, Name.asString());
               else
                 fillApptlist(*todaysDate);   // re-fill the MLE display
                                              // Disable NEW and OPEN
               menuBar.disableItem(MI_APPTBK_FILE_NEW);
               menuBar.disableItem(MI_APPTBK_FILE_OPEN);
               menuBar.enableItem(MI_APPTBK_FILE_DELETE);
                                              // Enable Appointment pulldown
               menuBar.enableItem(MI_APPTBK_APPOINTMENT_UPDATE);
               menuBar.enableItem(MI_APPTBK_APPOINTMENT_DELETE);
               menuBar.enableItem(MI_APPTBK_APPOINTMENT_NEW);
             }
           }  /* initOK = true */
         } /* DID_CANCEL */
         delete nameDialog;
         break;
       }

      case MI_APPTBK_FILE_DELETE:  // "Delete"
       {
         factory->deleteAppointmentBook (ev, apptBook);  // Delete the servers book
         ((SOMDClientProxy *)apptBook)->somdProxyFree(ev);  // Free the client proxy
         apptBook = (AppointmentBook *)OBJECT_NIL;   // Set to un-instantiated object

         userLine.setText(IString());                             // Clear the calendar
         mleAppointments.removeAll();
                                          // Enable NEW and OPEN
         menuBar.enableItem(MI_APPTBK_FILE_NEW);
         menuBar.enableItem(MI_APPTBK_FILE_OPEN);
         menuBar.disableItem(MI_APPTBK_FILE_DELETE);
                                          // Disable Appointment pulldown
         menuBar.disableItem(MI_APPTBK_APPOINTMENT_UPDATE);
         menuBar.disableItem(MI_APPTBK_APPOINTMENT_DELETE);
         menuBar.disableItem(MI_APPTBK_APPOINTMENT_NEW);

         break;
       }

       case MI_APPTBK_FILE_EXIT:  // "Exit"
       {
         if (apptBook) {
                                  // check in the book since this user is done
           factory->checkInAppointmentBook (ev, apptBook);
                                  // Release the book proxy and other proxys
           ((SOMDClientProxy *)apptBook)->somdProxyFree(ev);
         }
         if (factory) {
           ((SOMDClientProxy *)factory)->somdProxyFree(ev);
           ((SOMDClientProxy *)enc)->somdProxyFree(ev);
         }
         SOMD_Uninit(ev);         // Free the DSOM Environment
         close();
         break;
       }

       case MI_APPTBK_APPOINTMENT_NEW_MEETING:  // "Meeting"
       case MI_APPTBK_APPOINTMENT_NEW_CONFERENCE_CALL:  // "Conference call"
       {
         if (cmd==MI_APPTBK_APPOINTMENT_NEW_MEETING)
            appt = factory->createMeeting(ev);
         else
            appt = factory->createConferenceCall(ev);
                                       // Initialize with todays date
         appt->_set_month(ev, todaysDate->monthOfYear());
         appt->_set_day(ev, todaysDate->dayOfMonth());
         appt->_set_year(ev, todaysDate->year());

         infoArea.setInactiveText(STR_INFODLG);
         ApptDialog * mtgDialog=new ApptDialog((Meeting *)appt, this);
         mtgDialog->showModally();      // Show the meeting Dialog as Modal

         value=mtgDialog->result();     // Get result (eg OK or Cancel)
         if (value != DID_CANCEL) {
                                        // Add the appointment
            id = apptBook->addAppointment(ev, appt);
            if (!checkEnv(ev))
              fillApptlist(*todaysDate);// Refill the MLE area
         }
         factory->deleteAppointment(ev,appt);            // Delete the appointment to reduce user count
         infoArea.setInactiveText(STR_INFO);
         delete mtgDialog;
         break;
       }

       case MI_APPTBK_APPOINTMENT_UPDATE:  // "Update"
       {
                                 // Get the list of appointments for update
         apptList = apptBook->getDaysAppointments(ev,
                                         todaysDate->year(),
                                         todaysDate->monthOfYear(),
                                         todaysDate->dayOfMonth());
         if (sequenceLength(apptList)>0) {
           updlDialog=new UpdlDialog(apptList, &list_index, WND_UPDAPPTDIALOG, this);
           updlDialog->showModally();      // Show this Text Dialog as Modal
           value=updlDialog->result();     // Get result (eg OK or Cancel)
           if (value != DID_CANCEL) {
                                  // Get the appointment selected for update
             appt = sequenceElement(apptList,list_index);
                           // Create an appointment to add and set changed data
             if (appt->somIsA(_ConferenceCall)) {
                appt_tmp = factory->createConferenceCall(ev);
                ((ConferenceCall *)appt_tmp)->_set_phoneNumber(ev,
                            updlDialog->get_new_phone());
             }
             else {
                appt_tmp = factory->createMeeting(ev);
                ((Meeting *)appt_tmp)->_set_location(ev,
                            updlDialog->get_new_location());
             }
             appt_tmp->_set_month(ev, appt->_get_month(ev));
             appt_tmp->_set_day(ev, appt->_get_day(ev));
             appt_tmp->_set_year(ev, appt->_get_year(ev));
             appt_tmp->_set_start(ev, appt->_get_start(ev) );
             appt_tmp->_set_end(ev, appt->_get_end(ev));
             appt_tmp->_set_subject(ev, updlDialog->get_new_subject());
                        // Delete the old appointment using the appointment id
                        // Must delete first to avoid time/date overlap exception
             apptBook->deleteAppointment(ev, appt->_get_apptId(ev));
             if (!checkEnv(ev)) {
                                                // Add the changed book
               id = apptBook->addAppointment(ev, appt_tmp);
               if (!checkEnv(ev))
                 fillApptlist(*todaysDate);  // Refill the MLE area
             }
             factory->deleteAppointment(ev,appt_tmp); //delete appointment to reduce user count
           }
           delete updlDialog;
                                         // Free the sequence buffer
           ORBfree(apptList._buffer);
         }
         break;
       }

       case MI_APPTBK_APPOINTMENT_DELETE:  // "Delete"
       {
                                 // Get the list of appointments for delete
         apptList = apptBook->getDaysAppointments(ev,
                                         todaysDate->year(),
                                         todaysDate->monthOfYear(),
                                         todaysDate->dayOfMonth());
         if (sequenceLength(apptList)>0) {
           updlDialog=new UpdlDialog(apptList, &list_index, WND_DELAPPTDIALOG, this);
           updlDialog->showModally();      // Show this Text Dialog as Modal
           value=updlDialog->result();     // Get result (eg OK or Cancel)
           if (value != DID_CANCEL) {
                                  // Get the appointment selected for delete
             appt = sequenceElement(apptList,list_index);
                             // Delete the appointment using the appointment id
             apptBook->deleteAppointment(ev, appt->_get_apptId(ev));
             fillApptlist(*todaysDate); // Fill the MLE area
           }
           delete updlDialog;
           break;
                                           // Free the sequence buffer
           ORBfree(apptList._buffer);
         }

         break;
       }

       case MI_APPTBK_SETTINGS_APPOINTMENT_BOOK_SETTINGS:  // "Appointment book settings"
       {
         // Handle the menu item "Appointment book settings" here.
         // The menu item isn't linked to any action

         eventProcessed=false;             // the event wasn't processed
         break;
       }

       default:                            //Otherwise,
         eventProcessed=false;             // the event wasn't processed
     } /* end switch */
  } /* end if */

  return(eventProcessed);
} /* end ACommandHandler :: command(...) */

//*************************************************************************
// SOMSampWindow :: handleEvent - handle the event information            *
//************************************************************************/
void SOMSampWindow :: handleEvent(const char *)
{
 /*
   if this userid is in the string then
     if the date is todaysdate then fillapptlist
     display the msgbox to indicate the update
     mbox = new AMsgBox(WND_APPTBK,STR_INFOMESSAGE1);
 */
}

//*************************************************************************
// SOMSampWindow :: initSOMenv - Initialize the DSOM environment          *
//************************************************************************/
void SOMSampWindow :: initSOMenv(Environment *ev)
{

  if (!initOK) {                // Only initialize 1 time for each client
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
            initOK = TRUE;
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
  }
}

//*************************************************************************
// Initialize the sample classes                                          *
//************************************************************************/
void initClasses()
{
AppointmentNewClass(0,0);
AppointmentBookNewClass(0,0);
ConferenceCallNewClass(0,0);
Appt_KeyNewClass(0,0);
MeetingNewClass(0,0);
}
