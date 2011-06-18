//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <CoreServices/CoreServices.h>

#include "platform2/impls/mac/threads/macMutexImpl.h"

namespace Platform2
{
namespace Internal_
{
   MacMutexImpl::MacMutexImpl() : mSemaphore(0)
   {
   }
   
   MacMutexImpl::~MacMutexImpl()
   {
      MPDeleteSemaphore(mSemaphore);
   }
   
   bool MacMutexImpl::init()
   {
      return MPCreateBinarySemaphore(&mSemaphore) == noErr;
   }
   
   Threading::Status MacMutexImpl::lock(bool block)
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
   
   Threading::Status MacMutexImpl::unlock()
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