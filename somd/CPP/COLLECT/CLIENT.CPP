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
static char *sccsid = "@(#) 1.12 src/somples.os2/somd/cpp/collect/client.cpp, somx.somd, som2.5, s259604a 1/9/96 10:36:36 [1/24/96 17:51:30]";
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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <som.xh>
#include <somd.xh>
#include <somobj.xh>
#include <somdcprx.xh>
#include <somcls.xh>
#include <mcollect.xh>
#include <tsetitr.xh>
#include <tpll.xh>

#include <tss.xh>
#include "bank.xh"
#include "transact.xh"
#include "save.xh"
#include "check.xh"
#include "mf.xh"
#include "acct.xh"

Environment  *e;
SOMDServer *bankserv;
Bank *bank;

int do_balance (char *, int);
int do_trans (char *, int, int);
int do_deposit(char *, int, int);
int do_withdraw(char *, long, long, long);
int display_acct_char(int);
int show_user(char *);

#define checkEv(ev) ((ev)->_major != NO_EXCEPTION)

/* This function converts an error into a readable string */
print_error(int error)
{

   if (error == Account_BAD_PIN)
         somPrintf ("The PIN is invalid\n");
   else if (error == Account_ACCT_EXISTS)
           somPrintf ("An account by the same name already exits\n");
   else if (error == Account_NO_ACCT)
            somPrintf ("An account by the name does not exist\n");
   else if (error == Account_LESS_MONEY)
            somPrintf ("The balance in the account is less than the amount requested\n");
   else if (error == Account_NOT_MATURE)
            somPrintf ("The account has not matured yet\n");
   else if (error == 9)
            somPrintf ("Unknown account type specified.  Specify either c, s, or m\n");

   return 0;
}

/* This routine prints the options for the end user */

print_help_menu()
{
  char *p, input[100];
  static int init = 0;

  if (init == 0)
     init = 1;
  else
     do
     {
     somPrintf("\n\nPress Enter to continue.\n");
     fflush(stdout);
     gets(input);
     p = input;
  } while (isspace (*p));

  somPrintf("\n********  Specify one of the following Options  ********\n");
  somPrintf ("\n\nThe account types are: [c]hecking, [s]avings, [m]utual_funds\n\n");

  somPrintf("\n[p]rint <name> to display all info about a specific user\n");
  somPrintf("[p]rint ALL <c|s|m> to display ALL users in a specific account type\n");

  somPrintf("[o]pen <name> <c|s|m> <4-digit PIN> <amount>\n");
  somPrintf("[c]lose <name> <c|s|m> <4-digit PIN> \n");
  somPrintf("[w]ithdraw <name> <c|s|m> <4-digit PIN> <amount>\n");
  somPrintf("[d]eposit <name> <c|s|m> <amount>\n");
  somPrintf("[s]how_balance <name> <c|s|m>\n");
  somPrintf("[l]ist_transactions <name> <c|s|m> <4-digit PIN>\n");
/*
  somPrintf("[a]cct_characteristics <c|s|m>\n");
*/
  somPrintf("[q]uit this program \n");

  somPrintf("\n?: to print the help menu\n\n");

  return 0;
}


/* this funtion parses the user input and invokes the appropriate
 * method. It is an infinite loop until the user enters a "q" or
 * the program is exited via some signal (control C).
 * This function is NOT robust, it is ONLY a sample
 */
