//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <CoreServices/CoreServices.h>

#include "platform/impls/mac/threads/macSemaphoreImpl.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
MacSemaphoreImpl::MacSemaphoreImpl() : mSemaphore(0), mCreationErr(noErr) {}

MacSemaphoreImpl::~MacSemaphoreImpl() {
  if (mSemaphore)
    MPDeleteSemaphore(mSemaphore);
}

bool MacSemaphoreImpl::init(S32 initialCount, S32 maxCount) {
  mCreationErr = MPCreateSemaphore(maxCount, initialCount, &mSemaphore);
  return mCreationErr == noErr;
}

Threading::Status MacSemaphoreImpl::acquire(bool block) {
  Duration timeout = block ? kDurationForever : kDurationImmediate;
  OSStatus success = MPWaitOnSemaphore(mSemaphore, timeout);
  switch (success) {
  case noErr:
    return Threading::Status_NoError;
  case kMPTimeoutErr:
    return Threading::Status_Busy;
  default:
    return Threading::Status_PlatformError;
  }
}

Threading::Status MacSemaphoreImpl::release() {
  OSStatus success = MPSignalSemaphore(mSemaphore);
  switch (success) {
  case noErr:
    return Threading::Status_NoError;
  case kMPInsufficientResourcesErr:
    return Threading::Status_Resources;
  default:
    return Threading::Status_PlatformError;
  }
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib