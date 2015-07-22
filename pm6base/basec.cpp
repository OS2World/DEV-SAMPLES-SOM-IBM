/*
    Description: BaseContainer

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

#ifndef SOM_Module_basec_Source
#define SOM_Module_basec_Source
#endif
#define jmBaseContainer_Class_Source
#define M_jmBaseContainer_Class_Source


#define INCL_ODAPI
#define INCL_ODCANVAS
#define INCL_ODDRAFT
#define INCL_ODDRAGANDDROP
#define INCL_ODERRORS
#define INCL_ODFACET
#define INCL_ODFRAME
#define INCL_ODSESSION
#define INCL_ODSHAPE
#define INCL_ODSTORAGEUNIT
#define INCL_ODSTORAGEUNITVIEW
#define INCL_ODTRANSFORM

#if defined (_PLATFORM_OS2_)
  #define INCL_GPI
  #define INCL_WIN
  #include <os2.h>
#elif defined (_PLATFORM_WIN32_)
  #include <od.h>
  #include <storutil.h>
#endif

#include <stdio.h>

#define VARIABLE_MACROS
#include <basec.xih>
#include <baselist.xh>
#include <bfiter.xh>

#include <focuslib.h>


const ODType kPartHandlerName = "jmBaseContainer";
const ODType kDisplayName = "Base Container";
const ODType kKind = "jmBaseContainer";
const ODType kKindDisplayName = "Base Container";
const ODType kCategory = kODCategoryPlainText;
const ODType kCategoryDisplayName = "Plain text";

#define FIXED2LONG(f) (((f) + 0x8000) >> 16)

// #define DEBUG
#ifdef DEBUG
    #undef jmBaseContainerMethodDebug
    #undef M_jmBaseContainerMethodDebug

    #define jmBaseContainerMethodDebug(x,y) LogString(somSelf,x,y)
    #define M_jmBaseContainerMethodDebug(x,y) LogString(somSelf,x,y)

    #include <log.h>
#endif


SOM_Scope void SOMLINK somDefaultInit(jmBaseContainer *somSelf,
                                      somInitCtrl* ctrl)
{
    jmBaseContainerData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseContainerMethodDebug("jmBaseContainer","somDefaultInit");
    jmBaseContainer_BeginInitializer_somDefaultInit;

    jmBaseContainer_Init_jmBaseNonContainer_somDefaultInit(somSelf, ctrl);

    _embeddedFrames = kODNULL;
}


SOM_Scope void SOMLINK somDestruct(jmBaseContainer *somSelf, octet doFree,
                                   somDestructCtrl* ctrl)
{
    jmBaseContainerData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseContainerMethodDebug("jmBaseContainer","somDestruct");
    jmBaseContainer_BeginDestructor;

    if (_embeddedFrames != kODNULL)
        delete _embeddedFrames;

    jmBaseContainer_EndDestructor;
}

SOM_Scope string  SOMLINK PartKind(jmBaseContainer *somSelf,  Environment *ev)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","PartKind");

    return (kKind);
}


SOM_Scope void  SOMLINK InitPart(jmBaseContainer *somSelf,  Environment *ev,
                                 ODStorageUnit* storageUnit, ODPart *partWrapper)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","InitPart");

    jmBaseContainer_parent_jmBaseNonContainer_InitPart(somSelf, ev, storageUnit, partWrapper);
}

SOM_Scope void  SOMLINK InitPartFromStorage(jmBaseContainer *somSelf, Environment *ev,
                                            ODStorageUnit* storageUnit, ODPart *partWrapper)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","InitPartFromStorage");

    jmBaseContainer_parent_jmBaseNonContainer_InitPartFromStorage(somSelf, ev, storageUnit, partWrapper);
}

SOM_Scope void SOMLINK InitializePart(jmBaseContainer *somSelf, Environment *ev)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","InitializePart");

    jmBaseContainer_parent_jmBaseNonContainer_InitializePart(somSelf, ev);

    _embeddedFrames = new jmBaseList;
}

SOM_Scope void  SOMLINK InternalizeContent(jmBaseContainer *somSelf,  Environment *ev, ODStorageUnit *storageUnit)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","InternalizeContent");

    jmBaseContainer_parent_jmBaseNonContainer_InternalizeContent(somSelf, ev, storageUnit);
}

SOM_Scope void  SOMLINK ExternalizeContent(jmBaseContainer *somSelf,  Environment *ev, ODStorageUnit *storageUnit)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","ExternalizeContent");

    jmBaseContainer_parent_jmBaseNonContainer_ExternalizeContent(somSelf, ev, storageUnit);


}

SOM_Scope ODBoolean SOMLINK HandleEvent (jmBaseContainer *somSelf,  Environment *ev,
              ODEventData *event, ODFrame *frame, ODFacet *facet, ODEventInfo *eventinfo)
 {
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("jmBaseContainer","HandleEvent");

    ODBoolean handled = kODFalse;

    #if defined (_PLATFORM_OS2_)
      switch (event->msg)
    #elif defined (_PLATFORM_WIN32_)
      switch (event->message)
    #endif
     {
        default:
            break;
     }

    if (handled == kODFalse)
        handled = jmBaseContainer_parent_jmBaseNonContainer_HandleEvent (somSelf, ev, event, frame, facet, eventinfo);

    return (handled);
 }

SOM_Scope void  SOMLINK FacetAdded(jmBaseContainer *somSelf,  Environment *ev,
                                   ODFacet* facet)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"FacetAdded");

    jmBaseContainer_parent_jmBaseNonContainer_FacetAdded(somSelf, ev, facet);
}

// How to embed a part in FacetAdded method - change IBM:Launcher to the
// kind of the part you want to embed
/*
    ODFrame *frame = facet->GetFrame (ev);
    ODDraft *draft = somSelf->GetStorageUnit (ev)->GetDraft (ev);
    ODPart *part = draft->CreatePart (ev, "IBM:Launcher", kODNoEditor);
    if (part)
     {
        ODTransform *transformExternal = new ODTransform;
        ODPoint ptTemp  = {MAKEFIXED (20, 0), MAKEFIXED (20, 0)};
        transformExternal->SetOffset (ev, &ptTemp);

        ODShape *newShape = new ODShape;
        ODRect rect = { 0, MAKEFIXED (200, 0), MAKEFIXED (200, 0), 0 };
        newShape->SetRectangle (ev, &rect);

        ODFrame *newFrame = draft->CreateFrame (ev, kODFrameObject, frame, newShape, kODNULL, part,
            somSelf->_get_session (ev)->Tokenize (ev, kODViewAsFrame),
            somSelf->_get_session (ev)->Tokenize (ev, kODPresDefault), kODFalse, kODFalse);

        newFrame->Acquire (ev);
        newFrame->SetPropagateEvents (ev, kODTrue);

        ODFrameFacetIterator *facets = frame->CreateFacetIterator (ev);
        for (ODFacet *facet = facets->First (ev); facets->IsNotComplete (ev); facet = facets->Next (ev))
         {
            // ODShape *shape = new ODShape;
            // shape->CopyFrom (ev, newShape);
            // ODTransform *transform = new ODTransform;
            // transform->CopyFrom (ev, transformExternal);
            facet->CreateEmbeddedFacet (ev, newFrame, newShape, transformExternal, kODNULL, kODNULL, kODNULL, kODFrameInFront);
         }
        delete facets;

        _embeddedFrames->Add (ev, newFrame);
        newFrame->Invalidate (ev, kODNULL, kODNULL);
     }
*/