process_requests()
{

  char input[200];
  char *p, *temp;
  int rc, len;

  char name[32], xyz[10];
  int action, pin, amt, act_type;

  for (; ; )
  {
      action = pin = amt = act_type = -1;
      print_help_menu();

       somPrintf ("\n>> ");
       fflush(stdout);
       gets(input);
       p = input;
       while (isspace(*p))
            ++p;
       temp = p;

/* first get the operation */
       if (*temp == 'p') {
            rc = show_user(input);
         if (rc != Account_OK) print_error (rc);
            continue;
       }
       else if (*temp == 'o')
          action = Account_OPEN;
       else if (*temp == 'c')
          action = Account_CLOSE;
       else if (*temp == 'w')
           action = Account_DRAW;
       else if (*temp == 'd')
           action = Account_STORE;
       else if (*temp == 's')
           action = Account_BALANCE;
       else if (*temp == 'l')
           action = Account_TRANS;
       else if (*temp == 'q')
               return 0;
       else if (*temp == 'a')
      {
         while (isalpha (*temp)) temp++;
         while (isspace (*temp)) temp++;
               if (*temp == 'c')
                    display_acct_char(Account_CHECKING);
               else if (*temp == 's')
                    display_acct_char(Account_SAVINGS);
               else if (*temp == 's')
                    display_acct_char(Account_MF);
               continue;
             }
       else if (*temp == '?')
               continue;
       else
      {
         somPrintf("Incorrect input - Please enter a valid transaction type\n");
         continue;
      }

      /* finish reading the option */
      while (isalpha(*p))
               p++;

/* now process the account name */
      while (isspace(*p)) ++p;
      temp = p;
      while (isalpha(*p)) ++p;
      len = p - temp;
      if (len == 0) {
         somPrintf("No account name was entered. Please enter an account name\n");
         continue;
      }
      strncpy(name, temp, len);
      temp = name;
      temp += len;
      *temp = NULL;  /* this should put the NULL also */

/* process the account type */
      while (isspace(*p)) ++p;
      if (*p == 'c')
             act_type = Account_CHECKING;
      else if (*p == 's')
             act_type = Account_SAVINGS;
      else if (*p == 'm')
             act_type = Account_MF;
      else
      {
         somPrintf("Incorrect account type, specify either c or s or m\n");
           continue;
         }

      if (action == Account_BALANCE) {
             rc = do_balance(name, act_type);
         if (rc != Account_OK) print_error(rc);
             continue;
      }

/*  process the PIN */
      p++;
      while (isspace(*p)) ++p;
      temp = p;
      while (isdigit (*p)) p++;
      strncpy(xyz, temp, p - temp + 1);
      pin = atoi((char *)xyz);

      /* Check to see if the PIN or deposit amount was even entered */
      len = p - temp;
      if (len == 0) {
         if (action == Account_STORE)
            somPrintf("No amount was entered to be deposited\n");
         else
            somPrintf("No PIN was entered when a PIN was required - Please enter a PIN\n");
         continue;
      }

/* If the action is to deposit then pin = deposit amount
 * because to deposit we should never need to know the PIN
 */
      if (action == Account_STORE) {
             rc = do_deposit(name, act_type, pin);
         if (rc != Account_OK) print_error (rc);
             continue;
     }
/* process the amount */

      if ((action != Account_CLOSE) && (action != Account_TRANS))
      {
         while (isspace(*p)) ++p;
         temp = p;
         while (isdigit(*p)) ++p;
              strncpy(xyz, temp, p - temp + 1);
              amt = atoi((char *)xyz);

         /* Check to see if the amount was even entered */
         len = p - temp;
         if (len == 0) {
            somPrintf("No amount was entered - Please enter an amount\n");
            continue;
         }
      }

/* now for the real work */
    switch (action)
    {
         case Account_DRAW:
               rc = do_withdraw(name, act_type, pin, amt);
               if (rc != Account_OK)
                     print_error (rc);
            else
            {
               rc = do_balance(name, act_type);
               if (rc != Account_OK) print_error(rc);
            }
               break;
         case Account_CLOSE:
               rc = bank->Close (e, name, act_type, pin);
               if (rc == Account_OK)
                   somPrintf("Account successfully closed\n");
               else
                   print_error (rc);
               break;
         case Account_OPEN:
               rc = bank->Open (e, name, act_type, pin, amt);
            if (rc == Account_OK) {
                   somPrintf("Account successfully created\n");
               rc = do_trans(name, act_type, pin);
               if (rc != Account_OK) print_error (rc);
               }
               else
                   print_error (rc);
               break;
         case Account_TRANS:
               rc = do_trans(name, act_type, pin);
               if (rc != Account_OK)
                   print_error (rc);
               break;
         default:
               break;
     } /* closes the switch statement */

  } /* closes the infinite for statement */

  return 0;
}

/* This functions converts an option number into a
 * message string.
 */
char *convert(int type)
{
   static char name_of_trans[32];

   if (type == Account_STORE)
       strcpy (name_of_trans, "Deposit");
   else if (type == Account_DRAW)
       strcpy (name_of_trans, "Withdraw");
   else if (type == Account_OPEN)
       strcpy (name_of_trans, "Account Open");
   else if (type == Account_CLOSE)
       strcpy (name_of_trans, "Account Closed");
   else
       return (char *)NULL;

   return ((char *)name_of_trans);
}

