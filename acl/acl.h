//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _CORE_STDHEADERS_H_
#define _CORE_STDHEADERS_H_

// System headers
#include <stdlib.h>
#include <string.h>
#include <new>

// ACLib Core headers
#ifndef _ACL_TYPES_H_
#include "core/types/types.h"
#endif

#ifndef _CORE_MEMORYFUNCTIONS_H_
#include "core/memoryFunctions.h"
#endif

#ifndef ACL_CORE_ASSERT_H_
#include "core/assert.h"
#endif

//------------------------------------------------------------------------------
// Misc StdLib functions
#define QSORT_CALLBACK FN_CDECL
inline void dQsort(void *base, U32 nelem, U32 width, int (QSORT_CALLBACK *fcmp)(const void *, const void *))
{
   qsort(base, nelem, width, fcmp);
}

#endif