/* 3/8/96 15:52:53
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


#ifndef testutil_h
#define testutil_h

#include <stdio.h>
#ifdef __cplusplus
 #include <somobj.xh>
#else
 #include <somobj.h>
#endif

#include <somd.h>

#define PATH_SEPARATOR "\\"

/*
Stuff for cmpfiles.
------------------- */
short int cmpfiles(char *file1, char *file2);
#define CMP_EQUAL -1
#define CMP_NOT_EQUAL 0
#define CMP_ERROR 1
#define CMP_FILE1_MISSING 2
#define CMP_FILE2_MISSING 3
#define CHECK_EV(ev)  ((ev)->_major!=NO_EXCEPTION)

#ifdef __cplusplus
extern "C" {
  void DSOM_term(Environment *ev);
  void DSOM_thread(int argc, char **argv, Environment *ev);
}
#else
  void DSOM_term(Environment *ev);
  void DSOM_thread(int argc, char **argv, Environment *ev);
#endif

short int readln(FILE *file, char *buffer);
void trimln(char *line);
char *strcpy2(char *input);
boolean ExceptionCheck(Environment *ev, FILE* out);
void usage(void);
string createfullpath(string, string);
void init_signal_catching(void);


#endif /* testutil_h */

