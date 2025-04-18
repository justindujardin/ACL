//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <stdarg.h>
#include <stdio.h>

#include "../assert.h"
#include "../memoryFunctions.h"
#include "./stringFunctions.h"

#if defined(ACL_OS_WIN32) || defined(ACL_OS_XBOX) || defined(ACL_OS_XENON)
// This standard function is not defined when compiling with VC7...
#define vsnprintf _vsnprintf
#endif

namespace ACLib {

//-----------------------------------------------------------------------------

// Original code from: http://sourcefrog.net/projects/natsort/
// Somewhat altered here.
// TODO: proper UTF8 support; currently only working for single-byte characters

/* -*- mode: c; c-file-style: "k&r" -*-

strnatcmp.c -- Perform 'natural order' comparisons of strings in C.
Copyright (C) 2000, 2004 by Martin Pool <mbp sourcefrog net>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

/* partial change history:
 *
 * 2004-10-10 mbp: Lift out character type dependencies into macros.
 *
 * Eric Sosman pointed out that ctype functions take a parameter whose
 * value must be that of an unsigned int, even on platforms that have
 * negative chars in their default char type.
 */

typedef char nat_char;

/* These are defined as macros to make it easier to adapt this code to
 * different characters types or comparison functions. */
static inline int nat_isdigit(nat_char a) { return dIsdigit(a); }

static inline int nat_isspace(nat_char a) { return dIsspace(a); }

static inline nat_char nat_toupper(nat_char a) { return dToupper(a); }

static int compare_right(const nat_char *a, const nat_char *b) {
  int bias = 0;

  /* The longest run of digits wins.  That aside, the greatest
  value wins, but we can't know that it will until we've scanned
  both numbers to know that they have the same magnitude, so we
  remember it in BIAS. */
  for (;; a++, b++) {
    if (!nat_isdigit(*a) && !nat_isdigit(*b))
      return bias;
    else if (!nat_isdigit(*a))
      return -1;
    else if (!nat_isdigit(*b))
      return +1;
    else if (*a < *b) {
      if (!bias)
        bias = -1;
    } else if (*a > *b) {
      if (!bias)
        bias = +1;
    } else if (!*a && !*b)
      return bias;
  }

  return 0;
}

static int compare_left(const nat_char *a, const nat_char *b) {
  /* Compare two left-aligned numbers: the first to have a
  different value wins. */
  for (;; a++, b++) {
    if (!nat_isdigit(*a) && !nat_isdigit(*b))
      return 0;
    else if (!nat_isdigit(*a))
      return -1;
    else if (!nat_isdigit(*b))
      return +1;
    else if (*a < *b)
      return -1;
    else if (*a > *b)
      return +1;
  }

  return 0;
}

static int strnatcmp0(const nat_char *a, const nat_char *b, int fold_case) {
  int ai, bi;
  nat_char ca, cb;
  int fractional, result;

  ai = bi = 0;
  while (1) {
    ca = a[ai];
    cb = b[bi];

    /* skip over leading spaces or zeros */
    while (nat_isspace(ca))
      ca = a[++ai];

    while (nat_isspace(cb))
      cb = b[++bi];

    /* process run of digits */
    if (nat_isdigit(ca) && nat_isdigit(cb)) {
      fractional = (ca == '0' || cb == '0');

      if (fractional) {
        if ((result = compare_left(a + ai, b + bi)) != 0)
          return result;
      } else {
        if ((result = compare_right(a + ai, b + bi)) != 0)
          return result;
      }
    }

    if (!ca && !cb) {
      /* The strings compare the same.  Perhaps the caller
      will want to call strcmp to break the tie. */
      return 0;
    }

    if (fold_case) {
      ca = nat_toupper(ca);
      cb = nat_toupper(cb);
    }

    if (ca < cb)
      return -1;
    else if (ca > cb)
      return +1;

    ++ai;
    ++bi;
  }
}

int dStrnatcmp(const nat_char *a, const nat_char *b) {
  return strnatcmp0(a, b, 0);
}

/* Compare, recognizing numeric string and ignoring case. */
int dStrnatcasecmp(const nat_char *a, const nat_char *b) {
  return strnatcmp0(a, b, 1);
}

//------------------------------------------------------------------------------
// non-standard string functions

char *dStrdup_r(const char *src, const char *fileName, dsize_t lineNumber) {
  char *buffer = (char *)dMalloc(dStrlen(src) + 1);
  dStrcpy(buffer, src);
  return buffer;
}

// concatenates a list of src's onto the end of dst
// the list of src's MUST be terminated by a NULL parameter
// dStrcatl(dst, sizeof(dst), src1, src2, NULL);
char *dStrcatl(char *dst, dsize_t dstSize, ...) {
  const char *src = NULL;
  char *p = dst;

  AssertFatal(dstSize > 0, "dStrcatl: destination size is set zero");
  dstSize--; // leave room for string termination

  // find end of dst
  while (dstSize && *p++)
    dstSize--;

  va_list args;
  va_start(args, dstSize);

  // concatenate each src to end of dst
  while ((src = va_arg(args, const char *)) != NULL) {
    while (dstSize && *src) {
      *p++ = *src++;
      dstSize--;
    }
  }

  va_end(args);

  // make sure the string is terminated
  *p = 0;

  return dst;
}

// copy a list of src's into dst
// the list of src's MUST be terminated by a NULL parameter
// dStrccpyl(dst, sizeof(dst), src1, src2, NULL);
char *dStrcpyl(char *dst, dsize_t dstSize, ...) {
  const char *src = NULL;
  char *p = dst;

  AssertFatal(dstSize > 0, "dStrcpyl: destination size is set zero");
  dstSize--; // leave room for string termination

  va_list args;
  va_start(args, dstSize);

  // concatenate each src to end of dst
  while ((src = va_arg(args, const char *)) != NULL) {
    while (dstSize && *src) {
      *p++ = *src++;
      dstSize--;
    }
  }

  va_end(args);

  // make sure the string is terminated
  *p = 0;

  return dst;
}

int dStrcmp(const UTF16 *str1, const UTF16 *str2) {
#if defined(ACL_OS_WIN32) || defined(ACL_OS_XBOX) || defined(ACL_OS_XENON)
  return wcscmp(reinterpret_cast<const wchar_t *>(str1),
                reinterpret_cast<const wchar_t *>(str2));
#else
  int ret;
  const UTF16 *a, *b;
  a = str1;
  b = str2;

  while (((ret = *a - *b) == 0) && *a && *b)
    a++, b++;

  return ret;
#endif
}

char *dStrupr(char *str) {
#if defined(ACL_OS_WIN32) || defined(ACL_OS_XBOX) || defined(ACL_OS_XENON)
  return _strupr(str);
#else
  if (str == NULL)
    return (NULL);

  char *saveStr = str;
  while (*str) {
    *str = toupper(*str);
    str++;
  }
  return saveStr;
#endif
}

char *dStrlwr(char *str) {
#if defined(ACL_OS_WIN32) || defined(ACL_OS_XBOX) || defined(ACL_OS_XENON)
  return _strlwr(str);
#else
  if (str == NULL)
    return (NULL);

  char *saveStr = str;
  while (*str) {
    *str = tolower(*str);
    str++;
  }
  return saveStr;
#endif
}

//------------------------------------------------------------------------------
// standard I/O functions

void dPrintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
}

