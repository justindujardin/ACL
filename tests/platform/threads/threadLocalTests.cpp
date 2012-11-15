//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/delegate.h"
#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "platform/threads/waitObject.h"
#include "platform/threads/threadLocal.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace ACLib;
using namespace ACLib::Platform;


/// Verifies that a thread local stores different values for each accessing thread.
namespace ThreadLocalData
{
   static ThreadLocal l;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      l.set(reinterpret_cast<void*>(7));
      EXPECT_TRUE(reinterpret_cast<U32>(l.get()) == 7);//, "Thread local value incorrect on thread");
      return 0;
   }

   TEST(ThreadLocal,GetSet)
   {
      Thread t(MakeDelegate(&work));
      l.set(reinterpret_cast<void*>(42));
      t.start();
      t.finish();
      EXPECT_TRUE(reinterpret_cast<U32>(l.get()) == 42);//, "Thread local value incorrect");
   }
};
