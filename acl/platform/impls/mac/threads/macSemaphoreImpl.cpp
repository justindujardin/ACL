//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include <CoreServices/CoreServices.h>

#include "platform/impls/mac/threads/macSemaphoreImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      MacSemaphoreImpl::MacSemaphoreImpl() : mSemaphore(0), mCreationErr(noErr)
      {
      }

      MacSemaphoreImpl::~MacSemaphoreImpl()
      {
         if(mSemaphore)
            MPDeleteSemaphore(mSemaphore);
      }

      bool MacSemaphoreImpl::init(U32 initialCount, U32 maxCount)
      {
         mCreationErr = MPCreateSemaphore(maxCount, initialCount, &mSemaphore);
         return mCreationErr == noErr;
      }

      Threading::Status MacSemaphoreImpl::acquire(bool block)
      {
         Duration timeout = block ? kDurationForever : kDurationImmediate;
         OSStatus success = MPWaitOnSemaphore(mSemaphore, timeout);
         switch(success)
         {
         case noErr:
            return Threading::Status_NoError;
         case kMPTimeoutErr:
            return Threading::Status_Busy;
         default:
            return Threading::Status_PlatformError;
         }
      }

      Threading::Status MacSemaphoreImpl::release()
      {
         OSStatus success = MPSignalSemaphore(mSemaphore);
         switch(success)
         {
         case noErr:
            return Threading::Status_NoError;
         case kMPInsufficientResourcesErr:
            return Threading::Status_Resources;
         default:
            return Threading::Status_PlatformError;
         }
      }
   }
}