/*
    Description: Abstract base class for non-container parts.

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

#ifndef SOM_Module_basenc_Source
#define SOM_Module_basenc_Source
#endif
#define jmBaseNonContainer_Class_Source
#define M_jmBaseNonContainer_Class_Source


#if defined (_PLATFORM_OS2_)
  #include <altpoly.h>

  #define INCL_DOS
  #define INCL_WININPUT
  #define INCL_WINSTDDRAG
  #define INCL_WINWINDOWMGR
  #define INCL_WINPOINTERS
  #define INCL_WINMENUS
  #define INCL_GPI
#endif

#define INCL_ODAPI
#define INCL_ODARBITRATOR
#define INCL_ODCANVAS
#define INCL_ODCLIPBOARD
#define INCL_ODDRAGANDDROP
#define INCL_ODDRAFT
#define INCL_ODERRORS
#define INCL_ODFACET
#define INCL_ODFOCUSSET
#define INCL_ODFRAME
#define INCL_ODINFO
#define INCL_ODLINK
#define INCL_ODLINKSOURCE
#define INCL_ODLINKSPEC
#define INCL_ODMENUBAR
#define INCL_ODSHAPE
#define INCL_ODSESSION
#define INCL_ODSTORAGEUNIT
#define INCL_ODSTORAGEUNITVIEW
#define INCL_ODTRANSFORM
#define INCL_ODUNDO
#define INCL_ODWINDOW
#define INCL_ODWINDOWSTATE

#if defined (_PLATFORM_OS2_)
  #include <os2.h>
#elif defined (_PLATFORM_WIN32_)
  #include <od.h>
#endif

#define VARIABLE_MACROS
#include <basenc.xih>
#include <jmbase.h>

#include <stdio.h>
#include <string.h>

#include <focuslib.h>
#include <winutils.h>
#include <odmath.h>


#if defined (_PLATFORM_WIN32_)
  #include <storutil.h>
#endif


#define FIXED2LONG(f) (((f) + 0x8000) >> 16)

#ifndef _STDTYPIO_
  #include "stdtypio.h"
#endif

#include <baselist.xh>
#include <baseinfo.xh>
#include <image.xh>


const ODType kPartHandlerName = "jmBaseNonContainer";
const ODType kDisplayName = "Base Non-Container";
const ODType kKind = "jmBaseNonContainer";
const ODType kKindDisplayName = "Base Non-Container";
const ODType kCategory = "Abstract base part";
const ODType kCategoryDisplayName = "Abstract base part";
const ODType kPartInfo = "+//ISO 9070/ANSI::113722:US:CI LABS::jmBaseNonContainer:Display Frame Info";

// debug macro constants
const string kDebugPart = "jmBaseNonContainer";
const string kDebugPartM = "M_jmBaseNonContainer";


#if defined (_PLATFORM_WIN32_)
  #define kODKindPlainText kODWindowsIText
#endif


#ifdef DEBUG
    #undef jmBaseNonContainerMethodDebug
    #undef M_jmBaseNonContainerMethodDebug

    #define jmBaseNonContainerMethodDebug(x,y) LogString(somSelf,x,y)
    #define M_jmBaseNonContainerMethodDebug(x,y) LogString(somSelf,x,y)

    #include <log.h>
#endif


//  This is the first method called when an object of this class is created.
//  Initialization of all attributes and private variables.
//
//  Implementation: Call parent initializer, initialize attributes
//  Return        : None
//  Subclassing   : Call first
//  Notes         : None

SOM_Scope void SOMLINK somDefaultInit(jmBaseNonContainer *somSelf, somInitCtrl* ctrl)
{
    #ifdef DEBUG
      LogOpen (somSelf, "BASENC");
    #endif

    jmBaseNonContainerData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseNonContainerMethodDebug(kDebugPart,"somDefaultInit");
    jmBaseNonContainer_BeginInitializer_somDefaultInit;

    jmBaseNonContainer_Init_ODPart_somDefaultInit(somSelf, ctrl);

    _idWindow = 0;
    _focusSet = kODNULL;
    _session = kODNULL;
    _menuBar = kODNULL;
    _menuPopup = kODNULL;

    _frames = kODNULL;
    _facets = kODNULL;
    _linkSources = kODNULL;
    _linkTargets = kODNULL;
}


//  Last method called before this instance of the class is destroyed. Release
//  all remaining allocated resources.
//
//  Implementation: Clean up jmBaseNonContainer resources, then call parent
//  Return        : None
//  Subclassing   : Call last (or as your part requires)
//  Notes         : None

SOM_Scope void SOMLINK somDestruct(jmBaseNonContainer *somSelf, octet doFree, somDestructCtrl* ctrl)
{
    jmBaseNonContainerData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseNonContainerMethodDebug(kDebugPart,"somDestruct");
    jmBaseNonContainer_BeginDestructor;

    if (_frames != kODNULL)
        delete _frames;

    if (_facets != kODNULL)
        delete _facets;

    if (_linkSources != kODNULL)
        delete _linkSources;

    if (_linkTargets != kODNULL)
        delete _linkTargets;

    jmBaseNonContainer_EndDestructor;
}


//  PartKind returns the kind string, a required method to be overridden
//  by a derived class for persistence framework methods.
//
//  Implementation: Return BaseNonContainer kind
//  Return        : None
//  Subclassing   : Must be overridden
//  Notes         : None

SOM_Scope string  SOMLINK PartKind (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"PartKind");

    return (kKind);
}

/*
    When the part is initially created this method is called. This method will
    only be called once in the life of the part, otherwise InitPartFromStorage
    will be called.
    This method may be called during the registration process, or when a part
    is created dynamically.

    Implementation: Calls part to set up persistent storage
                    Creates initial PropContents with part kind
    Return        : None.
    Subclassing   : This method must be subclassed and called first.
    Notes         : None.
*/

SOM_Scope void  SOMLINK InitPart(jmBaseNonContainer *somSelf,  Environment *ev,
                                 ODStorageUnit* storageUnit, ODPart *partWrapper)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InitPart");

    jmBaseNonContainer_parent_ODPart_InitPart (somSelf, ev, storageUnit, partWrapper);

    _partWrapper = partWrapper;
    _contentChanged = kODTrue;
    _readOnly = kODFalse;
    _fromStorage = kODFalse;

    somSelf->InitializePart (ev);
    somSelf->ExternalizeHeader (ev, storageUnit);
}


/*
    When a part is created from an existing storage unit, this method is called.
    This method will only be called once in the life of the part.

    Implementation: Calls part to set up persistent storage then restores the
                    object to its stored state and content.
    Return        : None.
    Subclassing   : Call first, override RestoreState and/or Restore Content.
    Notes         : None.
*/

SOM_Scope void  SOMLINK InitPartFromStorage(jmBaseNonContainer *somSelf,
                                             Environment *ev,
                                            ODStorageUnit* storageUnit,
                                            ODPart *partWrapper)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InitPartFromStorage");

    jmBaseNonContainer_parent_ODPart_InitPartFromStorage (somSelf, ev, storageUnit, partWrapper);

    _partWrapper = partWrapper;
    _contentChanged = kODFalse;
    _readOnly = (storageUnit->GetDraft (ev)->GetPermissions (ev) == kODDPReadOnly);
    _fromStorage = kODTrue;

    somSelf->InitializePart (ev);
    somSelf->InternalizeState (ev, storageUnit);
    somSelf->InternalizeContent (ev, storageUnit);
    somSelf->InternalizeLinkSources (ev, storageUnit);
    somSelf->InternalizeLinkTargets (ev, storageUnit);
}


/*
    Initialization of part values.

    Implementation: Retrieves and stores the session id.
                    Creates tokens for selection, mouse and keyboard foci.
                    Creates focus set for selection, mouse and keyboard foci.
                    Creates menubar and popup menu objects.
    Return        : None.
    Subclassing   : Call parent first.
    Notes         : None.
*/

SOM_Scope void SOMLINK InitializePart (jmBaseNonContainer *somSelf, Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InitializePart");

    // get kind for current part
    _strKind = somSelf->PartKind (ev);

    // get session object from runtime
    _session = somSelf->GetStorageUnit (ev)->GetSession (ev);

    // tokenize commonly used strings for focus and view selections
    _focusSelection  = _session->Tokenize (ev, kODSelectionFocus);
    _focusMenu       = _session->Tokenize (ev, kODMenuFocus);
    _focusKey        = _session->Tokenize (ev, kODKeyFocus);
    _focusMouse      = _session->Tokenize (ev, kODMouseFocus);
    _focusModal      = _session->Tokenize (ev, kODModalFocus);
    _focusClipboard  = _session->Tokenize (ev, kODClipboardFocus);
    #if defined (_PLATFORM_OS2_)
      _focusStatusLine = _session->Tokenize (ev, kODStatusLineFocus);
    #endif

    _viewFrame     = _session->Tokenize (ev, kODViewAsFrame);
    _viewSmallIcon = _session->Tokenize (ev, kODViewAsSmallIcon);
    _viewLargeIcon = _session->Tokenize (ev, kODViewAsLargeIcon);
    _viewThumbnail = _session->Tokenize (ev, kODViewAsThumbnail);

    // create default focus set (selection, menu, keyboard)
    _focusSet = _session->GetArbitrator (ev)->CreateFocusSet (ev);
    _focusSet->Add (ev, _focusSelection);
    _focusSet->Add (ev, _focusMenu);
    // _focusSet->Add (ev, _focusKey);

    // create lists
    _frames = new jmBaseList;
    _facets = new jmBaseList;
    _linkSources = new jmBaseList;
    _linkTargets = new jmBaseList;

    // create menu bar object instance
    _menuBar = _session->GetWindowState (ev)->CopyBaseMenuBar (ev);
    _menuPopup = _session->GetWindowState (ev)->CopyBasePopup (ev);
}


/*
    Setup object state to state as defined in the storage unit
*/

SOM_Scope void  SOMLINK InternalizeState (jmBaseNonContainer *somSelf, Environment *ev,
                                                       ODStorageUnit *storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InternalizeState");
    ODStorageUnitRef  weakRef;

    // Restore display frame list.
    if (storageUnit->Exists (ev, kODPropDisplayFrames, kODWeakStorageUnitRefs, 0))
     {
        storageUnit->Focus (ev, kODPropDisplayFrames, kODPosUndefined, kODWeakStorageUnitRefs, 0, kODPosUndefined);
        ODULong size = storageUnit->GetSize (ev);
        storageUnit->SetOffset (ev, 0);

        for (ODULong offset = 0; offset < size; offset += sizeof (ODStorageUnitRef))
         {
            StorageUnitGetValue (storageUnit, ev, sizeof (ODStorageUnitRef), (ODPtr) &weakRef);

            if (storageUnit->IsValidStorageUnitRef (ev, weakRef))
             {
                ODFrame *frame = storageUnit->GetDraft (ev)->AcquireFrame (ev, storageUnit->GetIDFromStorageUnitRef (ev, weakRef));
                _frames->Add (ev, frame);
             }
         }
     }
}


/*
    Load part content from storage unit
*/

SOM_Scope void  SOMLINK InternalizeContent (jmBaseNonContainer *somSelf, Environment *ev, ODStorageUnit* storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InternalizeContent");

}

