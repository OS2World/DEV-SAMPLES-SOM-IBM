// 3/8/96 15:31:47
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

#include <ientryfd.hpp>                 // IEntryField Class
#include <icmdevt.hpp>                  // ICommandEvent
#include <istring.hpp>                  // IString Class
#include <ireslib.hpp>                  // IResourceLibrary/IResourceId Class
#include <itime.hpp>                    // ITime Class

#include "somsamp.h"                    // Include our Symbolic definitions
#include "mdialog.hpp"                  // ApptDialog Class

#include <appt.xh>
#include <mtg.xh>
#include <ccall.xh>

//**************************************************************************
//                         1                   2                   3       *
// Col-> 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2   *
// Row                                                                     *
//  1                                                                      *
//  2                  Hr    Min                                           *
//  3    Start time:  ÚÄÄ¿  ÚÄÄ¿                                           *
//  4                 ³  ³ :³  ³                                           *
//  5                 ÀÄÄÙ  ÀÄÄÙ                                           *
//  6                                                                      *
//  7    End time:    ÚÄÄ¿  ÚÄÄ¿                                           *
//  8                 ³  ³ :³  ³                                           *
//  9                 ÀÄÄÙ  ÀÄÄÙ                                           *
// 10                                                                      *
//  1    Phone:       ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿       *
//  2                 ³                                            ³       *
//  3                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ       *
//  4                                                                      *
//  5    Location:    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿       *
//  6                 ³                                            ³       *
//  7                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ       *
//  8                                                                      *
//  9    Subject:     ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿       *
// 20                 ³                                            ³       *
//  1                 ³                                            ³       *
//  2                 ³                                            ³       *
//  3                 ³                                            ³       *
//  4                 ³                                            ³       *
//  5                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ       *
//  6                                                                      *
//  7    For user:    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿       *
//  8                 ³                                            ³       *
//  9                 ³                                            ³       *
// 30                 ³                                            ³       *
//  1                 ³                                            ³       *
//  2                 ³                                            ³       *
//  3                 ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ       *
//  4                                                                      *
//  5   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿                                     *
//  6   ³            ³  ³            ³                                     *
//  7   ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ                                     *
//**************************************************************************
// ApptDialog :: ApptDialog - add a new meeting                            *
//**************************************************************************
ApptDialog :: ApptDialog(Appointment *appt, IWindow * ownerWnd)
            : IFrameWindow(WND_APPTDIALOG,
                           desktopWindow(), // parent
                           ownerWnd,        // owner
                           IRectangle(475,  // width
                                      500), // height
                           IWindow::synchPaint
                            |IWindow::clipSiblings
                            |IWindow::saveBits
                            |systemMenu
                            |titleBar
                            |dialogBorder
                            |dialogBackground,
                           "Create an Appointment"
                           ),
              clientCanvas(WND_CANVAS_APPTDIALOG,this,this),
              buttons(WND_CANVAS_APPTDIALOGBTN,this,this),
              txtStarthr(TXT_STARTHR,&clientCanvas,&clientCanvas),
              txtStophr(TXT_STOPHR,&clientCanvas,&clientCanvas),
              txtStartsep(TXT_STARTSEP,&clientCanvas,&clientCanvas),
              txtStopsep(TXT_STOPSEP,&clientCanvas,&clientCanvas),
              txtLoc(TXT_LOC,&clientCanvas,&clientCanvas),
              txtSubj(TXT_SUBJ,&clientCanvas,&clientCanvas),
              txtPhone(TXT_PHONE,&clientCanvas,&clientCanvas),
              txtUsers(TXT_USERS,&clientCanvas,&clientCanvas),
              fldStarthr(FLD_STARTHR,&clientCanvas,&clientCanvas),
              fldStophr(FLD_STOPHR,&clientCanvas,&clientCanvas),
              fldStartmin(FLD_STARTMIN,&clientCanvas,&clientCanvas),
              fldStopmin(FLD_STOPMIN,&clientCanvas,&clientCanvas),
              fldLoc(FLD_LOC,&clientCanvas,&clientCanvas),
              fldPhone(FLD_PHONE,&clientCanvas,&clientCanvas),
              mleSubj(FLD_SUBJ,&clientCanvas,&clientCanvas),
              lboxUsers(LB_USERS,&clientCanvas,&clientCanvas),
              btnOk(DID_OK,&buttons,&buttons),
              btnCancel(DID_CANCEL,&buttons,&buttons)
{
  ITime ctime;
  apptObject=(Appointment *)appt;
  if (appt->somIsA(_Meeting))
     apptType = MEETING;
  else
     apptType = CCALL;
  setClient(&clientCanvas);
  ICommandHandler::handleEventsFor(this); // Set self as command event handler

  fldStarthr.setInputType(ISpinButton::numeric)
                          .setLimit(2UL)
                          .setAlignment(ISpinButton::right)
                          .setRange(IRange(0,23))
                          .setCurrent((long)ctime.hours());
  fldStartmin.setInputType(ISpinButton::numeric)
                          .setLimit(2UL)
                          .setAlignment(ISpinButton::right)
                          .setRange(IRange(0,59))
                          .setCurrent((long)ctime.minutes());
  fldStophr.setInputType(ISpinButton::numeric)
                          .setLimit(2UL)
                          .setAlignment(ISpinButton::right)
                          .setRange(IRange(0,23))
                          .setCurrent((long)ctime.hours());
  fldStopmin.setInputType(ISpinButton::numeric)
                          .setLimit(2UL)
                          .setAlignment(ISpinButton::right)
                          .setRange(IRange(0,59))
                          .setCurrent((long)ctime.minutes());

  txtStarthr.setText(STR_START);
  txtStophr.setText(STR_STOP);
  txtStartsep.setText(":");
  txtStopsep.setText(":");
  if (apptType == CCALL)
     txtPhone.setText(STR_PHONE);
  else
     txtLoc.setText(STR_LOCATION);
  txtSubj.setText(STR_SUBJECT);
  txtUsers.setText(STR_USERS);
  btnOk.setText(STR_OK);
  btnCancel.setText(STR_CANCEL);
  //                      Field         Col  Row  #Col  #Row
  clientCanvas.addToCell(&txtStarthr  ,   1,   3,   6,   1);
  clientCanvas.addToCell(&txtStophr   ,   1,   7,   6,   1);
  if (apptType == CCALL)
     clientCanvas.addToCell(&txtPhone ,   1,  11,   6,   1);
  else
     clientCanvas.addToCell(&txtLoc   ,   1,  11,   6,   1);
  clientCanvas.addToCell(&txtSubj     ,   1,  15,   6,   1);
  clientCanvas.addToCell(&txtUsers    ,   1,  24,   6,   1);

  clientCanvas.addToCell(&fldStarthr  ,   7,   3,   2,   1);
  clientCanvas.addToCell(&fldStartmin ,  11,   3,   2,   1);
  clientCanvas.addToCell(&fldStophr   ,   7,   7,   2,   1);
  clientCanvas.addToCell(&fldStopmin  ,  11,   7,   2,   1);

  if (apptType == CCALL)
     clientCanvas.addToCell(&fldPhone ,   7,  11,  22,   2);
  else
     clientCanvas.addToCell(&fldLoc   ,   7,  11,  22,   2);
  clientCanvas.addToCell(&mleSubj     ,   7,  15,  22,   7);
  clientCanvas.addToCell(&lboxUsers   ,   7,  24,  22,   7);

  setFocus();
  buttons.setMargin(ISize());            // Set Canvas Margins to zero
  buttons.setPad(ISize());               // Set Button Canvas Pad to zero
  addExtension(&buttons, IFrameWindow::belowClient,
               (unsigned long)buttons.minimumSize().height()+10);
  fldStarthr.setFocus();
}

