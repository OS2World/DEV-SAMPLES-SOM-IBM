/*
 *
 *  25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1996,1996
 *  All Rights Reserved
 *  Licensed Materials - Property of IBM
 *  US Government Users Restricted Rights - Use, duplication or
 *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
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
 *
 */

/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using:
 *      SOM Emitter emitxtm: 2.44
 */

#ifndef SOM_Module_phone_Source
#define SOM_Module_phone_Source
#endif
#define phoneEntry_Class_Source

#include "phone.xih"


/*
 * person's name
 */

SOM_Scope string  SOMLINK _get_name(phoneEntry *somSelf,  Environment *ev)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","_get_name");

//+     	
    return(strcpy(((string)SOMMalloc(strlen(somThis->name)+1)) ,somThis->name));
//+

}

/*
 * person's name
 */

SOM_Scope void  SOMLINK _set_name(phoneEntry *somSelf,  Environment *ev,
                                  string name)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","_set_name");

//+
    if (somThis->name != NULL)
	SOMFree(somThis->name);

    somThis->name = (string) SOMMalloc(strlen(name)+1);

    if (somThis->name != NULL)
	strcpy(somThis->name, name);
//+


}

/*
 * person's phone number
 */

SOM_Scope string  SOMLINK _get_phone_number(phoneEntry *somSelf,
                                             Environment *ev)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","_get_phone_number");

//+
    return(strcpy(((string)SOMMalloc(strlen(somThis->phone_number)+1))      ,somThis->phone_number));
//+

}

/*
 * person's phone number
 */

SOM_Scope void  SOMLINK _set_phone_number(phoneEntry *somSelf,
                                           Environment *ev, string phone_number)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","_set_phone_number");

//+
    if (somThis->phone_number != NULL)
        SOMFree(somThis->phone_number);

    somThis->phone_number = (string) SOMMalloc(strlen(phone_number)+1);

    if (somThis->phone_number != NULL)
	strcpy(somThis->phone_number, phone_number);
//+

}

SOM_Scope SOMObject*  SOMLINK init_for_object_creation(phoneEntry *somSelf,
                                                        Environment *ev)
{

//+
    SOMObject* newsomSelf;
//+

    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","init_for_object_creation");

    phoneEntry_parent_somPersistencePO_PO_init_for_object_creation(somSelf,
                                                                   ev);
    newsomSelf=phoneEntry_parent_somStream_Streamable_init_for_object_creation(somSelf,
                                                                            ev);

//+
    somThis->name = (string)NULL;
    somThis->phone_number = (string)NULL;
    somThis->office = 0;

    return newsomSelf;
//+

}

SOM_Scope SOMObject*  SOMLINK init_for_object_reactivation(phoneEntry *somSelf,
                                                            Environment *ev)
{

//+
    SOMObject* newsomSelf;
//+

    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","init_for_object_reactivation");

    phoneEntry_parent_somPersistencePO_PO_init_for_object_reactivation(somSelf,
                                                                       ev);
    newsomSelf=phoneEntry_parent_somStream_Streamable_init_for_object_reactivation(somSelf,
                                                                                ev);

//+
    somThis->name = (string)NULL;
    somThis->phone_number = (string)NULL;
    somThis->office = 0;

    return newsomSelf;
//+

}

SOM_Scope SOMObject*  SOMLINK init_for_object_copy(phoneEntry *somSelf,
                                                    Environment *ev)
{

//+
    SOMObject* newsomSelf;
//+

    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","init_for_object_copy");

    phoneEntry_parent_somPersistencePO_PO_init_for_object_copy(somSelf,
                                                               ev);
    newsomSelf=phoneEntry_parent_somStream_Streamable_init_for_object_copy(somSelf,
                                                                        ev);

//+
    somThis->name = (string)NULL;
    somThis->phone_number = (string)NULL;
    somThis->office = 0;

    return newsomSelf;
//+

}

SOM_Scope void SOMLINK somDestruct(phoneEntry *somSelf, octet doFree,
                                   som3DestructCtrl* ctrl)
{
    phoneEntryData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    phoneEntryMethodDebug("phoneEntry","somDestruct");
    phoneEntry_BeginDestructor;

    /*
     * local phoneEntry deinitialization code added by programmer
     */

    phoneEntry_EndDestructor;

}


SOM_Scope void  SOMLINK uninit_for_object_destruction(phoneEntry *somSelf,
                                                             Environment *ev)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","uninit_for_object_destruction");

//+
    if (somThis->name != NULL)
{
          SOMFree(somThis->name);
	somThis->name = NULL;
}

    if (somThis->phone_number != NULL)
{
      SOMFree(somThis->phone_number);
      somThis->phone_number = NULL;
}
//+

    phoneEntry_parent_somPersistencePO_PO_uninit_for_object_destruction(somSelf,
                                                                        ev);
    phoneEntry_parent_somStream_Streamable_uninit_for_object_destruction(somSelf,
                                                                                 ev);
}

SOM_Scope void  SOMLINK uninit_for_object_passivation(phoneEntry *somSelf,
                                                             Environment *ev)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","uninit_for_object_passivation");

    phoneEntry_parent_somPersistencePO_PO_uninit_for_object_passivation(somSelf,
                                                                        ev);
    phoneEntry_parent_somStream_Streamable_uninit_for_object_passivation(somSelf,
                                                                                 ev);
}

SOM_Scope void  SOMLINK uninit_for_object_move(phoneEntry *somSelf,
                                                      Environment *ev)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","uninit_for_object_move");

    phoneEntry_parent_somPersistencePO_PO_uninit_for_object_move(somSelf,
                                                                 ev);
    phoneEntry_parent_somStream_Streamable_uninit_for_object_move(somSelf,
                                                                          ev);
}

SOM_Scope void  SOMLINK somDumpSelfInt(phoneEntry *somSelf, long level)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","somDumpSelfInt");

    phoneEntry_parent_somPersistencePO_PO_somDumpSelfInt(somSelf,
                                                         level);
    phoneEntry_parent_somStream_Streamable_somDumpSelfInt(somSelf,
                                                          level);
}

SOM_Scope void  SOMLINK externalize_to_stream(phoneEntry *somSelf,
                                               Environment *ev,
                                              CosStream_StreamIO* stream)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","externalize_to_stream");

    phoneEntry_parent_somStream_Streamable_externalize_to_stream(somSelf,
                                                                 ev,
                                                                 stream);

//+
    stream->write_string(ev, somThis->name);
    stream->write_string(ev, somThis->phone_number);
    stream->write_short(ev, somThis->office);
//+

}

SOM_Scope void  SOMLINK internalize_from_stream(phoneEntry *somSelf,
                                                 Environment *ev,
                                                CosStream_StreamIO* stream,
                                                CosLifeCycle_FactoryFinder* ff)
{
    phoneEntryData *somThis = phoneEntryGetData(somSelf);
    phoneEntryMethodDebug("phoneEntry","internalize_from_stream");

    phoneEntry_parent_somStream_Streamable_internalize_from_stream(somSelf,
                                                                   ev,
                                                                   stream,
                                                                   ff);

//+
    somThis->name = stream->read_string(ev);
    somThis->phone_number = stream->read_string(ev);
    somThis->office = stream->read_short(ev);
//+

}