SOM_Scope void  SOMLINK InternalizeLinkSources (jmBaseNonContainer *somSelf, Environment *ev, ODStorageUnit* storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InternalizeLinkSources");

    if (storageUnit->Exists (ev, kODPropLinkSource, kODStrongStorageUnitRef, 0) == kODTrue)
     {
        storageUnit->Focus (ev, kODPropLinkSource, kODPosUndefined, kODStrongStorageUnitRef, 0, kODPosUndefined);
        short count = storageUnit->GetSize (ev) / sizeof (ODStorageUnitRef);
        for (short sCtr = 0; sCtr < count; sCtr ++)
         {
            ODStorageUnitRef suRef;
            StorageUnitGetValue (storageUnit, ev, sizeof (ODStorageUnitRef), &suRef);

            if (storageUnit->IsValidStorageUnitRef (ev, suRef) == kODTrue)
             {
                ODID linkID = storageUnit->GetIDFromStorageUnitRef (ev, suRef);
                ODLinkSource *link = storageUnit->GetDraft (ev)->AcquireLinkSource (ev, linkID);
                link->SetSourcePart (ev, somSelf->GetStorageUnit (ev));

                _linkSources->Add (ev, link);
             }
         }
     }

    #ifdef DEBUG
      LogLong (somSelf, "Link Sources loaded: ", (long) _linkSources->Count (ev));
    #endif
}

SOM_Scope void  SOMLINK InternalizeLinkTargets (jmBaseNonContainer *somSelf, Environment *ev, ODStorageUnit* storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InternalizeLinkTargets");

    if (storageUnit->Exists (ev, kODPropLink, kODStrongStorageUnitRef, 0) == kODTrue)
     {
        storageUnit->Focus (ev, kODPropLink, kODPosUndefined, kODStrongStorageUnitRef, 0, kODPosUndefined);
        short count = storageUnit->GetSize (ev) / sizeof (ODStorageUnitRef);
        for (short sCtr = 0; sCtr < count; sCtr ++)
         {
            ODStorageUnitRef suRef;
            StorageUnitGetValue (storageUnit, ev, sizeof (ODStorageUnitRef), &suRef);

            if (storageUnit->IsValidStorageUnitRef (ev, suRef) == kODTrue)
             {
                ODID linkID = storageUnit->GetIDFromStorageUnitRef (ev, suRef);
                ODLink *link = storageUnit->GetDraft (ev)->AcquireLink (ev, linkID, kODNULL);
                link->RegisterDependent (ev, somSelf, kODUnknownUpdate);

                _linkTargets->Add (ev, link);
             }
         }
     }

    #ifdef DEBUG
      LogLong (somSelf, "Link Targets loaded: ", (long) _linkTargets->Count (ev));
    #endif
}


/*
    SECTION: OpenDoc part externalization/cloning
*/


SOM_Scope void  SOMLINK Externalize(jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"Externalize");

    if ((_contentChanged) && (_readOnly == kODFalse))
     {
        ODStorageUnit *su = somSelf->GetStorageUnit (ev);
        somSelf->ExternalizeHeader (ev, su);
        somSelf->ClearContentsProperty (ev, su);
        somSelf->ExternalizeState (ev, su, 0, kODNULL);
        somSelf->ExternalizeContent (ev, su);
        somSelf->ExternalizeLinkSources (ev, su);
        somSelf->ExternalizeLinkTargets (ev, su);

        _contentChanged = kODFalse;
     }
}


SOM_Scope void  SOMLINK ExternalizeKinds(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODTypeList* kindset)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ExternalizeKinds");

    somSelf->Externalize (ev);
}


SOM_Scope void  SOMLINK ClearContentsProperty (jmBaseNonContainer *somSelf, Environment *ev,
                                                               ODStorageUnit *storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ClearContentsProperty");

    storageUnit->Focus (ev, kODPropContents, kODPosUndefined, kODNULL, 0, kODPosAll);
    ODULong numValues = storageUnit->CountValues (ev);

    for (ODULong index = numValues; index >= 1; index --)
     {
        // Index from 1 to n through the values.
        storageUnit->Focus (ev, kODPropContents, kODPosUndefined, kODNULL, index, kODPosUndefined);

        ODValueType value = storageUnit->GetType (ev);

        // If the value type is not one we support, remove it.
        if (strcmp (value, _strKind) != 0 )
            storageUnit->Remove (ev);
     }
}


SOM_Scope void  SOMLINK ExternalizeHeader(jmBaseNonContainer *somSelf, Environment *ev,
                                                           ODStorageUnit *storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ExternalizeHeader");

    // create content property
    if (storageUnit->Exists (ev, kODPropContents, kODNULL, 0) == kODFalse)
        storageUnit->AddProperty (ev, kODPropContents);
    if (storageUnit->Exists(ev, kODPropContents, _strKind, 0) == kODFalse)
        storageUnit->AddValue (ev, _strKind);

    // create preferred kind property
    if (storageUnit->Exists (ev, kODPropPreferredKind, kODNULL, 0) == kODFalse)
        storageUnit->AddProperty (ev, kODPropPreferredKind);
    if (storageUnit->Exists (ev, kODPropPreferredKind, kODISOStr, 0) == kODFalse)
     {
        storageUnit->AddValue (ev, kODISOStr);

        // since we are setting up the preferred kind property, we just write
        // out our default "kind" for the editor.  We can write out the user
        // chosen kind in the ExternalizeStateInfo method.
        StorageUnitSetValue (storageUnit, ev, strlen (_strKind) + 1 , _strKind);
     }

    // create display frame list property
    if (storageUnit->Exists (ev, kODPropDisplayFrames, kODNULL, 0) == kODFalse)
        storageUnit->AddProperty (ev, kODPropDisplayFrames);
    if (storageUnit->Exists (ev, kODPropDisplayFrames, kODWeakStorageUnitRefs, 0) == kODFalse)
        storageUnit->AddValue (ev, kODWeakStorageUnitRefs);
}


SOM_Scope void  SOMLINK ExternalizeState (jmBaseNonContainer *somSelf, Environment *ev, ODStorageUnit* storageUnit,
                                                     ODDraftKey key, ODFrame* scopeFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ExternalizeState");

    ODStorageUnitRef  weakRef;
    ODID              frameID;
    ODID              scopeFrameID = (scopeFrame) ? scopeFrame->GetID (ev) : 0;
    ODDraft          *fromDraft = _partWrapper->GetStorageUnit (ev)->GetDraft (ev);

    // store display frame list
    storageUnit->Focus (ev, kODPropDisplayFrames, kODPosUndefined, kODWeakStorageUnitRefs, 0, kODPosUndefined);

    // Persistent object references are stored in a side table, rather than
    // in the property/value stream.  Thus, deleting the contents of a value
    // will not "delete" the references previously written to that value.  To
    // completely "delete" all references written to the value, we must
    // remove the value and add it back.
    storageUnit->Remove (ev);
    storageUnit->AddValue (ev, kODWeakStorageUnitRefs);

    jmBaseIterator *iterator = _frames->CreateIterator (ev);
    ODFrame *frame = (ODFrame *) iterator->First (ev);
    while (frame != NULL)
     {
        frameID = frame->GetID (ev);

        // If a draft key exists, then we are being cloned to another draft.
        // We must "weak" clone our display frame and reference the cloned
        // frame.  The part re-uses the frameID variable so there aren't two
        // different GetWeakStorageUnitRef calls.
        if (key)
            frameID = fromDraft->WeakClone (ev, key, frameID, 0, scopeFrameID);

        // Write out weak references to each of the part's display frames.
        storageUnit->GetWeakStorageUnitRef (ev, frameID, weakRef);
        StorageUnitSetValue (storageUnit, ev, sizeof (ODStorageUnitRef), (ODPtr) &weakRef);

        frame = (ODFrame *) iterator->Next (ev);
     }

    delete iterator;
}

SOM_Scope void  SOMLINK ExternalizeContent (jmBaseNonContainer *somSelf,  Environment *ev,
                                                       ODStorageUnit* storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ExternalizeContent");

}


SOM_Scope void  SOMLINK ExternalizeLinkSources (jmBaseNonContainer *somSelf, Environment *ev, ODStorageUnit* storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ExternalizeLinkSources");

    #ifdef DEBUG
      LogLong (somSelf, "Link Sources: ", (long) _linkSources->Count (ev));
    #endif

    if (_linkSources->Count (ev) > 0)
     {
        jmBaseIterator *iterator = _linkSources->CreateIterator (ev);
        ODLinkSource *link = (ODLinkSource *) iterator->First (ev);

        // if any link sources, create storage unit property to hold
        if (link != kODNULL)
         {
            if (storageUnit->Exists (ev, kODPropLinkSource, kODStrongStorageUnitRef, 0) == kODFalse)
                storageUnit->AddProperty (ev, kODPropLinkSource)->AddValue (ev, kODStrongStorageUnitRef);
            else
                storageUnit->Focus (ev, kODPropLinkSource, kODPosUndefined, kODStrongStorageUnitRef, 0, kODPosUndefined);

            while (link != kODNULL)
             {
                ODStorageUnitRef suRef;
                ODStorageUnit *partSU = somSelf->GetStorageUnit (ev);

                partSU->GetStrongStorageUnitRef (ev, link->GetStorageUnit (ev)->GetID (ev), suRef);
                StorageUnitSetValue (storageUnit, ev, sizeof (ODStorageUnitRef), &suRef);

                link = (ODLinkSource *) iterator->Next (ev);
             }
         }
        else
         {
            if (storageUnit->Exists (ev, kODPropLinkSource, kODStrongStorageUnitRef, 0) == kODTrue)
             {
                storageUnit->Focus (ev, kODPropLinkSource, kODPosUndefined, kODNULL, 0, kODPosUndefined);
                storageUnit->Remove (ev);
             }
         }

        delete iterator;
     }
}

SOM_Scope void  SOMLINK ExternalizeLinkTargets (jmBaseNonContainer *somSelf, Environment *ev, ODStorageUnit* storageUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ExternalizeLinkTargets");

    #ifdef DEBUG
      LogLong (somSelf, "Link Targets: ", (long) _linkTargets->Count (ev));
    #endif

    if (_linkTargets->Count (ev) > 0)
     {
        jmBaseIterator *iterator = _linkTargets->CreateIterator (ev);
        ODLink *link = (ODLink *) iterator->First (ev);
        if (link != kODNULL)
         {
            if (storageUnit->Exists (ev, kODPropLink, kODStrongStorageUnitRef, 0) == kODFalse)
                storageUnit->AddProperty (ev, kODPropLink)->AddValue (ev, kODStrongStorageUnitRef);
            else
                storageUnit->Focus (ev, kODPropLink, kODPosUndefined, kODStrongStorageUnitRef, 0, kODPosUndefined);

            while (link != kODNULL)
             {
                ODStorageUnitRef suRef;
                ODStorageUnit *partSU = somSelf->GetStorageUnit (ev);

                partSU->GetStrongStorageUnitRef (ev, link->GetStorageUnit (ev)->GetID (ev), suRef);
                StorageUnitSetValue (storageUnit, ev, sizeof (ODStorageUnitRef), &suRef);

                link = (ODLink *) iterator->Next (ev);
             }
         }
        else
         {
            if (storageUnit->Exists (ev, kODPropLink, kODStrongStorageUnitRef, 0) == kODTrue)
             {
                storageUnit->Focus (ev, kODPropLink, kODPosUndefined, kODNULL, 0, kODPosUndefined);
                storageUnit->Remove (ev);
             }
         }

        delete iterator;
     }
}

SOM_Scope void  SOMLINK CloneInto(jmBaseNonContainer *somSelf,  Environment *ev,
                                  ODDraftKey key, ODStorageUnit* toSU, ODFrame* initiatingFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"CloneInto");

    // Verify storage unit does not contain this part content. If not, write
    // content to storage unit.
    if (toSU->Exists (ev, kODPropContents, _strKind, 0) == kODFalse)
     {
        // store part information, state and content
        somSelf->ExternalizeHeader (ev, toSU);
        somSelf->ExternalizeState (ev, toSU, key, initiatingFrame);
        somSelf->ExternalizeContent (ev, toSU);
        somSelf->ExternalizeLinkSources (ev, toSU);
        somSelf->ExternalizeLinkTargets (ev, toSU);
     }

    jmBaseNonContainerMethodDebug(kDebugPart,"/CloneInto");
}


