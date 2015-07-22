/*
 *
 *   25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */

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

#include <iostream.h> // header file for the cout functions
#include <stdio.h> // Need this for the sprintf function
#include <ctype.h>
#include <somd.xh> // DSOM include file
#include <somlc.xh> // Life cycle header file
#include <somnm.xh> // Naming include file
#include "vehicle.xh" // Naming sample object header file
#include "NameMgr.h"  // Name manager object header

/*************************************************************************

 This sample demonstrates the major function of the Naming service as
 defined in SOMObjects 3.0.  Detailed information can be found in the
 Programmers Guide, Volume II: Object Services and in the Programmers
 Reference, Volume II: Object Services.  The sample takes you through
 creating a naming tree which will look something like this:
                     vehicles
                   /          \
             Antique         Modern
          /       \          /         \
    HotRod     Sedan     SportUtility   Vans
                          /       \
                      Blazer    Jimmy

  Our main context is the vehicle context, contexts are analagous to
 directories in many file systems.  The Antique and Modern contexts further
 define and the lowest sub-contexts are HotRod, Sedan, SportUtility, and Vans.
 Two objects are created under the SportUtility context, Blazer and Jimmy.

  Output from the sample will look like:

 Nametest:Main: begin execution

 Nametest:Main: Print naming context:
  Name<[Antique/classification]>type<context>PropertyNames<>
   Name<[HotRod/category]>type<context>PropertyNames<>
   Name<[Sedan/category]>type<context>PropertyNames<>
  Name<[Modern/classification]>type<context>PropertyNames<>
   Name<[SportUtility/category]>type<context>PropertyNames<>
   Name<[Vans/category]>type<context>PropertyNames<>

 Nametest:Main: Print naming context:
  Name<[Antique/classification]>type<context>PropertyNames<>
   Name<[HotRod/category]>type<context>PropertyNames<>
   Name<[Sedan/category]>type<context>PropertyNames<>
  Name<[Modern/classification]>type<context>PropertyNames<>
   Name<[SportUtility/category]>type<context>PropertyNames<>
    Name<[Blazer/vehicle]>type<object>PropertyNames<[EPAmpg]/21 [Miles]/36500 >
    Name<[Jimmy/vehicle]>type<object>PropertyNames<[EPAmpg]/22 [Miles]/40000 >
   Name<[Vans/category]>type<context>PropertyNames<>

Enter any character to continue: x

 Nametest:Main: Searching using constraint: 'EPAmpg < 23 and Miles < 39000'
  Make of found object: CHEVROLET
  Model of found object: Blazer
  Year of found object: 1993
  MPG of found object: 20
  Miles of found object: 36500

 Nametest:Main: Resolving using a name of 'Jimmy'
  Make of resolved object: GMC
  Model of resolved object: Jimmy
  Year of resolved object: 1995
  MPG of resolved object: 22
  Miles of resolved object: 40000

 Resolving using a compound name of 'Modern/SportUtility/Blazer'
  Make of resolved object: CHEVROLET
  Model of resolved object: Blazer
  Year of resolved object: 1993
  MPG of resolved object: 20
  Miles of resolved object: 36500

Enter any character to continue: x

 Nametest: Main: Adding Index......

 Nametest:Main: Listing Index......
    Index listed has id = EPAmpg

 Nametest:Main: Removing Index......

 Program complete, returning...

*************************************************************************/

    // Global pointer to the som environment
    Environment* ev;

