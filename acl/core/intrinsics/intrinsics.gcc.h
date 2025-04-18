//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _ACL_INTRINSICS_GCC_H_
#define _ACL_INTRINSICS_GCC_H_

/// @file
/// Compiler intrinsics for GCC.

#ifdef ACL_OS_MAC
#include <libkern/OSAtomic.h>
#endif

namespace ACLib {
// Fetch-And-Add

inline void dFetchAndAdd(volatile U32 &ref, U32 val) {
#ifndef ACL_OS_MAC
  __sync_fetch_and_add((volatile long *)&ref, val);
#else
  OSAtomicAdd32(val, (int32_t *)&ref);
#endif
}
inline void dFetchAndAdd(volatile S32 &ref, S32 val) {
#ifndef ACL_OS_MAC
  __sync_fetch_and_add((volatile long *)&ref, val);
#else
  OSAtomicAdd32(val, (int32_t *)&ref);
#endif
}

// Compare-And-Swap

inline bool dCompareAndSwap(volatile U32 &ref, U32 oldVal, U32 newVal) {
#ifndef ACL_OS_MAC
  return (__sync_val_compare_and_swap((volatile long *)&ref, newVal, oldVal) ==
          oldVal);
#else
  return OSAtomicCompareAndSwap32(oldVal, newVal, (int32_t *)&ref);
#endif
}
inline bool dCompareAndSwap(volatile U64 &ref, U64 oldVal, U64 newVal) {
#ifndef ACL_OS_MAC
  return (__sync_val_compare_and_swap((volatile long long *)&ref, newVal,
                                      oldVal) == oldVal);
#else
  return OSAtomicCompareAndSwap64(oldVal, newVal, (int64_t *)&ref);
#endif
}
};     // namespace ACLib
#endif // _ACL_INTRINSICS_GCC_H_
