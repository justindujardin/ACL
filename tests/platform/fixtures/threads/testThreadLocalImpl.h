//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_THREADLOCALIMPL_H_
#define ACL_PLATFORM_TEST_THREADLOCALIMPL_H_

#include "platform/impls/base/threads/threadLocalImpl.h"

using namespace Platform2;
using namespace Platform2::Internal_;

namespace TestPlatform
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

#endif