SOM_Scope ODEmbeddedFramesIterator*  SOMLINK CreateEmbeddedFramesIterator(jmBaseContainer *somSelf,  Environment *ev,
                                   ODFrame* frame)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"ODEmbeddedFramesIterator");

    jmBaseEmbeddedFramesIterator *iterator = new jmBaseEmbeddedFramesIterator;
    iterator->Initialize (ev, _embeddedFrames);

    return (iterator);
}

SOM_Scope ODShape*  SOMLINK AdjustBorderShape(jmBaseContainer *somSelf, Environment *ev, ODFacet* embeddedFacet, ODShape* shape)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug("BaseContainer","AdjustBorderShape");

    ODFacet *containingFacet = embeddedFacet->GetContainingFacet (ev);
    ODFacetIterator *facets = containingFacet->CreateFacetIterator (ev, kODChildrenOnly, kODFrontToBack);

    ODTransform *transform = embeddedFacet->AcquireFrameTransform (ev, kODNULL);
    shape->Transform (ev, transform);
    ODBoolean above = kODFalse;

    for (ODFacet* facet = facets->First (ev); (facets->IsNotComplete (ev)) && (facet != embeddedFacet); facet = facets->Next (ev))
     {
        ODShape *usedShape = facet->GetFrame (ev)->AcquireUsedShape (ev, kODNULL);
        ODTransform *facetTransform = facet->AcquireFrameTransform (ev, kODNULL);

        usedShape->Transform (ev, facetTransform);
        shape->Subtract (ev, usedShape);

        facetTransform->Release (ev);
        usedShape->Release (ev);
     }

    shape->InverseTransform (ev, transform);
    shape->Acquire (ev);

    transform->Release (ev);
    delete facets;

    return shape;
}

