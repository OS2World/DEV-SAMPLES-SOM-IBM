/*
    Description: Simple list and iterator implementation.

    Author:      Joseph H McIntyre

    (C) COPYRIGHT International Business Machines Corp. 1995, 1996
    All Rights Reserved
    Licensed Materials - Property of IBM

    US Government Users Restricted Rights - Use, duplication or
    disclosure restricted by GSA ADP Schedule Contract with IBM Corp.

    DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
    sample code created by IBM Corporation. This sample code is not
    part of any standard or IBM product and is provided to you solely
    for the purpose of assisting you in the development of your
    applications.  The code is provided "AS IS", without
    warranty of any kind.  IBM shall not be liable for any damages
    arising out of your use of the sample code, even if they have been
    advised of the possibility of such damages.
*/

#ifndef SOM_Module_baselist_Source
#define SOM_Module_baselist_Source
#endif
#define jmBaseIterator_Class_Source

#define VARIABLE_MACROS
#include "baselist.xih"


SOM_Scope void SOMLINK Iterator_jmBaseIterator_Init(jmBaseIterator *somSelf, Environment *ev, somInitCtrl* ctrl,
                           jmBaseList* list, unsigned long count)
{
    jmBaseIteratorData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseIteratorMethodDebug("jmBaseIterator","jmBaseIterator_Init");
    jmBaseIterator_BeginInitializer_jmBaseIterator_Init;

    jmBaseIterator_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    _list = list;
    _ulCount = count;
    _ulIndex = 0;
    _valid = TRUE;
}


SOM_Scope void SOMLINK Iterator_somDefaultInit(jmBaseIterator *somSelf, somInitCtrl* ctrl)
{
    jmBaseIteratorData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseIteratorMethodDebug("jmBaseIterator","somDefaultInit");
    jmBaseIterator_BeginInitializer_somDefaultInit;

    jmBaseIterator_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    _valid = FALSE;
    _ulIndex = 0;
    _ulCount = 0;
    _list = (jmBaseList *) 0;
}


SOM_Scope void SOMLINK Iterator_somDestruct(jmBaseIterator *somSelf, octet doFree, somDestructCtrl* ctrl)
{
    jmBaseIteratorData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseIteratorMethodDebug("jmBaseIterator","jmBaseIteratorsomDestruct");
    jmBaseIterator_BeginDestructor;

    // destructor code

    jmBaseIterator_EndDestructor;
}


SOM_Scope SOMObject*  SOMLINK Iterator_First(jmBaseIterator *somSelf, Environment *ev)
{
    jmBaseIteratorData *somThis = jmBaseIteratorGetData(somSelf);
    jmBaseIteratorMethodDebug("jmBaseIterator","jmBaseIteratorFirst");

    SOMObject *result;

    if (_valid)
     {
        _ulIndex = 1;
        result = _list->GetObject (ev, 0);
     }
    else
        result = (SOMObject *) 0;

    return (result);
}


SOM_Scope SOMObject*  SOMLINK Iterator_Next(jmBaseIterator *somSelf, Environment *ev)
{
    jmBaseIteratorData *somThis = jmBaseIteratorGetData(somSelf);
    jmBaseIteratorMethodDebug("jmBaseIterator","jmBaseIteratorNext");

    SOMObject *result = (SOMObject *) 0;

    if (_valid)
     {
        if (_ulIndex < _ulCount)
         {
            _ulIndex ++;
            result = _list->GetObject (ev, _ulIndex - 1);
         }
     }

    return (result);
}


SOM_Scope SOMObject*  SOMLINK Iterator_Previous(jmBaseIterator *somSelf, Environment *ev)
{
    jmBaseIteratorData *somThis = jmBaseIteratorGetData(somSelf);
    jmBaseIteratorMethodDebug("jmBaseIterator","jmBaseIteratorPrevious");

    SOMObject *result = (SOMObject *) 0;

    if (_valid)
     {
        if (_ulIndex > 0)
         {
            _ulIndex --;
            result = _list->GetObject (ev, _ulIndex);
         }
     }

    return (result);
}


