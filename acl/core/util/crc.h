//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _CRC_H_
#define _CRC_H_

#ifndef _ACL_TYPES_H_
#include "core/types.h"
#endif

namespace ACLib {
class Stream;
namespace CRC {
/// Initial value for CRCs
const U32 INITIAL_CRC_VALUE = 0xFFFFFFFF;

/// Value XORd with the CRC to post condition it (ones complement)
const U32 CRC_POSTCOND_VALUE = 0xFFFFFFFF;

/// An invalid CRC
const U32 INVALID_CRC = 0xFFFFFFFF;

U32 calculateCRC(const void *buffer, S32 len, U32 crcVal = INITIAL_CRC_VALUE);
U32 calculateCRCStream(Stream *stream, U32 crcVal = INITIAL_CRC_VALUE);
} // namespace CRC
}; // namespace ACLib
#endif