SOM_Scope void  SOMLINK DrawFrameView(jmBaseContainer *somSelf,  Environment *ev, ODFacet* facet, ODShape *invalidShape)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"DrawFrameView");

    #if defined (_PLATFORM_OS2_)
    HPS hps;
    CFocus f (ev, facet, invalidShape, &hps);

    ODRect rect;
    RECTL  frameRect;

    ODShape *shape = facet->GetFrame (ev)->AcquireFrameShape (ev, facet->GetCanvas (ev));
    shape->GetBoundingBox (ev, &rect);
    frameRect.xLeft = FIXED2LONG (rect.left);
    frameRect.xRight = FIXED2LONG (rect.right);
    frameRect.yTop = FIXED2LONG (rect.top);
    frameRect.yBottom = FIXED2LONG (rect.bottom);

    GpiSetColor (hps, CLR_WHITE);
    GpiBox (hps, DRO_FILL, (PPOINTL) &frameRect.xRight, 0, 0);
    #endif
}

/*
    Implementation:
    Return        :
    Subclassing   :
    Notes         :
*/

SOM_Scope ODBoolean SOMLINK DropValid (jmBaseContainer *somSelf,  Environment *ev, ODDragItemIterator *dragInfo)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"DropValid");

    return (kODTrue);
}

