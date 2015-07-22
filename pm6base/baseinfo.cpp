/*
    Description: Frame information class implementation.

    Author:      Joseph H McIntyre

    (C) COPYRIGHT Apple Computer Inc 1994
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

#ifndef SOM_Module_baseinfo_Source
#define SOM_Module_baseinfo_Source
#endif
#define jmBaseFrameInfo_Class_Source
#define VARIABLE_MACROS

#define INCL_ODAPI
#define INCL_ODDRAFT
#define INCL_ODFACET
#define INCL_ODFRAME
#define INCL_ODSTORAGEUNIT
#define INCL_ODSTORAGEUNITVIEW
#define INCL_ODWINDOW

#if defined (_PLATFORM_OS2_)
  #include <os2.h>
#elif defined (_PLATFORM_WIN32_)
  #include <od.h>
  #include <storutil.h>
#endif

#include <baseinfo.xih>
#include <baselist.xh>


#ifdef DEBUG
    #undef jmBaseFrameInfoMethodDebug
    #define jmBaseFrameInfoMethodDebug(x,y) LogString(somSelf,x,y)

    #include <log.h>
#endif


// constructor and destructor

SOM_Scope void SOMLINK somDefaultInit(jmBaseFrameInfo *somSelf, somInitCtrl* ctrl)
{
    #ifdef DEBUG
      LogOpen (somSelf, "BINFO");
    #endif

    jmBaseFrameInfoData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseFrameInfoMethodDebug("BaseFrameInfo","somDefaultInit");
    jmBaseFrameInfo_BeginInitializer_somDefaultInit;

    jmBaseFrameInfo_Init_SOMObject_somDefaultInit(somSelf, ctrl);

    _frameActive     = kODFalse;
    _reactivateFrame = kODFalse;
    _activeFacet     = kODNULL;
    _sourceFrame     = kODNULL;
    _attachedFrames  = kODNULL;
    _partWindow      = kODNULL;
}


SOM_Scope void SOMLINK somDestruct(jmBaseFrameInfo *somSelf, octet doFree, somDestructCtrl* ctrl)
{
    jmBaseFrameInfoData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseFrameInfoMethodDebug("BaseFrameInfo","somDestruct");
    jmBaseFrameInfo_BeginDestructor;

    if (_attachedFrames != kODNULL)
        delete _attachedFrames;

    jmBaseFrameInfo_EndDestructor;
}

// internalize and externalize

SOM_Scope void  SOMLINK InitFromStorage (jmBaseFrameInfo *somSelf, Environment *ev,
                            ODStorageUnitView* storageUnitView, ODType kPartInfo)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","InitFromStorage");

    ODStorageUnit *storageUnit = storageUnitView->GetStorageUnit (ev);
    if (storageUnit->Exists (ev, kODNULL, kPartInfo, 0) )
     {
        storageUnit->Focus(ev, kODNULL, kODPosSame, kPartInfo, 0 , kODPosUndefined);
        ODULong size = storageUnit->GetSize (ev);

        // If the frame does not have a source frame, make sure the value is empty.
        if (size > 0)
         {
            ODStorageUnitRef weakRef;

            StorageUnitGetValue (storageUnit, ev, sizeof (ODStorageUnitRef), (ODPtr) &weakRef);
            if (storageUnit->IsValidStorageUnitRef (ev, weakRef))
             {
                ODID sourceID = storageUnit->GetIDFromStorageUnitRef (ev, weakRef);
                ODFrame *frame = storageUnit->GetDraft (ev)->AcquireFrame (ev, sourceID);
                _sourceFrame = frame;
                frame->Release (ev);
             }
            else
                _sourceFrame = kODNULL;
         }
     }
}

SOM_Scope void  SOMLINK Externalize(jmBaseFrameInfo *somSelf, Environment *ev,
                            ODStorageUnitView* storageUnitView, ODType kPartInfo)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","Externalize");

    // storage unit has focus on a property, but no defined value
    ODStorageUnit *storageUnit = storageUnitView->GetStorageUnit (ev);
    somSelf->ExternalizeFrameInfo (ev, storageUnit, 0, kODNULL, kPartInfo);
}

SOM_Scope void  SOMLINK ExternalizeFrameInfo(jmBaseFrameInfo *somSelf, Environment *ev,
                            ODStorageUnit* storageUnit, ODDraftKey key, ODFrame* scopeFrame, ODType kPartInfo)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","ExternalizeFrameInfo");

    if (storageUnit->Exists (ev, kODNULL, kPartInfo, 0))
     {
        // Persistent object references are stored in a side table, rather
        // than in the property/value stream. Thus, deleting the contents of
        // a value will not "delete" the references previously written to
        // that value. To completely "delete" all references written to the
        // value, we must remove the value and add it back.

        storageUnit->Focus (ev, kODNULL, kODPosSame, kPartInfo, 0, kODPosUndefined);
        storageUnit->Remove (ev);
     }

    if (_sourceFrame)
     {
        ODStorageUnitRef weakRef;
        ODID frameID = _sourceFrame->GetID (ev);
        ODID scopeFrameID = (scopeFrame) ? scopeFrame->GetID (ev) : 0;
        ODDraft *fromDraft = _sourceFrame->GetStorageUnit (ev)->GetDraft (ev);

        storageUnit->AddValue(ev, kPartInfo);

        // If a draft key exists, then we are being cloned to another draft.
        // We must "weak" clone our display frame and reference the cloned
        // frame. The part re-uses the frameID variable so there aren't two
        // different GetWeakStorageUnitRef calls.
        if (key)
            frameID = fromDraft->WeakClone (ev, key, frameID, 0, scopeFrameID);

        // Write out weak references to each of the part's display frames.
        storageUnit->GetWeakStorageUnitRef (ev, frameID, weakRef);
        StorageUnitSetValue (storageUnit, ev, sizeof (ODStorageUnitRef), (ODPtr) &weakRef);
     }
}

SOM_Scope void  SOMLINK CloneInto(jmBaseFrameInfo *somSelf, Environment *ev,
                            ODDraftKey key, ODStorageUnitView* storageUnitView, ODFrame* scopeFrame, ODType kPartInfo)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","CloneInto");

    ODStorageUnit *storageUnit = storageUnitView->GetStorageUnit (ev);

    if (storageUnit->Exists (ev, kODNULL, kPartInfo, 0) == kODFalse)
        somSelf->ExternalizeFrameInfo (ev, storageUnit, key, scopeFrame, kPartInfo);
}

// frame related methods

SOM_Scope void  SOMLINK ReleaseSourceFrame(jmBaseFrameInfo *somSelf, Environment *ev)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","ReleaseSourceFrame");

    if (_sourceFrame != kODNULL)
     {
        _sourceFrame->Release (ev);
        _sourceFrame = NULL;
     }
}

SOM_Scope ODBoolean  SOMLINK HasSourceFrame(jmBaseFrameInfo *somSelf, Environment *ev)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","HasSourceFrame");

    return ((_sourceFrame != kODNULL) ? kODTrue : kODFalse);
}

SOM_Scope void  SOMLINK AttachFrame(jmBaseFrameInfo *somSelf, Environment *ev, ODFrame* frame)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","AttachFrame");

    if (_attachedFrames == kODNULL)
        _attachedFrames = new jmBaseList;

    if (_attachedFrames->Contains (ev, frame) == FALSE)
     {
        frame->Acquire (ev);
        _attachedFrames->Add (ev, frame);
     }
}

SOM_Scope void  SOMLINK DetachFrame(jmBaseFrameInfo *somSelf, Environment *ev, ODFrame* frame)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","DetachFrame");

    if (_attachedFrames->Contains (ev, frame) == TRUE)
     {
        _attachedFrames->Remove (ev, frame);
        frame->Release (ev);

        if (_attachedFrames->Count (ev) == 0)
         {
            delete _attachedFrames;
            _attachedFrames = NULL;
         }
     }
}

SOM_Scope ODBoolean  SOMLINK HasAttachedFrames(jmBaseFrameInfo *somSelf, Environment *ev)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","HasAttachedFrames");

    return ((_attachedFrames != kODNULL) ? kODTrue : kODFalse);
}

// window related methods

SOM_Scope ODBoolean  SOMLINK HasPartWindow(jmBaseFrameInfo *somSelf, Environment *ev)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","HasPartWindow");

    return ((_partWindow != kODNULL) ? kODTrue : kODFalse);
}

SOM_Scope ODBoolean  SOMLINK IsFrameActive(jmBaseFrameInfo *somSelf, Environment *ev)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","IsFrameActive");

    return (_frameActive);
}

SOM_Scope void  SOMLINK SetFrameActive(jmBaseFrameInfo *somSelf, Environment *ev, ODBoolean active)
{
    jmBaseFrameInfoData *somThis = jmBaseFrameInfoGetData(somSelf);
    jmBaseFrameInfoMethodDebug("jmBaseFrameInfo","SetFrameActive");

    _frameActive = active;
}


