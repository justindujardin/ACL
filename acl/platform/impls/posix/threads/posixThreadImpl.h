//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_THREADIMPL_H_
#define ACL_PLATFORM_POSIX_THREADIMPL_H_

#include "platform/impls/base/threads/threadImpl.h"
#include <pthread.h>

namespace ACLib {
namespace Platform {
namespace Internal_ {
class PosixThreadImpl : public ThreadImpl {
public:
  ~PosixThreadImpl();
  virtual Threading::Status start(Param *p);

private:
  pthread_t mThread;
  S32 mCreationErr;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
