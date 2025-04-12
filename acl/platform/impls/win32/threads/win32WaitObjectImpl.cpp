//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/threads/win32WaitObjectImpl.h"
#include "core/assert.h"
#include "core/intrinsics.h"
#include "platform/threads/threadingStatus.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
Win32WaitObjectImpl::Win32WaitObjectImpl() : mWaitQueue(0) {
  mSemaphore = CreateSemaphore(NULL, 0, S32_MAX, NULL);
}

Win32WaitObjectImpl::~Win32WaitObjectImpl() { CloseHandle(mSemaphore); }

Threading::Status Win32WaitObjectImpl::wait(S32 timeout /*= -1*/) {
  DWORD milli = timeout > 0 ? timeout : INFINITE;
  dFetchAndAdd(mWaitQueue, 1);
  DWORD ret = ::WaitForSingleObject(mSemaphore, milli);
  dFetchAndAdd(mWaitQueue, -1);
  switch (ret) {
  case WAIT_OBJECT_0:
    return Threading::Status_WaitSignaled;
  case WAIT_TIMEOUT:
    return Threading::Status_WaitTimeout;
  }
  return Threading::Status_PlatformError;
}

void Win32WaitObjectImpl::signalAll() {
  S32 all = mWaitQueue;
  dFetchAndAdd(mWaitQueue, -all);
  ReleaseSemaphore(mSemaphore, all, NULL);
}

void Win32WaitObjectImpl::signalOne() { ReleaseSemaphore(mSemaphore, 1, NULL); }
} // namespace Internal_
} // namespace Platform
} // namespace ACLib