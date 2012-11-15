//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/util/timer.h"
#include "platform/platform.h"

namespace ACLib
{
   namespace Platform
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
}