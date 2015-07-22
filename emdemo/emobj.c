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
/* @(#) somx/emobj.c 2.7 12/26/95 17:48:44 [1/24/96 17:31:11] */

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


#define EMObject_Class_Source
#include "emobj.ih"


SOM_Scope void   SOMLINK eventMethod(EMObject somSelf, Environment *env, SOMEEvent event, void  *targetData)
{
    SOMEEMRegisterData  data = SOMEEMRegisterDataNew( );
    unsigned int    time, state, key;
    int             x, y, w, h, count, fd, interval, status;
    static  int     reg11, reg12;
    char            str[25];
    int             reg0 = 0;
    Environment     *ev = somGetGlobalEnvironment();

    printf( "Data = %s\n", targetData );
    switch( _somevGetEventType( event, ev )) {

        case  EMTimerEvent:
              printf("EMObject Class: Perceived Timer event\n" );
              break;

        case  EMWorkProcEvent:
              printf("EMObject Class: Perceived WorkProc event\n" );
              break;

        case  EMSinkEvent:
              /* Not implemented in OS/2 sample.                     */
              break;

        case  EMClientEvent:
              printf("EMObject Class: Perceived Client Event event\n" );
              break;

        default:
              printf("EMObject Class: GOT EVENT I DIDN'T WANT !!!\n" );
              break;
    }
}

