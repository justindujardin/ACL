//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _PLATFORM_DYNAMIC_LIBRARY_H_
#define _PLATFORM_DYNAMIC_LIBRARY_H_

#include "../core/util/str.h"
#include "../core/util/refBase.h"

class DynamicLibrary : public StrongRefBase
{
public:
   ~DynamicLibrary();
   static DynamicLibrary * load(const String & path);
   void * getSymbol(const String & name);
private:
   DynamicLibrary();
   DynamicLibrary(void * handle);
private:
   void * mHandle;  
};

#endif
