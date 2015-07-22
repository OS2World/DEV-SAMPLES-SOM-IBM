#define INCL_ODAPI

#if defined (_PLATFORM_OS2_)
  #include <os2.h>
#elif defined (_PLATFORM_WIN32_)
  #include <od.h>
#endif

#include <baselist.xh>
#include <baseinfo.xh>
#include <bfiter.xh>
#include <image.xh>
#include <basenc.xh>
#include <basec.xh>

// DLL initialization function, list of classes included in this DLL
#ifdef __IBMC__
  #pragma linkage (SOMInitModule, system)
#endif

SOMEXTERN void SOMLINK BaseNonContainer_SOMInitModule (long majorVersion, long minorVersion,
                                 string className)
{
   jmBaseListNewClass (jmBaseList_MajorVersion, jmBaseList_MinorVersion);
   jmBaseFrameInfoNewClass (jmBaseFrameInfo_MajorVersion, jmBaseFrameInfo_MinorVersion);
   jmBaseEmbeddedFramesIteratorNewClass (jmBaseEmbeddedFramesIterator_MajorVersion, jmBaseEmbeddedFramesIterator_MinorVersion);
   jmBaseImageNewClass (jmBaseImage_MajorVersion, jmBaseImage_MinorVersion);
   jmBaseNonContainerNewClass (jmBaseNonContainer_MajorVersion, jmBaseNonContainer_MinorVersion);
   jmBaseContainerNewClass (jmBaseContainer_MajorVersion, jmBaseContainer_MinorVersion);
}