S32 dVprintf(const char *format, void *arglist) {
  return vprintf(format, (char *)arglist);
}

S32 dSprintf(char *buffer, U32 bufferSize, const char *format, ...) {
  va_list args;
  va_start(args, format);

  S32 len = vsnprintf(buffer, bufferSize, format, args);

  AssertFatal((U32)len < bufferSize,
              "dSprintf wrote to more memory than the specified buffer size");

  return (len);
}

S32 dVsprintf(char *buffer, U32 bufferSize, const char *format, void *arglist) {
  S32 len = vsnprintf(buffer, bufferSize, format, (char *)arglist);

  AssertFatal((U32)len < bufferSize,
              "dVsprintf wrote to more memory than the specified buffer size");

  return (len);
}

S32 dSscanf(const char *buffer, const char *format, ...) {
#if defined(ACL_OS_WIN32) || defined(ACL_OS_XBOX) || defined(ACL_OS_XENON)
  va_list args;
  va_start(args, format);

  // Boy is this lame.  We have to scan through the format string, and find out
  // how many
  //  arguments there are.  We'll store them off as void*, and pass them to the
  //  sscanf function through specialized calls.  We're going to have to put a
  //  cap on the number of args that can be passed, 8 for the moment.  Sigh.
  static void *sVarArgs[20];
  U32 numArgs = 0;

  for (const char *search = format; *search != '\0'; search++) {
    if (search[0] == '%' && search[1] != '%')
      numArgs++;
  }
  AssertFatal(numArgs <= 20, "Error, too many arguments to lame implementation "
                             "of dSscanf.  Fix implmentation");

  // Ok, we have the number of arguments...
  for (U32 i = 0; i < numArgs; i++)
    sVarArgs[i] = va_arg(args, void *);
  va_end(args);

  switch (numArgs) {
  case 0:
    return 0;
  case 1:
    return sscanf(buffer, format, sVarArgs[0]);
  case 2:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1]);
  case 3:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2]);
  case 4:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3]);
  case 5:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4]);
  case 6:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5]);
  case 7:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6]);
  case 8:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7]);
  case 9:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8]);
  case 10:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9]);
  case 11:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10]);
  case 12:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11]);
  case 13:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12]);
  case 14:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13]);
  case 15:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13], sVarArgs[14]);
  case 16:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13], sVarArgs[14],
                  sVarArgs[15]);
  case 17:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13], sVarArgs[14],
                  sVarArgs[15], sVarArgs[16]);
  case 18:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13], sVarArgs[14],
                  sVarArgs[15], sVarArgs[16], sVarArgs[17]);
  case 19:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13], sVarArgs[14],
                  sVarArgs[15], sVarArgs[16], sVarArgs[17], sVarArgs[18]);
  case 20:
    return sscanf(buffer, format, sVarArgs[0], sVarArgs[1], sVarArgs[2],
                  sVarArgs[3], sVarArgs[4], sVarArgs[5], sVarArgs[6],
                  sVarArgs[7], sVarArgs[8], sVarArgs[9], sVarArgs[10],
                  sVarArgs[11], sVarArgs[12], sVarArgs[13], sVarArgs[14],
                  sVarArgs[15], sVarArgs[16], sVarArgs[17], sVarArgs[18],
                  sVarArgs[19]);
  }
  return 0;
#else
  va_list args;
  va_start(args, format);
  return vsscanf(buffer, format, args);
#endif
}

/// Safe form of dStrcmp: checks both strings for NULL before comparing
bool dStrEqual(const char *str1, const char *str2) {
  if (!str1 || !str2)
    return false;
  else
    return (dStrcmp(str1, str2) == 0);
}

/// Check if one string starts with another
bool dStrStartsWith(const char *str1, const char *str2) {
  return !dStrnicmp(str1, str2, dStrlen(str2));
}

/// Check if one string ends with another
bool dStrEndsWith(const char *str1, const char *str2) {
  const char *p = str1 + dStrlen(str1) - dStrlen(str2);
  return ((p >= str1) && !dStricmp(p, str2));
}

/// Strip the path from the input filename
char *dStripPath(const char *filename) {
  const char *itr = filename + dStrlen(filename);
  while (--itr != filename) {
    if (*itr == '/' || *itr == '\\') {
      itr++;
      break;
    }
  }
  return dStrdup(itr);
}
}; // namespace ACLib