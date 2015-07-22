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
static char *sccsid = "@(#) 1.10 src/somples.os2/somd/cpp/collect/bank.cpp, somx.somd, som2.5, s259604a 12/27/95 13:53:14 [1/24/96 17:51:29]";
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
#include <stdlib.h>
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
#include "save.xh"
#include "check.xh"
#include "mf.xh"

#define Bank_Class_Source
#include "bank.xih"
#include "acct.xih"

Environment *ev;

SOM_Scope long  SOMLINK Open(Bank *somSelf,  Environment *ev,
                             string name, long acct_type, long pin,
                             long amount)
{
    BankData *somThis = BankGetData(somSelf);
    BankMethodDebug("Bank","Open");

    Account *temp;
    somf_TSetIterator *itr;
    trans_info *Tr;
    somf_TSet *set;
    somf_TPrimitiveLinkedList *llist;
    char *user_name;

/* first check if the account already exists */
/* we create an iterator and walk through the account list */

   itr = new somf_TSetIterator;

   switch (acct_type)
   {
        case Account_CHECKING:
            itr->somfTSetIteratorInit(ev, somSelf->_get_check_acct_set(ev));
            break;
        case Account_SAVINGS:
            itr->somfTSetIteratorInit(ev, somSelf->_get_save_acct_set(ev));
            break;
        case Account_MF:
            itr->somfTSetIteratorInit(ev, somSelf->_get_mf_acct_set(ev));
            break;
        default:
            somPrintf (" bad account type\n");
            return 9;
   }

   temp = (Account *)itr->somfFirst(ev);
   while (temp != SOMF_NIL )
   {
       user_name = temp->_get_user_name(ev);
       if (strcmp(name, user_name) == 0) {          /* acct already exists ? */
              somPrintf("<%s> exists\n", name);
              return Account_ACCT_EXISTS;
       }
       else
            temp = (Account *)itr->somfNext(ev);
   }

/* if we are here then we need to add the account
 * initialize all values (hardwire interest and min_balance)
 * allocate space for the new account
 */
    user_name = (char *) malloc (strlen(name) + 1);
    strcpy (user_name, name);

    temp = new Account;
    temp->_set_user_name (ev, user_name);
    temp->_set_min_balance (ev, 0);
    temp->_set_act_balance (ev, amount);
    temp->_set_interest (ev, 5);
    temp->_set_pin (ev, pin);

   llist = new  somf_TPrimitiveLinkedList;
/* initialize the transaction list also */
   Tr = new trans_info;
   Tr->_set_amount (ev, amount);
   Tr->_set_transaction (ev, Account_OPEN);
   Tr->_set_date (ev, time(0));
   Tr->_set_curr_bal (ev, amount);
   llist->somfAddFirst(ev, Tr);

/* now set up all the information to point correctly */
   temp->_set_trans_list (ev, llist);

    /* now add the account to the appropriate set */
    if (acct_type == Account_CHECKING) {
         set = somSelf->_get_check_acct_set(ev);
         set->somfAdd(ev, temp);
    }
    else if (acct_type == Account_SAVINGS) {
         set = somSelf->_get_save_acct_set(ev);
         set->somfAdd(ev, temp);
    }
    else if (acct_type == Account_MF)  {
         set = somSelf->_get_mf_acct_set(ev);
         set->somfAdd(ev, temp);
    }

   itr->somFree();
    return Account_OK;
}

SOM_Scope long  SOMLINK Close(Bank *somSelf,  Environment *ev,
                              string name, long acct_type, long pin)
{
    Account *temp;
    somf_TSetIterator *itr;
    somf_TPrimitiveLinkedList *l;
    int freed = 0;
    somf_TSet *setptr;
    string user_name;

    BankData *somThis = BankGetData(somSelf);
    BankMethodDebug("Bank","Close");

   itr = new somf_TSetIterator;

   switch (acct_type)
   {
        case Account_CHECKING:
            setptr = somSelf->_get_check_acct_set(ev);
            itr->somfTSetIteratorInit(ev, setptr);
            break;
        case Account_SAVINGS:
            setptr = somSelf->_get_save_acct_set(ev);
            itr->somfTSetIteratorInit(ev, setptr);
            break;
        case Account_MF:
            setptr = somSelf->_get_mf_acct_set(ev);
            itr->somfTSetIteratorInit(ev, setptr);
            break;
        default:
            somPrintf (" bad account type\n");
            return 9;
   }

   temp = (Account *) itr->somfFirst(ev);

   while (!((SOMDObject *)temp)->is_nil((SOMDObject *)temp,ev))
   {
      user_name = temp->_get_user_name(ev);
      if (strcmp(name, user_name) == 0) {
   if (temp->_get_pin(ev) == pin) {
   if (setptr->somfRemove(ev, temp) != SOMF_NIL)
        return Account_OK;
   else
        return 9;
}
   else
      return Account_BAD_PIN;
}
      else
         temp = (Account *)itr->somfNext(ev);
   }
   return Account_NO_ACCT;
}


SOM_Scope void SOMLINK somDefaultInit(Bank *somSelf, somInitCtrl* ctrl)
{
    BankData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somf_TSet *temp;
    somBooleanVector myMask;
    BankMethodDebug("Bank","somDefaultInit");
    Bank_BeginInitializer_somDefaultInit;

    Bank_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    ev = somGetGlobalEnvironment();
    /* allocate space for checking account */
    somSelf->_set_check_acct_set(ev, new somf_TSet);
    temp = somSelf->_get_check_acct_set(ev);
    temp->somfTSetInitL(ev, 25);

    /* allocate space for savings account */
    somSelf->_set_save_acct_set(ev, new somf_TSet);
    temp =  somSelf->_get_save_acct_set(ev);
    temp->somfTSetInitL(ev, 25);

    /* allocate space for checking account */
    somSelf->_set_mf_acct_set(ev, new somf_TSet);
    temp = somSelf->_get_mf_acct_set(ev);
    temp->somfTSetInitL(ev, 25);
}

