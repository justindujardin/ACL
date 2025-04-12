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

namespace ACLib {

extern U32 hashPointer(const U8 *k, U32 length, U32 initval);
extern U64 hashPointer64(const U8 *k, U32 length, U64 initval);

} // namespace ACLib

#endif // _HASHFUNCTION_H_
