// 3/8/96 15:32:06
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

#ifndef SOMSAMPLEWINDOW_H
#define SOMSAMPLEWINDOW_H

//**************************************************************************
// window ids - used by IWindow constructors (eg IStaticText, AHelloWindow)*
//**************************************************************************
#define WND_APPTBK            1000
#define WND_MCCANVAS          1010
#define WND_WEEKDAYS          1020         // range 1020 1027
#define WND_WEEKDAYSLAST      1027         //
#define WND_DAYS              1050         // range 1050 to 1092
#define WND_DAYSLAST          1092         //
#define WND_SPLTCANVAS        1100
#define WND_STATUS            1102         //Status Line Window ID
#define WND_MONTH             1103         // month part of status
#define WND_USER              1104         // User part of status
#define WND_MLE               1105
#define WND_BUTTONS           1106         //Button Canvas Window ID

#define WND_CANVAS_APPTDIALOG 1110         //Add a new appointment
#define WND_CANVAS_APPTDIALOGBTN 1112      //Add a new appointment
#define WND_CANVAS_NAMEDIALOG 1113         //Get the appointment book name
#define WND_CANVAS_NAMEDIALOGBTN 1114      //Get the appointment book name buttons

#define WND_CANVAS_UPDLDIALOG 1115         // Update/delete dialog canvas
#define WND_CANVAS_UPDLMCDIALOG 1116       // Update/delete dialog canvas
#define WND_CANVAS_UPDLDIALOGBTN 1117      // Update/del dialog buttons
#define WND_CANVAS_UPDLUSERDIALOG 1118     // Update/del dialog buttons

#define WND_DAYSAPPOINTMENTS  0x1120
#define WND_MLEDAYSAPPOINTMENTS 0x1121       // Output area
//**************************************************************************
// menu ids - used on relate command ID to Menu Items and Function Keys    *
//**************************************************************************
#define MI_APPTBK_FILE                                 5000
#define MI_APPTBK_FILE_NEW                          5001
#define MI_APPTBK_FILE_OPEN                         5002
#define MI_APPTBK_FILE_DELETE                      5003
#define MI_APPTBK_FILE_EXIT                          5004
#define MI_APPTBK_APPOINTMENT                             5010
#define MI_APPTBK_APPOINTMENT_NEW                      5011
#define MI_APPTBK_APPOINTMENT_NEW_MEETING         5012
#define MI_APPTBK_APPOINTMENT_NEW_CONFERENCE_CALL    5013
#define MI_APPTBK_APPOINTMENT_UPDATE                 5014
#define MI_APPTBK_APPOINTMENT_DELETE                  5015
#define MI_APPTBK_SETTINGS                                    5016
#define MI_APPTBK_SETTINGS_APPOINTMENT_BOOK_SETTINGS 5017
#define MI_VIEWP                                     5020   // previous month
#define MI_VIEWN                                     5021   // next month


//**************************************************************************
// string ids - used to relate resources to IStaticText and ITitle         *
//**************************************************************************
#define STR_INFO         1220         //Info String ID
#define STR_INFODLG      1221         //Info String ID
#define STR_NAME         1222
#define STR_PHONE        1223
#define STR_LOCATION     1224
#define STR_SUBJECT      1225
#define STR_USERS        1226
#define STR_START        1227
#define STR_STOP         1228
#define STR_APPOINTMENT  1229
#define STR_OK           1230
#define STR_CANCEL       1231
#define STR_SELECT       1232
#define STR_NAMEMESSAGE1 1233
#define STR_NAMEMESSAGE2 1234
#define STR_ERRORMESSAGE1 1235
#define STR_INFOMESSAGE1 1236
#define STR_MSGBOX_TITLE 1237

//**************************************************************************
// dialog ids - used to relate dialog fields to controls/commands          *
//**************************************************************************
#ifndef DID_OK
#define DID_OK           1 //OK Button (Defined by os/2)
#endif

#ifndef DID_CANCEL
#define DID_CANCEL       2 //Cancel Button (Defined by os/2)
#endif

#define DID_SELECT       3

#define WND_APPTDIALOG              100
#define FLD_STARTHR                 102
#define FLD_STARTMIN                104
#define FLD_STOPHR                  106
#define FLD_STOPMIN                 108
#define FLD_PHONE                   110
#define FLD_LOC                     111
#define FLD_SUBJ                    113
#define LB_USERS                    114
#define TXT_STARTHR                 101
#define TXT_STOPHR                  105
#define TXT_STARTSEP                103
#define TXT_STOPSEP                 107
#define HR                          109
#define TXT_LOC                     112
#define TXT_USERS                   115
#define TXT_SUBJ                    116
#define TXT_MIN                     117
#define TXT_HR                      118
#define TXT_PHONE                   120

#define WND_UPDAPPTDIALOG           300
#define WND_DELAPPTDIALOG           301
#define LB_APPTS                    302
#define DDL_PHONE                   303
#define DDL_LOCATION                304
#define DDL_SUBJECT                 305

#define WND_NEWAPPTDIALOG           200
#define DID_SETTINGS                203
#define WND_OPENAPPTDIALOG          201
#define DDL_APPTBKNAME              204

#define WND_APPTBKSETTINGS          600
#define TXT_OWNER                   602
#define CB_READ                     604
#define CB_UPDATE                   605
#define CB_DELETE                   606
#define CB_ADMIN                    607
#endif