int main (int argc, char* argv[])
{
    // Starting message
    cout << "Nametest:Main: begin execution\n" << endl;

    // Set the environment variable
    ev = somGetGlobalEnvironment();

    //Initialize the DSOM environment
    SOMD_Init(ev);

    // misc variables
    char** ptr;
    long* longptr;
    long mpg;
    long miles;
    int i, indent = 0;
    char pause;

  /***************************************************************
   *************  NAMING BASICS - ROOT NAMING CONTEXT   **********
   ***************************************************************
   *
   *   In this section we will get our own 'copy' of the root naming
   * context.  A context is analagous to a directory in many file
   * systems.  So the root directory or context is the same.  We will
   * need this root context to anchor the base of our naming tree.
   * We will need to instantiate an object of type NameMgr which is
   * part of this sample.  The NameMgr encapsulates much of the
   * function we will use.
   *
   *****************************************************************/

   // Get our NameMgr object
   NameMgr* NameManager = new NameMgr(1);

   // Variable to store the root Naming Context
   ExtendedNaming_ExtendedNamingContext*  rootNC;

   // Get the root naming context
   rootNC = NameManager->GetRootNC(ev);

  /***************************************************************
   *************     NAMING BASICS - CREATING NAMES     **********
   ***************************************************************
   *
   *
   *   The name itself is central to using the service.  A name is a
   * sequence, it consists of a length, a maximum, and a pointer to a buffer.
   * Length and maximum define how many components are in the buffer.
   * The buffer is a CosNaming_NameComponent, which is an array of id and
   * kind pointers.  The id is in actuality the name.  In our first name
   * the array is of length 1, so there is only one id and kind pair.
   * The same name will be used throughout the sample, the id and kind will
   * simply be changed to name the object we are working with
   ****************************************************************/

   // Reset the name
   NameManager->NewName(1);

  /***************************************************************
   *************    CREATING AND BINDING CONTEXTS       **********
   ***************************************************************
   *
   *
   *   A context is created and bound to the root Naming Context (rootNC)
   * Others are created and bound as subcontexts.
   * Binding is the process of associating an object (context) with another
   * context and associating a names also.
   * So in this section, we create the context vehicles and bind it to the root.
   * create and bind Modern and Antique contexts under vehicles, with
   * SportUtility/Vans and HotRod/Sedan bound under Modern and Antique
   * respectively.
   *
   *****************************************************************/

   // Declare our base context vehicles
   ExtendedNaming_ExtendedNamingContext* vehicles =
         NameManager->CreateContext(ev, rootNC);

   NameManager->checkEnv(ev);

   // Set the name's new id and kind.
   NameManager->SetName(0, "vehicles", "land vehicles");

   // Bind the vehicle context to the root.
   NameManager->BindContext(ev, rootNC, vehicles);
   NameManager->checkEnv(ev);


   // Below we will create the sub contexts and bind them also, repeating the
   // code from above for each new context
   ExtendedNaming_ExtendedNamingContext* Modern = NameManager->CreateContext(ev, vehicles);
   NameManager->SetName(0, "Modern", "classification");
   Modern = NameManager->BindContext(ev, vehicles, Modern);
   NameManager->checkEnv(ev);

   ExtendedNaming_ExtendedNamingContext* Antique = NameManager->CreateContext(ev, vehicles);
   NameManager->SetName(0, "Antique", "classification");
   Antique = NameManager->BindContext(ev, vehicles, Antique);
   NameManager->checkEnv(ev);

   ExtendedNaming_ExtendedNamingContext* SportUtility = NameManager->CreateContext(ev, Modern);
   NameManager->SetName(0, "SportUtility", "category");
   SportUtility = NameManager->BindContext(ev, Modern, SportUtility);
   NameManager->checkEnv(ev);

   ExtendedNaming_ExtendedNamingContext* Vans = NameManager->CreateContext(ev, Modern);
   NameManager->SetName(0, "Vans", "category");
   Vans = NameManager->BindContext(ev, Modern, Vans);
   NameManager->checkEnv(ev);

   ExtendedNaming_ExtendedNamingContext* HotRod = NameManager->CreateContext(ev, Antique);
   NameManager->SetName(0, "HotRod", "category");
   HotRod = NameManager->BindContext(ev, Antique, HotRod);
   NameManager->checkEnv(ev);

   ExtendedNaming_ExtendedNamingContext* Sedan = NameManager->CreateContext(ev, Antique);
   NameManager->SetName(0, "Sedan", "category");
   Sedan = NameManager->BindContext(ev, Antique, Sedan);
   NameManager->checkEnv(ev);

   // Call the dumpacontext function to show our basic naming tree.
   NameManager->DumpContext(ev, vehicles, indent);


  /***************************************************************
   ********   PROPERTY LISTS - CREATING, SETTING, BINDING   ******
   ***************************************************************
   *
   *
   * Here, we will create an ExtendedNaming_PropertyList.  a property list
   * is a sequence, consisting of a length, a maximum, and buffer which
   * is an ExtendedNaming_Property. An ExtendedNaming_property is simply
   * an array of structures consisting of name/value pairs.
   * In this section we create a property list with two properties EPAmpg
   * and Miles.  We also set some preliminary values into the property.
   * We do not actually use the properties here, they will be used in the next
   * section when we create some objects and bind the property list into the
   * objects.
   *
   ****************************************************************/

    // Create a property list to bind to the vehicle objects
    NameManager->NewPropertyList(2, 2);

    // The first property.
    mpg = 22;
    NameManager->SetProperties(0, "EPAmpg", TRUE, TC_long, (void*)&mpg);
    // Second property, naming it Miles, also sharable.
    miles = 40000;
    NameManager->SetProperties(1, "Miles", TRUE, TC_long, (void*)&miles);
    //   So we end up with two properties in our list, EPAmpg,
    // set to 22 and Miles, set to 40000

   /***************************************************************
    ***********   OBJECTS - CREATING, SETTING, BINDING   **********
    ***************************************************************
    *
    * In this section, we create two objects, Jimmy and Blazer, they will
    * both be bound under the SportUtilities context using the property
    * list defined above.  The vehicle class is a light object built for this
    * sample.
    ****************************************************************/

    // Create the Jimmy vehicle object
    vehicle* Jimmy = (vehicle*)somdCreate(ev, "vehicle", FALSE);

    // Check to be sure the vehicle was created
    NameManager->checkEnv(ev);

    // Initialize the object attributes
    Jimmy->setvehicle(ev, "GMC", "Jimmy", "1995", 22, 40000);

    // Change the name buffer's id/kind to the name of the object
    NameManager->SetName(0, "Jimmy", "vehicle");
    NameManager->checkEnv(ev);

    // Bind the Jimmy vehicle object to the SportUtility context
    // using the property list defined above
    NameManager->BindWithProperties(ev, Jimmy, SportUtility);

    // Create the second vehicle object
    vehicle* Blazer = (vehicle*)somdCreate(ev, "vehicle", FALSE);
    NameManager->checkEnv(ev);
    Blazer->setvehicle(ev, "CHEVROLET", "Blazer", "1993", 20, 36500);

    // Change the buffer/kind to the name/type of the object
    NameManager->SetName(0, "Blazer", "vehicle");

    // Now we must change the property values we will bind to this object
    // otherwise they will be the same as the Jimmy object above
    // The first property.
    mpg = 21;
    NameManager->SetProperties(0, "EPAmpg", TRUE, TC_long, (void*)&mpg);
    // Second property, naming it Miles, also sharable.
    miles = 36500;
    NameManager->SetProperties(1, "Miles", TRUE, TC_long, (void*)&miles);
    //   So we end up with two properties in our list, EPAmpg,
    // set to 21 and Miles, set to 36500

    // Bind the Jimmy vehicle object to the SportUtility context
    NameManager->BindWithProperties(ev, Blazer, SportUtility);

    // Call the dumpacontext function to show the updated naming tree
    cout<<"\nNametest:Main: Print naming context:"<<endl;
    NameManager->DumpContext(ev, vehicles, indent);
    NameManager->checkEnv(ev);

    cout<<endl<<"Enter any character to continue: ";
    cin>>pause;

    // This is an example of how we could use rebind_with_properties
    // at any time to rebind our object with  new or changed properties.
    // use rebind_context_with_properties if the object is a context.
    // First we change a property, then rebind
    mpg = 19;
    NameManager->SetProperties(0, "EPAmpg", TRUE, TC_long, (void*)&mpg);
    NameManager->RebindWithProperties(ev, Blazer, SportUtility);

    /***************************************************************
     *********   SEARCHING TREE USING PROPERTY CONSTRAINTS   *******
     ***************************************************************
     *
     *
     *    In this section we will search the naming tree using a constraint.
     *  A constraint is a string which is set to an expression.  The
     *  expression uses a constraint language to express arbitrarily complex
     *  expressions that involve property names and property values.
     *  for information on the constraint language, see the BNF for Naming
     *  Constraint Language section of the Naming Service chapter in the
     *  Programmers Guide, Volume II:Object Services.
     *
     ****************************************************************/

    // First we need to declare a string to hold the search constraint
    string constraint;

    // Declare a temporary object to hold the object returned
    // from find_any below.
    vehicle* retObj;

    // Declare and Set the depth of the search.
    // Note: If the depth is set to 0, only the named context will be searched
    unsigned long depth = 10;

    // Set the constraint value
    constraint = "EPAmpg < 23 and Miles < 39000";

    cout<<"\nNametest:Main: Searching using constraint: 'EPAmpg < 23 and Miles < 39000'\n";

    // Run the find_any method on the vehicle context.  This is an
    // ExtendedNaming_ExtendedNamingContext method which finds the first
    // object or context whose properties satisfy the constraint. The tree
    // will be searched to the number in the depth variable deep.
    retObj = (vehicle*)vehicles->find_any(ev, constraint, depth);
    NameManager->checkEnv(ev);

    // Stream out the returned objects attributes
    cout<<" Make of found object: "<<retObj->_get_make(ev)<<endl;
    cout<<" Model of found object: "<<retObj->_get_model(ev)<<endl;
    cout<<" Year of found object: "<<retObj->_get_year(ev)<<endl;
    cout<<" MPG of found object: "<<retObj->_get_mpg(ev)<<endl;
    cout<<" Miles of found object: "<<retObj->_get_miles(ev)<<endl;

    /***************************************************************
     *************    RESOLVING OBJECT USING A NAME     ************
     ***************************************************************
     *
     *    This section shows a simple resolve of an object.  First, using
     *  a simple name which assumes the user know the context the object
     *  is located in.  Next a compound resolve is done on the main context
     *  (vehicle).  The returned object's attributes are printed
     *
     ****************************************************************/

    // Declare an object of the type we will resolve.
    ExtendedNaming_ExtendedNamingContext* retnc;

    // Note: the id and kind must exactly match the bound name
    NameManager->SetName(0, "Jimmy", "vehicle");

    cout<<"\nNametest:Main: Resolving using a name of 'Jimmy'"<<endl;
    // resolve the context
    retObj = (vehicle*)NameManager->NameResolve(ev, SportUtility);
    NameManager->checkEnv(ev);
    cout<<" Make of resolved object: "<<retObj->_get_make(ev)<<endl;
    cout<<" Model of resolved object: "<<retObj->_get_model(ev)<<endl;
    cout<<" Year of resolved object: "<<retObj->_get_year(ev)<<endl;
    cout<<" MPG of resolved object: "<<retObj->_get_mpg(ev)<<endl;
    cout<<" Miles of resolved object: "<<retObj->_get_miles(ev)<<endl;

    // To perform a compound resolve on our root 'vehicles' you need to
    // set the length and max to the depth of the tree
    NameManager->NewName(3);
    NameManager->SetName(0, "Modern", "classification");
    NameManager->SetName(1, "SportUtility", "category");
    NameManager->SetName(2, "Blazer", "vehicle");

    cout<<"\nResolving using a compound name of 'Modern/SportUtility/Blazer'"<<endl;
    retObj = (vehicle*)NameManager->NameResolve(ev, vehicles);
    NameManager->checkEnv(ev);
    cout<<" Make of resolved object: "<<retObj->_get_make(ev)<<endl;
    cout<<" Model of resolved object: "<<retObj->_get_model(ev)<<endl;
    cout<<" Year of resolved object: "<<retObj->_get_year(ev)<<endl;
    cout<<" MPG of resolved object: "<<retObj->_get_mpg(ev)<<endl;
    cout<<" Miles of resolved object: "<<retObj->_get_miles(ev)<<endl;


    cout<<endl<<"Enter any character to continue: ";
    cin>>pause;

    /***************************************************************
     ***************     NAMING - USING AN INDEX      **************
     ***************************************************************
     *
     *
     *    Here, an index will be added to our simple naming tree.
     *  the the index will be listed.  Last, we will remove the index.
     *  The ExtendedNamingCOntext introduces methods with which you can create
     *  indexes on specific properties of and object or context.  Indices may
     *  greatly improve the performance of searching a context.  On the other
     *  hand, the performance of setting property values may be decreased as well
     *  as require additional storage.
     *    Indexes are automatically used by any of the ExtendedNamingContext's
     *  find_ methods.
     *
     ****************************************************************/

    // Declare an index descriptor
    ExtendedNaming_IndexDescriptor* indexDesc = new ExtendedNaming_IndexDescriptor;

    // Set the index descriptor name, type, and distance.
    // This defines which property will be indexed
    // and how deep the index will be.
    // Note: If distance is set to 0, only the named context will be indexed
    indexDesc->property_name = "EPAmpg";
    indexDesc->property_type = TC_long;
    indexDesc->distance = (unsigned long)4;

    // Add the index
    cout<<"\nNametest: Main: Adding Index......"<<endl;
    vehicles->add_index(ev, indexDesc);

    // To list the index(es) on a context we need to
    // declare an IndexDescriptorlist to hold the index(es) and an iterator.
    ExtendedNaming_IndexDescriptorList* il = new ExtendedNaming_IndexDescriptorList;
    ExtendedNaming_IndexIterator* rest = new ExtendedNaming_IndexIterator;

    // List the indexes
    cout<<"\nNametest:Main: Listing Index......"<<endl;
    vehicles->list_indexes(ev, 5, il, &rest);
    // Loop through the index list
    for (i = 0; i < il->_length; ++i)
         {
         cout<<"   Index listed has id = "<<il->_buffer[i].property_name<<endl;
         }
    // To remove the index ....
    cout<<"\nNametest:Main: Removing Index......"<<endl;
    vehicles->remove_index(ev, indexDesc);

  /***************************************************************
   ********************    CLEANUP       *************************
   ***************************************************************
   *
   *   This section will clean up our objects and contexts from the
   *  root naming context. First we will unbind the objects from the
   *  naming graph, so we can delete them.  We use the Life Cycle
   *  remove method on our objects because they inherit from LifeCycle.
   ****************************************************************/

   // Set up a new name with 1 id/kind pair
   NameManager->NewName(1);
   // Set the name (id/kind) for the Balzer object
   NameManager->SetName(0, "Blazer", "vehicle");
   // Unbind the object from the SportUtility Context
   NameManager->unBind(ev, SportUtility);
   // Delete the object using the Life Cycle remove method
   Blazer->remove(ev);

   // Do the same for the other objects
   NameManager->SetName(0, "Jimmy", "vehicle");
   NameManager->unBind(ev, SportUtility);
   Jimmy->remove(ev);

   // Do the same for the contexts
   NameManager->SetName(0, "SportUtility", "category");
   NameManager->unBind(ev, Modern);
   // here we call the CosNaming_NamingContext::destroy method
   // to destroy the SportUtility context
   SportUtility->destroy(ev);
   // Check the environment variable for exceptions
   NameManager->checkEnv(ev);

   NameManager->SetName(0, "Vans", "category");
   NameManager->unBind(ev, Modern);
   Vans->destroy(ev);
   NameManager->checkEnv(ev);

   NameManager->SetName(0, "Modern", "classification");
   NameManager->unBind(ev, vehicles);
   Modern->destroy(ev);
   NameManager->checkEnv(ev);

   NameManager->SetName(0, "Sedan", "category");
   NameManager->unBind(ev, Antique);
   Sedan->destroy(ev);
   NameManager->checkEnv(ev);

   NameManager->SetName(0, "HotRod", "category");
   NameManager->unBind(ev, Antique);
   HotRod->destroy(ev);
   NameManager->checkEnv(ev);

   NameManager->SetName(0, "Antique", "classification");
   NameManager->unBind(ev, vehicles);
   Antique->destroy(ev);
   NameManager->checkEnv(ev);

   NameManager->SetName(0, "vehicles", "land vehicles");
   NameManager->unBind(ev, rootNC);
   vehicles->destroy(ev);
   NameManager->checkEnv(ev);

   // Delete all pointers
   // delete name1;
   // delete pList1;
   delete indexDesc;
   delete il;
   delete rest;

   // Call the NameMgr destructor
   NameManager->~NameMgr();

   // Return we're all done
   cout<<"\nProgram complete, returning..."<<endl;
   return 1;
}

