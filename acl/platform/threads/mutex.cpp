//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/threads/mutex.h"
#include "platform/impls/base/threads/mutexImpl.h"
#include "platform/platform.h"
#include "core/assert.h"

#ifdef ACL_DEBUG_THREADING
#include "platform/threads/threadLocal.h"
#endif

namespace Platform2
{
   /// @cond 
   struct Mutex::Internal
   {
      ACLib::ScopedPtr<Internal_::MutexImpl> impl;
#ifdef ACL_DEBUG_THREADING
      /// Used to track ownership of the Mutex to detect deadlock and ownership
      /// bugs.
      ThreadLocal threadLocked;
#endif
      bool isValid;
      Internal() : 
      impl(GetPlatform()->getFactory().create<Internal_::MutexImpl>()),
         isValid(false)
      {
      }
   };
   /// @endcond

   Mutex::Mutex() : mImpl(new Internal) 
   {
      mImpl->isValid = mImpl->impl->init();
      AssertFatal(mImpl->isValid, "MutexImpl creation failed");
   }

   Mutex::~Mutex()
   {
   }

   Threading::Status Mutex::lock(bool block)
   {
      if(!mImpl->isValid)
      {
         AssertFatal(false, "MutexImpl is invalid, cannot lock");
         return Threading::Status_ObjectInvalid;
      }
#ifdef ACL_DEBUG_THREADING
      // If we already own the mutex and we're trying to lock on it, and blocking
      // we'll probably deadlock.
      if(reinterpret_cast<U32>(mImpl->threadLocked.get()) == 1 && block)
      {
         AssertFatal(false, "Locking mutex twice from same thread, this will usually deadlock");
         return Threading::Status_Deadlock;
      }
#endif
      Threading::Status ret = mImpl->impl->lock(block);
#ifdef ACL_DEBUG_THREADING
      // Track that we've taken ownership of the mutex.
      mImpl->threadLocked.set(reinterpret_cast<void*>(ret == Threading::Status_NoError));
#endif
      return ret;
   }

   Threading::Status Mutex::unlock()
   {
      if(!mImpl->isValid)
      {
         AssertFatal(false, "MutexImpl is invalid, cannot unlock");
         return Threading::Status_ObjectInvalid;
      }
#ifdef ACL_DEBUG_THREADING
      // If we don't own the mutex it isn't valid for us to unlock it.
      if(reinterpret_cast<U32>(mImpl->threadLocked.get()) == 0)
      {
         AssertFatal(false, "Unlocking non-locked mutex");
         return Threading::Status_Permission;
      }
#endif
      Threading::Status ret = mImpl->impl->unlock();
#ifdef ACL_DEBUG_THREADING
      // Track that we've released ownership of the mutex.
      mImpl->threadLocked.set(reinterpret_cast<void*>(ret != Threading::Status_NoError));
#endif
      return ret;
   }
}
