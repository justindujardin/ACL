//-----------------------------------------------------------------------------
// Torque 3D
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _PLATFORMINTRINSICS_H_
#define _PLATFORMINTRINSICS_H_

#ifndef _ACL_TYPES_H_
#  include "core/types.h"
#endif

#if defined( ACL_COMPILER_VISUALC )
#  include "core/intrinsics/intrinsics.visualc.h"
#elif defined ( ACL_COMPILER_GCC )
#  include "core/intrinsics/intrinsics.gcc.h"
#else
#  error No intrinsics implemented for compiler.
#endif

//TODO: 64bit safe

template< typename T >
inline bool dCompareAndSwap( T* volatile& refPtr, T* oldPtr, T* newPtr )
{
   return dCompareAndSwap( *reinterpret_cast< volatile U32* >( &refPtr ), ( U32 ) oldPtr, ( U32 ) newPtr );
}

// Test-And-Set

inline bool dTestAndSet( volatile U32& ref )
{
   return dCompareAndSwap( ref, 0, 1 );
}
inline bool dTestAndSet( volatile U64& ref )
{
   return dCompareAndSwap( ref, 0, 1 );
}

#endif // _PLATFORMINTRINSICS_H_
