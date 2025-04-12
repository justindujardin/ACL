//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _CORE_STDHEADERS_H_
#define _CORE_STDHEADERS_H_

// System headers
#include <new>
#include <stdlib.h>
#include <string.h>

// ACLib Core headers
#ifndef _ACL_TYPES_H_
#include "core/types.h"
#endif

#ifndef _CORE_MEMORYFUNCTIONS_H_
#include "core/memoryFunctions.h"
#endif

#ifndef ACL_CORE_ASSERT_H_
#include "core/assert.h"
#endif

namespace ACLib {
//------------------------------------------------------------------------------
// Misc StdLib functions
#define QSORT_CALLBACK FN_CDECL
inline void dQsort(void *base, U32 nelem, U32 width,
                   int(QSORT_CALLBACK *fcmp)(const void *, const void *)) {
  qsort(base, nelem, width, fcmp);
}
}; // namespace ACLib
#endif
