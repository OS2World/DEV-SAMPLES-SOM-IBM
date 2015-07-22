/*
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 *
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

/********************************************************************************
 *
 * This file contains the implementation methods which are broken down into
 * those that are of most interest based on their usage/relation with the
 * LifeCycle Service and those that simply support the 'application/business'
 * logic such as printing.  Other methods such as string attribute overrides,
 * somDefaultInit, somDestruct, etc have not been documented since they are
 * rather straight forward.
 *
 * Most focus should be on the methods that focus on LifeCycle and/or the main
 * business logic and therefore these methods have been documented in detail vs.
 * the other methods. The following list contains the methods of most interest:
 *
 * MovieFac_make_movie
 * GameFac_make_game
 *
 * Other methods that are implemented here focus on the business logic for the
 * sample, but use the LifeCycle Service as the underlying means for creating
 * and removing the objects. The majority of the this file contains implementations
 * for 3 separate logs that are used to manage RentalItem, CustomerAccount, and
 * RentObject objects. Each log has a add, remove, find, and display method on
 * it and these methods are not documented in detail since they are straight
 * forward and should not be difficult to follow.
 *
 ********************************************************************************/


#ifndef SOM_Module_rentalf_Source
#define SOM_Module_rentalf_Source
#endif
#define RentalFactory_Class_Source
#define SOMMSingleInstance_Class_Source

#include "rentalf.xih"


/********************************************************************************
 * The MovieFac_make_movie method is part of the 'user-defined' factory used
 * to create the actual Movie object itself. This method is called from
 * create_object code (from within the real_object_creation method) after the
 * GenericRentalFactory code determines that a Movie object has been requested.
 *
 * The 'title' parm is passed in to show how a user can customize the initialization
 * of an object...and where the attributes of the object are being set during
 * object creation.
 ********************************************************************************/
SOM_Scope Movie*  SOMLINK MovieFac_make_movie(MovieFactory *somSelf,
                                               Environment *ev,
                                              string title)
{
    Movie *myMovie;

    /* MovieFactoryData *somThis = MovieFactoryGetData(somSelf); */
    MovieFactoryMethodDebug("MovieFactory","MovieFac_make_movie");

    myMovie = new Movie();
    ((Movie*)myMovie)->_set_title(ev, title);

    return myMovie;
}


SOM_Scope void SOMLINK MovieFac_somDefaultInit(MovieFactory *somSelf,
                                               som3InitCtrl* ctrl)
{
    MovieFactoryData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    MovieFactoryMethodDebug("MovieFactory","somDefaultInit");
    MovieFactory_BeginInitializer_somDefaultInit;

    MovieFactory_Init_somLifeCycle_LifeCycleObject_somDefaultInit(somSelf, ctrl);

    /*
     * local MovieFactory initialization code added by programmer
     */
}


SOM_Scope void SOMLINK MovieFac_somDestruct(MovieFactory *somSelf,
                                            octet doFree, som3DestructCtrl* ctrl)
{
    MovieFactoryData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    MovieFactoryMethodDebug("MovieFactory","MovieFac_somDestruct");
    MovieFactory_BeginDestructor;

    /*
     * local MovieFactory deinitialization code added by programmer
     */

    MovieFactory_EndDestructor;
}

/********************************************************************************
 * The GameFac_make_game method is part of the 'user-defined' factory used
 * to create the actual Game object itself. This method is called from
 * create_object code (from within the real_object_creation method) after the
 * GenericRentalFactory code determines that a Game object has been requested.
 *
 * The 'title' parm is passed in to show how a user can customize the initialization
 * of an object...and where the attributes of the object are being set during
 * object creation.
 ********************************************************************************/
SOM_Scope Game*  SOMLINK GameFac_make_game(GameFactory *somSelf,
                                            Environment *ev,
                                           string title)
{
    /* GameFactoryData *somThis = GameFactoryGetData(somSelf); */
    GameFactoryMethodDebug("GameFactory","GameFac_make_game");

    Game     *myGame;

    myGame = new Game();
    ((Game*)myGame)->_set_title(ev, title);

    return myGame;
}

SOM_Scope void SOMLINK GameFac_somDefaultInit(GameFactory *somSelf,
                                              som3InitCtrl* ctrl)
{
    GameFactoryData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    GameFactoryMethodDebug("GameFactory","somDefaultInit");
    GameFactory_BeginInitializer_somDefaultInit;

    GameFactory_Init_somLifeCycle_LifeCycleObject_somDefaultInit(somSelf, ctrl);

    /*
     * local GameFactory initialization code added by programmer
     */
}


