//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
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