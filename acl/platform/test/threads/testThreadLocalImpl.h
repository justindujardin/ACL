//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_TEST_THREADLOCALIMPL_H_
#define TORQUE_PLATFORM2_TEST_THREADLOCALIMPL_H_

#include "platform2/impls/base/threads/threadLocalImpl.h"

namespace Platform2
{
namespace Internal_
{
   class TestThreadLocalImpl : public ThreadLocalImpl
   {
   public:
      void* data;
      
      TestThreadLocalImpl() : data(0)
      {
      }
      
      virtual void* get()
      {
         return data;
      }
      
      virtual void set(void* value)
      {
         data = value;
      }
   };
}
}

#endif