/*
 * This function lists the transactions that have occured on a specific account
 * From the "name" we access the account and then obtain the transactions one at a
 * time. An integer is sent to the server to indicate what transaction to get next.
 * The integer has a value of 1 to begin with, and if the server determines that
 * there are no more transactions, it resets the integer to 0, which the client
 * detects. This is an example of maintaining remote cursors.
*/

do_trans(char *name, int act_type, int pin)
{
    Account *temp;
    trans_info *Tr;
    somf_TSet *set;
    long i, num_acts, date;
    long cursor = 1;
    somf_TSetIterator *itr;

   switch (act_type)
   {
        case Account_CHECKING:
            set = bank->_get_check_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case  Account_SAVINGS:
            set = bank->_get_save_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case Account_MF:
            set = bank->_get_mf_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        default:
            return 9;
   }

   temp = (Account *)itr->somfFirst(e);
   num_acts = set->somfCount (e);
   for (i = 0; i < num_acts; i++)
   {
       if (strcmp(name, temp->_get_user_name(e)) == 0)
       {
              if (temp->_get_pin(e) != pin) {
                 ((SOMDClientProxy *) temp)->release(e);
                 ((SOMDClientProxy *) set)->somdReleaseResources(e);
                 itr->somFree();
                 return Account_BAD_PIN;
              }

              while (cursor != 0)
              {
                     Tr = temp->Transactions (e, &cursor);
                     if (!((SOMDObject *)Tr)->is_nil((SOMDObject *)Tr,e)) {
                         somPrintf ("Type of Transaction is: %s\n", convert (Tr->_get_transaction(e)));
                         somPrintf ("Amount is: %d\n", Tr->_get_amount(e));
                         date = Tr->_get_date(e);
                         somPrintf ("Date of Transaction is: %s", ctime ((time_t *)&date));
                         somPrintf ("Balance at Transaction is: %d\n\n", Tr->_get_curr_bal(e));

                         Tr = (trans_info *) SOMF_NIL;
                     }
              }
              ((SOMDClientProxy *) temp)->release(e);
              ((SOMDClientProxy *) set)->somdReleaseResources(e);
              itr->somFree();
              return Account_OK;
       }
       else {
            ((SOMDClientProxy *) temp)->release(e);
            temp = (Account *)itr->somfNext(e);
       }
   }
   if (temp) ((SOMDClientProxy *) temp)->release(e);
   ((SOMDClientProxy *) set)->somdReleaseResources(e);
   itr->somFree();
   return Account_NO_ACCT;
}

/*
 * This functions prints the balance on s specific account. We access the account
 * first and then read of the act_balance fields.
 */
do_balance (char *name, int act_type)
{
    Account *temp;
    somf_TSetIterator *itr;
    trans_info *Tr;
    char *user_name;
    somf_TSet *set;
    int i, num_acts;

   switch (act_type)
   {
        case Account_CHECKING:
            set = bank->_get_check_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case  Account_SAVINGS:
            set = bank->_get_save_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case Account_MF:
            set = bank->_get_mf_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        default:
            return 9;
   }

   temp = (Account *)itr->somfFirst(e);
   while (!((SOMDObject *)temp)->is_nil((SOMDObject *)temp,e))
   {
       user_name = temp->_get_user_name(e);
       if (strcmp(name, user_name) == 0) {
              somPrintf("The balance is: %d\n", temp->_get_act_balance(e));
              ((SOMDClientProxy *) temp)->release(e);
              ((SOMDClientProxy *) set)->somdReleaseResources(e);
              itr->somFree();
              return Account_OK;
       }
       else {
            ((SOMDClientProxy *) temp)->release(e);
            temp = (Account *)itr->somfNext(e);
       }
   }
   if (temp) ((SOMDClientProxy *) temp)->release(e);
   ((SOMDClientProxy *) set)->somdReleaseResources(e);
   itr->somFree();
   return Account_NO_ACCT;

}

/*
 * Withdraw money from a specific account
 */
