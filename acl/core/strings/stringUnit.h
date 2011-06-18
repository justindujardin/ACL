//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _STRINGUNIT_H_
#define _STRINGUNIT_H_

#include "core/types/types.h"

/// These functions are used for chunking up strings by delimiter.
/// Especially useful for handling ACLibScript space-delimited fields
namespace StringUnit
{
   const char *getUnit(const char *string, U32 index, const char *set);
   const char *getUnits(const char *string, S32 startIndex, S32 endIndex, const char *set);
   U32 getUnitCount(const char *string, const char *set);
   const char* setUnit(const char *string, U32 index, const char *replace, const char *set);
   const char* removeUnit(const char *string, U32 index, const char *set);
};

#endif