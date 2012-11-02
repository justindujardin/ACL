//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixMutexImpl.h"
#include <errno.h>

namespace Platform2
{
   namespace Internal_
   {
      PosixMutexImpl::PosixMutexImpl()
      {
      }

      PosixMutexImpl::~PosixMutexImpl()
      {
         pthread_mutex_destroy(&mMutex);
      }

      bool PosixMutexImpl::init()
      {
         return pthread_mutex_init(&mMutex, NULL) == 0;
      }

      Threading::Status PosixMutexImpl::lock(bool block)
      {
         if(!block)
         {
            S32 ret = pthread_mutex_trylock(&mMutex);
            switch(ret)
            {
            case 0:
               return Threading::Status_NoError;
            case EBUSY:
               return Threading::Status_Busy;
            default:
               return Threading::Status_PlatformError;
            }
         }
         S32 ret = pthread_mutex_lock(&mMutex);
         switch(ret)
         {
         case 0:
            return Threading::Status_NoError;
         default:
            return Threading::Status_PlatformError;
         }
      }

      Threading::Status PosixMutexImpl::unlock()
      {
         S32 ret = pthread_mutex_unlock(&mMutex);
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
