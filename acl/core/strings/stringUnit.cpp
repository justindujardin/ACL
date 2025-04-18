//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/strings/stringUnit.h"
#include "core/assert.h"
#include "core/strings/stringFunctions.h"

namespace ACLib {
namespace StringUnit {
static char _returnBuffer[1024];

const char *getUnit(const char *string, U32 index, const char *set) {
  U32 sz;
  while (index--) {
    if (!*string)
      return "";
    sz = dStrcspn(string, set);
    if (string[sz] == 0)
      return "";
    string += (sz + 1);
  }
  sz = dStrcspn(string, set);
  if (sz == 0)
    return "";

  AssertFatal(sz + 1 < sizeof(_returnBuffer),
              "Size of returned string too large for return buffer");

  char *ret = &_returnBuffer[0];
  dStrncpy(ret, string, sz);
  ret[sz] = '\0';
  return ret;
}

const char *getUnits(const char *string, S32 startIndex, S32 endIndex,
                     const char *set) {
  // [neo, 5/11/2007 - #2998]
  // Range check
  if (startIndex > endIndex)
    return "";

  S32 sz;
  S32 index = startIndex;
  while (index--) {
    if (!*string)
      return "";
    sz = dStrcspn(string, set);
    if (string[sz] == 0)
      return "";
    string += (sz + 1);
  }
  const char *startString = string;
  while (startIndex <= endIndex--) {
    sz = dStrcspn(string, set);
    string += sz;
    if (*string == 0)
      break;
    string++;
  }
  if (!*string)
    string++;

  // [neo, 5/11/2007 - #2998]
  // Another case of the U32 wrapping from 0 to boom! If totalSize < 0 it will
  // wrap to somewhere around U32 max and that is a lot for dStrncpy to overrun!
  // U32 totalSize = (U32(string - startString));
  S32 totalSize = (S32)(string - startString);

  AssertFatal(totalSize < sizeof(_returnBuffer),
              "Size of returned string too large for return buffer");

  if (totalSize > 0) {
    char *ret = &_returnBuffer[0];
    dStrncpy(ret, startString, totalSize - 1);
    ret[totalSize - 1] = '\0';

    return ret;
  }

  return "";
}

U32 getUnitCount(const char *string, const char *set) {
  U32 count = 0;
  U8 last = 0;
  while (*string) {
    last = *string++;

    for (U32 i = 0; set[i]; i++) {
      if (last == set[i]) {
        count++;
        last = 0;
        break;
      }
    }
  }
  if (last)
    count++;
  return count;
}

const char *setUnit(const char *string, U32 index, const char *replace,
                    const char *set) {
  U32 sz;
  const char *start = string;

  AssertFatal(dStrlen(string) + dStrlen(replace) + 1 < sizeof(_returnBuffer),
              "Size of returned string too large for return buffer");

  char *ret = &_returnBuffer[0];
  ret[0] = '\0';
  U32 padCount = 0;

  while (index--) {
    sz = dStrcspn(string, set);
    if (string[sz] == 0) {
      string += sz;
      padCount = index + 1;
      break;
    } else
      string += (sz + 1);
  }
  // copy first chunk
  sz = string - start;
  dStrncpy(ret, start, sz);
  for (U32 i = 0; i < padCount; i++)
    ret[sz++] = set[0];

  // replace this unit
  ret[sz] = '\0';
  dStrcat(ret, replace);

  // copy remaining chunks
  sz = dStrcspn(string, set); // skip chunk we're replacing
  if (!sz && !string[sz])
    return ret;

  string += sz;
  dStrcat(ret, string);
  return ret;
}

const char *removeUnit(const char *string, U32 index, const char *set) {
  U32 sz;
  const char *start = string;
  AssertFatal(dStrlen(string) + 1 < sizeof(_returnBuffer),
              "Size of returned string too large for return buffer");

  char *ret = &_returnBuffer[0];
  ret[0] = '\0';

  while (index--) {
    sz = dStrcspn(string, set);
    // if there was no unit out there... return the original string
    if (string[sz] == 0)
      return start;
    else
      string += (sz + 1);
  }
  // copy first chunk
  sz = string - start;
  dStrncpy(ret, start, sz);
  ret[sz] = 0;

  // copy remaining chunks
  sz = dStrcspn(string, set); // skip chunk we're removing

  if (string[sz] == 0) { // if that was the last...
    if (string != start) {
      ret[string - start - 1] = 0; // then kill any trailing delimiter
    }
    return ret; // and bail
  }

  string += sz + 1; // skip the extra field delimiter
  dStrcat(ret, string);
  return ret;
}
}; // namespace StringUnit
}; // namespace ACLib