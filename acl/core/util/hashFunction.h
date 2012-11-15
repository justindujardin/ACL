//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _HASHFUNCTION_H_
#define _HASHFUNCTION_H_

#ifndef _ACL_TYPES_H_
#include "core/types.h"
#endif

namespace ACLib
{

   extern U32 hashPointer(register const U8 *k, register U32 length, register U32 initval);
   extern U64 hashPointer64(register const U8 *k, register U32 length, register U64 initval);

}

#endif // _HASHFUNCTION_H_
