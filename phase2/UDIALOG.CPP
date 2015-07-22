// 3/8/96 15:31:36
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

#ifdef __OS2__
#include <strstrea.h>                  // String stream
#else
#include <strstream.h>                 // String stream
#endif

#include <istring.hpp>                  // IString Class
#include <ireslib.hpp>                  // IResourceLibrary/IResourceId Class
#include <itime.hpp>
#include <imchdr.hpp>

#include "somsamp.h"                    // Include our Symbolic definitions
#include "udialog.hpp"                  // NameDialog Class

//**************************************************************************
//                         1                   2                   3       *
// Col-> 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2   *
// Row                                                                     *
//  1    -----------------   Appointment                                   *
//  2    |               |              ---------------------              *
//  3    |               |   Phone :   |                    |              *
//  4    |               |              ---------------------              *
//  5    |               |              ---------------------              *
//  6    |               |   Location: |                    |              *
//  7    |               |              ---------------------              *
//  8    |               |             ----------------------              *
//  9    |               |   Subject:  |                    |              *
// 10    |               |             |                    |              *
// 11    |               |             |                    |              *
// 12    -----------------             |                    |              *
// 13                                  ----------------------              *
// 14    Users:       ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿       *
// 15                 ³                                            ³       *
// 16                 ³                                            ³       *
// 17                 ³                                            ³       *
// 18                 ³                                            ³       *
// 19                 ³                                            ³       *
// 20                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ       *
// 21   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿                     *
// 22   ³            ³  ³            ³  ³            ³                     *
// 23   ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ                     *
//**************************************************************************
// UpdlDialog :: UpdlDialog - Get a book name                              *
//**************************************************************************
UpdlDialog :: UpdlDialog(_IDL_SEQUENCE_Appointment apptList, long *index,
                         unsigned long windowId, IWindow * ownerWnd)
            : IFrameWindow(windowId,
                           desktopWindow(), // parent
                           ownerWnd,        // owner
                           IRectangle(500,  // width
                                      400), // height
                           IWindow::synchPaint
                            |IWindow::clipSiblings
                            |IWindow::saveBits
                            |systemMenu
                            |titleBar
                            |dialogBorder
                            |dialogBackground
                           ),
              clientCanvas(WND_CANVAS_UPDLDIALOG,this,this),
              buttons(WND_CANVAS_UPDLDIALOGBTN,this,this),
              listBox(LB_APPTS,&clientCanvas,&clientCanvas,IRectangle()),
              multicCanvas(WND_CANVAS_UPDLMCDIALOG,
                               &clientCanvas, &clientCanvas, IRectangle()),
              txtAppointment(STR_APPOINTMENT,&multicCanvas,&multicCanvas),
              txtPhone(STR_PHONE,&multicCanvas,&multicCanvas),
              txtLocation(STR_LOCATION,&multicCanvas,&multicCanvas),
              txtSubject(STR_SUBJECT,&multicCanvas,&multicCanvas),
              fldPhone(DDL_PHONE,&multicCanvas,&multicCanvas),
              fldLocation(DDL_LOCATION,&multicCanvas,&multicCanvas),
              fldSubject(DDL_SUBJECT,&multicCanvas,&multicCanvas),
