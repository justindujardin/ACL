//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MUTEXIMPL_H_
#define ACL_PLATFORM_MUTEXIMPL_H_

#include "core/util/noncopyable.h"
#include "platform/threads/threadingStatus.h"

namespace Platform2
{
   namespace Internal_
   {
      /// @ingroup p2threadimpl
      /// Base class for your platform Mutex implementation.
      class MutexImpl : private Noncopyable
      {
      public:
         virtual ~MutexImpl() {}

         /// You should create your mutex when this is called.
         /// @returns true if the mutex was successfully created.
         /// @returns false if the mutex could not be created for any reason.
         virtual bool init() = 0;

         /// Attempt to lock the mutex.
         /// @param block If @c true your implementation should block on the mutex.
         /// If @c false your implementation should immediately return if the mutex
         /// could not be locked.
         /// @returns Threading::Status_NoError if the mutex was successfully locked.
         /// @returns Threading::Status_Busy if the mutex was already locked and block
         /// was @c false.
         /// @returns Threading::Status_Deadlock if your implementation detected a deadlock
         /// condition. @note This return code is @e optional and the Platform2::Mutex
         /// class provides its own deadlock detection when ACL_DEBUG_THREADING is defined.
         /// @returns Threading::Status_PlatformError if the mutex could not be locked
         /// for any other reason.
         virtual Threading::Status lock(bool block) = 0;

         /// Attempt to unlock the mutex.
         /// @returns Threading::Status_NoError if the mutex was successfully unlocked.
         /// @returns Threading::Status_Permission if the mutex is not owned by the 
         /// calling thread. @note This return code is @e optional and the Platform2::Mutex
         /// class provides its own ownership check when ACL_DEBUG_THREADING is defined.
         /// @returns Threading::Status_PlatformError if the mutex could not be unlocked
         /// for any other reason.
         virtual Threading::Status unlock() = 0;
      };
   }
}

#endif