SOM_Scope ODDropResult SOMLINK ReceiveDropStorageUnit (jmBaseContainer *somSelf,  Environment *ev,
                           ODStorageUnit *su, ODFacet *facet, ODPoint *where)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"ReceiveDropStorageUnit");

    ODDropResult result = kODDropFail;
    ODULong      attributes = somSelf->_get_session (ev)->GetDragAndDrop (ev)->GetDragAttributes (ev);
    ODBoolean    movePart = ((attributes & kODDropIsInSourceFrame) && (attributes & kODDropIsMove)) ? kODTrue : kODFalse;

    // if creating a new part instance or moving from another application
    if (movePart == kODFalse)
     {
        ODDraft     *theDraft = somSelf->GetStorageUnit (ev)->GetDraft (ev);
        ODDraft     *fromDraft = su->GetDraft (ev);
        ODDraftKey   key;
        ODID         newPartID;
        ODCloneKind  cloneKind;

        if (attributes & kODDropIsMove)
         {
            cloneKind = kODCloneDropMove;
            result = kODDropMove;
         }
        else
         {
            cloneKind = kODCloneDropCopy;
            result = kODDropCopy;
         }

        key = fromDraft->BeginClone (ev, theDraft, facet->GetFrame (ev), cloneKind);
        newPartID = fromDraft->Clone (ev, key, su->GetID (ev), 0, 0);
        fromDraft->EndClone (ev, key);

//***        somSelf->AcquireEmbedShapeFromDrop (ev, su, facet, where);
        ODPoint newTransPt = *where;
        if (su->Exists(ev, kODPropMouseDownOffset, kODPoint, 0))
         {
            ODPoint offset;
            su->Focus (ev, kODPropMouseDownOffset, kODPosUndefined, kODPoint, 0, kODPosFirstSib);
            StorageUnitGetValue (su, ev, sizeof (offset), (ODValue) &offset);
            newTransPt.x -= offset.x;
            newTransPt.y -= offset.y;
         }

        ODTransform *newExternalTransform = facet->GetFrame (ev)->CreateTransform (ev);
        newExternalTransform->MoveBy (ev, &newTransPt);

        ODShape *newFrameShape = kODNULL;
        if (su->Exists (ev, kODPropFrameShape, NULL, 0))
         {
            su->Focus (ev, kODPropFrameShape, kODPosUndefined, NULL, 1, kODPosUndefined);
            newFrameShape = facet->CreateShape (ev);
            newFrameShape->ReadShape (ev, su);
         }

        ODPart  *newPart = theDraft->AcquirePart (ev, newPartID);

        // We should be internalizing the frame from the storage unit
        // somSelf->CreateProxyObject(ev, newFrameShape, newExternalXForm, newPart, kODFalse);

        if (newFrameShape == kODNULL)
         {
            ODRect rect = { 0, 200 << 16, 200 << 16, 0 };
            newFrameShape = facet->GetFrame (ev)->CreateShape (ev);
            newFrameShape->SetRectangle (ev, &rect);
         }

        ODFrame *containingFrame = facet->GetFrame (ev);
        ODFrame *newFrame = somSelf->GetStorageUnit(ev)->GetDraft(ev)->
           CreateFrame (ev, (containingFrame->GetStorageUnit(ev)) ? kODFrameObject : kODNonPersistentFrameObject,
                containingFrame, newFrameShape, (ODCanvas *) kODNULL, newPart,
                somSelf->_get_viewFrame (ev), somSelf->_get_session (ev)->Tokenize (ev, kODPresDefault), kODFalse, kODFalse);

        _embeddedFrames->Add (ev, newFrame);

        newFrame->SetPropagateEvents (ev, kODTrue);
        ODFrameFacetIterator *facets = containingFrame->CreateFacetIterator (ev);
        for (ODFacet* facet = facets->First (ev); facets->IsNotComplete (ev); facet = facets->Next (ev))
         {
            ODFacet* newFacet = facet->CreateEmbeddedFacet (ev, newFrame, newFrameShape, newExternalTransform,
                                           kODNULL, kODNULL, kODNULL, kODFrameInFront);

            // ODHighlight highlight = facet->GetHighlight(ev);
            // if( highlight != kODNoHighlight)
            //    newFacet->ChangeHighlight(ev, highlight);
         }

        // somSelf->ClipEmbeddedFrames (ev, containingFrame);
        newFrame->Invalidate (ev, kODNULL, kODNULL);

        newExternalTransform->Release (ev);
        newFrameShape->Release (ev);
        newFrame->Release (ev);
        newPart->Release (ev);
        if (newFrameShape != kODNULL)
            newFrameShape->Release (ev);
     }
