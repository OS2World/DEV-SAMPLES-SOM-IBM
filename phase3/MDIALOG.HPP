// 3/8/96 15:31:49
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

#ifndef STEXTDIALOG_HPP
#define STEXTDIALOG_HPP

#include <iframe.hpp>           // IFrameWindow Class (Parent)
#include <icmdhdr.hpp>          // ICommandHandler (Parent)
#include <ilistbox.hpp>         // IListBox
#include <ientryfd.hpp>         // IEntryField
#include <ispinbt.hpp>          // ISpinButton
#include <istattxt.hpp>         // IStaticText
#include <ipushbut.hpp>         // IPushButton
#include <imcelcv.hpp>          // IMultiCellCanvas
#include <isetcv.hpp>           // ISetCanvas
#include <imle.hpp>             // IMultiLineEdit

#include <appt.xh>
#include <mtg.xh>
#include <ccall.xh>

//**************************************************************************
// Class:   ApptDialog                                                    *
//                                                                         *
// Purpose: Dialog window for the SOM Toolkit sample application.          *
//          It is a subclass of IFrameWindow, ICommandHandler              *
//                                                                         *
//**************************************************************************
class ApptDialog : public IFrameWindow, public ICommandHandler
{
public:
  typedef enum {MEETING, CCALL} apptTypes;
  ApptDialog (Appointment *appt, IWindow *ownerWnd);
  ~ApptDialog();

protected:
  virtual Boolean
    command(ICommandEvent& cmdevt);     //Process the dialog command events

private:
  Appointment   *apptObject;
  IMultiCellCanvas clientCanvas;
  ISetCanvas    buttons;
  IStaticText   txtStarthr;
  IStaticText   txtStophr;
  IStaticText   txtStartsep;
  IStaticText   txtStopsep;
  IStaticText   txtLoc;
  IStaticText   txtSubj;
  IStaticText   txtPhone;
  IStaticText   txtUsers;
  IEntryField   fldLoc;
  IEntryField   fldPhone;
  ISpinButton   fldStarthr;
  ISpinButton   fldStophr;
  ISpinButton   fldStartmin;
  ISpinButton   fldStopmin;
  IMultiLineEdit mleSubj;
  IListBox      lboxUsers;
  IPushButton   btnOk;
  IPushButton   btnCancel;
  apptTypes     apptType;
};

#endif