//            UserCanvas(WND_CANVAS_UPDLUSERDIALOG, &clientCanvas, &clientCanvas),
//            lboxUsers(LB_USERS,&UserCanvas,&UserCanvas),
//            txtUsers(TXT_USERS,&UserCanvas,&UserCanvas),
              btnOk(DID_OK,&buttons,&buttons),
              btnCancel(DID_CANCEL,&buttons,&buttons),
              btnSelect(DID_SELECT,&buttons,&buttons),
              apptList(apptList),
              IDValue(index)
{
  string bp;
  strstream ss;
  Appointment *appt;

  Environment *ev = somGetGlobalEnvironment();

  setClient(&clientCanvas);
  clientCanvas.setOrientation(ISplitCanvas::verticalSplit);
  clientCanvas.setSplitWindowPercentage(&multicCanvas, 60);
  clientCanvas.setSplitWindowPercentage(&listBox, 40);

//UserCanvas.setOrientation(ISplitCanvas::horizontalSplit);

  IWindow::enable(1);
  IHandler::enable(1);
  listBox.enable(1);
  ICommandHandler::handleEventsFor(this); // Set self as command event handler

  if (sequenceLength(apptList)>0) {
     for (int i=0; sequenceLength(apptList)>i; i++) {
        appt = sequenceElement(apptList,i);
        ss << ITime(appt->_get_start(ev)) << " to " << ITime(appt->_get_end(ev)) << endl;
        listBox.addAsLast(bp=ss.str());
        ss.rdbuf()->seekoff(0, ios::beg, ios::in|ios::out);
     }
     ss.rdbuf()->freeze(0); // release string buffer
  }

  txtAppointment.setText(STR_APPOINTMENT);
  txtPhone.setText(STR_PHONE);
  txtLocation.setText(STR_LOCATION);
  txtSubject.setText(STR_SUBJECT);

                                  // Disable until the appointment is selected
  fldLocation.disable();
  fldPhone.disable();
  fldSubject.disable();
  *IDValue = -1;  // Initialize the IDValue

//txtUsers.setText(STR_USERS);

  btnOk.setText(STR_OK);
  btnOk.disable();    // disable until a selection is made
  btnCancel.setText(STR_CANCEL);
  btnSelect.setText(STR_SELECT);
  if (windowId == WND_DELAPPTDIALOG) {     // Set everything to read only
    fldPhone.enableDataUpdate(false);
    fldLocation.enableDataUpdate(false);
    fldSubject.enableDataUpdate(false);
  }

  //                      Field              Col  Row  #Col  #Row
  multicCanvas.addToCell(&txtAppointment  ,    2,   2,   12,   1);

  multicCanvas.addToCell(&txtPhone        ,    2,   3,    2,   1);
  multicCanvas.addToCell(&txtLocation     ,    2,   6,    2,   1);
  multicCanvas.addToCell(&txtSubject      ,    2,   9,    2,   1);

  multicCanvas.addToCell(&fldPhone        ,    6,   3,   10,   1);
  multicCanvas.addToCell(&fldLocation     ,    6,   6,   10,   1);
  multicCanvas.addToCell(&fldSubject      ,    6,   9,   10,   1);

  setFocus();
  buttons.setMargin(ISize());            // Set Canvas Margins to zero
  buttons.setPad(ISize());               // Set Button Canvas Pad to zero
  addExtension(&buttons, IFrameWindow::belowClient,
               (unsigned long)buttons.minimumSize().height()+10);
  listBox.setFocus();
}

//**************************************************************************
// NameDialog :: ~NameDialog - Destructor                                *
//**************************************************************************
UpdlDialog :: ~UpdlDialog()
{
}

//**************************************************************************
// NameDialog :: get_new_phone                                           *
//**************************************************************************
IString UpdlDialog :: get_new_phone()
{
  return (new_phone.asString());
}

//**************************************************************************
// NameDialog :: get_new_location                                          *
//**************************************************************************
IString UpdlDialog :: get_new_location()
{
  return (new_location.asString());
}

//**************************************************************************
// NameDialog :: get_new_subject                                           *
//**************************************************************************
IString UpdlDialog :: get_new_subject()
{
  return (new_subject.asString());
}

//**************************************************************************
// NameDialog :: command - Process Commands                               *
//**************************************************************************
Boolean UpdlDialog :: command(ICommandEvent& cmdevt)
{
  long index=-1;
  Appointment *appt;
  Environment *ev = somGetGlobalEnvironment();

  switch(cmdevt.commandId()) {
    case DID_OK:
                       // If a selection was made
      if ( *IDValue >= 0 ) {
                  // Ok is pressed get the modifications if there are some
        if (sequenceLength(apptList)>0) {
          appt = sequenceElement(apptList,*IDValue);
          if (appt->somIsA(_ConferenceCall)) {
                                              // Check to see if the entry changed
            if (fldPhone.hasChanged()) {
              fldPhone.selectRange();
              new_phone=fldPhone.selectedText();
            }
          } else {
            if (fldLocation.hasChanged()) {
              fldLocation.selectRange();
              new_location=fldLocation.selectedText();
            }
          }
          if (fldSubject.hasChanged()) {
            fldSubject.selectRange();
              new_subject=fldSubject.selectedText();
          }
        } /* endif */
      }
      dismiss(DID_OK);
      return(true);
      break;

    case DID_CANCEL:
      dismiss(DID_CANCEL);
      return(true);
      break;

    case DID_SELECT:
      index = listBox.selection();  // return index to selection
      if (sequenceLength(apptList)>0) {
         btnOk.enable();    // enable ok since a selection is made
         if (index >= 0) {
           appt = sequenceElement(apptList,index);
           if (!fldPhone.isEmpty()) {
             fldPhone.selectRange();
             fldPhone.clear();
           }
           if (!fldLocation.isEmpty()) {
             fldLocation.selectRange();
             fldLocation.clear();
           }
           if (!fldSubject.isEmpty()) {
             fldSubject.selectRange();
             fldSubject.clear();
           }
           if (appt->somIsA(_ConferenceCall)) {
             fldPhone.enable();
             fldPhone.setText(((ConferenceCall *)appt)->_get_phoneNumber(ev));
             fldLocation.disable();
           } else {
             fldLocation.enable();
             fldLocation.setText(((Meeting *)appt)->_get_location(ev));
             fldPhone.disable();
           }
           fldSubject.enable();
           fldSubject.setText(appt->_get_subject(ev));
           *IDValue = index;
         }
      }
      return(true);
      break;
  }/* end switch */

  return(false);  //Allow Default Processing to occur
}