/*
    SECTION: Content information
*/

SOM_Scope void  SOMLINK SetContentChanged (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"SetContentChanged");

    if ((_contentChanged == kODFalse) && (_readOnly == kODFalse))
     {
        _contentChanged = kODTrue;
        somSelf->GetStorageUnit (ev)->GetDraft (ev)->SetChangedFromPrev (ev);
     }
}

SOM_Scope ODBoolean  SOMLINK GetContentChanged (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"GetContentChanged");

    return (_contentChanged);
}

SOM_Scope ODBoolean  SOMLINK IsFromStorage (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"IsFromStorage");

    return (_fromStorage);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK EmbeddedFrameUpdated(jmBaseNonContainer *somSelf,
                                              Environment *ev,
                                             ODFrame* frame,
                                             ODUpdateID change)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"EmbeddedFrameChanged");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK LinkStatusChanged(jmBaseNonContainer *somSelf,
                                           Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug("BaseParc","LinkStatusChanged");

    return;
}

/*
    Clipboard support methods
*/

SOM_Scope ODBoolean SOMLINK MenuCutValid (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuCutValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuCopyValid (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuCopyValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuPasteValid (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuPasteValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuPasteAsValid (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuPasteAsValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuPasteLinkValid (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuPasteLinkValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuDeleteValid (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuDeleteValid");

    return (kODFalse);
}

#if defined (_PLATFORM_OS2_)
SOM_Scope ODBoolean SOMLINK MenuBreakLinkValid (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuBreakLinkValid");

    return ((_linkTargets->Count (ev) > 0) ? kODTrue : kODFalse);
}
#endif

//  Place data content on clipboard, default is a blank text string

SOM_Scope void SOMLINK PopulateClipboardStorageUnit (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"PopulateClipboardStorageUnit");

    // base part has no content, populate with empty text string
    su->AddProperty (ev, kODPropContents)->AddValue (ev, kODKindPlainText);
    StorageUnitSetValue (su, ev, 1, "");
}

//  Abstract method, no default data extraction from clipboard

SOM_Scope void SOMLINK ReceiveClipboardStorageUnit (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReceiveClipboardStorageUnit");

    // base part has no content in which to paste content
}

SOM_Scope ODBoolean SOMLINK MenuCut (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuCut");

    // base part has no content to cut, just call copy
    ODBoolean result = somSelf->MenuCopy (ev, frame);
    return (result);
}

SOM_Scope ODBoolean SOMLINK MenuCopy (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuCopy");

    ODBoolean result = kODFalse;

    // get clipboard focus
    ODArbitrator *arbitrator = _session->GetArbitrator (ev);
    arbitrator->RequestFocus (ev, _focusClipboard, frame);

    // clear clipboard, load clipboard storage unit, export
    ODClipboard *clipboard = _session->GetClipboard (ev);
    clipboard->Clear (ev);
    ODStorageUnit *su = clipboard->GetContentStorageUnit (ev);

    // place kind in kODPropContents, then let user populate part data
    somSelf->PopulateClipboardStorageUnit (ev, su);

    // if link source, add link specification to storage unit for clipboard
    if (somSelf->LinkSourceValid (ev))
     {
        ODByteArray *baLinkSpec = somSelf->PopulateLinkSpec (ev);
        ODLinkSpec *linkSpec = somSelf->GetStorageUnit (ev)->GetDraft (ev)->CreateLinkSpec (ev, (ODPart *)somSelf, baLinkSpec);
        if (linkSpec)
         {
            su->AddProperty (ev, kODPropLinkSpec)->AddValue (ev, kODLinkSpec);
            linkSpec->WriteLinkSpec (ev, su);
         }

        if (baLinkSpec != kODNULL)
            DisposeByteArray (baLinkSpec);
     }

    clipboard->ExportClipboard (ev);

    // release clipboard focus
    arbitrator->RelinquishFocus (ev, _focusClipboard, frame);

    result = kODTrue;
    return (result);
}

SOM_Scope ODBoolean SOMLINK MenuPaste (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuPaste");

    somSelf->ReceiveClipboardStorageUnit (ev, su);
    return (kODTrue);
}

SOM_Scope ODBoolean SOMLINK MenuPasteAs (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuPasteAs");

    ODBoolean        result;
    ODPasteAsResult  resultPasteAs;
    ODClipboard     *clipboard = _session->GetClipboard (ev);
    ODBoolean        fCanPasteLink = su->Exists (ev, kODPropLinkSpec, kODLinkSpec, 0);

    result = clipboard->ShowPasteAsDialog (ev, fCanPasteLink, kODPasteAsMergeOnly, (ODFacet *) _facets->GetObject (ev, 0),
                 _viewFrame, &resultPasteAs);
    if (result == kODTrue)
     {
        if (resultPasteAs.pasteLinkSetting == kODTrue)
            somSelf->MenuPasteLink (ev, frame, su);
        else
            somSelf->MenuPaste (ev, frame, su);
     }

    return (result);
}

SOM_Scope ODBoolean SOMLINK MenuPasteLink (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuPasteLink");

    if (su->Exists (ev, kODPropLinkSpec, kODLinkSpec, 0) == kODTrue)
     {
        ODDraft *draft = somSelf->GetStorageUnit (ev)->GetDraft (ev);
        ODLinkSpec *linkSpec = draft->CreateLinkSpec (ev, somSelf, kODNULL);
        su->Focus (ev, kODPropLinkSpec, kODPosSame, kODLinkSpec, 0, kODPosUndefined);
        linkSpec->ReadLinkSpec (ev, su);

        ODLink *link = draft->AcquireLink (ev, 0, linkSpec);
        if (link != kODNULL)
         {
            link->RegisterDependent (ev, somSelf, 0);
            _linkTargets->Add (ev, link);
         }

        delete linkSpec;
     }

    return (kODTrue);
}

SOM_Scope ODBoolean SOMLINK MenuDelete (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuDelete");

    return (kODTrue);
}

#if defined (_PLATFORM_OS2_)
SOM_Scope ODBoolean SOMLINK MenuBreakLink (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuBreakLink");

    // Break all links
    for (short sCtr = 0; sCtr < _linkTargets->Count (ev); sCtr ++)
        somSelf->BreakLink (ev, (ODLink *) _linkTargets->GetObject (ev, 0));

    return (kODTrue);
}
#endif

/*
    Drag and drop support methods
*/

SOM_Scope ODBoolean SOMLINK DragValid (jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DragValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK DropValid (jmBaseNonContainer *somSelf,  Environment *ev, ODDragItemIterator *dragInfo)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DropValid");

    return (kODFalse);
}

SOM_Scope void SOMLINK PopulateDragStorageUnit (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"PopulateDragStorageUnit");

    // base part has no content, populate with empty text string
    su->AddProperty (ev, kODPropContents)->AddValue (ev, kODKindPlainText);
    StorageUnitSetValue (su, ev, 1, "");
}

SOM_Scope ODDropResult SOMLINK ReceiveDropStorageUnit (jmBaseNonContainer *somSelf,  Environment *ev,
                           ODStorageUnit *su, ODFacet *facet, ODPoint *where)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReceiveDropStorageUnit");

    // base part has no content in which to paste content
    return (kODDropFail);
}

SOM_Scope jmBaseImage* SOMLINK AcquireDragImage (jmBaseNonContainer *somSelf, Environment *ev, unsigned long id)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AcquireDragImage");

    jmBaseImage *image = new jmBaseImage;
    image->LoadPointerResource (ev, id);

    return (image);
 }

SOM_Scope ODBoolean SOMLINK Drag (jmBaseNonContainer *somSelf,  Environment *ev, ODFrame *frame, ODEventData *event)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"Drag");

    ODBoolean  result = kODFalse;
    ODPart    *partDestination;

    // clear drag and drop object
    ODDragAndDrop *dd = _session->GetDragAndDrop (ev);
    dd->Clear (ev);

    jmBaseImage *image = somSelf->AcquireDragImage (ev, IDP_DRAG);

    #if defined (_PLATFORM_OS2_)
      DRAGIMAGE di;
      memset (&di, '\0', sizeof (di));
      di.hImage = image->_get_handle (ev);
      di.fl = image->_get_type (ev);

      // populate storage unit
      ODStorageUnit *su = dd->GetContentStorageUnit (ev);
      somSelf->PopulateDragStorageUnit (ev, su);

      // perform drag
      frame->SetDragging (ev, kODTrue);
      ODByteArray baDI = CreateByteArrayStruct (&di, sizeof (DRAGIMAGE));
      ODByteArray baEvent = CreateByteArrayStruct (&event, sizeof (ODEventData *));
      dd->StartDrag (ev, frame, 0, &baDI, &partDestination, &baEvent);
      DisposeByteArrayStruct (&baDI);
      DisposeByteArrayStruct (&baEvent);
      frame->SetDragging (ev, kODFalse);
    #endif

    image->Release (ev);
    delete image;

    return (kODTrue);
 }

/*
    Link source support methods
*/

SOM_Scope ODBoolean  SOMLINK LinkSourceValid(jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kPartHandlerName,"LinkSourceValid");

    return (kODTrue);
}

SOM_Scope ODByteArray * SOMLINK PopulateLinkSpec(jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kPartHandlerName,"PopulateLinkSpec");

    return (kODNULL);
}

SOM_Scope void SOMLINK PopulateLinkStorageUnit (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"PopulateDragStorageUnit");

    // base part has no content, populate with empty text string
    su->AddProperty (ev, kODPropContents)->AddValue (ev, kODKindPlainText);
    StorageUnitSetValue (su, ev, 1, "");
}

SOM_Scope void SOMLINK ReceiveLinkStorageUnit (jmBaseNonContainer *somSelf,  Environment *ev, ODStorageUnit *su)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReceiveLinkStorageUnit");

    // base part has no content in which to update
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK SequenceChanged(jmBaseNonContainer *somSelf,  Environment *ev,
                                        ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"SequenceChanged");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK CanvasChanged(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODFacet* facet)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"CanvasChanged");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK GeometryChanged(jmBaseNonContainer *somSelf,  Environment *ev,
                            ODFacet* facet, ODBoolean clipShapeChanged,
                            ODBoolean externalTransformChanged)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"GeometryChanged");

    return;
}


/*
    Utility method, invalidates all facets
*/

SOM_Scope void SOMLINK InvalidateAllFacets (jmBaseNonContainer *somSelf, Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"InvalidateAllFacets");

    if (_facets != NULL)
     {
        jmBaseIterator *iterator = _facets->CreateIterator (ev);
        ODFacet *facet = (ODFacet *) iterator->First (ev);
        while (facet != NULL)
         {
            facet->Invalidate (ev, kODNULL, kODNULL);
            facet = (ODFacet *) iterator->Next (ev);
         }

        delete iterator;
     }
}


/*
    This method will redispatch based on current view.

    Implementation: Call appropriate view drawing method.
    Return        : None.
    Subclassing   : Developer design.
    Notes         : None.
*/

