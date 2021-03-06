/*
 *
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 *
 *
 *  DISCLAIMER OF WARRANTIES.
 *  The following [enclosed] code is sample code created by IBM
 *  Corporation. This sample code is not part of any standard or IBM
 *  product and is provided to you solely for the purpose of assisting
 *  you in the development of your applications.  The code is provided
 *  "AS IS". IBM MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
 *  NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE, REGARDING THE FUNCTION OR PERFORMANCE OF
 *  THIS CODE.  IBM shall not be liable for any damages arising out of
 *  your use of the sample code, even if they have been advised of the
 *  possibility of such damages.
 *
 *  DISTRIBUTION.
 *  This sample code can be freely distributed, copied, altered, and
 *  incorporated into other software, provided that it bears the above
 *  Copyright notice and DISCLAIMER intact.
 */

/*
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *     SOM incremental update: 2.45
 */

#ifndef SOM_Module_cont_Source
#define SOM_Module_cont_Source
#endif
#define ContainedClass_Class_Source

#include "cont.xih"               // ContainedClass include file

static string nilString = "";

#define EXTERNALIZE_STRING(strm, env, var) \
    if (var) \
        strm->write_string(env, var); \
    else \
        strm->write_string(env, nilString); \
    if ((ev)->_major != NO_EXCEPTION) \
        return;



/*
 *Method from the IDL attribute statement:
 *"attribute string name"
 */

SOM_Scope void  SOMLINK _set_name(ContainedClass *somSelf,  Environment *ev,
                                  string name)
{
    ContainedClassData *somThis = ContainedClassGetData(somSelf);
    ContainedClassMethodDebug("ContainedClass","_set_name");

    if (somThis->name != NULL)
       SOMFree (somThis->name);

    somThis->name = (string) SOMMalloc(strlen(name));
    strcpy(somThis->name,name);

}


/*
 * Default object initializer
 */
SOM_Scope void SOMLINK somDefaultInit(ContainedClass *somSelf,
                                      som3InitCtrl* ctrl)
{
//+
 //   SOM_TraceLevel=1;
//+

    ContainedClassData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    ContainedClassMethodDebug("ContainedClass","somDefaultInit");
    ContainedClass_BeginInitializer_somDefaultInit;

    ContainedClass_Init_CosStream_Streamable_somDefaultInit(somSelf, ctrl);

    /*
     * local ContainedClass initialization code added by programmer
     */

    somThis->name = (char *)NULL;
}



/*
 * Default object uninitializer
 */
SOM_Scope void SOMLINK somDestruct(ContainedClass *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    ContainedClassData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    ContainedClassMethodDebug("ContainedClass","somDestruct");
    ContainedClass_BeginDestructor;

    /*
     * local ContainedClass deinitialization code added by programmer
     */

    if (somThis->name)
       SOMFree(somThis->name);

    ContainedClass_EndDestructor;
}


//---------------------------------------------------------------------------
// Writing the object's state data to a stream
//---------------------------------------------------------------------------
SOM_Scope void  SOMLINK externalize_to_stream(ContainedClass *somSelf,
                                              Environment *ev,
                                              CosStream_StreamIO* stream)
{
    ContainedClassData *somThis = ContainedClassGetData(somSelf);
    ContainedClassMethodDebug("ContainedClass","externalize_to_stream");

    ContainedClass_parent_CosStream_Streamable_externalize_to_stream(somSelf,
                                                                     ev,
                                                                     stream);
    EXTERNALIZE_STRING(stream, ev, somThis->name);
    if ((ev)->_major != NO_EXCEPTION)
       return;

}


//---------------------------------------------------------------------------
// Reading the object's state data from a stream
//---------------------------------------------------------------------------
SOM_Scope void  SOMLINK internalize_from_stream(ContainedClass *somSelf,
                                                Environment *ev,
                                                CosStream_StreamIO* stream,
                                                CosLifeCycle_FactoryFinder* ff)
{
    ContainedClassData *somThis = ContainedClassGetData(somSelf);
    ContainedClassMethodDebug("ContainedClass","internalize_from_stream");

    ContainedClass_parent_CosStream_Streamable_internalize_from_stream(somSelf,
                                                                       ev,
                                                                       stream,
                                                                       ff);
    somThis->name = stream->read_string(ev);
    if ((ev)->_major != NO_EXCEPTION)
       return;
}

