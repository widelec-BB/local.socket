/* libvstring.h */

#ifndef __NOLIBBASE__
#define __NOLIBBASE__
#endif

#ifndef STRPTR_TYPEDEF
typedef char* STRPTR;
typedef const char* CONST_STRPTR;
#define STRPTR_TYPEDEF
#endif

#include <exec/types.h>
#include <exec/memory.h>
#include <stdarg.h>

extern struct Library *SysBase;
extern struct Library *LocaleBase;

LONG StrLen(CONST_STRPTR str);
STRPTR StrCopy(CONST_STRPTR src, STRPTR dest);
STRPTR StrNew(CONST_STRPTR str);

/* memory alloc and free functions must be a matched pair */

#define StrFree(s) FreeVecTaskPooled(s)
#define internal_alloc(l) AllocVecTaskPooled(l)