do_withdraw(char *name, long act_type, long pin, long amount)
{
   int rc, num_acts, i;
   Account *temp;
   somf_TSetIterator *itr;
    somf_TSet *set;

/* first get access to the appropriate account set */

   switch (act_type)
   {
        case Account_CHECKING:
            set = bank->_get_check_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case  Account_SAVINGS:
            set = bank->_get_save_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case Account_MF:
            set = bank->_get_mf_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        default:
            somPrintf(" Incorrect account type, specify either c or s or m\n");
            return 9;
   }

/* make sure that we are pointing to the right account */

   temp = (Account *)itr->somfFirst(e);
   while (!((SOMDObject *)temp)->is_nil((SOMDObject *)temp,e))
   {
       if (strcmp(name, temp->_get_user_name(e)) == 0)
       {      rc = temp->Withdraw (e, amount, pin);
              if (rc == Account_OK) {
                  somPrintf ("Amount <%d> successfully withdrawn\n", amount);
                  ((SOMDClientProxy *) temp)->release(e);
                  ((SOMDClientProxy *) set)->somdReleaseResources(e);
                  itr->somFree();
                  return Account_OK;
              }
              ((SOMDClientProxy *) temp)->release(e);
              ((SOMDClientProxy *) set)->somdReleaseResources(e);
              itr->somFree();
              return rc;
       }
       else {
            ((SOMDClientProxy *) temp)->release(e);
            temp = (Account *)itr->somfNext(e);
       }
   }

   if (temp) ((SOMDClientProxy *) temp)->release(e);
   ((SOMDClientProxy *) set)->somdReleaseResources(e);
   itr->somFree();
   return Account_NO_ACCT;
}

do_deposit(char *name, int act_type, int amount)
{
   int rc, i;
   Account *temp;
   somf_TSetIterator *itr;
    somf_TSet *set;

   switch (act_type)
   {
        case Account_CHECKING:
            set = bank->_get_check_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case  Account_SAVINGS:
            set = bank->_get_save_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        case Account_MF:
            set = bank->_get_mf_acct_set(e);
            itr = (somf_TSetIterator *) set->somfCreateIterator(e);
            break;
        default:
            return 9;
   }

/* make sure that we are pointing to the right account */

   temp = (Account *) itr->somfFirst(e);
   while (!((SOMDObject *)temp)->is_nil((SOMDObject *)temp,e))
   {
       if (strcmp(name, temp->_get_user_name(e)) == 0)
       {
              rc = temp->Deposit (e, amount);
              if (rc == Account_OK) {
                  somPrintf ("Amount <%d> successfully deposited\n", amount);
                  ((SOMDClientProxy *) temp)->release(e);
                  ((SOMDClientProxy *) set)->somdReleaseResources(e);
                  itr->somFree();
                  return Account_OK;
              }
              else
                        print_error (rc);
              break;
       }
       else {
            ((SOMDClientProxy *) temp)->release(e);
            temp = (Account *) itr->somfNext(e);
       }
   }
   if (temp) ((SOMDClientProxy *) temp)->release(e);
   ((SOMDClientProxy *) set)->somdReleaseResources(e);
   itr->somFree();
   return Account_NO_ACCT;
}

display_acct_char(int act_type)
{

   int rc;
   Account *temp;
   mutualfundsAcc *mfp;
   somf_TSet *set;
   somf_TSetIterator *itr;

/* first get access to the appropriate account set */
   if (act_type == Account_CHECKING) {
         set = bank->_get_check_acct_set(e);
         itr = (somf_TSetIterator *) set->somfCreateIterator(e);
   }
   else  if (act_type == Account_SAVINGS) {
         set = bank->_get_save_acct_set(e);
         itr = (somf_TSetIterator *) set->somfCreateIterator(e);
   }
   else  if (act_type == Account_MF)
   {
         set = bank->_get_mf_acct_set(e);
         itr = (somf_TSetIterator *) set->somfCreateIterator(e);

         mfp = (mutualfundsAcc *)itr->somfFirst(e);
      if (mfp != SOMF_NIL) {
         somPrintf("The interest rate is: %d\n", ((Account *) mfp)->_get_interest(e));
         somPrintf("The minimum balance required is: %d\n", ((Account *)mfp)->_get_min_balance(e));
         somPrintf("The time for maturing is: %d\n", mfp->_get_matured(e));
         itr->somFree();
         return 0;
     }
        ((SOMDClientProxy *) set)->somdReleaseResources(e);
        itr->somFree();
        return 0;   /* return success anyway */
   }
   else {
      ((SOMDClientProxy *) set)->somdReleaseResources(e);
      itr->somFree();
      return 9;
   }

   temp = (Account *)itr->somfFirst(e);
   if (temp != SOMF_NIL) {
         somPrintf("The interest rate is: %d\n", temp->_get_interest(e));
         somPrintf("The minimum balance required is: %d\n", temp->_get_min_balance(e));
   }
   if (temp) ((SOMDClientProxy *) temp)->release(e);
   ((SOMDClientProxy *) set)->somdReleaseResources(e);
   itr->somFree();
   return 0;
}