/*
    else
     {
        ODTransform *tempTransform = facet->AcquireWindowFrameTransform (ev, kODNULL);
        ODPoint ms = *where;
        tempTransform->InvertPoint (ev, &ms);
        tempTransform->Release (ev);

        ODID    draggedFrameID;
        ODPoint delta;

        delta.x = where->x - ms.x;
        delta.y = where->y - ms.y;
        su->Focus (ev, kPropFrameInfo, kODPosUndefined, kFrameRef, 0, kODPosFirstSib);
        StorageUnitGetValue (su, ev, sizeof (ODID), (ODValue) &draggedFrameID);

        ODFrame *frame = somSelf->GetStorageUnit (ev)->GetDraft (ev)->AcquireFrame (ev, frameID);
        ODTransform *frameTransform = frame->AcquireFrameTransform (ev);
        frameTransform->MoveBy (ev, &delta);

        frameTransform->Release (ev);
        frame->Release (ev);
     }
*/
    // somSelf->InvalidateSelection (ev, _fSelectionFrame);
    // somSelf->GetStorageUnit (ev)->GetDraft (ev)->SetChangedFromPrev (ev);

    return (result);
}


SOM_Scope ODPoint* SOMLINK GetEmbedLocation (jmBaseContainer *somSelf,  Environment *ev, ODPoint *where, ODBoolean fDrop)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"DropValid");

    return (where);
}

SOM_Scope ODShape* SOMLINK GetEmbedShape (jmBaseContainer *somSelf,  Environment *ev, ODPoint *where, ODBoolean fDrop)
{
    jmBaseContainerData *somThis = jmBaseContainerGetData(somSelf);
    jmBaseContainerMethodDebug(kPartHandlerName,"DropValid");

    return ((ODShape *) kODNULL);
}


// registry methods

SOM_Scope ISOString  SOMLINK clsGetODPartHandlerName(M_jmBaseContainer *somSelf,
                                                     Environment *ev)
{
    /* M_jmBaseContainerData *somThis = M_jmBaseContainerGetData(somSelf); */
    M_jmBaseContainerMethodDebug("M_jmBaseContainer","clsGetODPartHandlerName");

    ISOString handlerName = (ISOString) SOMMalloc (strlen (kPartHandlerName) + 1);
    strcpy (handlerName, kPartHandlerName);
    return (handlerName);
}

SOM_Scope string  SOMLINK clsGetODPartHandlerDisplayName(M_jmBaseContainer *somSelf,
                                                         Environment *ev)
{
    /* M_jmBaseContainerData *somThis = M_jmBaseContainerGetData(somSelf); */
    M_jmBaseContainerMethodDebug("M_jmBaseContainer","clsGetODPartHandlerDisplayName");

    string displayName = (string) SOMMalloc (strlen (kDisplayName) + 1);
    strcpy (displayName, kDisplayName);
    return (displayName);
}

SOM_Scope _IDL_SEQUENCE_PartKindInfo  SOMLINK clsGetODPartKinds(M_jmBaseContainer *somSelf,
                                                                             Environment *ev)
{
    /* M_jmBaseContainerData *somThis = M_jmBaseContainerGetData(somSelf); */
    M_jmBaseContainerMethodDebug("M_jmBaseContainer","clsGetODPartKinds");

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

SOM_Scope string  SOMLINK clsGetOLE2ClassId(M_jmBaseContainer *somSelf,
                                                         Environment *ev)
{
    /* M_jmBaseContainerData *somThis = M_jmBaseContainerGetData(somSelf); */
    M_jmBaseContainerMethodDebug("M_jmBaseContainer","clsGetOLE2ClassId");

    string classID = (string) SOMMalloc (1);
    classID[0] = '\0';
    return (classID);
}

SOM_Scope string  SOMLINK clsGetWindowsIconFileName(M_jmBaseContainer *somSelf,
                                                                 Environment *ev)
{
    /* M_jmBaseContainerData *somThis = M_jmBaseContainerGetData(somSelf); */
    M_jmBaseContainerMethodDebug("M_jmBaseContainer","clsGetWindowsIconFileName");

    string fileName = (string) SOMMalloc (1);
    fileName[0] = '\0';
    return (fileName);
}
