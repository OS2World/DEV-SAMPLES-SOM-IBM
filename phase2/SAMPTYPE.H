// 3/8/96 09:36:40
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
#ifndef SAMPTYPE_H
#define SAMPTYPE_H
#ifndef __OS2__
   typedef int           Boolean;
#endif
#define BUF_SIZE 512
#define MAX_LENGTH 80
#define MAX_BUFFER_SIZE 1000
// Define the error code values for the client
#define CONTEXT_NOT_FOUND  1
#define NAME_ALREADY_BOUND 2
#define ADD_APPOINTMENT_OVERLAP 3
#define SOM_SYSTEM_ERROR 4

SOMEXTERN int SOMLINK checkEnv(Environment *);
// Define the error code values here for the server
#define OVERLAP            100   // Error code for a duplicate appointment Phase 1
#define NOABCONTEXT        110   // The AppointmentBooks context was not created  Phase3
#define BOOKEXISTS         120   // Cannot create, a book by this name exists Phase 3
#define BOOKNOEXIST        130   // Cannot find, a Book by this name does not exist Phase3
#define NOTAPPTBOOK        140   // Cannot find, a Book by this name does not exist Phase3
#define NOAPPTCONTEXT      150   // The Appointment context was not created  Phase3
#define APPOINTMENTEXISTS  160   // Cannot create, an Appointment by this name exists Phase 3
#define APPOINTMENTNOEXIST 170   // Cannot find, a Book by this name does not exist Phase3
#define NOTAPPOINTMENT     180   // Cannot find, a Book by this name does not exist Phase3
#endif

