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
static char *sccsid = "@(#) 1.6 src/somples.os2/somd/cpp/stack/stack.cpp, somx.somd, som2.5, s259604a 12/26/95 20:34:36 [1/24/96 17:51:00]";
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

#define Stack_Class_Source
#include "stack.xih"


SOM_Scope boolean  SOMLINK full(Stack *somSelf, Environment *ev)
{
    StackData *somThis = StackGetData(somSelf);
    StackMethodDebug("Stack","full");

    /* Return TRUE if stack is full. */
    return (somThis->stackTop == stackSize);
}


SOM_Scope boolean  SOMLINK empty(Stack *somSelf, Environment *ev)
{
    StackData *somThis = StackGetData(somSelf);
    StackMethodDebug("Stack","empty");

    /* Return TRUE if stack is empty.*/
    return (somThis->stackTop == 0);
}


SOM_Scope long  SOMLINK top(Stack *somSelf, Environment *ev)
{
    StackData *somThis = StackGetData(somSelf);
    StackMethodDebug("Stack","top");

    if (somThis->stackTop > 0)
    {
       /* Return top element in stack without removing it from the stack. */
       return (somThis->stackValues[somThis->stackTop-1]);
    }
    else
    {
       somSetException(ev, USER_EXCEPTION, ex_STACK_UNDERFLOW, NULL);
       return (-1L);
    }
}


SOM_Scope long  SOMLINK pop(Stack *somSelf, Environment *ev)
{
    StackData *somThis = StackGetData(somSelf);
    StackMethodDebug("Stack","pop");

    if (somThis->stackTop > 0)
    {
       /* Return top element in stack and remove it from the stack. */
       somThis->stackTop--;
       return (somThis->stackValues[somThis->stackTop]);
    }
    else
    {
       somSetException(ev, USER_EXCEPTION, ex_STACK_UNDERFLOW, NULL);
       return (-1L);
    }
}


SOM_Scope void  SOMLINK push(Stack *somSelf, Environment *ev, long el)
{
    StackData *somThis = StackGetData(somSelf);
    StackMethodDebug("Stack","push");

    if (somThis->stackTop < stackSize)
    {
      /* Add element to top of the stack. */
      somThis->stackValues[somThis->stackTop] = el;
      somThis->stackTop++;
    }
    else
    {
       somSetException(ev, USER_EXCEPTION, ex_STACK_OVERFLOW, NULL);
    }
}
SOM_Scope void SOMLINK somDefaultInit(Stack *somSelf, somInitCtrl* ctrl)
{
    StackData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    StackMethodDebug("Stack","somDefaultInit");
    Stack_BeginInitializer_somDefaultInit;

    Stack_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    /* stackTop is index into stackValues for next pushed stack element.
     * stackValues[0..(stackSize-1)] holds stack elements.
     */
    somThis->stackTop = 0;
}

