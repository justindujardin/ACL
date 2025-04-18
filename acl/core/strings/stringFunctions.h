//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _STRINGFUNCTIONS_H_
#define _STRINGFUNCTIONS_H_

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#ifndef _ACL_TYPES_H_
#include "core/types.h"
#endif

#if defined(ACL_OS_WIN32) || defined(ACL_OS_XBOX) || defined(ACL_OS_XENON)
// These standard functions are not defined on Win32 and other Microsoft
// platforms...
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtof (float)strtod
#define dT(s) L##s
#else
#define dT(s) s
#endif

namespace ACLib {

//------------------------------------------------------------------------------
// standard string functions [defined in platformString.cpp]

inline char *dStrcat(char *dst, const char *src) { return strcat(dst, src); }

inline char *dStrncat(char *dst, const char *src, dsize_t len) {
  return strncat(dst, src, len);
}

inline int dStrcmp(const char *str1, const char *str2) {
  return strcmp(str1, str2);
}

inline int dStrncmp(const char *str1, const char *str2, dsize_t len) {
  return strncmp(str1, str2, len);
}

inline int dStricmp(const char *str1, const char *str2) {
  return strcasecmp(str1, str2);
}

inline int dStrnicmp(const char *str1, const char *str2, dsize_t len) {
  return strncasecmp(str1, str2, len);
}

inline char *dStrcpy(char *dst, const char *src) { return strcpy(dst, src); }

inline char *dStrncpy(char *dst, const char *src, dsize_t len) {
  return strncpy(dst, src, len);
}

inline dsize_t dStrlen(const char *str) { return (dsize_t)strlen(str); }

inline char *dStrchr(char *str, int c) { return strchr(str, c); }

inline const char *dStrchr(const char *str, int c) { return strchr(str, c); }

inline char *dStrrchr(char *str, int c) { return strrchr(str, c); }

inline const char *dStrrchr(const char *str, int c) { return strrchr(str, c); }

inline dsize_t dStrspn(const char *str, const char *set) {
  return (dsize_t)strspn(str, set);
}

inline dsize_t dStrcspn(const char *str, const char *set) {
  return (dsize_t)strcspn(str, set);
}

inline char *dStrstr(const char *str1, const char *str2) {
  return strstr((char *)str1, str2);
}

inline char *dStrtok(char *str, const char *sep) { return strtok(str, sep); }

inline S32 dAtoi(const char *str) { return strtol(str, NULL, 10); }

inline U32 dAtoui(const char *str, U32 base = 10) {
  return strtoul(str, NULL, base);
}

inline F32 dAtof(const char *str) { return strtof(str, NULL); }

inline char dToupper(const char c) { return toupper(c); }

inline char dTolower(const char c) { return tolower(c); }

inline bool dIsalnum(const char c) { return isalnum(c); }

inline bool dIsalpha(const char c) { return isalpha(c); }

inline bool dIsspace(const char c) { return isspace(c); }

inline bool dIsdigit(const char c) { return isdigit(c); }

//------------------------------------------------------------------------------
// non-standard string functions [defined in stringFunctions.cpp]

#define dStrdup(x) dStrdup_r(x, __FILE__, __LINE__)
extern char *dStrdup_r(const char *src, const char *, dsize_t);

extern char *dStrcpyl(char *dst, dsize_t dstSize, ...);
extern char *dStrcatl(char *dst, dsize_t dstSize, ...);

extern char *dStrupr(char *str);
extern char *dStrlwr(char *str);

extern int dStrcmp(const UTF16 *str1, const UTF16 *str2);
extern int dStrnatcmp(const char *str1, const char *str2);
extern int dStrnatcasecmp(const char *str1, const char *str2);

inline bool dAtob(const char *str) {
  return !dStricmp(str, "true") || dAtof(str);
}

bool dStrEqual(const char *str1, const char *str2);

bool dStrStartsWith(const char *str1, const char *str2);

bool dStrEndsWith(const char *str1, const char *str2);

char *dStripPath(const char *filename);

//------------------------------------------------------------------------------
// standard I/O functions [defined in stringFunctions.cpp]

extern void dPrintf(const char *format, ...);
extern int dVprintf(const char *format, void *arglist);
extern int dSprintf(char *buffer, U32 bufferSize, const char *format, ...);
extern int dVsprintf(char *buffer, U32 bufferSize, const char *format,
                     void *arglist);
extern int dSscanf(const char *buffer, const char *format, ...);
}; // namespace ACLib
#endif