//**************************************************************************
// ApptDialog :: ~ApptDialog - Destructor                                *
//**************************************************************************
ApptDialog :: ~ApptDialog()
{
}

//**************************************************************************
// ApptDialog :: command - Process Commands                               *
//**************************************************************************
Boolean ApptDialog :: command(ICommandEvent& cmdevt)
{
  Environment *ev = somGetGlobalEnvironment();
  ITime starttime(fldStarthr.value(), fldStartmin.value());
  ITime stoptime(fldStophr.value(), fldStopmin.value());

  switch(cmdevt.commandId()) {
    case DID_OK:
      switch (apptType) {
         case MEETING:
           apptObject->_set_start(ev,starttime.asSeconds());
           apptObject->_set_end(ev,stoptime.asSeconds());
           apptObject->_set_subject(ev,mleSubj.text());
           ((Meeting *)apptObject)->_set_location(ev,fldLoc.text());
           break;
         case CCALL:
           apptObject->_set_start(ev,starttime.asSeconds());
           apptObject->_set_end(ev,stoptime.asSeconds());
           apptObject->_set_subject(ev,mleSubj.text());
           ((ConferenceCall *)apptObject)->_set_phoneNumber(ev,fldPhone.text());
           break;
         default:
           break;
      } /* End switch*/
      dismiss(DID_OK);
      return(true);
      break;

    case DID_CANCEL:
      dismiss(DID_CANCEL);
      return(true);
      break;
  }/* end switch */

  return(false);  //Allow Default Processing to occur
}
