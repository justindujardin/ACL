//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/threads/semaphore.h"
#include "core/assert.h"
#include "platform/impls/base/threads/semaphoreImpl.h"
#include "platform/platform.h"

#ifdef ACL_DEBUG_THREADING
#include "platform/threads/threadLocal.h"
#endif

namespace ACLib {
namespace Platform {
/// @cond
struct Semaphore::Internal {
  ScopedPtr<Internal_::SemaphoreImpl> impl;
  S32 maxCount;
  bool valid;

  Internal()
      : impl(GetPlatform()->getFactory().create<Internal_::SemaphoreImpl>()),
        maxCount(-1), valid(false) {}
};
/// @endcond

Semaphore::Semaphore(S32 initialCount, S32 maxCount) : mImpl(new Internal) {
  AssertFatal(maxCount > 0, "Invalid maxCount 0");
  AssertFatal(initialCount <= maxCount, "Invalid initialCount > maxCount");
  AssertFatal(initialCount >= 0, "Invalid initialCount < 0");

  if (maxCount <= 0)
    maxCount = S32_MAX;

  if (initialCount > maxCount || initialCount < 0)
    initialCount = maxCount;

  mImpl->maxCount = maxCount;
  mImpl->valid = mImpl->impl->init(initialCount, maxCount);
  AssertFatal(mImpl->valid, "SemaphoreImpl init failed");
}

Semaphore::~Semaphore() {}

Threading::Status Semaphore::acquire(bool block) {
  if (!mImpl->valid) {
    AssertFatal(false, "SemaphoreImpl is invalid, cannot acquire");
    return Threading::Status_ObjectInvalid;
  }

  return mImpl->impl->acquire(block);
}

Threading::Status Semaphore::release() {
  if (!mImpl->valid) {
    AssertFatal(false, "SemaphoreImpl is invalid, cannot release");
    return Threading::Status_ObjectInvalid;
  }

  return mImpl->impl->release();
}
} // namespace Platform
} // namespace ACLib