SOM_Scope void  SOMLINK Draw(jmBaseNonContainer *somSelf,  Environment *ev,
                             ODFacet* facet, ODShape* invalidShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"Draw");

    ODFrame *df = facet->GetFrame (ev);
    ODTypeToken viewCurrent = df->GetViewType (ev);

    if (viewCurrent == _viewFrame)
        somSelf->DrawFrameView (ev, facet, invalidShape);
    else if (viewCurrent == _viewThumbnail)
        somSelf->DrawThumbnailView (ev, facet, invalidShape);
    else if (viewCurrent == _viewSmallIcon)
        somSelf->DrawSmallIconView (ev, facet, invalidShape);
    else if (viewCurrent == _viewLargeIcon)
        somSelf->DrawLargeIconView (ev, facet, invalidShape);

    jmBaseNonContainerMethodDebug(kDebugPart,"/Draw");
    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK DrawFrameView(jmBaseNonContainer *somSelf,  Environment *ev, ODFacet *facet, ODShape *invalidShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DrawFrameView");

    return;
}


void DrawIcon (jmBaseNonContainer *somSelf, Environment *ev, ODFacet *facet, SHORT id, SHORT size)
{
    #if defined (_PLATFORM_OS2_)
      ODCanvas *canvas = facet->GetCanvas (ev);
      HPS hps = canvas->GetPlatformCanvas (ev, kODGPI)->GetPS (ev, facet);

      ODRect rect;
      ODShape *shape = facet->GetFrame (ev)->AcquireFrameShape (ev, canvas);
      shape->GetBoundingBox (ev, &rect);
      ODPoint bottomLeft;
      bottomLeft.x = rect.left;
      bottomLeft.y = rect.bottom;

      ODTransform *transform = facet->AcquireFrameTransform (ev, kODNULL);
      transform->TransformPoint (ev, &bottomLeft);

      HMODULE hmodule;
      DosQueryModuleHandle ("basenc.dll", &hmodule);

      HBITMAP hbm = GpiLoadBitmap (hps, hmodule, id, size, size);

      BITMAPINFOHEADER2 bmp;
      bmp.cbFix = sizeof (bmp);
      GpiQueryBitmapInfoHeader (hbm, &bmp);

      POINTL aptl[4];
      aptl[0].x = FIXED2LONG (bottomLeft.x);
      aptl[0].y = FIXED2LONG (bottomLeft.y);
      aptl[1].x = aptl[0].x + size;
      aptl[1].y = aptl[0].y + size;
      aptl[2].x = 0;
      aptl[2].y = 0;
      aptl[3].x = bmp.cx;
      aptl[3].y = bmp.cy;

      GpiWCBitBlt (hps, hbm, 4, aptl, ROP_SRCCOPY, BBO_IGNORE);

      transform->Release (ev);
      shape->Release (ev);
      canvas->GetPlatformCanvas (ev, kODGPI)->ReleasePS (ev, facet);
    #endif
}


SOM_Scope void  SOMLINK DrawSmallIconView(jmBaseNonContainer *somSelf,  Environment *ev, ODFacet *facet, ODShape *invalidShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DrawSmallIconView");

    DrawIcon (somSelf, ev, facet, IDI_SMALL, 16);
}


SOM_Scope void  SOMLINK DrawLargeIconView(jmBaseNonContainer *somSelf,  Environment *ev, ODFacet *facet, ODShape *invalidShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DrawLargeIconView");

    DrawIcon (somSelf, ev, facet, IDI_LARGE, 32);
}


SOM_Scope void  SOMLINK DrawThumbnailView(jmBaseNonContainer *somSelf,  Environment *ev, ODFacet *facet, ODShape *invalidShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DrawThumbnailView");

    DrawIcon (somSelf, ev, facet, IDI_THUMBNAIL, 64);
}


// Load icon view images

SOM_Scope jmBaseImage* SOMLINK AcquireSmallIcon (jmBaseNonContainer *somSelf, Environment *ev, unsigned long id)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AcquireSmallIcon");

    return (kODNULL);
}

SOM_Scope jmBaseImage* SOMLINK AcquireLargeIcon (jmBaseNonContainer *somSelf, Environment *ev, unsigned long id)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AcquireLargeIcon");

    return (kODNULL);
}

/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK CanvasUpdated(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODCanvas* canvas)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"CanvasUpdated");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK HighlightChanged(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODFacet* facet)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"HighlightChanged");

    return;
}



#if defined (_PLATFORM_OS2_)
  // DR4 event constant definitions

  const kODEvtMouseUp = WM_BUTTON1CLICK;
  const kODEvtMenu = WM_COMMAND;
  const kODCommandCut = EDIT_CUT;
  const kODCommandCopy = EDIT_COPY;
  const kODCommandPaste = EDIT_PASTE;
  const kODCommandPasteAs = EDIT_PASTEAS;
  const kODCommandPasteLink = EDIT_PASTELINK;
  const kODEvtActivate = WM_ACTIVATE;
#endif


SOM_Scope ODBoolean  SOMLINK HandleEvent(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODEventData* event, ODFrame* frame,
                                         ODFacet* facet, ODEventInfo *eventInfo)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);

    ODBoolean handled = kODFalse;

    #if defined (_PLATFORM_OS2_)
      switch (event->msg)
    #elif defined (_PLATFORM_WIN32_)
      switch (event->message)
    #endif
     {
        case kODEvtMouseUp:
            jmBaseNonContainerMethodDebug(kDebugPart,"Event: Mouse Up");

            if (facet != kODNULL)
             {
                handled = kODTrue;
                if (facet->GetWindow (ev)->IsActive (ev) == kODFalse)
                    facet->GetWindow (ev)->Select (ev);
                else
                 {
                    jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) frame->GetPartInfo (ev);
                    if (frameInfo->IsFrameActive (ev) == kODFalse)
                     {
                        if (somSelf->ActivateFrame (ev, frame) == kODTrue)
                            frameInfo->_set_activeFacet (ev, facet);
                        else
                            handled = kODFalse;
                     }
                 }
             }
            break;

        #if defined (_PLATFORM_OS2_)
          case WM_BUTTON1DBLCLK:
        #elif defined (_PLATFORM_WIN32_)
          case WM_LBUTTONDBLCLK:
        #endif
            jmBaseNonContainerMethodDebug(kDebugPart,"Event: LMB double click");

            somSelf->Open (ev, frame);
            handled = kODTrue;
            break;

        #if defined (_PLATFORM_OS2_)
        case WM_BUTTON2MOTIONSTART:
            jmBaseNonContainerMethodDebug(kDebugPart,"Event: RMB motion start");

            if (somSelf->DragValid (ev))
                handled = somSelf->Drag (ev, frame, event);
            break;
        #endif

        case kODEvtMenu:
         {
            // if any action is defined, set handled flag otherwise default
            // case statement will reset flag to false
            handled = kODTrue;

            #if defined (_PLATFORM_OS2_)
              short id = SHORT1FROMMP (event->mp1);
            #elif defined (_PLATFORM_WIN32_)
              short id = LOWORD (event->wParam);
            #endif

            switch (id)
             {
                case kODCommandCut:
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: Cut");

                    if (somSelf->MenuCutValid (ev))
                        somSelf->MenuCut (ev, frame);
                    break;

                case kODCommandCopy:
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: Copy");

                    if (somSelf->MenuCopyValid (ev))
                        somSelf->MenuCopy (ev, frame);
                    break;

                case kODCommandPaste:
                case kODCommandPasteAs:
                case kODCommandPasteLink:
                 {
                    // Get storage unit from clipboard
                    ODArbitrator *arbitrator = _session->GetArbitrator (ev);
                    arbitrator->RequestFocus (ev, _focusClipboard, frame);
                    ODClipboard *clipboard = _session->GetClipboard (ev);
                    ODStorageUnit *su = clipboard->GetContentStorageUnit (ev);

                    switch (id)
                     {
                        case kODCommandPaste:
                            jmBaseNonContainerMethodDebug(kDebugPart,"Event: Paste");

                            if (somSelf->MenuPasteValid (ev, su))
                                somSelf->MenuPaste (ev, frame, su);
                            break;

                        case kODCommandPasteAs:
                            jmBaseNonContainerMethodDebug(kDebugPart,"Event: Paste as");

                            if (somSelf->MenuPasteAsValid (ev, su))
                                somSelf->MenuPasteAs (ev, frame, su);
                            break;

                        case kODCommandPasteLink:
                            jmBaseNonContainerMethodDebug(kDebugPart,"Event: Paste link");

                            if (somSelf->MenuPasteLinkValid (ev, su))
                                somSelf->MenuPasteLink (ev, frame, su);
                            break;
                     }

                    arbitrator->RelinquishFocus (ev, _focusClipboard, frame);
                    break;
                 }

                case VIEW_SALARGEICON:
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: Show as large icon");

                    frame->ChangeViewType (ev, _viewLargeIcon);
                    break;

                case VIEW_SASMALLICON:
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: Show as small icon");

                    frame->ChangeViewType (ev, _viewSmallIcon);
                    break;

                case VIEW_SATHUMBNAIL:
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: Show as thumbnail");

                    frame->ChangeViewType (ev, _viewThumbnail);
                    break;

                case VIEW_SAFRAME:
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: Show as frame");

                    frame->ChangeViewType (ev, _viewFrame);
                    break;

                case VIEW_PROPERTIES:
                 {
                    jmBaseNonContainerMethodDebug(kDebugPart,"Event: View properties");

                    ODFrameFacetIterator *facets = frame->CreateFacetIterator (ev);
                    _session->GetInfo (ev)->ShowPartFrameInfo (ev, facets->First (ev), kODFalse);
                    delete facets;
                    break;
                 }

                default:
                 {
                    char szBuffer[20];
                    sprintf (szBuffer, "Event: 0x%08lx", (unsigned long) id);

                    jmBaseNonContainerMethodDebug(kDebugPart,szBuffer);

                    handled = kODFalse;
                    break;
                 }
             }
            break;
         }

        #if defined (_PLATFORM_OS2_)
        case WM_CONTEXTMENU:
         {
            jmBaseNonContainerMethodDebug(kDebugPart,"Event: Popup menu");

            jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) frame->GetPartInfo (ev);
            if (frameInfo->IsFrameActive (ev) == kODFalse)
                somSelf->ActivateFrame (ev, frame);

            _menuPopup->Display (ev);
            handled = kODTrue;
            break;
         }
        #endif

        case kODEvtActivate:
         {
            jmBaseNonContainerMethodDebug(kDebugPart,"Event: Activate");

            #if defined (_PLATFORM_OS2_)
              ODBoolean fActivate = (SHORT1FROMMP (event->mp1) != 0) ? kODTrue : kODFalse;
            #elif defined (_PLATFORM_WIN32_)
              ODBoolean fActivate = (LOWORD (event->wParam) != WA_INACTIVE) ? kODTrue : kODFalse;
            #endif

            somSelf->WindowActivate (ev, frame, fActivate);
            handled = kODTrue;
            break;
         }
     }

    return (handled);
}


/*
    When this part has a frame activated, this method will acquire the focus set and call
    FocusAcquired for each focus of interest.

    Implementation: Request focusSet and call FocusAcquired on default foci.
    Return        : None.
    Subclassing   : Call first, then call FocusAcquired on any additional foci.
    Notes         : Any additional foci must be added to the focusSet after CommonInitPart call.
*/

SOM_Scope ODBoolean SOMLINK ActivateFrame(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ActivateFrame");

    ODBoolean result = kODFalse;
    if (frame != kODNULL)
     {
        if (_session->GetArbitrator (ev)->RequestFocusSet (ev, _focusSet, frame))
         {
            somSelf->FocusAcquired (ev, _focusSelection, frame);
            somSelf->FocusAcquired (ev, _focusMenu, frame);
            // somSelf->FocusAcquired (ev, _focusKey, frame);
            result = kODTrue;
         }
     }

    return (result);
}

