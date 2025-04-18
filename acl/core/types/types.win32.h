//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _TYPESWIN32_H_
#define _TYPESWIN32_H_

#define FN_CDECL __cdecl ///< Calling convention
#define STDCALL __stdcall

// size_t is needed to overload new
// size_t tends to be OS and compiler specific and may need to
// be if/def'ed in the future
typedef unsigned int dsize_t;

/// Platform dependent file date-time structure.  The definition of this
/// structure will likely be different for each OS platform.
struct FileTime {
  U32 v1;
  U32 v2;
};

#ifndef NULL
#define NULL 0
#endif

#endif //_TYPESWIN32_H_
