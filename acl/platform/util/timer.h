//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TIMER_H_
#define ACL_PLATFORM_TIMER_H_

#include "core/types.h"

namespace ACLib {
namespace Platform {
class Timer {
public:
  Timer();
  ~Timer();
  U32 getElapsedMs();
  void reset();

private:
  U32 mStartTime;
};
} // namespace Platform
} // namespace ACLib
#endif
