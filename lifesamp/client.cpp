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
 * This client.cpp is the client code for the overall rental sample. Although
 * the client code drives the sample application, it's main intent is to show
 * many of the methods that are available on the LifeCycle Service interfaces
 * and explain how they can be used to tailor and simplify the work that needs
 * to be done for object creation and deletion.
 ********************************************************************************/

#include <stdio.h>
#include <iostream.h>

#include <somd.xh>
#include <somlc.xh>
#include <somlcdef.h>
#include <somnm.xh>

#include "rental.xh"
#include "rentalf.xh"
#include "rentalg.xh"
#include <somnm.xh>
#include <somosutl.h>

int checkEnv (Environment *);  // prototype for a function that checks/clears
                               // environment variable.
//-------------------------------------------------------------------------
//  Global declares
//-------------------------------------------------------------------------


SOMEXTERN main()
{
  Environment   *ev;
  ev = somGetGlobalEnvironment();
  SOM_InitEnvironment(ev);
  SOMD_Init(ev);

 /*************************************************************************
 *  Global declares
 **************************************************************************/

  CosNaming_NameComponent         NC[7];

  somLifeCycle_GenericFactory     *myGF;

  RentalItemLog                   *myRIL;
  CustomerLog                     *myCL;
  RentalLog                       *myRL;

  GenericRentalFactory            *myGRF;
  RentalManager                   *myRM;
  GenericCounter                  *myGC;

  somLifeCycle_FactoryFinder      *myFF;
  somLifeCycle_FactoryFinder      *currentFF;

 /**********************************************************************
  *
  * Define a Key and Criteria to be used on the create_object method.
  * The Key is a sequence of structures that each contain 2 strings:
  * the 'kind' and 'Id'. These structures are used to identify the
  * properties used when searching for a factory to create the actual
  * object.
  *
    **********************************************************************/
  CosNaming_NameComponent     elements[2];
  CosLifeCycle_Key            myKey = {2,0,elements};

  CosLifeCycle_NameValuePair  nvPair[3];
  CosLifeCycle_Criteria       myCriteria = {3,0,nvPair};
  boolean                     InitVal = TRUE;

 /*************************************************************************
  *
  * The following code declares a naming context and then resolves to the
  * local root naming context using resolve_initial_references.
  *************************************************************************/

  ExtendedNaming_ExtendedNamingContext* rootNC;

  rootNC = (ExtendedNaming_ExtendedNamingContext *)
        SOMD_ORBObject->resolve_initial_references (ev, "NameService");

  checkEnv(ev);

  CosNaming_Name* GenericName;
  GenericName = new CosNaming_Name;

  GenericName->_buffer = (CosNaming_NameComponent*)SOMMalloc(sizeof(CosNaming_NameComponent));
  GenericName->_buffer[0].id = ".:";
  GenericName->_buffer[0].kind = "";
  GenericName->_length = GenericName->_maximum = 1;

 /*****************************************************************************
  * The code then resolves to the global root naming context performing a
  * resolve on the local root.  The id of the GenericName (set to ".:" ) indicates
  * the global root naming context.
  ******************************************************************************/

  ExtendedNaming_ExtendedNamingContext* GrootNC;
  GrootNC = (ExtendedNaming_ExtendedNamingContext*)rootNC->resolve(ev, GenericName);

 /*****************************************************************************
  * Resolve to the default GenericFactory and
  * default FactoryFinder objects that are bound into the Global root  context.
  * These to objects were bound at som_cfg time and the user can resolve to
  * these objects and use them as appropriate.
  *****************************************************************************/

  GenericName->_buffer[0].id = "LifeCycleGenericFactory";
  GenericName->_buffer[0].kind = "";

  myGF = (somLifeCycle_GenericFactory*)(void*)GrootNC->resolve(ev, GenericName);

  checkEnv(ev);

  GenericName->_buffer[0].id = "LifeCycleFactoryFinder";
  GenericName->_buffer[0].kind = "";

  myFF = (somLifeCycle_FactoryFinder*)(void*)GrootNC->resolve(ev, GenericName);

  checkEnv(ev);

 /***************************************************************************
  *
  * Create the key to be used on the create_object for a GenericRentalFactory.
  * A CosLifeCycle_Key is a sequence of structures that each contain two
  * strings. A 'kind' string that identifies if an object interface, factory
  * interface, constraint, ConstraintBuilder, or FactoryFilter is being
  * defined. A 'Id' string which give the details for the associated 'kind'.
  * In this case, the key contains only 1 kind/id pair which specifies that
  * a factory should be used that supports the creation of objects that
  * has a 'RentObject' as its principal interface. The interface (Id) itself
  * needs to be defined in standard CORBA IDL.
  *
  ***************************************************************************/

  myKey._buffer[0].kind = KIND_OBJ_INF;
  myKey._buffer[0].id = "GenericRentalFactory";
  myKey._length = 1;

 /**********************************************************************
  * Create a simple criteria to do initialization on the object. There
  * is only one name/value pair in this case, and it specifies that
  * SOM Initiatization should occur (based on the InitVal = TRUE). The
  * object will be created somNew rather than somNewNoinit.
  **********************************************************************/

  myCriteria._buffer[0].name = SOM_INIT;
  myCriteria._buffer[0].value._type = TypeCodeNew(tk_boolean);
  myCriteria._buffer[0].value._value = &InitVal;
  myCriteria._length = myCriteria._length + 1;

 /**********************************************************************
  * Create the GenericRentalFactory.
  **********************************************************************/

  myGRF = (GenericRentalFactory*)((void*)myGF->create_object(ev, &myKey, &myCriteria));

  checkEnv(ev);

  /****************************************************************************
   * When using create_object on a Generic Factory(in this case the
   * GenericRentalFactory) a factory finder needs to be registerd with it.
   * set_factory_finder performs the registration.
   ****************************************************************************/

  myGRF->set_factory_finder(ev, myFF);

  /****************************************************************************
   * If the application wants to get the factory finder that is currently
   * registered with myGRF (the GenericRentalFactory), use get_factory_finder
   ****************************************************************************/

  currentFF = myGRF->get_factory_finder(ev);

  checkEnv(ev);

 /********************************************************************************
  * Create the RentalManager object and bind to the local root naming context
  * with name 'RentalManagerNm'. The criteria is the same as was used
  * in the previous create_object.
  ********************************************************************************/
  myKey._buffer[0].kind = KIND_OBJ_INF;
  myKey._buffer[0].id = "RentalManager";

  myRM = (RentalManager*)((void*)myGRF->create_object(ev, &myKey, &myCriteria));
  checkEnv(ev);

  // GenericName->_buffer[0].id = "RentalManagerNm";
  // rootNC->bind(ev, GenericName, myRM);

  // checkEnv(ev);

  myRM->RentalManager::admin(ev);

  // GenericName->_buffer[0].id = "RentalManagerNm";
  // GenericName->_buffer[0].kind = "";

  // myRM = (RentalManager*)(void*)GrootNC->resolve(ev, GenericName);

  checkEnv(ev);


 /************************************************************************
  * purchase multiple rental items   (both movies and games)
  ************************************************************************/

    string title = "Superman strikes back";
    long category = 1;
    long rating = 1;
    string buyer = "Jane Jungler";
    char movie_er_game = 'M';

    myRM->purchase(ev, title, category, rating, buyer, movie_er_game);

    title = "Ghost of MJ Thompson";
    category = 2;
    rating = 2;
    buyer = "Shelley Austin";
    movie_er_game = 'M';

    myRM->purchase(ev, title, category, rating, buyer, movie_er_game);

    title = "Mortal Combat";
    category = 1;
    rating = 1;
    buyer = "Kristin Hafner";
    movie_er_game = 'G';

    myRM->purchase(ev, title, category, rating, buyer, movie_er_game);

    title = "Revenge of the Nomads";
    category = 2;
    rating = 2;
    buyer = "Jeff Naig";
    movie_er_game = 'G';

    myRM->purchase(ev, title, category, rating, buyer, movie_er_game);


   /************************************************************************
    *  Add several customers
    ************************************************************************/

    string lastname = "Nungusser";
    string firstname = "Fred";
    long customer_id = 1111111;

    myRM->add_customer(ev, lastname, firstname, customer_id);


    lastname = "Atkinson";
    firstname = "Brian";
    customer_id = 3333333;

    myRM->add_customer(ev, lastname, firstname, customer_id);

    lastname = "Gieske";
    firstname = "Larry";
    customer_id = 2222222;

    myRM->add_customer(ev, lastname, firstname, customer_id);



    /************************************************************************
    *  check out and check in some rental items
    ************************************************************************/

    long custid = 3333333;
    long rentalid = 1;

    myRM->check_out(ev, custid, rentalid);

    custid = 2222222;
    rentalid = 2;

    myRM->check_out(ev, custid, rentalid);

    custid = 3333333;
    rentalid = 3;

    myRM->check_out(ev, custid, rentalid);

    custid = 1111111;
    rentalid = 4;

    myRM->check_out(ev, custid, rentalid);

   /************************************************************************
    *  Print individual rental data
    ************************************************************************/
    string  Print_Buffer;

    Print_Buffer = myRM->display_rental_info(ev, 1);
    somPrintf(Print_Buffer);

    Print_Buffer = myRM->display_rental_info(ev, 2);
    somPrintf(Print_Buffer);

    Print_Buffer = myRM->display_rental_info(ev, 3);
    somPrintf(Print_Buffer);


   /************************************************************************
    *  Print individual customer data
    ************************************************************************/


    Print_Buffer = myRM->display_cust_info(ev, 1111111);
    somPrintf(Print_Buffer);

    Print_Buffer = myRM->display_cust_info(ev, 22222222);
    somPrintf(Print_Buffer);

    Print_Buffer = myRM->display_cust_info(ev, 33333333);
    somPrintf(Print_Buffer);



   /************************************************************************
    * Print the data in all Logs (RentalItem, Customer, and Rental)
    ************************************************************************/

    somPrintf("\n this is the log info \n \n");

    RentalItemLog   *Print_RentalItemLog;
    Print_RentalItemLog = myRM->_get_RentalItem_Log(ev);

    RentalLog   *Print_RentalLog;
    Print_RentalLog = myRM->_get_Rental_Log(ev);

    CustomerLog   *Print_CustomerLog;
    Print_CustomerLog = myRM->_get_Customer_Log(ev);

    Print_Buffer = Print_RentalItemLog->display_movies(ev);
    somPrintf(Print_Buffer);

    Print_Buffer = Print_RentalItemLog->display_games(ev);
    somPrintf(Print_Buffer);

    somPrintf("\n this is the rental item log list \n \n");

    Print_Buffer = Print_RentalItemLog->display_log(ev);
    somPrintf(Print_Buffer);

    somPrintf("\n this is the customer log list \n \n");

    Print_Buffer = Print_CustomerLog->display_customer_list(ev);
    somPrintf(Print_Buffer);

    somPrintf("\n this is the rental log list \n \n");

    Print_Buffer = Print_RentalLog->display_rental_list(ev);
    somPrintf(Print_Buffer);


   /************************************************************************
    *  check in all rentals (remove the rental from the rental log)
    ************************************************************************/

    rentalid = 1;
    myRM->check_in(ev, rentalid);

    rentalid = 2;
    myRM->check_in(ev, rentalid);

    rentalid = 3;
    myRM->check_in(ev, rentalid);

    rentalid = 4;
    myRM->check_in(ev, rentalid);

   /************************************************************************
    *  Remove the customers
    ************************************************************************/

    myRM->remove_customer(ev, 1111111);
    myRM->remove_customer(ev, 2222222);
    myRM->remove_customer(ev, 3333333);

   /************************************************************************
    *  Remove the Rental Items
    ************************************************************************/

    myRM->remove_RentalItem(ev, 1);
    myRM->remove_RentalItem(ev, 2);
    myRM->remove_RentalItem(ev, 3);
    myRM->remove_RentalItem(ev, 4);

    return 1;
}