SOM_Scope boolean  SOMLINK Iterator_IsNotComplete(jmBaseIterator *somSelf, Environment *ev)
{
    jmBaseIteratorData *somThis = jmBaseIteratorGetData(somSelf);
    jmBaseIteratorMethodDebug("jmBaseIterator","jmBaseIteratorIsNotComplete");

    boolean result = FALSE;
    if (_valid)
     {
        if ((_ulIndex > 0) && (_ulIndex < _ulCount))
            result = TRUE;
     }

    return (result);
}


/*
    jmBaseList class implementation
*/

SOM_Scope void SOMLINK List_somDefaultInit(jmBaseList *somSelf, somInitCtrl* ctrl)
{
    jmBaseListData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseListMethodDebug("jmBaseList","somDefaultInit");
    jmBaseList_BeginInitializer_somDefaultInit;

    jmBaseList_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    _list._length = 0;
    _list._maximum = 8;
    _list._buffer = (SOMObject **) SOMMalloc (8 * sizeof (SOMObject *));
}


SOM_Scope void SOMLINK List_somDestruct(jmBaseList *somSelf, octet doFree, somDestructCtrl* ctrl)
{
    jmBaseListData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseListMethodDebug("jmBaseList","jmBaseListsomDestruct");
    jmBaseList_BeginDestructor;

    SOMFree (_list._buffer);

    jmBaseList_EndDestructor;
}



SOM_Scope jmBaseIterator*  SOMLINK List_CreateIterator(jmBaseList *somSelf, Environment *ev)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListCreateIterator");

    jmBaseIterator *iterator = new jmBaseIterator (ev, somSelf, _list._length);
    return (iterator);
}


SOM_Scope void  SOMLINK List_Add(jmBaseList *somSelf,  Environment *ev, SOMObject* object)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListAdd");

    if (_list._length == _list._maximum)
     {
        _list._buffer = (SOMObject **) SOMRealloc (_list._buffer, (_list._maximum + 8) * sizeof (SOMObject *));
        _list._maximum += 8;
     }

    _list._buffer[_list._length] = object;
    _list._length ++;
}


SOM_Scope void  SOMLINK List_Remove(jmBaseList *somSelf,  Environment *ev, SOMObject* object)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListRemove");

    unsigned long ulCtr;
    for (ulCtr = 0; ulCtr < _list._length; ulCtr ++)
        if (_list._buffer[ulCtr] == object)
            break;

    if (ulCtr < _list._length)
     {
        for (;ulCtr < (_list._length - 1); ulCtr ++)
            _list._buffer[ulCtr] = _list._buffer[ulCtr + 1];
        _list._length --;
     }
}


SOM_Scope void  SOMLINK List_RemoveAll(jmBaseList *somSelf, Environment *ev)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListRemoveAll");

    _list._length = 0;
    if (_list._maximum != 8)
     {
        _list._maximum = 8;
        _list._buffer = (SOMObject **) SOMRealloc (_list._buffer, (8 * sizeof (SOMObject *)));
     }
}


SOM_Scope SOMObject*  SOMLINK List_GetObject(jmBaseList *somSelf, Environment *ev, unsigned long index)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListGetObject");

    SOMObject *result = (index < _list._length) ? _list._buffer[index] : (SOMObject *) 0;
    return (result);
}


SOM_Scope boolean  SOMLINK List_Contains(jmBaseList *somSelf, Environment *ev, SOMObject *object)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListContains");

    boolean result = FALSE;
    unsigned long ulCtr;
    for (ulCtr = 0; ulCtr < _list._length; ulCtr ++)
        if (_list._buffer[ulCtr] == object)
            break;

    if (ulCtr < _list._length)
        result = TRUE;

    return (result);
}


SOM_Scope unsigned long  SOMLINK List_Count(jmBaseList *somSelf, Environment *ev)
{
    jmBaseListData *somThis = jmBaseListGetData(somSelf);
    jmBaseListMethodDebug("jmBaseList","jmBaseListCount");

    return (_list._length);
}

