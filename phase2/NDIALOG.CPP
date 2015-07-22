// 3/8/96 15:31:58
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
#include <imsgbox.hpp>

#include "somsamp.h"                    // Include our Symbolic definitions
#include "ndialog.hpp"                  // NameDialog Class

//**************************************************************************
//                         1                   2                   3       *
// Col-> 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2   *
// Row                                                                     *
//  1                                                                      *
//  2          ---------------------------------------                     *
//  3    Name: |                                     |                     *
//  4          ---------------------------------------                     *
//  5                                                                      *
//  5   ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿  ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿                     *
//  6   ³            ³  ³            ³  ³            ³                     *
//  7   ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ  ÀÄÄÄÄÄÄÄÄÄÄÄÄÙ                     *
//**************************************************************************
// NameDialog :: NameDialog - Get a book name                              *
//**************************************************************************
NameDialog :: NameDialog(IString & Name, unsigned long windowId, IWindow * ownerWnd)
            : IFrameWindow(windowId,
                           desktopWindow(), // parent
                           ownerWnd,        // owner
                           IRectangle(300,  // width
                                      100), // height
                           IWindow::synchPaint
                            |IWindow::clipSiblings
                            |IWindow::saveBits
                            |systemMenu
                            |titleBar
                            |dialogBorder
                            |dialogBackground
                           ),
              clientCanvas(WND_CANVAS_NAMEDIALOG,this,this),
              buttons(WND_CANVAS_NAMEDIALOGBTN,this,this),
              txtName(STR_NAME,&clientCanvas,&clientCanvas),
              fldName(DDL_APPTBKNAME,&clientCanvas,&clientCanvas),
              btnOk(DID_OK,&buttons,&buttons),
              btnCancel(DID_CANCEL,&buttons,&buttons),
              NameValue(Name)
{
  ICommandHandler::handleEventsFor(this); // Set self as command event handler

  txtName.setText(STR_NAME);
  fldName.setText(Name); // Set the text to passed in string
  fldName.setLimit(8);

  btnOk.setText("Ok");
  btnCancel.setText("Cancel");
  if (windowId == WND_NEWAPPTDIALOG) {     // CREATE SETTING BUTTON
    btnSettings = new IPushButton(DID_SETTINGS, &buttons, &buttons);
    btnSettings->setText("Settings...");
    btnSettings->disable();  // Disable for phase 2
  }

  //                      Field         Col  Row  #Col  #Row
  clientCanvas.addToCell(&txtName     ,   1,   3,   6,   1);

  clientCanvas.addToCell(&fldName     ,   7,   3,   10,   1);

  buttons.setMargin(ISize());            // Set Canvas Margins to zero
  buttons.setPad(ISize());               // Set Button Canvas Pad to zero
  addExtension(&buttons, IFrameWindow::belowClient,
               (unsigned long)buttons.minimumSize().height()-10);

  setClient(&clientCanvas);
  fldName.setFocus();
}

//**************************************************************************
// NameDialog :: ~NameDialog - Destructor                                *
//**************************************************************************
NameDialog :: ~NameDialog()
{
}

//**************************************************************************
// NameDialog :: command - Process Commands                               *
//**************************************************************************
Boolean NameDialog :: command(ICommandEvent& cmdevt)
{
  IMessageBox msgBox(this);;

  switch(cmdevt.commandId()) {
    case DID_OK:
      NameValue = fldName.text();
      NameValue = IString::stripBlanks(NameValue);  // strip leading and trailing blanks
      if (NameValue.numWords() > 1) {
        msgBox.show(STR_NAMEMESSAGE1, IMessageBox::okButton | IMessageBox::errorIcon);
      }else if (!NameValue.isAlphanumeric()) {
        msgBox.show(STR_NAMEMESSAGE2, IMessageBox::okButton | IMessageBox::errorIcon);
      }else {
        dismiss(DID_OK);  // Dismiss the main update/delete dialog
      }
      return(true);
      break;

    case DID_SETTINGS:
      dismiss(DID_CANCEL);
      return(true);
      break;

    case DID_CANCEL:
      dismiss(DID_CANCEL);
      return(true);
      break;
  }/* end switch */

  return(false);  //Allow Default Processing to occur
}
