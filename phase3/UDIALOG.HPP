// 3/8/96 15:32:12
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

#ifndef UPDLDIALOG_HPP
#define UPDLDIALOG_HPP

#include <iframe.hpp>           // IFrameWindow Class (Parent)
#include <icmdhdr.hpp>          // ICommandHandler (Parent)
#include <imchdr.hpp>
#include <ientryfd.hpp>         // IEntryField
#include <istattxt.hpp>         // IStaticText
#include <ipushbut.hpp>         // IPushButton
#include <imcelcv.hpp>          // IMultiCellCanvas
#include <isetcv.hpp>           // ISetCanvas
#include <ilistbox.hpp>         // IListBox
#include <isplitcv.hpp>

#include "appt.xh"
#include "ccall.xh"
#include "mtg.xh"
#include "apptbook.xh"
//**************************************************************************
// Class:   UpdlDialog                                                    *
//                                                                         *
// Purpose: Dialog window for the SOM Toolkit sample application.          *
//          It is a subclass of IFrameWindow, ICommandHandler              *
//                                                                         *
//**************************************************************************
class UpdlDialog : public IFrameWindow, public ICommandHandler
{
public:
  UpdlDialog (_IDL_SEQUENCE_Appointment apptList, long *index,
                unsigned long windowId, IWindow *ownerWnd);
  ~UpdlDialog();
  IString get_new_phone();
  IString get_new_location();
  IString get_new_subject();

protected:
  virtual Boolean
    command(ICommandEvent& cmdevt);     //Process the dialog command events

private:
  ISplitCanvas      clientCanvas;
  IMultiCellCanvas  multicCanvas;
  IListBox          listBox;
  IStaticText       txtPhone,
                    txtLocation,
                    txtAppointment,
                    txtSubject;
//                  txtUsers;
  IEntryField       fldPhone,
                    fldLocation,
                    fldSubject;
  ISetCanvas        buttons;
//ISplitCanvas      UserCanvas;
//IListBox          lboxUsers;
  IPushButton       btnOk,
                    btnCancel,
                    btnSelect;
  _IDL_SEQUENCE_Appointment apptList;
  long             *IDValue;
  IString           new_subject,
                    new_location,
                    new_phone;
};

#endif
