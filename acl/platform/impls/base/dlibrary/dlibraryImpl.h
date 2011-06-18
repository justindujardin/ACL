//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_DLIBRARYIMPL_H_
#define ACL_PLATFORM_DLIBRARYIMPL_H_

#include "core/util/noncopyable.h"

class String;

namespace Platform2 
{ 
namespace Internal_
{
   class DLibraryImpl : private Noncopyable
   {
   public:
      virtual ~DLibraryImpl() {}
      virtual bool init(const String& file) = 0;
      virtual void* bind(const String& name) = 0;
      virtual void close() = 0;
   };
}
}

#endif