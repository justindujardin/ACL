//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/base/platformCreator.h"
#include "platform/impls/base/threads/threadImpl.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
void InitializePlatform() { ThreadImpl::RegisterCurrentThreadAsMain(); }

TypeRebind &GetPlatformFactory() {
  static TypeRebind factory;
  return factory;
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib