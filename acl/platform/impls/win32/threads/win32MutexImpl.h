//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_MUTEXIMPL_H_
#define ACL_PLATFORM_WIN32_MUTEXIMPL_H_

#include "platform/impls/base/threads/mutexImpl.h"
#include <Windows.h>

namespace ACLib {
namespace Platform {
namespace Internal_ {
class Win32MutexImpl : public MutexImpl {
public:
  Win32MutexImpl();
  virtual ~Win32MutexImpl();
  virtual bool init();
  virtual Threading::Status lock(bool block);
  virtual Threading::Status unlock();

private:
  HANDLE mMutex;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
