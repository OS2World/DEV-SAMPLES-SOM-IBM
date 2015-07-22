/* 3/8/96 15:41:44
*/
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
/* Command file to start a build for the sample                           */
/* buildsmp.exe accepts the following options:                            */
/* -c start a C compile                                                   */
/* -p start a C++ compile                                                 */
/* Either select -c or -p but not both.                                   */
/* The following options determine the compilers supported by the sample. */
/* Omit appropriate options when the compiler is not supported.           */
/* -i Enables selection of the IBM CSET Compiler from the Dialog          */
/* -b Enables selection of the Borland Compiler from the Dialog           */
/* -m Enables selection of the Metaware Compiler from the Dialog          */
/* -w Enables selection of the Watcom Compiler from the Dialog            */
/* -v Enables selection of the VisualAge Compiler from the Dialog         */
'@echo off'
sombase = value('SOMBASE',,'OS2ENVIRONMENT')
otp_b   = value('OTP_B',,'OS2ENVIRONMENT')
if otp_b="" then do /* Normal install environment */
   bin = "bin"
   end
else do
   bin = "bin.os2"
   sombase = value('_OTP',,'OS2ENVIRONMENT')
   end
sombin = sombase'\'bin
sombin'\'buildsmp.exe '-p -i -b -m -w -v'