SOM_Scope void SOMLINK GameFac_somDestruct(GameFactory *somSelf,
                                           octet doFree, som3DestructCtrl* ctrl)
{
    GameFactoryData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    GameFactoryMethodDebug("GameFactory","GameFac_somDestruct");
    GameFactory_BeginDestructor;

    /*
     * local GameFactory deinitialization code added by programmer
     */

    GameFactory_EndDestructor;
}



SOM_Scope void  SOMLINK RentalItemLg_add_Rental_Item(RentalItemLog *somSelf,
                                                      Environment *ev,
                                                     RentalItem* RentalEntry)
{
    LogEntry *entry;

    RentalItemLogData *somThis = RentalItemLogGetData(somSelf);
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_add_Rental_Item");




    entry = (LogEntry*)SOMMalloc(sizeof(LogEntry));
    entry->Rental = (RentalItem*)((SOMDObject*)RentalEntry)->duplicate(ev);
    entry->prevRental = NULL;
    entry->nextRental = NULL;
    entry->prevMovie = NULL;
    entry->nextMovie = NULL;
    entry->prevGame = NULL;
    entry->nextGame = NULL;

    if (somThis->firstRental == NULL)
        somThis->firstRental = entry;
    else {
        somThis->lastRental->nextRental = entry;
        entry->prevRental = somThis->lastRental;
    }
    somThis->RentalCount++;
    somThis->lastRental = entry;

    if (_Movie && RentalEntry->somIsA(_Movie)) {
        if (somThis->firstMovie == NULL)
            somThis->firstMovie = entry;
        else {
            somThis->lastMovie->nextMovie = entry;
            entry->prevMovie = somThis->lastMovie;
        }
        somThis->MovieCount++;
        somThis->lastMovie = entry;
    }

    if (_Game && RentalEntry->somIsA(_Game)) {
        if (somThis->firstGame == NULL)
            somThis->firstGame = entry;
        else {
            somThis->lastGame->nextGame = entry;
            entry->prevGame = somThis->lastGame;
        }
        somThis->GameCount++;
        somThis->lastGame = entry;
    }

}



SOM_Scope void  SOMLINK RentalItemLg_remove_Rental_Item(RentalItemLog *somSelf,
                                                         Environment *ev,
                                                        long RentalItem_ID)
{
    long remID, curID;
    LogEntry *entry;

    RentalItemLogData *somThis = RentalItemLogGetData(somSelf);
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_remove_Rental_Item");
 /********************************************************************************
 * This method does not have a return value on it to indicate if the remove
 * was successful or not. This sample in general did not do specific exception
 * and error condition handling that would be done in non-sample programs.
 ********************************************************************************/
    entry = somThis->firstRental;
    remID = RentalItem_ID;
    while (entry)
    {
        curID = (entry->Rental)->_get_id(ev);
        if (remID == curID)
            break;
        if (remID > curID)
            return;
        entry = entry->nextRental;
    }

    if (entry == NULL)
        return ;

    somThis->RentalCount--;
    if (somThis->firstRental == entry)
        somThis->firstRental = entry->nextRental;
    if (somThis->lastRental == entry)
        somThis->lastRental = entry->prevRental;
    if (entry->nextRental != NULL)
        entry->nextRental->prevRental = entry->prevRental;
    if (entry->prevRental != NULL)
        entry->prevRental->nextRental = entry->nextRental;

    if (entry->prevMovie || entry->nextMovie) {
        somThis->MovieCount--;
        if (somThis->firstMovie == entry)
            somThis->firstMovie = entry->nextMovie;
        if (somThis->lastMovie == entry)
            somThis->lastMovie = entry->prevMovie;
        if (entry->nextMovie != NULL)
            entry->nextMovie->prevMovie = entry->prevMovie;
        if (entry->prevMovie != NULL)
            entry->prevMovie->nextMovie = entry->nextMovie;
    }

    if (entry->prevGame || entry->nextGame) {
        somThis->GameCount--;
        if (somThis->firstGame == entry)
            somThis->firstGame = entry->nextGame;
        if (somThis->lastGame == entry)
            somThis->lastGame = entry->prevGame;
        if (entry->nextGame != NULL)
            entry->nextGame->prevGame = entry->prevGame;
        if (entry->prevGame != NULL)
            entry->prevGame->nextGame = entry->nextGame;
    }

    ((SOMDObject*)(entry->Rental))->release(ev);
    SOMFree(entry);

}


