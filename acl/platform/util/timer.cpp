//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/util/timer.h"
#include "platform/platform.h"

namespace Platform2
{
   Timer::Timer() : mStartTime(0)
   {
      reset();
   }

   Timer::~Timer()
   {
   }

   U32 Timer::getElapsedMs()
   {
      return GetPlatform()->getRealMilliseconds() - mStartTime;
   }

   void Timer::reset()
   {
      mStartTime = GetPlatform()->getRealMilliseconds();
   }
}