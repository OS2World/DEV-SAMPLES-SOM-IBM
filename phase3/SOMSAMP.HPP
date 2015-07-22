// 3/8/96 15:42:20
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

#ifndef _SOMSAMP_
#define _SOMSAMP_
#include <iframe.hpp>         // IFrameWindow class header
#include <istattxt.hpp>       // IStaticText class header
#include <iinfoa.hpp>         // IInfoArea class header
#include <imenubar.hpp>       // IMenuBar class header
#include <ikeyevt.hpp>        // IKeyboardEvent class header
#include <ikeyhdr.hpp>        // IKeyboardHandler class header
#include <icmdhdr.hpp>        // ICommandHandler class header
#include <isetcv.hpp>         // ISetCanvas class header
#include <ipushbut.hpp>       // IPushButton class header
#include <isplitcv.hpp>       // Split canvas class
#include <imcelcv.hpp>        // IMultiCellCanvas
#include <imle.hpp>           // Multiline edit
#include <idate.hpp>          // IDate

#include "appt.xh"
#include "apptbook.xh"
#include "fapptbk.xh"
#include "somsamp.h"

#define MAXDAYS 42

class SOMSampWindow : public IFrameWindow,
                      public ICommandHandler,
                      public IKeyboardHandler
{
  public:
    SOMSampWindow(unsigned long windowId);
    virtual ~SOMSampWindow();     // Destructor for this class
    setupCanvas(IDate adate);     // Setup the main day buttons canvas
    setupButtonText(IDate adate); // Setup the main window button text
    void fillApptlist(IDate adate); // Fill the appointment list
    void handleEvent(const char *string); // Process the event string

  protected:
    Boolean command(ICommandEvent& cmdEvent);
    Boolean key(IKeyboardEvent &keyEvent);

  private:
    // Data
    AppointmentBook           *apptBook;
    ExtendedNaming_ExtendedNamingContext *enc;
    AppointmentBookFactory    *factory;
    Boolean                    initOK;
    // Window Data
    IStaticText     *month_txt;
    IDate           *todaysDate;
    IDate           *prevDate;
    IDate           *nextDate;
    IMenuBar         menuBar;
    ISplitCanvas     statusLine;
    IStaticText      monthLine,
                     userLine;
    ISetCanvas       buttons;
    IPushButton      leftButton;
    IPushButton      rightButton;
    IInfoArea        infoArea;
    ISplitCanvas     splitCanvas;
    IMultiCellCanvas daysCanvas;
    IMultiLineEdit   mleAppointments;
    IStaticText     *weekDays[7];
    IPushButton     *monthDays[MAXDAYS]; // some are hidden
    int              monthDay;           // The monthDays button to set focus to
    int              firstDayOfMonth;    // index of first day of the month

    // Methods
    void initSOMenv(Environment *);   // Init the DSOM environment for this client
    void highlightDay(int dayButton); // highlight the given button of monthDays
    SOMSampWindow(const SOMSampWindow&); //Default copy constructor
    SOMSampWindow &operator=(const SOMSampWindow&); //Default assignment operator
};
#endif
