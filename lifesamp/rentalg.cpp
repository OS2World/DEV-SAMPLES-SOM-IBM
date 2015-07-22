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


#ifndef SOM_Module_rentalg_Source
#define SOM_Module_rentalg_Source
#endif
#define GenericRentalFactory_Class_Source
#define SOMMSingleInstance_Class_Source

#include "rentalg.xih"
#include "rental.xh"
#include "rentalf.xh"
#include <somd.xh>


  /*******************************************************************************************
   The GenericRentalFac is designed to be able to create Movie, Game and CustomerAccount
   objects.  The GenericRentalFac has overridden the real_object_creation method in order
   to handle the two user-defined factories; MovieFactory and GameFactory.  There is no
   user-defined factory for the CustomerAccount object, so no additional logic will need
   to be added to this method to handle that.  The parent (provided by LifeCycle Service)
   real_object_creation method will handle the creation of CustomerAccount objects.
   NOTE:  This method raises no errors, but real scenarios will want to check the validity
   of the criteria passed in and the associated values.  The CosLifeCycle::InvalidCriteria
   or CosLifeCycle::CannotMeetCriteria exceptions could be raised if necessary.  Also, it
   may be beneficial to combine the criteria extraction code into one subroutine that will
   suit the needs of the user environment.  In this sample we keep the code separate to
   illustrate the logic.
   *******************************************************************************************/

SOM_Scope SOMObject*  SOMLINK GenericRentalFac_real_object_creation(GenericRentalFactory *somSelf,
                                                                     Environment *ev,
                                                                    CosLifeCycle_Factory* factory,
                                                                    CosLifeCycle_Criteria* the_criteria)
{

    SOMObject*   obj_to_return;              /* object reference to return after creation.  freed by the caller  */
    string       item_title;                 /* title information for use on the make_movie or make_game methods */
    int          index;                      /* index used to traverse the_criteria */
    boolean      found = FALSE;              /* indicates if the item has been found in the_criteria */
    CosLifeCycle_NVP *nvp;

    /* GenericRentalFactoryData *somThis = GenericRentalFactoryGetData(somSelf); */
    GenericRentalFactoryMethodDebug("GenericRentalFactory","GenericRentalFac_real_object_creation");

    /* Check to see if the factory we are working with is a MovieFactory */
    if (factory->somIsA(_MovieFactory))
       {
       /*************************************************************************************
        * extract the title from the_criteria since it's needed for the make_movie call.
        * Traverse the_criteria until the title is found or all portions have been examined
        *************************************************************************************/

          for (index=0;index<the_criteria->_length;index++)
             {
             nvp = the_criteria->_buffer + index;
             if (strcmp("crit_title",nvp->name) == 0)
                {
                 item_title = (string)(nvp->value._value);
                 break;
                }
             }
       /* call make_movie to create an instance of a Movie */
       obj_to_return = ((MovieFactory*)factory)->make_movie(ev, item_title);

       /* return the newly created object to the caller */
       return obj_to_return;
       }

    if (factory->somIsA(_GameFactory))
       {
       /*************************************************************************************
        * extract the title from the_criteria since it's needed for the make_game call.
        * Traverse the_criteria until the title is found or all portions have been examined
        *************************************************************************************/

             for (index=0;index<the_criteria->_length;index++)
               {
               nvp = the_criteria->_buffer + index;
               if (strcmp("crit_title",nvp->name) == 0)
                  {
                   item_title = (string)(nvp->value._value);
                   break;
                  }
               }

       /* call make_game to create an instance of a Game */
       obj_to_return = ((GameFactory*)factory)->make_game(ev, item_title);

       /* return the newly created object to the caller */
       return obj_to_return;
       }
    /************************************************************************************
     * if we get here, we must be working with a somClass factory, so let the LifeCycle
     * real_object_creation method do the processing to create the object.
     ************************************************************************************/

    return (GenericRentalFactory_parent_somLifeCycle_GenericFactory_real_object_creation(somSelf,
                                                                                         ev,
                                                                                         factory,
                                                                                         the_criteria));


}

  /**************************************************************************************
   * The GenericRentalFac is designed to create Movie, Game and CustomerAccount objects
   * and initialize the newly created objects based on what is in the_criteria.  As a
   * result, the initialize_object method was overridden and will process the
   * initialization necessary based on the type of object.
   * NOTE:  This sample override does not raise any exceptions, but in a real-life
   * scenario it may be desirable to raise the CosLifeCycle::InvalidCriteria or
   * CosLifeCycle::CannotMeetCriteria exceptions in certain cases.
   *
   * NOTE:  In this sample the criteria extraction logic is shown separately for each
   * object.  It may be desirable to combine this logic into one subroutine that would
   * meet the needs of all objects.
   *
   * The initialize_object method should be overridden anytime there are specific
   * initialization requirements for an object.  This method is always called by
   * the create_object method regardless of what type of factory (SOMClass or user-
   * defined) creates the object.
   **************************************************************************************/


