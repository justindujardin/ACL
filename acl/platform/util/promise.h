//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _ACL_PLATFORM_PROMISE_H_
#define _ACL_PLATFORM_PROMISE_H_

#include "core/types.h"

namespace ACLib {
namespace Platform {
template <typename T> class Promise {
public:
  virtual bool isReady() const = 0;
  virtual T *resolve() = 0;

protected:
  virtual T *get() = 0;
};
} // namespace Platform
} // namespace ACLib
#endif
