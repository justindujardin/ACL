//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "platform/threads/threadLocal.h"
#include "platform/impls/base/threads/threadLocalImpl.h"
#include "platform/platform.h"

namespace Platform2
{
   /// @cond
   struct ThreadLocal::Internal
   {
      Torque::ScopedPtr<Internal_::ThreadLocalImpl> impl;
      
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