SOM_Scope void  SOMLINK GenericRentalFac_initialize_object(GenericRentalFactory *somSelf,
                                                            Environment *ev,
                                                           SOMObject* obj,
                                                           CosLifeCycle_Criteria* the_criteria)
{
    string        item_purchaser;   /* purchaser information for Movie or Game initialization */
    int           index;            /* index used to traverse the_criteria */
    long          item_category;    /* category information for Movie or Game initialization */
    long          item_rating;      /* rating informatino for Movie or Game initialization */
    RentalItemLog *item_log;        /* the log to add the item to for inventory purposes */
    string        item_lname;       /* last name information for CustomerAccount initialization */
    string        item_fname;       /* first name information for CustomerAccount initialization */
    long          item_custID;      /* Customer ID information for CustomerAccount initialization */
    CustomerLog   *item_cust_log;   /* the log to add the CustomerAccount to for recording purposes */
    CosLifeCycle_NVP *nvp;

    /* GenericRentalFactoryData *somThis = GenericRentalFactoryGetData(somSelf); */
    GenericRentalFactoryMethodDebug("GenericRentalFactory","GenericRentalFac_initialize_object");

    /*******************************************************************************************
     * check to see if we are working with a Movie or Game.  If so, the initialization process
     * is similar, so follow the same logic when extracting from the_criteria.
     *******************************************************************************************/

    if ((obj->somIsA(_Movie)) || (obj->somIsA(_Game)))
       {
       /***************************************************************
        * extract the initialization information from the_criteria.
        * Traverse the_criteria until all portions have been examined
        ***************************************************************/

       for (index=0; index<the_criteria->_length; index++)
          {
          nvp = the_criteria->_buffer + index;

          /*************************************************************************
           * check to see if the current index is the buyer. If so, initialize obj
           * with the purchaser information.
           *************************************************************************/
          if (strcmp("crit_buyer", nvp->name) == 0)
             {
             item_purchaser = (string)(nvp->value._value);
             ((RentalItem*)obj)->_set_buyer(ev, item_purchaser);
             }
          /*************************************************************************
           * check to see if the current index is the category. If so,
           * initialize obj with the category information.
           *************************************************************************/

          if (strcmp("crit_category", nvp->name) == 0)
             {
             item_category = *(long*)(nvp->value._value);
             ((Movie*)obj)->_set_category(ev, item_category);
             }
          /************************************************************************
           * check to see if the current index is the rating. If so,
           * initialize obj with the rating information.
           ************************************************************************/

          if (strcmp("crit_rating", nvp->name) == 0)
             {
             item_rating = *(long*)(nvp->value._value);
             ((Movie*)obj)->_set_rating(ev, item_rating);
             }

          /************************************************************************
           * check to see if the current index is the log. If so,
           * add the object to the movie or game log.
           ************************************************************************/

          if (strcmp("crit_log_ref", nvp->name) == 0)
             {
             item_log = (RentalItemLog*)(nvp->value._value);
             item_log->add_Rental_Item(ev, (Movie*)obj);
             }
          /************************************************************************
           * if it is none of the above options, this criteria portion will be
           * ignored and the next NameValuePair will be examined.
           ************************************************************************/
          }

       /***************************************************************************
        * when we reach this point, all of the pieces have been extracted
        * from the criteria for the initialization of a Movie or Game.
        * NOTE:  This sample assumes that the user passes in all necessary
        * initialization information in the_criteria.  Others that override
        * this method may want to perform additional validation and error
        * checking.
        ***************************************************************************/

       /* Return to caller, since the Movie or Game is now initialized. */
       return;
       }

    if (obj->somIsA(_CustomerAccount))
       {
       /******************************************************************
        * extract the initialization information from the_criteria.
        * Traverse the_criteria until all portions have been examined
        ******************************************************************/

       for (index=0; index<the_criteria->_length; index++)
          {
          nvp = the_criteria->_buffer + index;
          /**************************************************************
           * check to see if the current index is the firstName. If so,
           * set item_fName to the value
           **************************************************************/

          if (strcmp("crit_fname", nvp->name) == 0)
             {
             item_fname = (string)(nvp->value._value);
             ((CustomerAccount*)obj)->_set_firstName(ev, item_fname);
             }
          /***************************************************************
           * check to see if the current index is the lastName. If so,
           * set item_lName to the value
           ***************************************************************/

          if (strcmp("crit_lname", nvp->name) == 0)
             {
             item_lname = (string)(nvp->value._value);
             ((CustomerAccount*)obj)->_set_lastName(ev, item_lname);
             }

          /***************************************************************
           * check to see if the current index is the CustomerID. If so,
           * set item_custID to the value
           ***************************************************************/
          if (strcmp("crit_custID", nvp->name) == 0)
             {
             item_custID = *(long*)(nvp->value._value);
             ((CustomerAccount*)obj)->_set_CustomerID(ev, item_custID);
             }

          /***************************************************************
           * check to see if the current index is the log. If so,
           * add the object to the CustomerAccount log.
           ***************************************************************/
          if (strcmp("crit_cust_log_ref", nvp->name) == 0)
             {
             item_cust_log = (CustomerLog*)(nvp->value._value);
             item_cust_log->add_customer(ev, (CustomerAccount*)obj);
             }

          /*********************************************************************
           * if it is none of the above options, this criteria portion will be
           * ignored and the next NameValuePair will be examined.
           *********************************************************************/
          }

       /************************************************************************
        * when we reach this point, all of the pieces have been extracted
        * from the criteria for the initialization of a CustomerAccount.
        * NOTE:  This sample assumes that the user passes in all necessary
        * initialization information in the_criteria.  Others that override
        * this method may want to perform additional validation and error
        * checking.
        * Return to caller, since the CustomerAccount is now initialized.
        ************************************************************************/

       return;
       }
    /*******************************************************************************
     * if we get here, we don't have any specific initialization requests, so just
     * call the parent initialize_object method.
     *******************************************************************************/
    GenericRentalFactory_parent_somLifeCycle_GenericFactory_initialize_object(somSelf,
                                                                              ev,
                                                                              obj,
                                                                              the_criteria);
}

SOM_Scope void SOMLINK GenericRentalFac_somDefaultInit(GenericRentalFactory *somSelf,
                                                       som3InitCtrl* ctrl)
{
    GenericRentalFactoryData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    GenericRentalFactoryMethodDebug("GenericRentalFactory","somDefaultInit");
    GenericRentalFactory_BeginInitializer_somDefaultInit;

    GenericRentalFactory_Init_somLifeCycle_GenericFactory_somDefaultInit(somSelf, ctrl);

    /*
     * local GenericRentalFactory initialization code added by programmer
     */
}


SOM_Scope void SOMLINK GenericRentalFac_somDestruct(GenericRentalFactory *somSelf,
                                                    octet doFree,
                                                    som3DestructCtrl* ctrl)
{
    GenericRentalFactoryData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    GenericRentalFactoryMethodDebug("GenericRentalFactory","GenericRentalFac_somDestruct");
    GenericRentalFactory_BeginDestructor;

    /*
     * local GenericRentalFactory deinitialization code added by programmer
     */

    GenericRentalFactory_EndDestructor;
}





