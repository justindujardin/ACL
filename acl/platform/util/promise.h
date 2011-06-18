//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _ACL_PLATFORM_PROMISE_H_
#define _ACL_PLATFORM_PROMISE_H_

#include "core/types/types.h"

namespace Platform2
{
   template<typename T>
   class Promise
   {
   public:
      virtual bool isReady() const = 0;
      virtual T* resolve() = 0;
      
   protected:
      virtual T* get() = 0;
   };
}

#endif