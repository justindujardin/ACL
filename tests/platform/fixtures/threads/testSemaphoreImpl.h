//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_SEMAPHOREIMPL_H_
#define ACL_PLATFORM_TEST_SEMAPHOREIMPL_H_

#include "platform/impls/base/threads/semaphoreImpl.h"

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib {
namespace TestPlatform {
class TestSemaphoreImpl : public SemaphoreImpl {
public:
  U32 initCt;
  U32 acquireCt;
  U32 releaseCt;

  S32 initialCount;
  S32 maxCount;

  bool initSuccess;
  Threading::Status acquireSuccess;
  Threading::Status releaseSuccess;

  TestSemaphoreImpl(bool semaphoreValid) {
    maxCount = initialCount = 0;
    initCt = acquireCt = releaseCt = 0;
    acquireSuccess = Threading::Status_PlatformError;
    releaseSuccess = Threading::Status_NoError;
    initSuccess = semaphoreValid;
  }

  virtual bool init(S32 _initCount, S32 _maxCount) {
    initialCount = _initCount;
    maxCount = _maxCount;
    initCt++;
    return initSuccess;
  }

  virtual Threading::Status acquire(bool block) {
    acquireCt++;
    return acquireSuccess;
  }

  virtual Threading::Status release() {
    releaseCt++;
    return releaseSuccess;
  }
};
} // namespace TestPlatform
} // namespace ACLib
#endif
