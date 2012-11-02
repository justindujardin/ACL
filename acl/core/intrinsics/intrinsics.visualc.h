//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _ACL_INTRINSICS_VISUALC_H_
#define _ACL_INTRINSICS_VISUALC_H_

/// @file
/// Compiler intrinsics for Visual C++.

#if defined(ACL_OS_XENON)
#  include <Xtl.h>
#  define _InterlockedExchangeAdd InterlockedExchangeAdd
#  define _InterlockedExchangeAdd64 InterlockedExchangeAdd64
#else
#	include <intrin.h>
#endif

// Fetch-And-Add

inline void dFetchAndAdd( volatile U32& ref, U32 val )
{
   _InterlockedExchangeAdd( ( volatile long* ) &ref, val );
}
inline void dFetchAndAdd( volatile S32& ref, S32 val )
{
   _InterlockedExchangeAdd( ( volatile long* ) &ref, val );
}

#if defined(ACL_OS_XENON)
// Not available on x86
inline void dFetchAndAdd( volatile U64& ref, U64 val )
{
   _InterlockedExchangeAdd64( ( volatile __int64* ) &ref, val );
}
#endif

// Compare-And-Swap

inline bool dCompareAndSwap( volatile U32& ref, U32 oldVal, U32 newVal )
{
   return ( _InterlockedCompareExchange( ( volatile long* ) &ref, newVal, oldVal ) == oldVal );
}
inline bool dCompareAndSwap( volatile U64& ref, U64 oldVal, U64 newVal )
{
   return ( _InterlockedCompareExchange64( ( volatile __int64* ) &ref, newVal, oldVal ) == oldVal );
}

#endif // _ACL_INTRINSICS_VISUALC_H_