/*
    When a frame is deactivated, the focus set related to the frame must be released.

    Implementation: FocusSet is released, FocusLost called for each of default foci.
    Returns       : None.
    Subclassing   : Call first, then call FocusLost on any additional foci.
    Notes         : Any additional foci must be added to the focusSet after CommonInitPart call.
*/

SOM_Scope void  SOMLINK DeActivateFrame(jmBaseNonContainer *somSelf,  Environment *ev,
                                        ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DeActivateFrame");

    if (frame != kODNULL)
     {
        _session->GetArbitrator (ev)->RelinquishFocusSet (ev, _focusSet, frame);
        somSelf->FocusLost (ev, _focusSelection, frame);
        somSelf->FocusLost (ev, _focusMenu, frame);
        // somSelf->FocusLost (ev, _focusKey, frame);
     }
}


/*
    When the window changes activation, handle activation change for the appropriate frame.

    Implementation: If activating, reactivate frame otherwise save frame to activate.
    Return        : None.
    Subclassing   : Developer design.
    Notes         : None.
*/

SOM_Scope void  SOMLINK WindowActivate(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODFrame* frame, ODBoolean activating)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ActivatingWindow");

    jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) frame->GetPartInfo (ev);

    // If window activating and part has an active frame ready, then activate the frame
    // If window deactivating and a frame is active, mark active frame as ready

    if ((activating == kODTrue) && (frameInfo->_get_reactivateFrame (ev) == kODTrue))
     {
        somSelf->ActivateFrame (ev, frame);
        frameInfo->_set_reactivateFrame (ev, kODFalse);
     }
    else
     {
        if ((activating == kODFalse) && (frameInfo->IsFrameActive (ev) == kODTrue))
            frameInfo->_set_reactivateFrame (ev, kODTrue);
     }
}


/*
    The root part creates the initial window, and may create alternate views of itself. An
    embedded part creates a new window with itself as the root part. For root parts, use the
    default settings.

    Implementation: If not the root part, create 400x400 window at offset (100, 100).
    Return        : ODWindow object created.
    Subclassing   : Developer design.
    Notes         : None.
*/

SOM_Scope ODWindow*  SOMLINK MakeWindow(jmBaseNonContainer *somSelf,
                                           Environment *ev, ODFrame* sourceFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MakeWindow");

    ODPlatformWindow  platformWindow;
    ODWindow         *window;
    ODWindowState *ws = _session->GetWindowState (ev);

    // if this is the initial window creation or root part
    if ((sourceFrame == kODNULL) || (sourceFrame->IsRoot (ev)))
     {
        // create new window with default creation options
        #if defined (_PLATFORM_OS2_)
          platformWindow = _session->CreatePlatformWindow (ev, ODPlatformWindowDefaultCreateOptions);
        #elif defined (_PLATFORM_WIN32_)
          platformWindow = ws->CreatePlatformWindow (ev, kODFalse);
        #endif
     }
    else
     {
        // create new window with scroll bars
        #if defined (_PLATFORM_OS2_)
          platformWindow = _session->CreatePlatformWindow (ev, ODPlatformWindowDefaultCreateOptions | FCF_HORZSCROLL | FCF_VERTSCROLL);
          WinSetWindowPos (platformWindow, HWND_TOP, 100, 100, 400, 400, SWP_SIZE | SWP_MOVE);
        #elif defined (_PLATFORM_WIN32_)
          platformWindow = ws->CreatePlatformWindow (ev, kODFalse | WS_HSCROLL | WS_VSCROLL);
          SetWindowPos (platformWindow, HWND_TOP, 100, 100, 400, 400, 0);
        #endif
     }

    // register the window, creating an ODWindow object
    window = ws->RegisterWindow (ev, platformWindow, kODFrameObject,(sourceFrame == kODNULL), kODTrue, kODFalse, kODTrue,
                     kODTrue, _partWrapper, _viewFrame, _session->Tokenize (ev, kODPresDefault), sourceFrame);

    return (window);
}


/*
    Implementation: jmBaseNonContainer implements no properties.
    Return        : NULL value, indicating no properties.
    Subclassing   : Replace with part specific action.
    Notes         : None.
*/

SOM_Scope ODStorageUnit*  SOMLINK AcquireContainingPartProperties(jmBaseNonContainer *somSelf,
                                                               Environment *ev,
                                                              ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"GetContainingPartProperties");

    return (kODNULL);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK ContainingPartPropertiesUpdated(jmBaseNonContainer *somSelf,
                                                         Environment *ev,
                                                        ODFrame* frame,
                                                        ODStorageUnit* propertyUnit)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ContainingPartPropertiesChanged");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODBoolean  SOMLINK RevealFrame(jmBaseNonContainer *somSelf,  Environment *ev,
                                    ODFrame* embeddedFrame, ODShape* revealShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"RevealFrame");

    return (kODFalse);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK EmbeddedFrameSpec(jmBaseNonContainer *somSelf,
                                           Environment *ev, ODFrame *embeddedFrame,
                                          ODObjectSpec *spec)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"EmbeddedFrameSpec");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODEmbeddedFramesIterator*  SOMLINK CreateEmbeddedFramesIterator(jmBaseNonContainer *somSelf,
                                                                           Environment *ev,
                                                                          ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"CreateEmbeddedFramesIterator");

    return (kODNULL);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK AttachSourceFrame(jmBaseNonContainer *somSelf,
                                           Environment *ev, ODFrame* frame,
                                          ODFrame* sourceFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AttachSourceFrame");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK DisplayFrameAdded(jmBaseNonContainer *somSelf,  Environment *ev,
                                        ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DisplayFrameAdded");

    if (frame->GetViewType (ev) == kODNullTypeToken)
        frame->SetViewType (ev, _viewFrame);

    jmBaseFrameInfo *frameInfo = new jmBaseFrameInfo;
    if (frame->IsRoot (ev))
        frameInfo->_set_reactivateFrame (ev, kODTrue);

    frame->SetPartInfo (ev, (ODInfoType) frameInfo);

    _frames->Add (ev, frame);
    frame->Acquire (ev);
    frame->SetDroppable (ev, somSelf->DropValid (ev, kODNULL));

    return;
}

SOM_Scope void  SOMLINK DisplayFrameConnected(jmBaseNonContainer *somSelf,  Environment *ev,
                                        ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DisplayFrameConnected");

    if (_frames->Contains (ev, frame) == kODFalse)
     {
        _frames->Add (ev, frame);
        frame->Acquire (ev);
     }

    frame->SetDroppable (ev, somSelf->DropValid (ev, kODNULL));
}


void HandleDisplayFrameRemoval (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame, ODBoolean fRemove)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);

    ODArbitrator* arbitrator = _session->GetArbitrator (ev);

    // Make sure the frame going away doesn't own any foci.  Forgetting
    // to do this, will cause a "refcounting" error when the frame
    // is deleted by the draft.

    ODFrame* tempFrame = arbitrator->AcquireFocusOwner (ev, _focusSelection);
    if (tempFrame != kODNULL)
     {
        if (tempFrame == frame)
         {
            arbitrator->RelinquishFocus (ev, _focusSelection, frame);
            somSelf->FocusLost (ev, _focusSelection, frame);
         }
        tempFrame->Release (ev);
     }

    tempFrame = arbitrator->AcquireFocusOwner (ev, _focusMenu);
    if (tempFrame != kODNULL)
     {
        if (tempFrame == frame)
         {
           arbitrator->RelinquishFocus (ev, _focusMenu, frame);
           somSelf->FocusLost (ev, _focusMenu, frame);
         }
        tempFrame->Release (ev);
     }
    jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) frame->GetPartInfo (ev);

    // If we are the root of a child window, we need to notify
    // our source frame that we are going away.
    if (frameInfo->HasSourceFrame (ev))
     {
        ODFrame         *sourceFrame = frameInfo->_get_sourceFrame (ev);
        jmBaseFrameInfo *sourceFrameInfo = (jmBaseFrameInfo *) sourceFrame->GetPartInfo (ev);

        if (fRemove)
         {
            // Invalidate the source frame. We do this because the
            // source frame may have a unique display when it has
            // been opened into a part window.  This forces the
            // frame to redraw "not opened".
            sourceFrame->Invalidate (ev, kODNULL, kODNULL);
         }

        // Release our reference to the source frame.
        frameInfo->ReleaseSourceFrame (ev);

        // Detach ourself from the source frame.
        sourceFrameInfo->DetachFrame (ev, frame);

        // If the frame is the root, it is a part window going away
        // and we need to notify our source frame that it no longer
        // has a part window.
        if (frame->IsRoot (ev))
            sourceFrameInfo->_set_partWindow (ev, kODNULL);
     }

    if (fRemove)
     {
        // If we have a child window, we need to close it.
        if (frameInfo->HasPartWindow (ev))
         {
            frameInfo->_get_partWindow (ev)->CloseAndRemove (ev);
            frameInfo->_set_partWindow (ev, kODNULL);
         }
     }

    // If we have attached frames, we need to notify them that we
    // are going away.
    if (frameInfo->HasAttachedFrames (ev))
     {
        ODFrame              *attachedFrame;
        jmBaseFrameInfo *attachedFrameInfo;
        jmBaseList      *attachedFrames = frameInfo->_get_attachedFrames (ev);

        jmBaseIterator *iterator = attachedFrames->CreateIterator (ev);
        attachedFrame = (ODFrame *) iterator->First (ev);
        while (attachedFrame != kODNULL)
         {
            attachedFrameInfo = (jmBaseFrameInfo *) attachedFrame->GetPartInfo (ev);

            // Tell the attached frame that it's source is going away.
            attachedFrameInfo->ReleaseSourceFrame (ev);

            // Remove the attached frame from our frame info list.
            frameInfo->DetachFrame (ev, attachedFrame);

            attachedFrame = (ODFrame *) iterator->Next (ev);
         }

        delete iterator;
     }

    // Clean up the display frame, remove it from our internal list,
    // and let go of it.
    frame->SetPartInfo (ev, (ODInfoType) kODNULL);
    delete frameInfo;

    _frames->Remove (ev, frame);
    frame->Release (ev);
}

/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK DisplayFrameRemoved(jmBaseNonContainer *somSelf,
                                            Environment *ev,
                                           ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DisplayFrameRemoved");

    HandleDisplayFrameRemoval (somSelf, ev, frame, kODTrue);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK DisplayFrameClosed(jmBaseNonContainer *somSelf,
                                           Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DisplayFrameClosed");

    HandleDisplayFrameRemoval (somSelf, ev, frame, kODFalse);
    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK FrameShapeChanged(jmBaseNonContainer *somSelf,
                                           Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"FrameShapeChanged");

    return;
}


/*
    PartInfo (FrameInfo) internalize and externalize methods. For information on the object and
    its associated data and operations, see the BaseInfo class documentation. All extensions
    should be made to the BaseInfo class where possible, leaving this implementation clean.
*/

/*
    Implementation: Create and populate frame info class instance.
    Return        : Instance of class.
    Subclassing   : See BaseInfo documentation.
    Notes         : None.
*/

SOM_Scope ODInfoType  SOMLINK ReadPartInfo(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODFrame* frame, ODStorageUnitView* storageUnitView)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReadPartInfo");

    // create a new instance and populate it from the storage unit
    jmBaseFrameInfo *frameInfo = new jmBaseFrameInfo;
    frameInfo->InitFromStorage (ev, storageUnitView, kPartInfo);

    // if a source frame exists, add this frame to its list of attached frames
    if (frameInfo->HasSourceFrame (ev) == kODTrue)
     {
        jmBaseFrameInfo *frameSource = (jmBaseFrameInfo *) frameInfo->_get_sourceFrame (ev)->GetPartInfo (ev);
        frameSource->AttachFrame (ev, frame);
     }

    return ((ODInfoType) frameInfo);
}


