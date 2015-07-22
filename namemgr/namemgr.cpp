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
#include <somd.xh> // DSOM include file
#include <somnm.xh> // Naming service include file
#include "namemgr.h" // Our naming manager object header


  /******************************************************************
   * Method: Constructor
   *
   *    When a NameMgr object is instantiated it's name object
   * is allocated and set.  The NameMgr maintains the name and uses it
   * throughout. The buffer is set to default value(s).
   *
   ******************************************************************/

NameMgr::NameMgr(int maximum=1)
 {
   // allocate space for the name pointer
   name = new CosNaming_Name;

   // set the namemax private variable to input maximum
   namemax = maximum;

   // Set lenght, maximum, and allocate the buffer
   name->_length = name->_maximum = maximum;
   name->_buffer = (CosNaming_NameComponent*)
            SOMMalloc(sizeof(CosNaming_NameComponent)*maximum);

   // for 1 to max
   for (int i = 0; i < maximum; i++)
      {
      // Set the id/Kind pair in the name
      name->_buffer[i].id = "";
      name->_buffer[i].kind = "";
      }
   // Create a property list to bind to the vehicle objects
   pList = new ExtendedNaming_PropertyList;
   // Set a default length and maximum and allocate the default buffer
   pList->_maximum = 1;
   pList->_length = 1;
   pList->_buffer = (Property*)SOMMalloc(sizeof(ExtendedNaming_Property));

 }


  /******************************************************************
   * Method: GetRootNC
   *
   *   In this method we will get our own 'copy' of the root naming
   * context.  A context is analagous to a directory in many file
   * systems.  So the root directory or context is the same.  We will
   * need this root context to anchor the base of our naming tree.
   *
   ******************************************************************/

ExtendedNaming_ExtendedNamingContext* NameMgr::GetRootNC(Environment* ev)
 {
    ExtendedNaming_ExtendedNamingContext* newrootNC;

    newrootNC = (ExtendedNaming_ExtendedNamingContext *)
             SOMD_ORBObject->resolve_initial_references (ev, "NameService");

    if (ev->_major != NO_EXCEPTION)
    {
            somPrintf("\n NameMgr:Get_RootNC:EXCEPTION CAUGHT IN SETUP=> ");
            somPrintf(" << %s >> !!\n", somExceptionId(ev));
            return( NULL );
    }

    if (((SOMDObject*)newrootNC)->is_nil(((SOMDObject*)newrootNC), ev)) {
            somPrintf("NameMgr:Get_RootNC:Failed to get root Naming Context");
            return(NULL);
    }

    return(newrootNC);
 }


  /******************************************************************
   * Method: NewName
   *
   *   The name itself is central to using the service.  A CosNaming_Name is a
   * sequence, it consists of a length, a maximum, and a pointer to a buffer.
   * Length and maximum define how many components are in the buffer.
   * The buffer is a CosNaming_NameComponent, which is an array of id and
   * kind pointers.  The id is in actuality the name.  In this method, we
   * will free the space previously used by the buffer, reset the length
   * and maximum, reallocate the buffer space, and set the buffer id/kind
   * pairs to default values.
   *
   ******************************************************************/

void NameMgr::NewName(int maximum=1)
 {
    // Free old space used by the the name variable
    SOMFree(name->_buffer);

    // Set lenght, maximum, and allocate the buffer
    name->_length = name->_maximum = maximum;
    name->_buffer = (CosNaming_NameComponent*)
             SOMMalloc(sizeof(CosNaming_NameComponent)*maximum);

    // for 1 to max
    for (int i = 0; i < namemax; i++)
       {
       // Set the id/Kind pair in the name
       name->_buffer[i].id = "";
       name->_buffer[i].kind = "";
       }
 }


  /******************************************************************
   *
   * Method: SetName
   *
   *   This method sets a name id/kind pair at the passed in offset.
   *
   ******************************************************************/

void NameMgr::SetName(int index, char* id, char* kind)
 {
    // Change the name buffer's id/kind to the passed in name
    name->_buffer[index].id = id;
    name->_buffer[index].kind = kind;

 }

  /******************************************************************
   *
   * Method: CreateContext
   *
   *   A context is created and bound to the root Naming Context (rootNC)
   *
   * Binding is the process of associating an object (context) with another
   * context and associating a names also.
   *
   ******************************************************************/

