//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_SEMAPHOREIMPL_H_
#define ACL_PLATFORM_MAC_SEMAPHOREIMPL_H_

#include "platform/impls/base/threads/semaphoreImpl.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
class MacSemaphoreImpl : public SemaphoreImpl {
public:
  MacSemaphoreImpl();
  virtual ~MacSemaphoreImpl();
  virtual bool init(S32 initialCount, S32 maxCount);
  virtual Threading::Status acquire(bool block);
  virtual Threading::Status release();

private:
  MPSemaphoreID mSemaphore;
  OSStatus mCreationErr;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