SOM_Scope string  SOMLINK RentalItemLg_display_movies(RentalItemLog *somSelf,
                                                       Environment *ev)
{
    string display, bp, newdisplay, temp;
    long i, totalspace, remaining, newspace;
    LogEntry  *entry;

    RentalItemLogData *somThis = RentalItemLogGetData(somSelf);
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_display_movies");

    if (somThis->MovieCount > 0) {
        totalspace = somThis->MovieCount * 180;
        remaining = totalspace;
        display = bp = (string)SOMMalloc(totalspace);
        entry = somThis->firstMovie;
        for (i=somThis->MovieCount;i>0;i--) {
           temp = ((Movie*)entry->Rental)->display(ev);
           if (strlen(temp) > 0){
             if (strlen(temp) >= remaining) {
                    newspace = totalspace + (i * 180);
                    newdisplay = (string)SOMMalloc(newspace);
                    strcpy(newdisplay, display);
                    SOMFree(display);
                    display = newdisplay;
                    bp = display + strlen(display);
                    remaining = newspace - strlen(display);
                    totalspace = newspace;
             }
                strcpy(bp, temp);
                remaining -= strlen(bp);
                bp += strlen(bp);
            }
            entry = entry->nextMovie;
        }
    } else
        display = NULL;

    return display;

}



SOM_Scope string  SOMLINK RentalItemLg_display_games(RentalItemLog *somSelf,
                                                      Environment *ev)
{
    string display, bp, newdisplay, temp;
    long i, totalspace, remaining, newspace;
    LogEntry  *entry;

    RentalItemLogData *somThis = RentalItemLogGetData(somSelf);
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_display_games");

    if (somThis->GameCount > 0) {
        totalspace = somThis->GameCount * 180;
        remaining = totalspace;
        display = bp = (string)SOMMalloc(totalspace);
        entry = somThis->firstGame;
        for (i=somThis->GameCount;i>0;i--) {
           temp = (entry->Rental)->display(ev);
           if (strlen(temp) > 0){
             if (strlen(temp) >= remaining) {
                    newspace = totalspace + (i * 180);
                    newdisplay = (string)SOMMalloc(newspace);
                    strcpy(newdisplay, display);
                    SOMFree(display);
                    display = newdisplay;
                    bp = display + strlen(display);
                    remaining = newspace - strlen(display);
                    totalspace = newspace;
             }
                strcpy(bp, temp);
                remaining -= strlen(bp);
                bp += strlen(bp);
            }
            entry = entry->nextGame;
        }
    } else
        display = NULL;

     return display;

}



SOM_Scope string  SOMLINK RentalItemLg_display_log(RentalItemLog *somSelf,
                                                    Environment *ev)
{
    string display, bp, newdisplay, temp;
    long i, totalspace, remaining, newspace;
    LogEntry  *entry;

    RentalItemLogData *somThis = RentalItemLogGetData(somSelf);
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_display_log");

    if (somThis->RentalCount > 0) {
        totalspace = somThis->RentalCount * 180;
        remaining = totalspace;
        display = bp = (string)SOMMalloc(totalspace);
        entry = somThis->firstRental;
        for (i=somThis->RentalCount;i>0;i--) {
            temp = ((RentalItem*)entry->Rental)->display(ev);
           if (strlen(temp) > 0){
             if (strlen(temp) >= remaining) {
                    newspace = totalspace + (i * 180);
                    newdisplay = (string)SOMMalloc(newspace);
                    strcpy(newdisplay, display);
                    SOMFree(display);
                    display = newdisplay;
                    bp = display + strlen(display);
                    remaining = newspace - strlen(display);
                    totalspace = newspace;
             }
                strcpy(bp, temp);
                remaining -= strlen(bp);
                bp += strlen(bp);
            }
            entry = entry->nextRental;
        }
    } else
        display = NULL;

    return display;

}

SOM_Scope RentalItem*  SOMLINK RentalItemLg_find_RentalItem(RentalItemLog *somSelf,
                                                             Environment *ev,
                                                            long RentalID_to_find)
{
    long findID, curID;
    LogEntry *entry;

    RentalItemLogData *somThis = RentalItemLogGetData(somSelf);
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_find_RentalItem");

    entry = somThis->firstRental;
    findID = RentalID_to_find;
 /********************************************************************************
 * This method does not have a return value on it to indicate if the find
 * was successful or not. This sample in general did not do specific exception
 * and error condition handling that would be done in non-sample programs.
 ********************************************************************************/
    while (entry)
    {
        curID = ((RentalItem*)entry->Rental)->_get_id(ev);
        if (findID == curID)
            break;
        entry = entry->nextRental;
    }

    if (entry == NULL)
        return  (somThis->firstRental->Rental);

    return (entry->Rental);

}