ExtendedNaming_ExtendedNamingContext* NameMgr::CreateContext
    (Environment* ev,  ExtendedNaming_ExtendedNamingContext* current)
  {
    // Declare our context
    ExtendedNaming_ExtendedNamingContext* newContext;

    // Create the context
    newContext = (ExtendedNaming_ExtendedNamingContext*)
                  current->new_context(ev);

    // return the newly created context
    return(newContext);
  }


  /******************************************************************
   *
   * Method: BindContext
   *
   *   This method creates a new Context, then binds the name with
   * the context to the passed in context.  The method then returns the newly
   * created context.
   *
   ******************************************************************/

ExtendedNaming_ExtendedNamingContext* NameMgr::BindContext
    (Environment* ev, ExtendedNaming_ExtendedNamingContext* rcontext,
     ExtendedNaming_ExtendedNamingContext* context)
  {
    // Bind the vehicle context to the passed in root context.
    // The bind_context method is used so that the bound context will
    // participate in name resolution when compound names are resolved
    rcontext->bind_context(ev, name, context);

    // return the new context
    return(context);
  }


  /******************************************************************
   *
   * Method: BindWithProperties
   *
   *   This method will take the passed in object, using the name and property
   * list held by the NameMgr object, will bind the object to the passed in
   * context.
   *
   ***********************************************-*******************/

void NameMgr::BindWithProperties
    (Environment* ev, SOMObject* obj, ExtendedNaming_ExtendedNamingContext* rcontext)
  {

    // Do a simple Bind of the object
    rcontext->bind(ev, name, obj);

    // Now add the properties with add_properties
    rcontext->add_properties(ev, name, pList);

    //  The above two lines can be replaced with the bind_with_properties method
    // The bind_with_properties method is used so that the bound object
    // will participate in nam resolution when compound names are resolved
    // rcontext->bind_with_properties(ev, name, obj, pList);

  }


  /******************************************************************
   *
   * Method: RebindWithProperties
   *
   *   This method will take the passed in object, using the name and property
   * list held by the NameMgr object, will bind the object to the passed in
   * context.
   *
   ******************************************************************/

void NameMgr::RebindWithProperties
    (Environment* ev, SOMObject* obj, ExtendedNaming_ExtendedNamingContext* rcontext)
  {

    // Bind the object to the passed in context.
    // The bind_with_properties method is used so that the bound object
    // will participate in nam resolution when compound names are resolved
    rcontext->rebind_with_properties(ev, name, obj, pList);

  }



  /******************************************************************
   * Method: DumpContext
   *
   *    The following method will print the passed-in context,
   *  it walks through the properties(if any) and names associated with the context
   *  and streams them out.  It does a recursive search of any contexts found in the
   *  passed-in context and streams them also.  Any objects in a context will also be
   *  streamed.
   ******************************************************************/

void NameMgr::DumpContext
    (Environment* ev, ExtendedNaming_ExtendedNamingContext* nc, int indent)
  {
        BindingList bl;
        Binding bind;
        CosNaming_BindingIterator *biter;
        int i,k,l;
        LName* anLName;
        LNameComponent* lnc;
        unsigned long numComps;
        char buf500[500];
        PropertyBindingList pbl;
        PropertyBinding propBind;
        ExtendedNaming_PropertyBindingIterator *pbIterator;
        ExtendedNaming_ExtendedNamingContext* passNC;
        CosNaming_Name anyName;
        ExtendedNaming_Property prop;

        // get a binding list of the current (passed) naming context
        nc->list(ev, 100000, &bl, &biter);

        if (this->checkEnv(ev)) return ;

        // create an Lname
        anLName = create_lname();

        // for the number of objects/contexts in our context...
        for (i = 0; i < bl._length; ++i)
            {
            // for however many level we are down in the naming tree
            for (l=0;l<=indent;++l)
                // print a space so the indetation is correct
                {cout<<" ";}
            // fill in the LName from the binding list buffer
            anLName->from_idl_form (ev, &(bl._buffer[i].binding_name));
            // print the first text for the object
            cout<<"Name<";
            // for as many components as ther are in the object
            for (k=1,numComps=anLName->num_components(ev);k<=numComps;k++)
                {
                // get the component
                lnc=anLName->get_component(ev,k);
                // print the id and kind of the objects
                cout<<"["<<lnc->get_id(ev)<<"/"<<lnc->get_kind(ev)<<"]";
                // if there are more components, separate with a blank
                if(k<numComps) cout<<" ";
                }
             // print the end of name symbol
            cout<<">";

            // print the type of the object
            if (bl._buffer[i].binding_type==CosNaming_nobject)
                cout<<"type<object>";
            else
                cout<<"type<context>";

            // Set the property binding list length, maximun, and buffer
            pbl._maximum = pbl._length = 0;
            pbl._buffer = NULL;
            // convert the LName to idl form
            anyName = anLName->to_idl_form(ev);
            checkEnv(ev);
            // get a list of the properties associated with the object/context
            nc->list_properties (ev,&anyName,100000,&pbl,&pbIterator);
            // if there are no properties, do nothing else, print 'em
            if (this->checkEnv(ev))
               {}
            else
                {
                // property names leading string
                cout<<"PropertyNames<";
                // for as many properties as there are
                for (k=0;k<pbl._length;k++)
                    {
                    // print the name of the property
                    cout<<"["<<pbl._buffer[k].property_name<<"]";
                    // from the name, get the property itself
                    nc->get_property(ev, &anyName, pbl._buffer[k].property_name, &prop);
                    // print the value of the property. in this sample I know
                    // that the _value pointer points to a long, so I can readily cast and
                    // stream out the result.  If I did not know what type the value was I
                    // would have to use a large case structure on prop.value._type.
                    cout<<"/";
                    cout<<*(long*)prop.value._value<<" ";
                    // cout<<"/"anyPrint(prop.value, ev)<<" "<<endl;
                    }
                }
            // end of properties character
            cout<<">"<<endl;

            // if the object we are looking at is a context,
            // we need to see if there are other objects bound in
            if (bl._buffer[i].binding_type==CosNaming_ncontext)
               {
               // resolve the context into our temp variable
               passNC = (ExtendedNaming_ExtendedNamingContext*)nc->resolve(ev, &anyName);
               // recursively call dumpacontext on the child context
               this->DumpContext(ev, passNC, ++indent);
               --indent;
               }
            else
               {}
            }
  }


  /******************************************************************
   *
   * Method: CreateNewPropertyList
   *
   *   Here, we will create an ExtendedNaming_PropertyList.  a property list
   * is a sequence, consisting of a length, a maximum, and buffer which
   * is an ExtendedNaming_Property. An ExtendedNaming_property is simply
   * an array of structures consisting of name/value pairs.
   *
   ******************************************************************/

