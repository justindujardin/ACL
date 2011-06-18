//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _HASHFUNCTION_H_
#define _HASHFUNCTION_H_

#ifndef _ACL_TYPES_H_
#include "core/types/types.h"
#endif

namespace ACLib
{

   extern U32 hash(register const U8 *k, register U32 length, register U32 initval);
   extern U64 hash64(register const U8 *k, register U32 length, register U64 initval);

}

#endif // _HASHFUNCTION_H_
