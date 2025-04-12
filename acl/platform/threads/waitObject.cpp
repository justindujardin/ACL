//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/threads/waitObject.h"
#include "platform/impls/base/threads/waitObjectImpl.h"
#include "platform/platform.h"

namespace ACLib {
namespace Platform {
/// @cond
struct WaitObject::Internal {
  ScopedPtr<Internal_::WaitObjectImpl> impl;

  Internal()
      : impl(GetPlatform()->getFactory().create<Internal_::WaitObjectImpl>()) {}
};
/// @endcond

WaitObject::WaitObject() : mImpl(new Internal) {}

WaitObject::~WaitObject() {}

Threading::Status WaitObject::wait(S32 timeout) {
  return mImpl->impl->wait(timeout);
}

void WaitObject::signalAll() { mImpl->impl->signalAll(); }

void WaitObject::signalOne() { mImpl->impl->signalOne(); }
} // namespace Platform
} // namespace ACLib