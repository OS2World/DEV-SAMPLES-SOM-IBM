#if defined (_PLATFORM_OS2_)
  #include <os2.h>
#elif defined (_PLATFORM_WIN32_)
  #ifndef EXPENTRY
    #include <windows.h>
  #endif
#endif

void EXPENTRY LogOpen (void *, const char *);
void EXPENTRY LogString (void *, const char *, const char *);
void EXPENTRY LogShort (void *, const char *, short);
void EXPENTRY LogUShort (void *, const char *, unsigned short);
void EXPENTRY LogLong (void *, const char *, long);
void EXPENTRY LogULong (void *, const char *, unsigned long);
void EXPENTRY LogFixed (void *, const char *, unsigned long);