void NameMgr::NewPropertyList(int length, int maximum)
  {
    // Free any space that may have been used by the property list
    SOMFree(pList->_buffer);
    // Initialize the property list by setting the maximum, length and buffer pointers.
    pList->_maximum = maximum;
    pList->_length = length;
    pList->_buffer = (Property*)SOMMalloc
       (pList->_maximum*sizeof(ExtendedNaming_Property));
  }


  /******************************************************************
   *
   * Method: SetProperties
   *
   *   This method sets a single property buffer's binding as well as the type and value
   * of the property.
   *
   ******************************************************************/

 void NameMgr::SetProperties
   (int propnum, char* propname, boolean sharable, TypeCode type, void* value)
  {
    // Add properties to the list
    // The first property name. Note we are making this property sharable.
    pList->_buffer[propnum].binding.property_name = propname;
    pList->_buffer[propnum].binding.sharable = sharable;

    // Setting the first property values
    pList->_buffer[propnum].value._type = type;

    // Set the value into the property
    pList->_buffer[propnum].value._value = value;

  }


  /******************************************************************
   *
   * method: NameResolve
   *
   *    This method does a simple resolve of an object.  First, the
   *  name is set. This assumes the user know the context the object
   *  is located in and passes it in. A SOMObject is returned.
   *
   *
   ******************************************************************/

 SOMObject* NameMgr::NameResolve
   (Environment* ev, ExtendedNaming_ExtendedNamingContext* context)
  {
    // declare a SOMObject to return
    SOMObject* retObj;

    // resolve the context
    retObj = context->resolve(ev, name);
    this->checkEnv(ev);

    // Return resolved object
    return(retObj);
   }


  /******************************************************************
   *
   * method: unBind
   *
   *    This method simply unbinds an object from the passed in context.
   *
   ******************************************************************/

void NameMgr::unBind
  (Environment* ev, ExtendedNaming_ExtendedNamingContext* context)
 {
    // Unbind the named object from the passed in context.
    context->unbind(ev, name);
 }


  /******************************************************************
   *
   * method: Destructor
   *
   *  This method frees the space of the buffer and property list
   *
   ******************************************************************/

NameMgr::~NameMgr()
 {
    // Free the space we have last allocated for the name and pList
    SOMFree(name->_buffer);
    SOMFree(pList->_buffer);
 }


// Neat routine to check the environment variable for Name not found, Name already bound
// or just print out the exception.
int NameMgr::checkEnv(Environment *ev)
  {
        int ret = 0;

    // check the major code in ev
    if (ev->_major != NO_EXCEPTION)
    {
        cout<<"\nEXCEPTION=> Nametest:checkenv: ";
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