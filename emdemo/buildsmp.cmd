/* Command file to start a build for the sample                           */
/* buildsmp.exe accepts the following options:                            */
/* -c start a C compile                                                   */
/* -p start a C++ compile                                                 */
/* Either select -c or -p but not both.                                   */
/* The following options determine the compilers supported by the sample. */
/* Omit appropriate options when the compiler is not supported.           */
/* -i Enables selection of the IBM CSET Compiler from the Dialog          */
/* -v Enables selection of the VisualAge Compiler from the Dialog         */
/* -b Enables selection of the Borland Compiler from the Dialog           */
/* -m Enables selection of the Metaware Compiler from the Dialog          */
/***!Start!***
 *
 *   ORIGINS: 27
 *
 *   IBM CONFIDENTIAL -- (IBM Confidential Restricted when
 *   combined with the aggregated modules for this product)
 *   OBJECT CODE ONLY SOURCE MATERIALS
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   The source code for this program is not published or otherwise divested
 *   of its trade secrets, irrespective of what has been deposited with the
 *   U.S. Copyright Office.
 *
 ***!End!***/

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
sombin'\'buildsmp.exe '-c -i -b -m -v '
