//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/delegate.h"
#include "platform/platform.h"
#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "platform/threads/waitObject.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;


/// Verifies that locking a mutex twice results in the second lock blocking the calling
/// thread until the first thread unlocks the mutex.
namespace MutexBlock
{
   static Mutex m;
   static WaitObject wait;
   S32 lock(Thread::MessageQueue& messageQueue)
   {
      GetPlatform()->sleep(500);
      wait.signalOne();
      EXPECT_TRUE(m.lock(true) == Threading::Status_NoError);//, "Failed a blocking lock");
      return 0;
   }
   TEST(Mutex, Blocking)
   {
      EXPECT_TRUE(m.lock(true) == Threading::Status_NoError);//, "Failed to lock unlocked mutex");
      Thread t(MakeDelegate(&lock));
      t.start();
      EXPECT_TRUE(wait.wait(1500) == Threading::Status_WaitSignaled);
      EXPECT_TRUE(m.unlock() == Threading::Status_NoError);
      t.finish();
      EXPECT_TRUE(t.getReturnCode() == 0);
   }
};

/// Verifies that an attempt to lock a locked mutex returns false if block is false.
namespace MutexNonBlock
{
   static Mutex m;
   static WaitObject w;

   S32 lock(Thread::MessageQueue& messageQueue)
   {
      // Cannot non-blocking lock the mutex
      EXPECT_TRUE(m.lock(false) == Threading::Status_Busy);
      return 0;
   }

   TEST(Mutex, NonBlocking)
   {
      EXPECT_TRUE(m.lock(true) == Threading::Status_NoError);
      Thread t(MakeDelegate(&lock));
      t.start();
      t.finish();
      m.unlock();
   }
};
