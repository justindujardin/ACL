//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixSemaphoreImpl.h"
#include <errno.h>

namespace Platform2
{
   namespace Internal_
   {
      PosixSemaphoreImpl::PosixSemaphoreImpl() : mMaxCount(S32_MAX)
      {
      }

      PosixSemaphoreImpl::~PosixSemaphoreImpl()
      {
         sem_destroy(&mSemaphore);
      }

      bool PosixSemaphoreImpl::init(S32 initialCount, S32 maxCount)
      {
         mMaxCount = maxCount;
         S32 ret = sem_init(&mSemaphore,0,initialCount);
         return ret == 0;
      }

      Threading::Status PosixSemaphoreImpl::acquire(bool block)
      {
         S32 ret = block ? sem_wait(&mSemaphore) : sem_trywait(&mSemaphore);
         if(ret == -1 && errno == EAGAIN)
            return Threading::Status_Resources;
         else if (ret == 0)
            return Threading::Status_NoError;
         return Threading::Status_PlatformError;
      }

      Threading::Status PosixSemaphoreImpl::release()
      {
         S32 count;
         if(sem_getvalue(&mSemaphore, &count) == -1)
            return Threading::Status_PlatformError;
         if(count == mMaxCount)
            return Threading::Status_Resources;

         S32 ret = sem_post(&mSemaphore);
         switch(ret)
         {
         case 0:
            return Threading::Status_NoError;
         default:
            return Threading::Status_PlatformError;
         }
      }
   }
}
