//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _TYPESVISUALC_H
#define _TYPESVISUALC_H


// For more information on VisualC++ predefined macros
// http://support.microsoft.com/default.aspx?scid=kb;EN-US;q65472

//--------------------------------------
// Types
typedef signed _int64   S64;
typedef unsigned _int64 U64;     


//--------------------------------------
// Compiler Version
#define ACL_COMPILER_VISUALC _MSC_VER

//--------------------------------------
// Identify the compiler string
#if _MSC_VER < 1200
// No support for old compilers
#  error "VC: Minimum VisualC++ 6.0 or newer required"
#else _MSC_VER >= 1200
#  define ACL_COMPILER_STRING "VisualC++"
#endif


//--------------------------------------
// Identify the Operating System
#if _XBOX_VER >= 200 
#  define ACL_OS_STRING "Xenon"
#  ifndef ACL_OS_XENON
#     define ACL_OS_XENON
#  endif
#  include "./types.xenon.h"
#elif defined( _XBOX_VER )
#  define ACL_OS_STRING "Xbox"
#  define ACL_OS_XBOX
#  include "./types.win32.h"
#elif defined(_WIN32)
#  define ACL_OS_STRING "Win32"
#  define ACL_OS_WIN32
#  include "./types.win32.h"
#else 
#  error "VC: Unsupported Operating System"
#endif


//--------------------------------------
// Identify the CPU
#if defined(_M_IX86)
#  define ACL_CPU_STRING "x86"
#  define ACL_CPU_X86
#  define ACL_LITTLE_ENDIAN
#  define ACL_SUPPORTS_NASM
#  define ACL_SUPPORTS_VC_INLINE_X86_ASM
#elif defined(ACL_OS_XENON)
#  define ACL_CPU_STRING "ppc"
#  define ACL_CPU_PPC
#  define ACL_BIG_ENDIAN
#else
#  error "VC: Unsupported Target CPU"
#endif

#ifndef FN_CDECL
#  define FN_CDECL __cdecl            ///< Calling convention
#endif

#ifndef STDCALL
#  define STDCALL __stdcall
#endif

#ifndef Offset
#define Offset(x, cls) _Offset_Normal(x, cls)
#define OffsetNonConst(x, cls) _Offset_Normal(x, cls)
#endif

#define ACL_PRETTY_FUNCTION __FUNCSIG__
#define ACL_DEBUG_BREAK __debugbreak();
#define ACL_NO_INLINE __declspec(noinline)


//--------------------------------------
// Visual C warnings that don't apply to us

// acl string functions validate len
#pragma warning(disable: 4996) 

// disable warning caused by memory layer
// see msdn.microsoft.com "Compiler Warning (level 1) C4291" for more details
#pragma warning(disable: 4291) 

// Disable various warnings about type casts and conversions that we know to be safe
#pragma warning(disable: 4800) // forcing bool to 'true' or 'false' performance warning 
#pragma warning(disable: 4311) // pointer truncation fro const void* to U32 (dictionary hash)
#pragma warning(disable: 4244) // pointer truncation fro const S64 to U32 (dictionary hash)

// Hack to work around Microsoft VC's non-C++ compliance on variable scoping
#define for if(false) {} else for   


#endif // INCLUDED_TYPES_VISUALC_H

