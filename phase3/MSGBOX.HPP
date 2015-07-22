// 3/8/96 15:31:55
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

#ifndef _MSGBOX_
#define _MSGBOX_
//****************************************************************************
//Include User Interface Class Library class headers:
  #include <imsgbox.hpp>                //Include IMessageBox class header
  #include <iwindow.hpp>                //Include IWindow class header
  #include <iframe.hpp>                 //Include IFrameWindow class header
  #include <ireslib.hpp>                //Include IResourceId class header
  #include <istring.hpp>                //Include IString class header

#include "somsamp.h"                    //Include symbolic definitions

/**************************************************************************
* Class:   AMsgBox                                                        *
*                                                                         *
* Purpose: A class that displays a message text to the user               *
*                                                                         *
**************************************************************************/
class AMsgBox  : public IFrameWindow
{
public:
   AMsgBox(unsigned long windowId, const IResourceId &resId); //Constructor
   AMsgBox(unsigned long windowId, const char *amessage    ); //Constructor
virtual ~AMsgBox() { };             //Default destructor

private:
   IMessageBox          mb;
};

#endif
