//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_MAC_THREADLOCALIMPL_H_
#define TORQUE_PLATFORM2_MAC_THREADLOCALIMPL_H_

#include "platform2/impls/base/threads/threadLocalImpl.h"

namespace Platform2
{
namespace Internal_
{
   class MacThreadLocalImpl : public ThreadLocalImpl
   {
   public:      
      MacThreadLocalImpl();
      virtual ~MacThreadLocalImpl();
      virtual void* get();
      virtual void set(void* value);
      
   private:
      TaskStorageIndex mTaskIndex;
   };
}
}

#endif