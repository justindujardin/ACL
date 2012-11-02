//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _CORE_MEMORYFUNCTIONS_H_
#define _CORE_MEMORYFUNCTIONS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <new>
#include "core/types.h"

#if defined( ACL_OS_WIN32 ) || defined( ACL_OS_XBOX ) || defined( ACL_OS_XENON )
#include <malloc.h>
#endif

// Always use aligned memory allocation
#if defined( ACL_OS_WIN32 ) || defined( ACL_OS_XBOX ) || defined( ACL_OS_XENON )
#define _MALLOC(size)      _aligned_malloc(size, 16)
#define _REALLOC(ptr,size) _aligned_realloc(ptr,size, 16)
#define _FREE(ptr)         _aligned_free(ptr)
#elif defined( ACL_OS_MAC )
// Mac OS X always aligns malloc'd memory on 16-byte boundaries, so just use normal malloc routines
//    <http://developer.apple.com/technotes/tn2005/tn2130.html#TNTAG6>
#define _MALLOC(size)      malloc(size)
#define _REALLOC(ptr,size) realloc(ptr,size)
#define _FREE(ptr)         free(ptr)
#else
#error "Check aligned memory allocation on your platform"
#endif

inline void* dMalloc(dsize_t in_size)
{
   return _MALLOC( in_size );
}

inline void* dRealloc(void* in_pResize, dsize_t in_size)
{
   return _REALLOC( in_pResize, in_size );
}

inline void dFree(void* in_pFree)
{
   _FREE( in_pFree );
}

inline void* dMemcpy(void *dst, const void *src, dsize_t size)
{
   return memcpy(dst,src,size);
}   

inline void* dMemmove(void *dst, const void *src, dsize_t size)
{
   return memmove(dst,src,size);
}  

inline void* dMemset(void *dst, int c, dsize_t size)
{
   return memset(dst,c,size);   
}   

inline int dMemcmp(const void *ptr1, const void *ptr2, dsize_t len)
{
   return(memcmp(ptr1, ptr2, len));
}
///@defgroup ObjTrickery Object Management Trickery
///
/// These functions are to construct and destruct objects in memory
/// without causing a free or malloc call to occur. This is so that
/// we don't have to worry about allocating, say, space for a hundred
/// NetAddresses with a single malloc call, calling delete on a single
/// NetAdress, and having it try to free memory out from under us.
///
/// @{

/// Constructs an object that already has memory allocated for it.
template <class T>
inline T* constructInPlace(T* p)
{
   return new(p) T;
}

/// Copy constructs an object that already has memory allocated for it.
template <class T>
inline T* constructInPlace(T* p, const T& copy)
{
   return new(p) T(copy);
}

template <class T, class T2> inline T* constructInPlace(T* ptr, const T2& t2)
{
   return new(ptr) T(t2);
}

/// Destructs an object without freeing the memory associated with it.
template <class T>
inline void destructInPlace(T* p)
{
   p->~T();
}

/// @}

#endif
