/*
 *   COMPONENT_NAME: somx
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/* @(#) somx/irdump.c 2.6 12/26/95 17:49:50 [1/24/96 17:31:12] */

/*
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

/*
 *	   File:    irdump.c.
 *     Contents:    This is a simple convenience utility for 
 *                  examining the contents of an Interface Repository.
 */

#include <stdio.h>
#include <som.h>
#include <stdlib.h>
#include <repostry.h>
#include <containr.h>

#ifdef __IBMC__
  #define PUTENV _putenv
#else
  #define PUTENV putenv
#endif /* __IBMC__ */

static int handleOption (string option, string pgmname);
static int dumpObject (Container ctr, Environment *ev, string name);
static string getErrorType (irOpenErrorCodes ec);
static void irExit (Repository repo, int rc);

/*  Globals  */
boolean sWithin = FALSE;

int main (int argc, string argv[])
{
    int rc = -1, i, orc;
    Repository repo;
    Environment *ev = somGetGlobalEnvironment ();
    irOpenError *oep;
    string exname;
    
    somEnvironmentNew ();

    if (!(repo = RepositoryNew ())) { /* assignment */
        exname = somExceptionId (ev);
        oep = (irOpenError *) somExceptionValue (ev);
        if ((ev->_major == USER_EXCEPTION) && exname && oep &&
            (strcmp (ex_irOpenError, exname) == 0)) {
	    somPrintf ("IR file \"%s\" open failed: %s\n",
                oep->fileName, getErrorType (oep->errorCode));
            somExceptionFree (ev);
        } else
	    somPrintf ("Unable to access IR\n");
	irExit (repo, 1);
    }

    /* 
     *  Return code semantics are as follows:
     *
     *  rc = -1   -- operation pending
     *  rc = 0    -- operation completed normally
     *  rc = 1    -- operation completed with error
     */
    
    for (i=1; i<argc; i++) {
        if ((*(argv[i]) == '-') 
	  #ifdef __OS2__
	    || (*(argv[i]) == '/')
          #endif /* __OS2__ */
	    )
            orc = handleOption (argv[i]+1, argv[0]);
        else
	    orc = dumpObject ((Container) repo, ev, argv[i]);
        if (rc != 1)
	    rc = orc;
    }
    
    if (rc == -1) {
	Repository_somDumpSelf (repo, 0);
	irExit (repo, 0);
    }

    irExit (repo, rc);
}

static void showUsage (string pgm)
{
    static boolean alreadyDone = FALSE;

    if (alreadyDone)
	return;
    somPrintf ("Interface Repository Dump, 1.0\n");
    somPrintf ("usage: %s [-ow?] [object] ...\n", pgm);
    somPrintf ("Where:\n");
    somPrintf ("\t-o     - include file offset information.\n");
    somPrintf ("\t-w     - follow dump with \"within\" operation.\n");
    somPrintf ("\t-?     - show this usage information.\n");
    somPrintf ("\tobject - a simple or fully-qualified name of an\n");
    somPrintf ("\t         object in the Interface Repository.\n");
    somPrintf ("\n");
    somPrintf ("Default action is to dump all objects.\n\n");
    alreadyDone = TRUE;
}

static int handleOption (string option, string pgmname)
{
    int i, rc = 0;
    
    for (i=0; i<strlen(option); i++) {
        switch (option[i]) {
          case '?':
          case 'h':
	      showUsage (pgmname);
	      break;
          case 'o':
	      PUTENV ("SOMIR_DUMPALL=1");
	      rc = -1;
	      break;
          case 'w':
	      sWithin = TRUE;
	      rc = -1;
	      break;
	  default:
	      somPrintf ("Unrecognized option '%c'\n", option[i]);
	      rc = 1;
	}
    }
    return rc;
}

static int dumpObject (Container ctr, Environment *ev, string name)
{
    sequence(Contained) sc;
    sequence(Container) sr;
    Contained c;
    int i, j, rc = 0;
    
    if (strncmp ("::", name, 2) == 0) {
        if (strlen (name) == 2) {  /* We take "::" to be the Repository! */
	    Container_somDumpSelf (ctr, 0);
            somPrintf ("\n");
	    return 0;
        }
        if (c = Repository_lookup_id ((Repository) ctr, ev, name)) {
	    sc._length = 1; sc._maximum = 1;
	    sc._buffer = (Contained *) SOMMalloc (sizeof (Contained));
	    sc._buffer[0] = c;
	} else {
	    sc._length = 0; sc._maximum = 0;
	    sc._buffer = (Contained *) NULL;
	}
    } else
        sc = Container_lookup_name (ctr, ev, name, -1, "all", TRUE);
    if (sc._length) {
	somPrintf ("\"%s\": %d entr%s found\n", name, sc._length,
	    sc._length == 1 ? "y" : "ies");
	for (i=0; i<sc._length; i++) {
	    Contained_somDumpSelf (sc._buffer[i], 0);
            if (sWithin) {
                sr = Contained_within (sc._buffer[i], ev);
		if (sr._length) {
		    somPrintf ("\n%s is within %ld entr%s:\n",
		        Contained__get_id (sc._buffer[i], ev), sr._length,
		        sr._length == 1 ? "y" : "ies");
                    for (j=0; j<sr._length; j++) {
			somPrintf ("  %s\n", __get_id (sr._buffer[j], ev));
			Container_somFree (sr._buffer[j]);
                    }
                    SOMFree (sr._buffer);
               } else
		   somPrintf ("\n%s is only within the Interface Repository.\n",
		       Contained__get_id (sc._buffer[i], ev));
	    }
	    Contained_somFree (sc._buffer[i]);
	}
	somPrintf ("\n");
    } else {
	somPrintf ("\"%s\" not found.\n\n", name);
        rc = 1;
    }
    if (sc._buffer)    
	SOMFree (sc._buffer);
    return rc;
}

static string getErrorType (irOpenErrorCodes ec)
{
    switch (ec) {
        case Repository_NOACCESS:
            return "NOACCESS";
        case Repository_BADMAGICNUMBER:
            return "BADMAGICNUMBER";
        case Repository_MISSINGVERSIONINFO:
            return "MISSINGVERSIONINFO";
        case Repository_IOERROR:
            return "IOERROR";
        case Repository_VERSIONMISMATCH:
            return "VERSIONMISMATCH";
    }
    return "UNKNOWN";
}

static void irExit (Repository repo, int rc)
{
    if (repo)
        Repository_somFree (repo);
    exit (rc);
}