/******************************************************************************
 * Routine to check the environment variable for Name not found, Name already bound
 * or just print out the exception.
 ******************************************************************************/

int checkEnv(Environment *ev)
  {
        int ret = 0;

    // check the major code in ev
    if (ev->_major != NO_EXCEPTION)
    {
        cout<<"\nEXCEPTION=> LifeCycle Sample:checkenv: ";
        if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_NotFound) == 0)
            {
            cout<<"Name not found !\n"<<endl;
            somExceptionFree(ev);
            ret = 1;
            }
        else
            if (strcmp(somExceptionId(ev), ex_CosNaming_NamingContext_AlreadyBound) == 0)
                {
                cout<<"Name already bound !\n"<<endl;
                somExceptionFree(ev);
                ret = 1;
                }
            else
                if (strcmp(somExceptionId(ev), "::LNameComponent::NotSet") == 0)
                   {
                   // Ignore this one, trivial exception set if ID or KIND
                   // in the name is not set.
                   cout<<"LName Exception- ID or KIND not set!\n"<<endl;
                   somExceptionFree(ev);
                   ret = 1;
                   }
                else
                   {
                   cout<<" <<"<<somExceptionId(ev)<<">> !! \n"<<endl;
                   somExceptionFree(ev);
                   ret = 1;
                   }
    }
    return ret;
  }