/*
    Implementation: Call externalize method from BaseInfo class.
    Return        : None.
    Subclassing   : See BaseInfo documentation.
    Notes         : None.
*/

SOM_Scope void  SOMLINK WritePartInfo(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODInfoType partInfo, ODStorageUnitView* storageUnitView)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"WritePartInfo");

    // store the current frame state
    ((jmBaseFrameInfo *) partInfo)->Externalize (ev, storageUnitView, kPartInfo);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODID  SOMLINK Open(jmBaseNonContainer *somSelf,  Environment *ev,
                             ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"Open");

    ODWindow      *window = kODNULL;
    ODBoolean      fCreate = kODFalse;
    ODWindowState *ws = _session->GetWindowState (ev);

    // if 'View as Window' or opening root part
    if (frame)
     {
        if (frame->IsRoot (ev))
         {
            WindowProperties props;
            ODULong flCreateFlags;

            BeginGetWindowProperties (ev, frame, &props);
            if (props.createFlags != 0)
                flCreateFlags = props.createFlags;
            else
                #if defined (_PLATFORM_OS2_)
                  flCreateFlags = ODPlatformWindowDefaultCreateOptions;
                #elif defined (_PLATFORM_WIN32_)
                  flCreateFlags = kODFalse;
                #endif

            #if defined (_PLATFORM_OS2_)
              HWND hwndFrame = _session->CreatePlatformWindow (ev, flCreateFlags);
              WinSetWindowPos (hwndFrame, HWND_TOP, props.boundsRect.xLeft, props.boundsRect.yBottom, props.boundsRect.xRight,
                  props.boundsRect.yTop, SWP_SIZE | SWP_MOVE | props.swpFlags);
            #elif defined (_PLATFORM_WIN32_)
              HWND hwndFrame = ws->CreatePlatformWindow (ev, flCreateFlags);
              SetWindowPos (hwndFrame, HWND_TOP, props.boundsRect.left, props.boundsRect.bottom, props.boundsRect.right,
                  props.boundsRect.top, props.swpFlags);
            #endif

            window = _session->GetWindowState (ev)->RegisterWindowForFrame (ev, hwndFrame, frame, props.isRootWindow, kODTrue,
                         kODFalse, kODTrue, kODTrue, props.sourceFrame);

            EndGetWindowProperties (ev, &props);
            window->Open (ev);
            window->Show (ev);
         }
        else
         {
            window = _session->GetWindowState (ev)->AcquireWindow (ev, _idWindow);
            if (window)
             {
                window->Select (ev);
                window->Release (ev);
             }
            else
                fCreate = kODTrue;
         }
     }
    else
        fCreate = kODTrue;

    if (fCreate)
     {
        window = somSelf->MakeWindow (ev, frame);
        _idWindow = window->GetID (ev);
        window->Open (ev);
        window->Show (ev);
        window->Select (ev);
        window->Release (ev);
     }

    return (_idWindow);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODFrame*  SOMLINK RequestEmbeddedFrame(jmBaseNonContainer *somSelf,
                                                  Environment *ev,
                                                 ODFrame* containingFrame,
                                                 ODFrame* baseFrame,
                                                 ODShape* frameShape,
                                                 ODPart* embedPart,
                                                 ODTypeToken viewType,
                                                 ODTypeToken presentation,
                                                 ODBoolean isOverlaid)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"RequestEmbeddedFrame");

    return (baseFrame);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK RemoveEmbeddedFrame(jmBaseNonContainer *somSelf,
                                             Environment *ev,
                                            ODFrame* embeddedFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"RemoveEmbeddedFrame");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODShape*  SOMLINK RequestFrameShape(jmBaseNonContainer *somSelf,
                                               Environment *ev,
                                              ODFrame* embeddedFrame,
                                              ODShape* frameShape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"RequestFrameShape");

    return (frameShape);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK UsedShapeChanged(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODFrame* embeddedFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"UsedShapeChanged");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODShape*  SOMLINK AdjustBorderShape(jmBaseNonContainer *somSelf,
                                               Environment *ev,
                                              ODFacet* embeddedFacet,
                                              ODShape* shape)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AdjustBorderShape");

    return (shape);
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK FacetAdded(jmBaseNonContainer *somSelf,  Environment *ev,
                                   ODFacet* facet)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"FacetAdded");

    ODFrame       *frame = facet->GetFrame (ev);
    jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) frame->GetPartInfo (ev);

    // If root part, activate self
    if (frame->IsRoot (ev))
     {
        if (somSelf->ActivateFrame (ev, frame))
            frameInfo->_set_activeFacet (ev, facet);
     }

    _facets->Add (ev, facet);
    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK FacetRemoved(jmBaseNonContainer *somSelf,  Environment *ev,
                                     ODFacet* facet)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"FacetRemoved");

    _facets->Remove (ev, facet);
    return;
}


SOM_Scope ODBoolean  SOMLINK BeginRelinquishFocus(jmBaseNonContainer *somSelf,
                                                   Environment *ev,
                                                  ODTypeToken focus,
                                                  ODFrame* ownerFrame,
                                                  ODFrame* proposedFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"BeginRelinquishFocus");

    return (kODTrue);
}

SOM_Scope void  SOMLINK CommitRelinquishFocus(jmBaseNonContainer *somSelf,
                                               Environment *ev,
                                              ODTypeToken focus,
                                              ODFrame* ownerFrame,
                                              ODFrame* proposedFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"CommitRelinquishFocus");

    somSelf->FocusLost(ev, focus, ownerFrame);
}

SOM_Scope void  SOMLINK AbortRelinquishFocus(jmBaseNonContainer *somSelf,
                                              Environment *ev,
                                             ODTypeToken focus,
                                             ODFrame* ownerFrame,
                                             ODFrame* proposedFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AbortRelinquishFocus");

    return;
}

SOM_Scope void  SOMLINK FocusAcquired(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODTypeToken focus, ODFrame* ownerFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"FocusAcquired");

    if (focus == _focusSelection)
     {
        jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) ownerFrame->GetPartInfo (ev);
        frameInfo->SetFrameActive (ev, kODTrue);
     }
    else
     {
        if ((focus == _focusMenu) && (_menuBar != kODNULL))
            _menuBar->Display (ev);
     }

    return;
}

SOM_Scope void  SOMLINK FocusLost(jmBaseNonContainer *somSelf,  Environment *ev,
                                  ODTypeToken focus, ODFrame* ownerFrame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"FocusLost");

    if (focus == _focusSelection)
     {
        jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) ownerFrame->GetPartInfo (ev);
        frameInfo->SetFrameActive (ev, kODFalse);
        ownerFrame->SetPartInfo (ev, (ODInfoType) frameInfo);
     }

    return;
}

SOM_Scope void  SOMLINK ChangeKind(jmBaseNonContainer *somSelf,  Environment *ev,
                                   ODType kind)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ChangeKind");

    return;
}


/*
    Implementation: Enable/Disable Edit menu items based on framework enablement
    Return        : None
    Subclassing   : No restrictions
    Notes         :
*/

SOM_Scope void  SOMLINK AdjustMenus(jmBaseNonContainer *somSelf,  Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AdjustMenus");

    if (_menuBar != kODNULL)
     {
        jmBaseFrameInfo *frameInfo = (jmBaseFrameInfo *) frame->GetPartInfo (ev);
        if (frameInfo->IsFrameActive (ev) == kODFalse)
         {
            if (frame->IsRoot (ev))
             {
                // Open As and Print menu support
             }
         }
        else
         {
            // Get storage unit from clipboard
            ODArbitrator *arbitrator = _session->GetArbitrator (ev);
            arbitrator->RequestFocus (ev, _focusClipboard, frame);
            ODClipboard *clipboard = _session->GetClipboard (ev);
            ODStorageUnit *su = clipboard->GetContentStorageUnit (ev);

            // Update Edit menu
            _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_CUT, (_readOnly) ? kODFalse : somSelf->MenuCutValid (ev));
            _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_COPY, somSelf->MenuCopyValid (ev));

            #if defined (_PLATFORM_OS2_)
              _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_PASTE_MENU, (_readOnly) ? kODFalse : somSelf->MenuPasteValid (ev, su));
            #endif
            _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_PASTEAS, (_readOnly) ? kODFalse : somSelf->MenuPasteAsValid (ev, su));

            // if link spec is in clipboard storage unit
            if (su->Exists (ev, kODPropLinkSpec, kODLinkSpec, 0) == kODTrue)
                _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_PASTELINK, (_readOnly) ? kODFalse : somSelf->MenuPasteLinkValid (ev, su));
            else
                _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_PASTELINK, kODFalse);

            _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_DELETE, (_readOnly) ? kODFalse : somSelf->MenuDeleteValid (ev));
            #if defined (_PLATFORM_OS2_)
            _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_BREAKLINK, (_readOnly) ? kODFalse : somSelf->MenuBreakLinkValid (ev));
            #endif

            #if defined (_PLATFORM_OS2_)
              _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_SEL_SHOWAS, somSelf->MenuSelectionShowAsValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_SEL_SASMALLICON, somSelf->MenuSelectionShowAsSmallIconValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_SEL_SALARGEICON, somSelf->MenuSelectionShowAsLargeIconValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_SEL_SATHUMBNAIL, somSelf->MenuSelectionShowAsThumbnailValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_EDIT, EDIT_SEL_SAFRAME, somSelf->MenuSelectionShowAsFrameValid (ev, frame));

              _menuBar->EnableMenuItem (ev, IDMS_VIEW, VIEW_SHOWAS, somSelf->MenuShowAsValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_VIEW, VIEW_SASMALLICON, somSelf->MenuShowAsSmallIconValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_VIEW, VIEW_SALARGEICON, somSelf->MenuShowAsLargeIconValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_VIEW, VIEW_SATHUMBNAIL, somSelf->MenuShowAsThumbnailValid (ev, frame));
              _menuBar->EnableMenuItem (ev, IDMS_VIEW, VIEW_SAFRAME, somSelf->MenuShowAsFrameValid (ev, frame));

              _menuBar->EnableMenuItem (ev, IDMS_VIEW, VIEW_PROPERTIES, kODTrue);
            #endif

            arbitrator->RelinquishFocus (ev, _focusClipboard, frame);
         }
     }

    if (_menuPopup != kODNULL)
     {
        #if defined (_PLATFORM_OS2_)
          _menuPopup->EnableMenuItem (ev, IDMS_VIEW, VIEW_SHOWAS, somSelf->MenuShowAsValid (ev, frame));
          _menuPopup->EnableMenuItem (ev, IDMS_VIEW, VIEW_SASMALLICON, somSelf->MenuShowAsSmallIconValid (ev, frame));
          _menuPopup->EnableMenuItem (ev, IDMS_VIEW, VIEW_SALARGEICON, somSelf->MenuShowAsLargeIconValid (ev, frame));
          _menuPopup->EnableMenuItem (ev, IDMS_VIEW, VIEW_SATHUMBNAIL, somSelf->MenuShowAsThumbnailValid (ev, frame));
          _menuPopup->EnableMenuItem (ev, IDMS_VIEW, VIEW_SAFRAME, somSelf->MenuShowAsFrameValid (ev, frame));
        #endif
     }

    return;
}

SOM_Scope void  SOMLINK UndoAction(jmBaseNonContainer *somSelf,  Environment *ev,
                                   ODActionData *actionState)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"UndoAction");

    return;
}

