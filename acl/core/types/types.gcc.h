//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _TYPESGCC_H
#define _TYPESGCC_H


// For additional information on GCC predefined macros
// http://gcc.gnu.org/onlinedocs/gcc-3.0.2/cpp.html


//--------------------------------------
// Types
typedef signed long long    S64;
typedef unsigned long long  U64;


//--------------------------------------
// Compiler Version
#define ACL_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)


//--------------------------------------
// Identify the compiler string

#if defined(__MINGW32__)
#  define ACL_COMPILER_STRING "GCC (MinGW)"
#  define ACL_COMPILER_MINGW
#elif defined(__CYGWIN__)
#  define ACL_COMPILER_STRING "GCC (Cygwin)"
#  define ACL_COMPILER_MINGW
#else
#  define ACL_COMPILER_STRING "GCC "
#endif


//--------------------------------------
// Identify the Operating System
#if defined(__WIN32__) || defined(_WIN32)
#  define ACL_OS_STRING "Win32"
#  define ACL_OS_WIN32
#  define ACL_SUPPORTS_NASM
#  define ACL_SUPPORTS_GCC_INLINE_X86_ASM
#  include "./types.win32.h"

#elif defined(linux)
#  define ACL_OS_STRING "Linux"
#  define ACL_OS_LINUX
#  define ACL_SUPPORTS_NASM
#  define ACL_SUPPORTS_GCC_INLINE_X86_ASM
#  include "./types.posix.h"

#elif defined(__OpenBSD__)
#  define ACL_OS_STRING "OpenBSD"
#  define ACL_OS_OPENBSD
#  define ACL_SUPPORTS_NASM
#  define ACL_SUPPORTS_GCC_INLINE_X86_ASM
#  include "./types.posix.h"

#elif defined(__FreeBSD__)
#  define ACL_OS_STRING "FreeBSD"
#  define ACL_OS_FREEBSD
#  define ACL_SUPPORTS_NASM
#  define ACL_SUPPORTS_GCC_INLINE_X86_ASM
#  include "./types.posix.h"

#elif defined(__APPLE__)
#  define ACL_OS_STRING "MacOS X"
#  define ACL_OS_MAC
#  include "./types.mac.h"
#  if defined(i386)
#     define ACL_SUPPORTS_NASM
#  endif
#else 
#  error "GCC: Unsupported Operating System"
#endif


//--------------------------------------
// Identify the CPU
#if defined(i386)
#  define ACL_CPU_STRING "Intel x86"
#  define ACL_CPU_X86
#  define ACL_LITTLE_ENDIAN
#  define ACL_DEBUG_BREAK asm("int $3");

#elif defined(__ppc__)
#  define ACL_CPU_STRING "PowerPC"
#  define ACL_CPU_PPC
#  define ACL_BIG_ENDIAN
#  define ACL_DEBUG_BREAK asm{trap}

#else
#  error "GCC: Unsupported Target CPU"
#endif

#ifndef Offset
/// Offset macro:
/// Calculates the location in memory of a given member x of class cls from the
/// start of the class.  Need several definitions to account for various
/// flavors of GCC.

// now, for each compiler type, define the Offset macros that should be used.
// The Engine code usually uses the Offset macro, but OffsetNonConst is needed
// when a variable is used in the indexing of the member field (see
// TSShapeConstructor::initPersistFields for an example)

// compiler is non-GCC, or gcc < 3
#if (__GNUC__ < 3)
#define Offset(x, cls) _Offset_Normal(x, cls)
#define OffsetNonConst(x, cls) _Offset_Normal(x, cls)

// compiler is GCC 3 with minor version less than 4
#elif defined(ACL_COMPILER_GCC) && (__GNUC__ == 3) && (__GNUC_MINOR__ < 4)
#define Offset(x, cls) _Offset_Variant_1(x, cls)
#define OffsetNonConst(x, cls) _Offset_Variant_1(x, cls)

// compiler is GCC 3 with minor version greater than 4
#elif defined(ACL_COMPILER_GCC) && (__GNUC__ == 3) && (__GNUC_MINOR__ >= 4)
#include <stddef.h>
#define Offset(x, cls) _Offset_Variant_2(x, cls)
#define OffsetNonConst(x, cls) _Offset_Variant_1(x, cls)

// compiler is GCC 4
#elif defined(ACL_COMPILER_GCC) && (__GNUC__ == 4)
#include <stddef.h>
#define Offset(x, cls) _Offset_Normal(x, cls)
#define OffsetNonConst(x, cls) _Offset_Variant_1(x, cls)

#endif
#endif

#define ACL_PRETTY_FUNCTION __PRETTY_FUNCTION__
#define ACL_NO_INLINE __attribute__((__noinline__))


#endif // INCLUDED_TYPES_GCC_H