/* this function displays all the accounts in the bank in a specific
 * account type, OR will display a spcific user's account(s).
 */
show_user(char *input)
{

  char *p, *temp, name[32];
  int num_acts, i, rc;
  Account *acct_info;;
  somf_TSet *set;
  somf_TSetIterator *itr;

  p = input;
  while (isalpha(*p))
       p++;
  while (isspace(*p))
       p++;

  if (strcmp(p,"") == 0) return Account_NO_ACCT;;

  if (strncmp(p, "ALL", 3) == 0)
  {
          /* implies all accounts of a specific type are to be displayed */
          while (isalpha(*p))
                  p++;
          while (isspace(*p))
                  p++;
          if (*p == 'c') {
              set = bank->_get_check_acct_set(e);
              itr = (somf_TSetIterator *) set->somfCreateIterator(e);
              somPrintf("\nPrinting ALL accounts in checking:\n");
          }
          else if (*p == 's') {
              set = bank->_get_save_acct_set(e);
              itr = (somf_TSetIterator *) set->somfCreateIterator(e);
              somPrintf("\nPrinting ALL accounts in savings:\n");
          }
          else if (*p == 'm') {
              set = bank->_get_mf_acct_set(e);
              itr = (somf_TSetIterator *) set->somfCreateIterator(e);
              somPrintf("\nPrinting ALL accounts in mutual funds:\n");
          }
          else {
               return 9;
          }

          /* now start walking on the specific account set and display
           * info about each account
           */
          num_acts = set->somfCount (e);
          acct_info = (Account *) itr->somfFirst(e);

          for (i = 0; i < num_acts; i++)
          {
             somPrintf("The name of the account is: %s\n", acct_info->_get_user_name(e));
             acct_info = (Account *) itr->somfNext(e);
          }
          if (acct_info) ((SOMDClientProxy *) acct_info)->release(e);
          ((SOMDClientProxy *) set)->somdReleaseResources(e);
          itr->somFree();
          return Account_OK;
   }  /* closes the if ALL statement */

   else /* must be a specific user name */
   {
        /* first get the user name */
        temp = p;
        while (isalpha(*p))
              ++p;
        strncpy(name, temp, (p - temp) + 1);

/* iterate over all the accounts in each type of account, i.e checking, savings and mf. */
        for (i = 1; i <= 3; i++)
        {
           somPrintf ("\n\n");
           if (i == 1)
                 somPrintf("The Account type is: Checking\n");
           else if (i == 2)
                 somPrintf("The Account type is: Savings\n");
           else if (i == 3)
                 somPrintf("The Account type is: Mutual Funds\n");

           rc = do_balance(name, i);
           if (rc != Account_OK)
                 print_error(rc);
        }
         return 0;
     }    /* closes the else statement */
   return 8;  /* we should not be coming here */
}


/* All of this should actually be done in the server, but
 * since this example has no persistance, we create everything
 * from the client.
 */

main(int argc, char *argv[])
{
    e = somGetGlobalEnvironment();
    SOMD_Init(e);

    /* Create the bank object */
    bank = (Bank *) somdCreate (e, "Bank", TRUE);
    if (checkEv(e)) {
        somPrintf("Unable to create the <Bank> object in the <BankServer>\n");
        SOMD_Uninit(e);
        exit (1);
    }
    somPrintf("\n********  Initialization Complete  ********\n");

    /* Enter routine to process requests */
    process_requests();

    /* Free bank object and uninitialize DSOM environment */
    bank->somFree();
    SOMD_Uninit(e);

}