SOM_Scope void SOMLINK RentalItemLg_somDefaultInit(RentalItemLog *somSelf,
                                                   som3InitCtrl* ctrl)
{
    RentalItemLogData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    RentalItemLogMethodDebug("RentalItemLog","somDefaultInit");
    RentalItemLog_BeginInitializer_somDefaultInit;

    RentalItemLog_Init_somLifeCycle_LifeCycleObject_somDefaultInit(somSelf, ctrl);

    somThis->firstRental = NULL;
    somThis->RentalCount = 0;
    somThis->firstMovie = NULL;
    somThis->MovieCount = 0;
    somThis->firstGame = NULL;
    somThis->GameCount = 0;

}


SOM_Scope void SOMLINK RentalItemLg_somDestruct(RentalItemLog *somSelf,
                                                octet doFree,
                                                som3DestructCtrl* ctrl)
{
    long i;
    LogEntry *entry, *nentry;
    Environment *ev = somGetGlobalEnvironment();

    RentalItemLogData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    RentalItemLogMethodDebug("RentalItemLog","RentalItemLg_somDestruct");
    RentalItemLog_BeginDestructor;

    /*
     * local RentalItemLog deinitialization code added by programmer
     */
    entry = somThis->firstRental;
    for (i=0;i<somThis->RentalCount;i++)
    {
        ((SOMDObject*)(entry->Rental))->release(ev);
        nentry = entry->nextRental;
        SOMFree(entry);
        entry = nentry;
    }
    somThis->firstRental = NULL;
    somThis->RentalCount = 0;
    somThis->firstMovie = NULL;
    somThis->MovieCount = 0;
    somThis->firstGame = NULL;
    somThis->GameCount = 0;

    RentalItemLog_EndDestructor;
}



SOM_Scope void  SOMLINK CustomerLg_add_customer(CustomerLog *somSelf,
                                                 Environment *ev,
                                                CustomerAccount* Customer_to_Add)
{
    CustEntry  *entry;

    CustomerLogData *somThis = CustomerLogGetData(somSelf);
    CustomerLogMethodDebug("CustomerLog","CustomerLg_add_customer");

    entry = (CustEntry*)SOMMalloc(sizeof(CustEntry));
    entry->Customer = (CustomerAccount*)((SOMDObject*)Customer_to_Add)->duplicate(ev);
    entry->prev = NULL;
    entry->next = NULL;

    if (somThis->firstCustomer == NULL)
        somThis->firstCustomer = entry;
    else {
        somThis->lastCustomer->next = entry;
        entry->prev = somThis->lastCustomer;
    }
    somThis->customerCount++;
    somThis->lastCustomer = entry;

    return;
}



SOM_Scope void  SOMLINK CustomerLg_remove_customer(CustomerLog *somSelf,
                                                    Environment *ev,
                                                   long CustomerID_to_Remove)
{
    long remID, curID;
    CustEntry *entry;

    CustomerLogData *somThis = CustomerLogGetData(somSelf);
    CustomerLogMethodDebug("CustomerLog","CustomerLg_remove_customer");

    entry = somThis->firstCustomer;
    remID = CustomerID_to_Remove;

 /********************************************************************************
  * This method does not have a return value on it to indicate if the remove
  * was successful or not. This sample in general did not do specific exception
  * and error condition handling that would be done in non-sample programs.
  ********************************************************************************/
    while (entry)
    {
        curID = (entry->Customer)->_get_CustomerID(ev);
        if (remID == curID)
            break;
        entry = entry->next;
    }

    if (entry == NULL)
        return ;

    somThis->customerCount--;
    if (somThis->firstCustomer == entry)
        somThis->firstCustomer = entry->next;
    if (somThis->lastCustomer == entry)
        somThis->lastCustomer = entry->prev;
    if (entry->next != NULL)
        entry->next->prev = entry->prev;
    if (entry->prev != NULL)
        entry->prev->next = entry->next;

    ((SOMDObject*)(entry->Customer))->release(ev);
    SOMFree(entry);

    return;

}


