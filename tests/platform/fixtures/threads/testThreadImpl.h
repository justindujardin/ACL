//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_THREADIMPL_H_
#define ACL_PLATFORM_TEST_THREADIMPL_H_

#include "platform/impls/base/threads/threadImpl.h"

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib {
namespace TestPlatform {
class TestThreadImpl : public ThreadImpl {
public:
  U32 startCt;
  Param *param;
  Threading::Status startSuccess;

  TestThreadImpl() {
    startCt = 0;
    startSuccess = Threading::Status_PlatformError;
  }

  virtual Threading::Status start(Param *p) {
    param = p;
    startCt++;
    return startSuccess;
  }

  void run() {
    // Delegate just returns, so this should immediately post
    CommonThreadEntry(param);
  }
};
} // namespace TestPlatform
} // namespace ACLib
#endif
