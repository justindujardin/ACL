//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_TYPES_LINT_H_
#define ACL_TYPES_LINT_H_

typedef signed long long S64;
typedef unsigned long long U64;

typedef unsigned int dsize_t;

struct FileTime {
  U32 v1;
  U32 v2;
};

#define ACL_OS_STRING "Lint"
#define ACL_CPU_STRING "x86"
#define ACL_LITTLE_ENDIAN
#define ACL_SUPPORTS_NASM
#define ACL_SUPPORTS_VC_INLINE_X86_ASM
#define ACL_OS_WIN32
#define ACL_COMPILER_VISUALC 1500

#ifndef FN_CDECL
#define FN_CDECL
#endif

#ifndef STDCALL
#define STDCALL
#endif

#ifndef Offset
#define Offset(x, cls) _Offset_Normal(x, cls)
#define OffsetNonConst(x, cls) _Offset_Normal(x, cls)
#endif

void lint_debugbreak() {}

#define ACL_PRETTY_FUNCTION ""
#define ACL_DEBUG_BREAK lint_debugbreak();
#define ACL_NO_INLINE __declspec(noinline)

#ifndef NULL
#define NULL 0
#endif

#endif
