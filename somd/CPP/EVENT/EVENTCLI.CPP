//
//   COMPONENT_NAME: somx
//
//   ORIGINS: 27
//
//
//    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996  
//   All Rights Reserved
//   Licensed Materials - Property of IBM
//   US Government Users Restricted Rights - Use, duplication or
//   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
//
#ifndef lint
static char *sccsid = "@(#) 1.9 src/somples.os2/somd/cpp/event/eventcli.cpp, somx.somd, som2.5, s259604a 12/26/95 20:30:46 [1/24/96 17:50:57]";
#endif

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

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>

#include <somd.xh>
#include <orb.xh>

/* Local includes follow */
#include "cell.xh"

char   dispBuf[512];
string dispp;
string dispStr;

Environment ev;

int main(int argc, char *argv[])
{
    char       answer[10];
    Cell       *cellObj = (Cell *) NULL;
    int        rc;
    long       init_val = 10;
    FILE       *fd;
    string     objRefStr; 

    SOM_InitEnvironment(&ev);
    SOMD_Init(&ev);

    if ((fd = fopen ("cell.rep", "r")) == (FILE *) NULL)
    {
        somPrintf ("Unable to find object ref string in %s\n", "cell.rep");
        SOMD_Uninit(&ev);
        SOM_UninitEnvironment(&ev);
        exit(1);
    }
    else
    {
        objRefStr = (string) SOMMalloc (1024);
        fscanf(fd, "%s", objRefStr);
        cellObj  = (Cell *) SOMD_ORBObject->string_to_object (&ev, objRefStr);
     
        if (cellObj == NULL)
        {
            somPrintf ("Unable to get objref from string %s.\n", objRefStr);
            SOMFree (objRefStr);
            fclose (fd);
            SOMD_Uninit(&ev);
            SOM_UninitEnvironment(&ev);
            exit(1);
        }

        SOMFree (objRefStr);
        fclose(fd);
     }

    somPrintf("Setting initial value of cell to: %ld\n", init_val);
    cellObj->_set_val(&ev, init_val);
    somPrintf("Increment cell value by 1? <return to continue or Q/q to quit>\n");
    gets(answer);

    while (strcmp(answer,"q") != 0 && strcmp(answer, "Q") != 0)
    {
       init_val++;
       cellObj->_set_val(&ev, init_val);
       somPrintf("Increment cell value by 1? <return to continue or Q/q to quit>\n");
       gets(answer);
    }

    ((SOMDObject *) cellObj)->release(&ev);

    SOMD_Uninit(&ev);
    SOM_UninitEnvironment(&ev);

    return 0;
}