SOM_Scope void  SOMLINK RedoAction(jmBaseNonContainer *somSelf,  Environment *ev,
                                   ODActionData *actionState)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"RedoAction");

    return;
}

SOM_Scope void  SOMLINK DisposeActionState(jmBaseNonContainer *somSelf,
                                            Environment *ev,
                                           ODActionData *actionState,
                                           ODDoneState doneState)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DisposeActionState");

    return;
}

SOM_Scope void  SOMLINK WriteActionState(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODActionData *actionState, ODStorageUnitView* storageUnitView)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"WriteActionState");

    return;
}

SOM_Scope ODActionData  SOMLINK ReadActionState(jmBaseNonContainer *somSelf,  Environment *ev,
                                         ODStorageUnitView* storageUnitView)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReadActionState");

    ODActionData data;
    data._maximum = 0;
    data._length = 0;
    data._buffer = kODNULL;

    return (data);
}


SOM_Scope void  SOMLINK Release(jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"Release");

    jmBaseNonContainer_parent_ODPart_Release (somSelf, ev);
    if (somSelf->GetRefCount (ev) == 0)
        somSelf->GetStorageUnit (ev)->GetDraft (ev)->ReleasePart (ev, somSelf);
}

SOM_Scope void  SOMLINK ReleaseAll(jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReleaseAll");

}


/*
    Implementation: BaseNonContainer supports the four standard views, using
                    a bitmap for thumbnail view.
    Return        : None
    Subclassing   : Derived class may call or replace as desired
    Notes         : None
*/

SOM_Scope void  SOMLINK ViewTypeChanged(jmBaseNonContainer *somSelf, Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ViewTypeChanged");

    // if no view is set, set initial view to frame
    ODTypeToken view = frame->GetViewType (ev);
    if (view == kODNullTypeToken)
        frame->SetViewType (ev, _viewFrame);

    ODFrameFacetIterator *facets = frame->CreateFacetIterator (ev);
    ODFacet *facet = facets->First (ev);
    ODCanvas *canvas = facet->GetCanvas (ev);

    ODShape *newUsedShape = frame->CreateShape (ev);
    ODShape *frameShape = frame->AcquireFrameShape (ev, canvas);
    newUsedShape->CopyFrom (ev, frameShape);

    if (view == _viewFrame)
        frame->ChangeUsedShape (ev, kODNULL, canvas);
    else
     {
        ODCoordinate size;
        if (view == _viewSmallIcon)
            size = ODIntToFixed (16);
        else if (view == _viewLargeIcon)
            size = ODIntToFixed (32);
        else
            size = ODIntToFixed (64);

        ODRect rect;
        rect.left = 0;
        rect.bottom = 0;
        rect.top = size;
        rect.right = size;

        newUsedShape->SetRectangle (ev, &rect);
        frame->ChangeUsedShape (ev, newUsedShape, canvas);
     }

    for (ODFacet *childFacet = facets->First (ev); facets->IsNotComplete (ev); childFacet = facets->Next (ev))
     {
        ODShape *newActiveShape = frame->CreateShape (ev);
        newActiveShape->CopyFrom (ev, newUsedShape);
        childFacet->ChangeActiveShape (ev, newActiveShape, canvas);
        newActiveShape->Release (ev);
     }

    delete facets;
    newUsedShape->Release (ev);
    frameShape->Release (ev);

    // Invalidate frame, redraw to new view
    ODFrame *parentFrame = frame->AcquireContainingFrame (ev);
    if (parentFrame != kODNULL)
     {
        parentFrame->Invalidate (ev, kODNULL, canvas);
        parentFrame->Release (ev);
     }
}


/*
    Implementation: No implementation, no presentations implemented
    Return        : None
    Subclassing   : Derived parts that support multiple presentations must
                    implement method
    Notes         : None
*/

SOM_Scope void  SOMLINK PresentationChanged(jmBaseNonContainer *somSelf, Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"PresentationChanged");

    return;
}


//  View related menu item setting methods

//  Embedding not supported, therefore there are never selected parts

SOM_Scope ODBoolean SOMLINK MenuSelectionShowAsValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuSelectionShowAsValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuSelectionShowAsSmallIconValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuSelectionShowAsSmallIconValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuSelectionShowAsLargeIconValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuSelectionShowAsLargeIconValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuSelectionShowAsThumbnailValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuSelectionShowAsThumbnailValid");

    return (kODFalse);
}

SOM_Scope ODBoolean SOMLINK MenuSelectionShowAsFrameValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuSelectionShowAsFrameValid");

    return (kODFalse);
}


//  Default behaviour allows all four view types for parts, except root parts
//  where the part must maintain frame view and the other views are disabled.

SOM_Scope ODBoolean SOMLINK MenuShowAsValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuShowAsValid");

    return ((frame->IsRoot (ev)) ? kODFalse : kODTrue);
}

SOM_Scope ODBoolean SOMLINK MenuShowAsSmallIconValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuShowAsSmallIconValid");

    return ((frame->IsRoot (ev)) ? kODFalse : kODTrue);
}

SOM_Scope ODBoolean SOMLINK MenuShowAsLargeIconValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuShowAsLargeIconValid");

    return ((frame->IsRoot (ev)) ? kODFalse : kODTrue);
}

SOM_Scope ODBoolean SOMLINK MenuShowAsThumbnailValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuShowAsThumbnailValid");

    return ((frame->IsRoot (ev)) ? kODFalse : kODTrue);
}

SOM_Scope ODBoolean SOMLINK MenuShowAsFrameValid (jmBaseNonContainer *somSelf, Environment *ev, ODFrame *frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"MenuShowAsFrameValid");

    return ((frame->IsRoot (ev)) ? kODFalse : kODTrue);
}


/*
    Drag and Drop

        When the user drags over the part, the DragEnter/Within/Leave methods determine the
        appropriate UI display for the user. If a drop operation is valid, the part should be
        highlighted appropriately (this implementation places an inverted area around the inside
        perimeter of the part).

        This implementation draws directly to the HPS during this operation, though calling the
        Draw method is a preferable technique (Draw manages the appropriate state management for
        highlighting the part). Draw also allows more flexibility in managing parts that may have
        drop areas within the part, as it likely contains the information necessary for current
        location and shape for each of the candidate drop areas.
*/

/*
    When the mouse pointer is dragged over the part, the perimeter of the part is highlighted
    with a thick line to indicate to the user that this is a valid drop target. If no drop occurs
    and the mouse leaves the area over the part, the highlight is removed.

    Implementation: Display thick line around perimeter of part.
    Return        : None.
    Subclassing   : Private method, may not be subclassed.
    Notes         : None.
*/

void InvertDragHighlight (Environment *ev, ODFacet *facet)
{
   #if defined (_PLATFORM_OS2_)
     HPS hps;

     CFocusWindow f (ev, facet, (ODShape *) kODNULL, &hps, (HWND *) kODNULL, CFocusWindow::DragPS);

     ODTempPolygon  poly;
     POLYGON        polygon;
     ODContour     *pContour;
     LONG           lCtr;
     ODShape    *clipShape = facet->AcquireAggregateClipShape (ev, kODNULL);

     clipShape->CopyPolygon (ev, &poly);

     GpiSetMix (hps, FM_INVERT);
     GpiBeginPath (hps, 1);
     for (pContour = poly.FirstContour (), lCtr = 0;
          lCtr < poly.GetNContours();
          pContour = pContour->NextContour (), lCtr ++)
      {
         pContour->AsPOLYGON (polygon);
         GpiMove (hps, &polygon.aPointl[polygon.ulPoints-1]);
         GpiPolyLine (hps, polygon.ulPoints, polygon.aPointl);
         SOMFree (polygon.aPointl);
      }

     GpiEndPath (hps);
     GpiSetLineWidthGeom (hps, 8);
     GpiStrokePath (hps, 1, 0);

     clipShape->Release (ev);
   #endif
}


/*
    When the mouse enters the space over the part during a drag operation, this method is called.

    Implementation: If this part is a valid drop target, drag highlight the part.
    Return        : Flags indicating drop target status.
    Subclassing   : Developer design (to change behaviour).
    Notes         : Implement DropValid method to indicate drop validity.
*/

SOM_Scope ODDragResult  SOMLINK DragEnter(jmBaseNonContainer *somSelf,  Environment *ev,
                                     ODDragItemIterator* dragInfo,
                                     ODFacet* facet, ODPoint* where)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DragEnter");

    ODDragResult result;

    if (somSelf->DropValid (ev, dragInfo) == kODTrue)
     {
        InvertDragHighlight (ev, facet);
        _fDragHighlight = kODTrue;
        #if defined (_PLATFORM_OS2_)
          result = MRFROM2SHORT (DOR_DROP, DO_MOVE);
        #elif defined (_PLATFORM_WIN32_)
          result = kODTrue;
        #endif
     }
    else
     {
        _fDragHighlight = kODFalse;
        #if defined (_PLATFORM_OS2_)
          result = MRFROM2SHORT (DOR_NEVERDROP, DO_UNKNOWN);
        #elif defined (_PLATFORM_WIN32_)
          result = kODFalse;
        #endif
     }

    return (result);
}


/*
    When the mouse moves within the part space during a drag operation, this method is
    called to determine the validity of a drop operation at the particular point.

    Implementation: Check validity of this point is a valid drop target.
    Return        : Flags indicating drop target status.
    Subclassing   : Developer design (to change behaviour).
    Notes         : Implement DropValid method to indicate drop validity.
*/

SOM_Scope ODDragResult  SOMLINK DragWithin(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODDragItemIterator* dragInfo,
                                      ODFacet* facet, ODPoint* where)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DragWithin");

    ODDragResult result;

    #if defined (_PLATFORM_OS2_)
      result = (_fDragHighlight == kODTrue) ? MRFROM2SHORT (DOR_DROP, DO_MOVE) : MRFROM2SHORT (DOR_NEVERDROP, DO_UNKNOWN);
    #elif defined (_PLATFORM_WIN32_)
      result = (_fDragHighlight == kODTrue) ? kODTrue : kODFalse;
    #endif

    return (result);
}


/*
    When the user leaves the part space during a drag operation, this method is called.

    Implementation: Remove the highlight marking.
    Return        : None.
    Subclassing   : Developer design.
    Notes         : None.
*/

SOM_Scope void  SOMLINK DragLeave(jmBaseNonContainer *somSelf,  Environment *ev,
                                  ODFacet* facet, ODPoint* where)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DragLeave");

    if (_fDragHighlight == kODTrue)
     {
        InvertDragHighlight (ev, facet);
        _fDragHighlight = kODFalse;
     }
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK DropCompleted(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODPart* destPart, ODDropResult dropResult)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"DropCompleted");

    return;
}


/*
    When transferring data, it may be preferable not to place the data into the storage unit
    before it is required by another part. Examples include data objects that are large or have
    many representations. When the target part requests the data, this method will be invoked
    to populate the storage unit with the promised data.

    Implementation: No implementation.
    Return        : None.
    Subclassing   : Must be subclassed to provide function according to developer design.
    Notes         : Setup required in clipboard, drag/drop or linking methods.
*/

