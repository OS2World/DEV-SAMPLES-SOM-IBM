
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <log.h>


static void **objects = NULL;
static FILE **files = NULL;
static long   count = 0;
static long   maxcount = 0;


static FILE *GetFile (void *object)
 {
    FILE *file = NULL;

    for (long ctr = 0; ctr < count; ctr ++)
        if (objects[ctr] == object)
            file = files[ctr];

    return (file);
 }

void LogOpen (void *object, const char *pszPrefix)
 {
    FILE *file = GetFile (object);
    if (file == NULL)
     {
        if (objects == NULL)
         {
            objects = (void **) malloc (10 * sizeof (void *));
            files = (FILE **) malloc (10 * sizeof (FILE *));
            maxcount = 10;
         }

        if (count == maxcount)
         {
            maxcount += 10;
            objects = (void **) realloc (objects, maxcount * sizeof (void *));
            files = (FILE **) realloc (files, maxcount * sizeof (FILE *));
         }

        if ((objects != NULL) && (files != NULL))
         {
            char  szNamePrefix[7];
            char  szName[13];
            short sNameSuffix = 0;
            short fFinished = 0;

            strncpy (szNamePrefix, pszPrefix, 6);
            szNamePrefix[6] = '\0';

            while ((fFinished == 0) && (sNameSuffix < 99))
             {
                struct stat fileinfo;

                sprintf (szName, "%s%02d.DBG", szNamePrefix, sNameSuffix);
                if (stat (szName, &fileinfo) == -1)
                 {
                    files[count] = fopen (szName, "w");

                    if (files[count] != NULL)
                     {
                        objects[count] = object;
                        count ++;
                     }

                    fFinished = 1;
                 }
                else
                    sNameSuffix ++;
             }
         }
     }
 }

void LogString (void *object, const char *x, const char *y)
 {
    FILE *file;
    if ((file = GetFile (object)) != NULL)
     {
        fprintf (file, "%s:%s\n", x, y);
        fflush (file);
     }
 }

void LogShort (void *object, const char *x, short value)
 {
    FILE *file;
    if ((file = GetFile (object)) != NULL)
     {
        fprintf (file, "%s:%hd\n", x, value);
        fflush (file);
     }
 }

void LogUShort (void *object, const char *x, unsigned short value)
 {
    FILE *file;
    if ((file = GetFile (object)) != NULL)
     {
        fprintf (file, "%s:%hu\n", x, value);
        fflush (file);
     }
 }

void LogLong (void *object, const char *x, long value)
 {
    FILE *file;
    if ((file = GetFile (object)) != NULL)
     {
        fprintf (file, "%s:%ld\n", x, value);
        fflush (file);
     }
 }

void LogULong (void *object, const char *x, unsigned long value)
 {
    FILE *file;
    if ((file = GetFile (object)) != NULL)
     {
        fprintf (file, "%s:%lu\n", x, value);
        fflush (file);
     }
 }

void LogFixed (void *object, const char *x, unsigned long value)
 {
    FILE *file;
    if ((file = GetFile (object)) != NULL)
     {
        fprintf (file, "%s:%hu.%hu\n", x, (unsigned short)(value >> 16), (unsigned short) value);
        fflush (file);
     }
 }

