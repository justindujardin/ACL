//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixSemaphoreImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      PosixSemaphoreImpl::PosixSemaphoreImpl() 
      {
         //mSemaphore = dMalloc(sizeof(sem_t),0);
      }

      PosixSemaphoreImpl::~PosixSemaphoreImpl()
      {
         sem_destroy(&mSemaphore);
      }

      bool PosixSemaphoreImpl::init(U32 initialCount, U32 maxCount)
      {
         int result = sem_init(&mSemaphore,0,initialCount);
         return result != -1;
      }

      Threading::Status PosixSemaphoreImpl::acquire(bool block)
      {
         return Threading::Status_PlatformError;
      }

      Threading::Status PosixSemaphoreImpl::release()
      {
         return Threading::Status_PlatformError;
      }
   }
}
