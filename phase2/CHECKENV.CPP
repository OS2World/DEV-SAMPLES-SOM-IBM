// 3/8/96 09:37:05
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

#include <somd.xh>
#include <somdcprx.xh>

#include <appt.xh>
#include <mtg.xh>
#include <ccall.xh>
#include <apptbook.xh>
#include <key.xh>

#include <stdio.h>
#include <samptype.h>

SOMEXTERN int SOMLINK checkEnv(Environment *ev) {
    int ret = 0;

    if (ev->_major != NO_EXCEPTION) {
        somPrintf("\nEXCEPTION CAUGHT => ");
        if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) == 0){
            somPrintf("Naming Context not found !\n");
            somExceptionFree(ev);
            ret = CONTEXT_NOT_FOUND;
        }
        else if (strcmp(somExceptionId(ev),
                        ex_CosNaming_NamingContext_AlreadyBound) == 0) {
            somPrintf("Naming Context already bound !\n");
            somExceptionFree(ev);
            ret = NAME_ALREADY_BOUND;
        }
        else if (strcmp(somExceptionId(ev),"Appointment::OverLap")== 0) {
            somPrintf("Error on add ! \n << %s >> \n", somExceptionId(ev));
            somExceptionFree(ev);
            ret = ADD_APPOINTMENT_OVERLAP;
        }
        else  {
            somPrintf(" << %s >> !!\n", somExceptionId(ev));
            somExceptionFree(ev);
            ret = SOM_SYSTEM_ERROR;
        }
    }
    return ret;
}
