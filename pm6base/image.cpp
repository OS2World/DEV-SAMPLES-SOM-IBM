/*
    Description: Image

    Author:      Joseph H McIntyre

    (C) COPYRIGHT International Business Machines Corp. 1996
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

#ifndef SOM_Module_image_Source
#define SOM_Module_image_Source
#endif
#define jmBaseImage_Class_Source
#define VARIABLE_MACROS

#define INCL_ODAPI

#if defined (_PLATFORM_OS2_)
  #define INCL_DOSMODULEMGR
  #define INCL_WIN
  #include <os2.h>
#elif defined (_PLATFORM_WIN32_)
  #include <od.h>
#endif

#include <image.xih>


// #define DEBUG
#ifdef DEBUG
    #undef jmBaseImageMethodDebug
    #define jmBaseImageMethodDebug(x,y) Log(x,y)

    #include <stdio.h>
    #include <string.h>

    static void Log (const char *x, const char *y)
     {
        static FILE *fp = fopen ("image.dbg", "w");
        if (fp != NULL)
         {
            fprintf (fp, "%s:%s\n", x, y);
            fflush (fp);

            if (strcmp (y, "somDestruct") == 0)
             {
                fclose (fp);
                fp = NULL;
             }
         }
     }
#endif


SOM_Scope void SOMLINK somDefaultInit (jmBaseImage *somSelf, somInitCtrl* ctrl)
{
    jmBaseImageData *somThis; /* set in BeginInitializer */
    somInitCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseImageMethodDebug ("jmBaseImage", "somDefaultInit");
    jmBaseImage_BeginInitializer_somDefaultInit;

    jmBaseImage_Init_SOMObject_somDefaultInit (somSelf, ctrl);

    _type = 0;
    _handle = 0;
    _stream._buffer = kODNULL;
}

SOM_Scope void SOMLINK somDestruct (jmBaseImage *somSelf, octet doFree, somDestructCtrl* ctrl)
{
    jmBaseImageData *somThis; /* set in BeginDestructor */
    somDestructCtrl globalCtrl;
    somBooleanVector myMask;
    jmBaseImageMethodDebug("jmBaseImage","somDestruct");
    jmBaseImage_BeginDestructor;

    if (_stream._buffer != kODNULL)
        free (_stream._buffer);

    jmBaseImage_EndDestructor;
}

// Load icon from resource file
SOM_Scope ODBoolean SOMLINK LoadIconResource (jmBaseImage *somSelf, Environment *ev, unsigned long id)
{
    jmBaseImageData *somThis = jmBaseImageGetData(somSelf);
    jmBaseImageMethodDebug ("jmBaseImage", "LoadIconResource");


    return (kODTrue);
}


// Load bitmap from resource file
SOM_Scope ODBoolean SOMLINK LoadBitmapResource (jmBaseImage *somSelf, Environment *ev, unsigned long id)
{
    jmBaseImageData *somThis = jmBaseImageGetData(somSelf);
    jmBaseImageMethodDebug ("jmBaseImage", "LoadBitmapResource");

    return (kODTrue);
}


// Load pointer from resource file
SOM_Scope ODBoolean SOMLINK LoadPointerResource (jmBaseImage *somSelf, Environment *ev, unsigned long id)
{
    jmBaseImageData *somThis = jmBaseImageGetData(somSelf);
    jmBaseImageMethodDebug ("jmBaseImage", "LoadPointerResource");

    #if defined (_PLATFORM_OS2_)
    HMODULE hmodule;
    DosQueryModuleHandle ("basenc.dll", &hmodule);

    _type = DRG_ICON;
    _handle = WinLoadPointer (HWND_DESKTOP, hmodule, id);
    #endif

    return (kODTrue);
}

SOM_Scope void SOMLINK Release (jmBaseImage *somSelf, Environment *ev)
{
    jmBaseImageData *somThis = jmBaseImageGetData(somSelf);
    jmBaseImageMethodDebug ("jmBaseImage", "LoadPointerResource");

    #if defined (_PLATFORM_OS2_)
    if (_type == DRG_ICON)
        WinDestroyPointer (_handle);
    #endif
}

