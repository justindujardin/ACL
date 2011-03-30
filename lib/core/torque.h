//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _CORE_STDHEADERS_H_
#define _CORE_STDHEADERS_H_

// System headers
#include <stdlib.h>
#include <string.h>
#include <new>

// TorqueCore headers
#ifndef _TORQUE_TYPES_H_
#include "./types/types.h"
#endif

#ifndef _CORE_MEMORYFUNCTIONS_H_
#include "./memoryFunctions.h"
#endif

#ifndef TORQUE_CORE_ASSERT_H_
#include "./assert.h"
#endif

//------------------------------------------------------------------------------
// Misc StdLib functions
#define QSORT_CALLBACK FN_CDECL
inline void dQsort(void *base, U32 nelem, U32 width, int (QSORT_CALLBACK *fcmp)(const void *, const void *))
{
   qsort(base, nelem, width, fcmp);
}

#endif