SOM_Scope CustomerAccount*  SOMLINK CustomerLg_find_customer(CustomerLog *somSelf,
                                                              Environment *ev,
                                                             long Custid_to_find)
{
    long findID, curID;
    CustEntry *entry;

    CustomerLogData *somThis = CustomerLogGetData(somSelf);
    CustomerLogMethodDebug("CustomerLog","CustomerLg_find_customer");

    entry = somThis->firstCustomer;
    findID = Custid_to_find;
    while (entry)
    {
        curID = ((CustomerAccount*)entry->Customer)->_get_CustomerID(ev);
        if (findID == curID)
            break;
        entry = entry->next;
    }
 /********************************************************************************
  * This method does not have a return value on it to indicate if the find
  * was successful or not. This sample in general did not do specific exception
  * and error condition handling that would be done in non-sample programs. In
  * this case, the first Customer in the log is returned if customer is not found.
  * There would be problems if the Log was empty and this occurred.
  ********************************************************************************/
    if (entry == NULL)
        return (somThis->firstCustomer->Customer);

    return ((CustomerAccount*)entry->Customer);
}



SOM_Scope string  SOMLINK CustomerLg_display_customer_list(CustomerLog *somSelf,
                                                            Environment *ev)
{
    string display, bp, newdisplay, temp;
    long i, totalspace, remaining, newspace;
    CustEntry  *entry;

    CustomerLogData *somThis = CustomerLogGetData(somSelf);
    CustomerLogMethodDebug("CustomerLog","CustomerLg_display_customer_list");

    if (somThis->customerCount > 0) {
        totalspace = somThis->customerCount * 180;
        remaining = totalspace;
        display = bp = (string)SOMMalloc(totalspace);
        entry = somThis->firstCustomer;
        for (i=somThis->customerCount;i>0;i--) {
            temp = ((CustomerAccount*)entry->Customer)->rentalHistory(ev);
           if (strlen(temp) > 0){
             if (strlen(temp) >= remaining) {
                    newspace = totalspace + (i * 180);
                    newdisplay = (string)SOMMalloc(newspace);
                    strcpy(newdisplay, display);
                    SOMFree(display);
                    display = newdisplay;
                    bp = display + strlen(display);
                    remaining = newspace - strlen(display);
                    totalspace = newspace;
             }
                strcpy(bp, temp);
                remaining -= strlen(bp);
                bp += strlen(bp);
            }
            entry = entry->next;
        }
    } else
        display = NULL;

    return display;

}



SOM_Scope void SOMLINK CustomerLg_somDefaultInit(CustomerLog *somSelf,
                                                 som3InitCtrl* ctrl)
{
    CustomerLogData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    CustomerLogMethodDebug("CustomerLog","somDefaultInit");
    CustomerLog_BeginInitializer_somDefaultInit;

    CustomerLog_Init_somLifeCycle_LifeCycleObject_somDefaultInit(somSelf, ctrl);

    somThis->firstCustomer = NULL;
    somThis->lastCustomer = NULL;
    somThis->customerCount = 0;
 }


SOM_Scope void SOMLINK CustomerLg_somDestruct(CustomerLog *somSelf,
                                              octet doFree, som3DestructCtrl* ctrl)
{
    CustomerLogData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    CustomerLogMethodDebug("CustomerLog","CustomerLg_somDestruct");
    CustomerLog_BeginDestructor;

    /*
     * local CustomerLog deinitialization code added by programmer
     */

    CustomerLog_EndDestructor;
}


SOM_Scope void  SOMLINK RentalLg_add_rental(RentalLog *somSelf,
                                             Environment *ev,
                                            RentObject* Rental_to_Add)
{

    RentalEntry  *entry;

    RentalLogData *somThis = RentalLogGetData(somSelf);
    RentalLogMethodDebug("RentalLog","RentalLg_add_rental");

    entry = (RentalEntry*)SOMMalloc(sizeof(RentalEntry));
    entry->RentalObject = (RentObject*)((SOMDObject*)Rental_to_Add)->duplicate(ev);
    entry->prev = NULL;
    entry->next = NULL;

    if (somThis->firstRental == NULL)
        somThis->firstRental = entry;
    else {
        somThis->lastRental->next = entry;
        entry->prev = somThis->lastRental;
    }
    somThis->rentalCount++;
    somThis->lastRental = entry;

    return;
}


