//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_THREADLOCALIMPL_H_
#define ACL_PLATFORM_WIN32_THREADLOCALIMPL_H_

#include "platform/impls/base/threads/threadLocalImpl.h"
#include <Windows.h>

namespace ACLib {
namespace Platform {
namespace Internal_ {
class Win32ThreadLocalImpl : public ThreadLocalImpl {
public:
  Win32ThreadLocalImpl();
  virtual ~Win32ThreadLocalImpl();
  virtual void *get();
  virtual void set(void *value);

private:
  DWORD mTLSIndex;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
