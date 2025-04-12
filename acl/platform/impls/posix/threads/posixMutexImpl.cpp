//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixMutexImpl.h"
#include <errno.h>

namespace ACLib {
namespace Platform {
namespace Internal_ {
PosixMutexImpl::PosixMutexImpl() {}

PosixMutexImpl::~PosixMutexImpl() { pthread_mutex_destroy(&mMutex); }

bool PosixMutexImpl::init() { return pthread_mutex_init(&mMutex, NULL) == 0; }

Threading::Status PosixMutexImpl::lock(bool block) {
  S32 ret =
      block ? pthread_mutex_lock(&mMutex) : pthread_mutex_trylock(&mMutex);
  switch (ret) {
  case 0:
    return Threading::Status_NoError;
  case EBUSY:
    return Threading::Status_Busy;
  }
  return Threading::Status_PlatformError;
}

Threading::Status PosixMutexImpl::unlock() {
  S32 ret = pthread_mutex_unlock(&mMutex);
  switch (ret) {
  case 0:
    return Threading::Status_NoError;
  default:
    return Threading::Status_PlatformError;
  }
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib