//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/threads/threadLocal.h"
#include "platform/impls/base/threads/threadLocalImpl.h"
#include "platform/platform.h"

namespace Platform2
{
   /// @cond
   struct ThreadLocal::Internal
   {
      ACLib::ScopedPtr<Internal_::ThreadLocalImpl> impl;

      Internal() :
      impl(GetPlatform()->getFactory().create<Internal_::ThreadLocalImpl>())
      {
      }
   };
   /// @endcond

   ThreadLocal::ThreadLocal() : mImpl(new Internal)
   {
   }

   ThreadLocal::~ThreadLocal()
   {
   }

   void* ThreadLocal::get()
   {
      return mImpl->impl->get();
   }

   void ThreadLocal::set(void* value)
   {
      mImpl->impl->set(value);
   }
}
