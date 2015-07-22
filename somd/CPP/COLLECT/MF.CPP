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
static char *sccsid = "@(#) 1.8 src/somples.os2/somd/cpp/collect/mf.cpp, somx.somd, som2.5, s259604a 12/27/95 13:54:08 [1/24/96 17:51:30]";
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


#include <time.h>
#include <som.xh>
#include <somd.xh>
#include <somobj.xh>
#include <somcls.xh>
#include <mcollect.xh>
#include <tss.xh>
#include <tset.xh>
#include <tsetitr.xh>
#include <tpll.xh>
#include "transact.xh"
#include "acct.xh"

#define mutualfundsAcc_Class_Source
#include "mf.xih"

SOM_Scope long  SOMLINK Withdraw(mutualfundsAcc *somSelf,  Environment *ev,
                                 long amount, long pin)
{
    mutualfundsAccData *somThis = mutualfundsAccGetData(somSelf);
    mutualfundsAccMethodDebug("mutualfundsAcc","Withdraw");

/* we will mandate that a Mututal Fund requires a 1 hour maturing period :-) */
/* Note the code below is NOT correct, it is just a trivial test */
    if ((somSelf->_get_matured(ev) - time(0)) < 3600)
                 return  Account_NOT_MATURE;

    return (mutualfundsAcc_parent_Account_Withdraw(somSelf, ev,
                                                   amount, pin));
}

SOM_Scope long  SOMLINK Deposit(mutualfundsAcc *somSelf,  Environment *ev,
                                long amount)
{
    mutualfundsAccData *somThis = mutualfundsAccGetData(somSelf);
    mutualfundsAccMethodDebug("mutualfundsAcc","Deposit");

/* we will mandate that a minimum of $500 deposit is required in Mutual funds */
    if (amount / 500 == 0)
        return Account_LESS_MONEY;

    return (mutualfundsAcc_parent_Account_Deposit(somSelf, ev,

                                                  amount));
}


