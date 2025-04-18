//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _TYPESPOSIX_H_
#define _TYPESPOSIX_H_

#define FN_CDECL ///< Calling convention
#define STDCALL

// size_t is needed to overload new
// size_t tends to be OS and compiler specific and may need to
// be if/def'ed in the future
typedef unsigned int dsize_t;

/** Platform dependent file date-time structure.  The defination of this
 * structure will likely be different for each OS platform.
 */
typedef S32 FileTime;

#ifndef NULL
#define NULL (0)
#endif

#endif //_TYPESPOSIX_H_
