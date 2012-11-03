//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_SEMAPHOREIMPL_H_
#define ACL_PLATFORM_POSIX_SEMAPHOREIMPL_H_

#include <semaphore.h>
#include "platform/impls/base/threads/semaphoreImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class PosixSemaphoreImpl : public SemaphoreImpl
      {
      public:
         PosixSemaphoreImpl();
         virtual ~PosixSemaphoreImpl();
         virtual bool init(U32 initialCount, U32 maxCount);
         virtual Threading::Status acquire(bool block);
         virtual Threading::Status release();

      private:
         sem_t mSemaphore;
         int mMaxCount;
      };
   }
}

#endif
