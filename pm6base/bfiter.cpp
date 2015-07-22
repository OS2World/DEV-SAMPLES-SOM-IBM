/*
    Description: Embedded frame iterator

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

#ifndef SOM_Module_bfiter_Source
#define SOM_Module_bfiter_Source
#endif
#define jmBaseEmbeddedFramesIterator_Class_Source
#define VARIABLE_MACROS

#include <bfiter.xih>
#include <baselist.xh>


SOM_Scope void  SOMLINK Initialize (jmBaseEmbeddedFramesIterator *somSelf, Environment *ev, jmBaseList *baselist)
{
    jmBaseEmbeddedFramesIteratorData *somThis = jmBaseEmbeddedFramesIteratorGetData(somSelf);
    jmBaseEmbeddedFramesIteratorMethodDebug("jmBaseEmbeddedFramesIterator","jmBaseEmbeddedFramesIteratorInitjmBaseEmbeddedFramesIterator");

    _iterator = baselist->CreateIterator (ev);
}

SOM_Scope ODFrame*  SOMLINK First(jmBaseEmbeddedFramesIterator *somSelf, Environment *ev)
{
    jmBaseEmbeddedFramesIteratorData *somThis = jmBaseEmbeddedFramesIteratorGetData(somSelf);
    jmBaseEmbeddedFramesIteratorMethodDebug("jmBaseEmbeddedFramesIterator","jmBaseEmbeddedFramesIteratorFirst");

    return ((ODFrame *) _iterator->First (ev));
}

SOM_Scope ODFrame*  SOMLINK Next(jmBaseEmbeddedFramesIterator *somSelf, Environment *ev)
{
    jmBaseEmbeddedFramesIteratorData *somThis = jmBaseEmbeddedFramesIteratorGetData(somSelf);
    jmBaseEmbeddedFramesIteratorMethodDebug("jmBaseEmbeddedFramesIterator","jmBaseEmbeddedFramesIteratorNext");

    return ((ODFrame *) _iterator->Next (ev));
}

SOM_Scope ODBoolean  SOMLINK IsNotComplete(jmBaseEmbeddedFramesIterator *somSelf, Environment *ev)
{
    jmBaseEmbeddedFramesIteratorData *somThis = jmBaseEmbeddedFramesIteratorGetData(somSelf);
    jmBaseEmbeddedFramesIteratorMethodDebug("jmBaseEmbeddedFramesIterator","jmBaseEmbeddedFramesIteratorIsNotComplete");

    return (_iterator->IsNotComplete (ev));
}


SOM_Scope void SOMLINK somDefaultInit(jmBaseEmbeddedFramesIterator *somSelf, somInitCtrl* ctrl)
{
    jmBaseEmbeddedFramesIteratorData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseEmbeddedFramesIteratorMethodDebug("jmBaseEmbeddedFramesIterator","somDefaultInit");
    jmBaseEmbeddedFramesIterator_BeginInitializer_somDefaultInit;

    jmBaseEmbeddedFramesIterator_Init_ODEmbeddedFramesIterator_somDefaultInit(somSelf, ctrl);

    _iterator = 0;
}


SOM_Scope void SOMLINK somDestruct(jmBaseEmbeddedFramesIterator *somSelf, octet doFree, somDestructCtrl* ctrl)
{
    jmBaseEmbeddedFramesIteratorData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseEmbeddedFramesIteratorMethodDebug("jmBaseEmbeddedFramesIterator","somDestruct");
    jmBaseEmbeddedFramesIterator_BeginDestructor;

    if (_iterator != 0)
        delete _iterator;

    jmBaseEmbeddedFramesIterator_EndDestructor;
}
