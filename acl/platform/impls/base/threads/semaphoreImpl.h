//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_SEMAPHOREIMPL_H_
#define ACL_PLATFORM_SEMAPHOREIMPL_H_

#include "core/types.h"
#include "core/util/noncopyable.h"
#include "platform/threads/threadingStatus.h"

namespace Platform2
{
   namespace Internal_
   {
      /// @ingroup p2threadimpl
      /// Base class for you platform's Semaphore implementation.
      class SemaphoreImpl : private Noncopyable
      {
      public:
         virtual ~SemaphoreImpl() {}

         /// You should initialize your semaphore such that it has count initialCount
         /// and attempting to release it beyond maxCount results in an error.
         virtual bool init(U32 initialCount, U32 maxCount) = 0;

         /// This should decrement the count of your semaphore.
         /// @param block If @c true, your semaphore should block if its count is
         /// already 0.  If @c false, your semaphore should immediately return 
         /// Threading::Status_Busy if its count is already 0.
         /// @returns Threading::Status_NoError if the semaphore was successfully acquired.
         /// @returns Threading::Status_Busy if the semaphore's count was already 0
         /// and block is @c false.
         /// @returns Threading::Status_PlatformError if the semaphore could not be
         /// acquired for any other reason.
         virtual Threading::Status acquire(bool block) = 0;

         /// This should increment the count of your semaphore.
         /// @returns Threading::Status_NoError if the semaphore was successfully released.
         /// @returns Threading::Status_Resources if releasing the semaphore would
         /// increment its count beyond the maxCount given to the constructor.
         /// @returns Threading::Status_PlatformError if the semaphore could not be
         /// released for any other reason.
         virtual Threading::Status release() = 0;
      };
   }
}

#endif
