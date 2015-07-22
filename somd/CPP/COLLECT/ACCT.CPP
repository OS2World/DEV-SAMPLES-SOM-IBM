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
static char *sccsid = "@(#) 1.9 src/somples.os2/somd/cpp/collect/acct.cpp, somx.somd, som2.5, s259604a 1/9/96 10:36:33 [1/24/96 17:51:29]";
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
#include <time.h>
#include <som.xh>
#include <somd.xh>
#include <somobj.xh>
#include <somcls.xh>
#include <mcollect.xh>
#include <tss.xh>
#include <tpll.xh>
#include <tpllitr.xh>
#include "transact.xh"
#include "check.xh"
#include "save.xh"
#include "mf.xh"

#define Account_Class_Source
#include "acct.xih"

SOM_Scope long  SOMLINK Withdraw(Account *somSelf,  Environment *ev,
                                 long amount, long pin)
{
    trans_info *Tr;
    somf_TPrimitiveLinkedList *llist;

    AccountData *somThis = AccountGetData(somSelf);
    AccountMethodDebug("Account","Withdraw");

    if (somThis->pin != pin)
           return Account_BAD_PIN;

    if (somSelf->_get_act_balance(ev) < amount)
           return Account_LESS_MONEY;

    /* if we are here we need to give money, adjust balance  */
    somSelf->_set_act_balance(ev, (somSelf->_get_act_balance(ev) - amount));

    /* create a transaction object and put it in the linked list */
   Tr = new trans_info;
   Tr->_set_amount(ev, amount);
   Tr->_set_transaction(ev, Account_DRAW);
   Tr->_set_date(ev, time(0));
   Tr->_set_curr_bal(ev, somSelf->_get_act_balance(ev));

   llist = (somf_TPrimitiveLinkedList *) somSelf->_get_trans_list(ev);
   llist->somfAddLast(ev, Tr);

    return Account_OK;
}

SOM_Scope long  SOMLINK Deposit(Account *somSelf,  Environment *ev,
                                long amount)
{
    trans_info *Tr;
    somf_TPrimitiveLinkedList *llist;

    AccountData *somThis = AccountGetData(somSelf);
    AccountMethodDebug("Account","Deposit");

    /* NO checks in this because we let anyone deposit into any account */
    somSelf->_set_act_balance(ev, (somSelf->_get_act_balance(ev) + amount));

    /* create a transaction object and put it in the linked list */

   Tr = new trans_info;
   Tr->_set_amount(ev, amount);
   Tr->_set_transaction(ev, Account_STORE);
   Tr->_set_date(ev, time(0));
   Tr->_set_curr_bal(ev, somSelf->_get_act_balance(ev));

   llist = (somf_TPrimitiveLinkedList *) somSelf->_get_trans_list(ev);
   llist->somfAddLast(ev, Tr);

    return Account_OK;
}


SOM_Scope trans_info*  SOMLINK Transactions(Account *somSelf,
                                             Environment *ev,
                                            long* cursor)
{
    somf_TPrimitiveLinkedList *llist;
    somf_TPrimitiveLinkedListIterator *llitr;
    trans_info *Tr_info;
    int i;

    AccountData *somThis = AccountGetData(somSelf);
    AccountMethodDebug("Account","Transactions");

   llitr = new somf_TPrimitiveLinkedListIterator;
   llist = (somf_TPrimitiveLinkedList *) somSelf->_get_trans_list(ev);

   llitr->somfTPrimitiveLinkedListIteratorInit (ev, llist);

   if (*cursor <= 0) {
        llitr->somFree();
        return ((trans_info *) 0);
   }

   Tr_info = (trans_info *)llitr->somfFirst (ev);
   if (*cursor == 1) {
        (*cursor)++;
        llitr->somFree();
        return (Tr_info);
   }
   else
   {
        for (i = 0; (i < (*cursor -1)) && (Tr_info != SOMF_NIL); i++)
               Tr_info = (trans_info *)llitr->somfNext(ev);
        if (Tr_info == SOMF_NIL) {
                *cursor = 0;
                llitr->somFree();
                return SOMF_NIL;
        }
        else {
                (*cursor)++;
                llitr->somFree();
                return (Tr_info);
        }
   }

}


SOM_Scope boolean  SOMLINK somfIsEqual(Account *somSelf,  Environment *ev,
                                       somf_MCollectible* obj)
{
    AccountData *somThis = AccountGetData(somSelf);
    AccountMethodDebug("Account","somfIsEqual");

    if (strcmp (somSelf->_get_user_name(ev),  ((Account *)obj)->_get_user_name(ev)) == 0)
            return TRUE;
    else
            return FALSE;
}