SOM_Scope void  SOMLINK FulfillPromise(jmBaseNonContainer *somSelf,  Environment *ev,
                                       ODStorageUnitView* promiseSUView)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"FulfillPromise");

    return;
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODDropResult  SOMLINK Drop(jmBaseNonContainer *somSelf,  Environment *ev,
                                     ODDragItemIterator* dropInfo,
                                     ODFacet* facet, ODPoint* where)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"Drop");

    // Remove drag highlighting around part
    if (_fDragHighlight == kODTrue)
     {
        InvertDragHighlight (ev, facet);
        _fDragHighlight = kODFalse;
     }

    ODStorageUnit     *dropSU;
    ODDropResult       dropResult = kODDropMove;
    ODStorageUnitView *dropView;
    ODStorageUnit     *renderedSU;

    dropSU = dropInfo->First (ev);
    while (dropSU != kODNULL)
     {
        // verify dragitem value in storage unit
        #if defined (_PLATFORM_OS2_)
          ODValueType vt = kODDragitem;
        #elif defined (_PLATFORM_WIN32_)
          ODValueType vt = kODWeakStorageUnitRef;
        #endif

        if (dropSU->Exists (ev, kODPropContents, vt, 0) == kODTrue)
         {
            // get the dragitem
            dropSU->Focus (ev, kODPropContents, kODPosUndefined, vt, 0, kODPosUndefined);
            ODStorageUnitView *dropView = dropSU->CreateView (ev);

            #if defined (_PLATFORM_OS2_)
            if (_session->GetDragAndDrop (ev)->GetDataFromDragManager (ev, dropView, &renderedSU) == kODTrue)
                dropResult = somSelf->ReceiveDropStorageUnit (ev, renderedSU, facet, where);
            #endif
         }

        dropSU = dropInfo->Next (ev);
     }

    facet->GetFrame (ev)->Invalidate (ev, kODNULL, kODNULL);
    return (dropResult);
}


/*
    Implementation: Adds link to list
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODLinkSource*  SOMLINK CreateLink(jmBaseNonContainer *somSelf,  Environment *ev,
                                      ODByteArray *data)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"CreateLink");

    ODLinkSource *linkSource;

    // This implementation supports multiple targets, but only one source data type
    if (_linkSources->Count (ev) == 0)
     {
        linkSource = somSelf->GetStorageUnit (ev)->GetDraft (ev)->CreateLinkSource (ev, somSelf);
        _linkSources->Add (ev, linkSource);

        jmBaseIterator *iterator = _frames->CreateIterator (ev);
        ODFrame *frame = (ODFrame *) iterator->First (ev);
        while (frame != NULL)
         {
           frame->ChangeLinkStatus (ev, kODInLinkSource);
           frame = (ODFrame *) iterator->Next (ev);
         }
        delete iterator;
     }
    else
     {
        jmBaseIterator *iterator = _linkSources->CreateIterator (ev);
        linkSource = (ODLinkSource *) iterator->First (ev);
        delete iterator;
     }

    return (linkSource);
}


SOM_Scope void  SOMLINK UpdateLink(jmBaseNonContainer *somSelf,  Environment *ev)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"UpdateLink");

    ODLinkSource *linkSource;
    ODBoolean     fLocked = kODFalse;
    ODLinkKey     linkKey;

    if (_linkSources->Count (ev) > 0)
     {
        jmBaseIterator *iterator = _linkSources->CreateIterator (ev);
        linkSource = (ODLinkSource *) iterator->First (ev);
        delete iterator;

        while (fLocked == kODFalse)
         {
            if (linkSource->Lock (ev, 0, &linkKey))
             {
                fLocked = kODTrue;
                try
                 {
                    linkSource->Clear (ev, _session->UniqueUpdateID (ev), linkKey);
                    ODStorageUnit *su = linkSource->GetContentStorageUnit (ev, linkKey);

                    somSelf->PopulateLinkStorageUnit (ev, su);

                    linkSource->ContentUpdated (ev, _session->UniqueUpdateID (ev), linkKey);
                    linkSource->Unlock (ev, linkKey);
                 }
                catch (...)
                 {
                    linkSource->Clear (ev, _session->UniqueUpdateID (ev), linkKey);
                    linkSource->Unlock (ev, linkKey);
                 }
             }
            else
             {
                #if defined (_PLATFORM_OS2_)
                    DosSleep (1000);
                #elif defined (_PLATFORM_WIN32_)
                    Sleep (1000);
                #endif
             }
         }
     }
}

/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK LinkUpdated(jmBaseNonContainer *somSelf,  Environment *ev,
                                    ODLink* updatedLink, ODUpdateID id)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"LinkUpdated");

    ODLinkKey  linkKey;
    ODLink    *link = updatedLink;

    if (link->Lock (ev, 0, &linkKey))
     {
        ODStorageUnit *su = link->GetContentStorageUnit (ev, linkKey);
        somSelf->ReceiveLinkStorageUnit (ev, su);

        link->Unlock (ev, linkKey);
     }
}


/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope void  SOMLINK RevealLink(jmBaseNonContainer *somSelf,  Environment *ev,
                                   ODLinkSource* linkSource)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"RevealLink");

    return;
}

#if defined (_PLATFORM_OS2_)
SOM_Scope void  SOMLINK BreakLink(jmBaseNonContainer *somSelf, Environment *ev, ODLink* link)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"BreakLink");

    jmBaseList *links = somSelf->_get_linkTargets (ev);
    if (links->Contains (ev, link))
     {
        links->Remove (ev, link);
        link->UnregisterDependent (ev, somSelf);
        link->Release (ev);

        somSelf->SetContentChanged (ev);
     }
}

SOM_Scope void  SOMLINK BreakLinkSource(jmBaseNonContainer *somSelf, Environment *ev, ODLinkSource* linkSource)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"BreakLinkSource");

    jmBaseList *links = somSelf->_get_linkSources (ev);
    if (links->Contains (ev, linkSource))
     {
        links->Remove (ev, linkSource);
        linkSource->SetSourcePart (ev, kODNULL);
        linkSource->Release (ev);

        somSelf->SetContentChanged (ev);
     }
}

SOM_Scope void  SOMLINK LinkBroken(jmBaseNonContainer *somSelf, Environment *ev, ODLink* link)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"LinkBroken");

    jmBaseList *links = somSelf->_get_linkTargets (ev);
    if (links->Contains (ev, link))
     {
        links->Remove (ev, link);
        link->UnregisterDependent (ev, somSelf);
        link->Release (ev);

        somSelf->SetContentChanged (ev);
     }
}

SOM_Scope void  SOMLINK UpdateFromLinkSource(jmBaseNonContainer *somSelf, Environment *ev, ODLinkSource* linkSource)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"UpdateFromLinkSource");

    return;
}

SOM_Scope void  SOMLINK ShowLink(jmBaseNonContainer *somSelf, Environment *ev, ODLink* link)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ShowLink");

    return;
}
#endif

// Section: Extension handling

SOM_Scope ODExtension* SOMLINK AcquireExtension(jmBaseNonContainer *somSelf, Environment *ev, ODType extensionName)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"AcquireExtension");

    return (kODNULL);
}

SOM_Scope void SOMLINK ReleaseExtension(jmBaseNonContainer *somSelf, Environment *ev, ODExtension *extension)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"ReleaseExtension");

}

SOM_Scope ODBoolean SOMLINK HasExtension(jmBaseNonContainer *somSelf, Environment *ev, ODType extensionName)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"HasExtension");

    return (jmBaseNonContainer_parent_ODPart_HasExtension (somSelf, ev, extensionName));
}

SOM_Scope ODULong SOMLINK GetPrintResolution(jmBaseNonContainer *somSelf, Environment *ev, ODFrame* frame)
{
    jmBaseNonContainerData *somThis = jmBaseNonContainerGetData(somSelf);
    jmBaseNonContainerMethodDebug(kDebugPart,"GetPrintResolution");

    return (0);
}


/*
    The following methods are defined in the metaclass, and are called during
    the part registration process.
*/

// Returning a pointer to the name of the class/part handler name.
SOM_Scope ISOString  SOMLINK clsGetODPartHandlerName(M_jmBaseNonContainer *somSelf,
                                                     Environment *ev)
{
    /* M_jmBaseNonContainerData *somThis = M_jmBaseNonContainerGetData(somSelf); */
    M_jmBaseNonContainerMethodDebug(kDebugPartM,"clsGetODPartHandlerName");

    ISOString handlerName = (ISOString) SOMMalloc (strlen (kPartHandlerName) + 1);
    strcpy (handlerName, kPartHandlerName);
    return (handlerName);
}


// Return the plain text name to be displayed as the name of the part
SOM_Scope string  SOMLINK clsGetODPartHandlerDisplayName(M_jmBaseNonContainer *somSelf,
                                                         Environment *ev)
{
    /* M_jmBaseNonContainerData *somThis = M_jmBaseNonContainerGetData(somSelf); */
    M_jmBaseNonContainerMethodDebug(kDebugPartM,"clsGetODPartHandlerDisplayName");

    string displayName = (string) SOMMalloc (strlen (kDisplayName) + 1);
    strcpy (displayName, kDisplayName);
    return (displayName);
}


// Return part kind information including list of supported part kinds and
// operating system file associations
SOM_Scope _IDL_SEQUENCE_PartKindInfo  SOMLINK clsGetODPartKinds(M_jmBaseNonContainer *somSelf,
                                                                             Environment *ev)
{
    /* M_jmBaseNonContainerData *somThis = M_jmBaseNonContainerGetData(somSelf); */
    M_jmBaseNonContainerMethodDebug(kDebugPartM,"clsGetODPartKinds");

    _IDL_SEQUENCE_PartKindInfo kindInfo;

    // Create structure PartKindInfo  and allocate memory for variable
    PartKindInfo *info = (PartKindInfo *) SOMMalloc (sizeof (PartKindInfo));
    info->partKindName = (ISOString) SOMMalloc (strlen (kKind) + 1);
    info->partKindDisplayName = (string) SOMMalloc (strlen (kKindDisplayName) + 1);
    info->filenameFilters =  (string) SOMMalloc (strlen ("") + 1);
    info->filenameTypes =  (string) SOMMalloc (strlen ("") + 1);
    info->categories =  (string) SOMMalloc (strlen (kCategory) + 1);
    info->categoryDisplayName =  (string) SOMMalloc (strlen (kCategoryDisplayName) + 1);
    info->objectID =  (string) SOMMalloc (strlen ("") + 1);

    // Copy the information into the structure
    strcpy (info->partKindName , kKind);
    strcpy (info->partKindDisplayName, kKindDisplayName);
    strcpy (info->filenameFilters, "");
    strcpy (info->filenameTypes, "");
    strcpy (info->categories, kCategory);
    strcpy (info->categoryDisplayName, kCategoryDisplayName);
    strcpy (info->objectID, "");

    kindInfo._maximum = 1;
    kindInfo._length = 1;
    kindInfo._buffer = info;

    return (kindInfo);
}


// Return OLE 2 class id, used for OpenDoc/OLE interoperability support
SOM_Scope string  SOMLINK clsGetOLE2ClassId(M_jmBaseNonContainer *somSelf,
                                                         Environment *ev)
{
    /* M_jmBaseNonContainerData *somThis = M_jmBaseNonContainerGetData(somSelf); */
    M_jmBaseNonContainerMethodDebug(kDebugPartM,"clsGetOLE2ClassId");

    string classID = (string) SOMMalloc (1);
    classID[0] = '\0';
    return (classID);
}


// Return the file name of the icon to use for the part handler
SOM_Scope string  SOMLINK clsGetWindowsIconFileName(M_jmBaseNonContainer *somSelf,
                                                                 Environment *ev)
{
    /* M_jmBaseNonContainerData *somThis = M_jmBaseNonContainerGetData(somSelf); */
    M_jmBaseNonContainerMethodDebug(kDebugPartM,"clsGetWindowsIconFileName");

    string fileName = (string) SOMMalloc (1);
    fileName[0] = '\0';
    return (fileName);
}


