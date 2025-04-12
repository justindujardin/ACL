//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _TYPESXENON_H_
#define _TYPESXENON_H_

///< Calling convention
#ifdef FN_CDECL
#undef FN_CDECL
#endif
#define FN_CDECL __cdecl

#ifdef STDCALL
#undef STDCALL
#endif
#define STDCALL __stdcall

// size_t is needed to overload new
// size_t tends to be OS and compiler specific and may need to
// be if/def'ed in the future
typedef size_t dsize_t;

struct FileTime {
  U32 v1;
  U32 v2;
};

#ifndef NULL
#define NULL (0)
#endif

#endif //_TYPESXENON_H_
