//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/threads/waitObject.h"
#include "platform/impls/base/threads/waitObjectImpl.h"
#include "platform/platform.h"

namespace Platform2
{
   /// @cond
   struct WaitObject::Internal
   {
      ACLib::ScopedPtr<Internal_::WaitObjectImpl> impl;

      Internal() :
      impl(GetPlatform()->getFactory().create<Internal_::WaitObjectImpl>())
      {
      }
   };
   /// @endcond

   WaitObject::WaitObject() : mImpl(new Internal)
   {
   }

   WaitObject::~WaitObject()
   {
   }

   Threading::Status WaitObject::wait(Mutex *mutex, S32 timeout/*=-1*/)
   {
      return mImpl->impl->wait(mutex,timeout);
   }
  
   void WaitObject::signalAll()
   {
      mImpl->impl->signalAll();
   }
  
   void WaitObject::signalOne()
   {
      mImpl->impl->signalOne();
   }
  
}
