//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_THREADLOCALIMPL_H_
#define TORQUE_PLATFORM2_POSIX_THREADLOCALIMPL_H_

#include "platform2/impls/base/threads/threadLocalImpl.h"
#include <pthread.h>

namespace Platform2
{
namespace Internal_
{
   class PosixThreadLocalImpl : public ThreadLocalImpl
   {
   public:      
      PosixThreadLocalImpl();
      virtual ~PosixThreadLocalImpl();
      virtual void* get();
      virtual void set(void* value);
      
   private:
      pthread_key_t mKey;
   };
}
}

#endif
