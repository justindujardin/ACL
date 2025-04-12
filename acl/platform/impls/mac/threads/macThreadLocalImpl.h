//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_THREADLOCALIMPL_H_
#define ACL_PLATFORM_MAC_THREADLOCALIMPL_H_

#include "platform/impls/base/threads/threadLocalImpl.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
class MacThreadLocalImpl : public ThreadLocalImpl {
public:
  MacThreadLocalImpl();
  virtual ~MacThreadLocalImpl();
  virtual void *get();
  virtual void set(void *value);

private:
  TaskStorageIndex mTaskIndex;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
