//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TIMER_H_
#define ACL_PLATFORM_TIMER_H_

#include "core/types/types.h"

namespace Platform2
{   
   class Timer
   {
   public:
      Timer();
      ~Timer();
      U32 getElapsedMs();
      void reset();

   private:
      U32 mStartTime;
   };
}

#endif