SOM_Scope void  SOMLINK RentalLg_remove_rental(RentalLog *somSelf,
                                                Environment *ev,
                                               long rentalID_to_Remove)
{
    long remID, curID;
    RentalEntry *entry;

    RentalLogData *somThis = RentalLogGetData(somSelf);
    RentalLogMethodDebug("RentalLog","RentalLg_remove_rental");

    entry = somThis->firstRental;
    remID = rentalID_to_Remove;
    while (entry)
    {
        curID = (entry->RentalObject)->_get_rentalID(ev);
        if (remID == curID)
            break;

        entry = entry->next;
    }
 /********************************************************************************
  * This method does not have a return value on it to indicate if the remove
  * was successful or not. This sample in general did not do specific exception
  * and error condition handling that would be done in non-sample programs.
  ********************************************************************************/
    if (entry == NULL)
        return ;

    somThis->rentalCount--;
    if (somThis->firstRental == entry)
        somThis->firstRental = entry->next;
    if (somThis->lastRental == entry)
        somThis->lastRental = entry->prev;
    if (entry->next != NULL)
        entry->next->prev = entry->prev;
    if (entry->prev != NULL)
        entry->prev->next = entry->next;

    ((SOMDObject*)(entry->RentalObject))->release(ev);
    SOMFree(entry);

    return;
}


SOM_Scope string  SOMLINK RentalLg_display_rental_list(RentalLog *somSelf,
                                                        Environment *ev)
{
    string display, bp, newdisplay, temp;
    long i, totalspace, remaining, newspace;
    RentalEntry  *entry;

    RentalLogData *somThis = RentalLogGetData(somSelf);
    RentalLogMethodDebug("RentalLog","RentalLg_display_rental_list");

    if (somThis->rentalCount > 0) {
        totalspace = somThis->rentalCount * 180;
        remaining = totalspace;
        display = bp = (string)SOMMalloc(totalspace);
        entry = somThis->firstRental;
        for (i=somThis->rentalCount;i>0;i--) {
            temp = ((RentObject*)entry->RentalObject)->display(ev);
           if (strlen(temp) > 0){
             if (strlen(temp) >= remaining) {
                    newspace = totalspace + (i * 180);
                    newdisplay = (string)SOMMalloc(newspace);
                    strcpy(newdisplay, display);
                    SOMFree(display);
                    display = newdisplay;
                    bp = display + strlen(display);
                    remaining = newspace - strlen(display);
                    totalspace = newspace;
             }
                strcpy(bp, temp);
                remaining -= strlen(bp);
                bp += strlen(bp);
            }
            entry = entry->next;
        }
    } else
        display = NULL;

    return display;
}


SOM_Scope RentObject*  SOMLINK RentalLg_find_rental(RentalLog *somSelf,
                                                     Environment *ev,
                                                    long rental_to_find)
{
    long findID, curID;
    RentalEntry *entry;

    RentalLogData *somThis = RentalLogGetData(somSelf);
    RentalLogMethodDebug("RentalLog","RentalLg_find_rental");

    entry = somThis->firstRental;
    findID = rental_to_find;
    while (entry)
    {
        curID = ((RentObject*)entry->RentalObject)->_get_rentalID(ev);
        if (findID == curID)
            break;
        entry = entry->next;
    }
 /********************************************************************************
  * This method does not have a return value on it to indicate if the find
  * was successful or not. This sample in general did not do specific exception
  * and error condition handling that would be done in non-sample programs. In
  * this case, the first rental in the log is returned if rental is not found.
  * There would be problems if the Log was empty and this occurred.
  ********************************************************************************/
    if (entry == NULL)
        return (somThis->firstRental->RentalObject);

    return ((RentObject*)entry->RentalObject);
}

SOM_Scope void SOMLINK RentalLg_somDefaultInit(RentalLog *somSelf,
                                               som3InitCtrl* ctrl)
{
    RentalLogData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    RentalLogMethodDebug("RentalLog","somDefaultInit");
    RentalLog_BeginInitializer_somDefaultInit;

    RentalLog_Init_somLifeCycle_LifeCycleObject_somDefaultInit(somSelf, ctrl);

    somThis->firstRental = NULL;
    somThis->lastRental = NULL;
    somThis->rentalCount = 0;
  }


SOM_Scope void SOMLINK RentalLg_somDestruct(RentalLog *somSelf,
                                            octet doFree, som3DestructCtrl* ctrl)
{


    RentalLogData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    RentalLogMethodDebug("RentalLog","RentalLg_somDestruct");
    RentalLog_BeginDestructor;



    RentalLog_EndDestructor;
}


