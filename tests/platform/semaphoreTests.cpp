//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------


#include "core/strings/str.h"
#include "core/util/delegate.h"
#include "platform/platform.h"
#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "platform/threads/semaphore.h"
#include "platform/threads/waitObject.h"
#include "core/assert.h"
#include "platform/platform.h"
#include "core/util/autoPtr.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;

/// Verifies that when a semaphore of count 0 is acquired the calling thread is blocked
/// until the semaphores count is > 0.
namespace ThreadSemaphoreBlock
{
   static AutoPtr<Semaphore> semaphore;
   static WaitObject wait;
   static Mutex mutex;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      wait.signalOne();
      U32 start = GetPlatform()->getRealMilliseconds();
      EXPECT_TRUE(semaphore->acquire(true) == Threading::Status_NoError);//, "Failed to acquire blocking semaphore with count > 0");
      EXPECT_TRUE(semaphore->acquire(true) == Threading::Status_NoError);//, "Failed to acquire blocking semaphore");
      return GetPlatform()->getRealMilliseconds() - start; 
   }

   TEST(Semaphore, Blocking)
   {
      semaphore = new Semaphore(2);
      Thread t(MakeDelegate(&work));
      semaphore->acquire(true);
      t.start();
      wait.wait(&mutex);
      GetPlatform()->sleep(100);
      semaphore->release();
      t.finish();
      EXPECT_TRUE(t.getReturnCode() >= 100);//, "Semaphore wasn't blocking");
   }
};

/// Verifies that an attempt to acquire a semaphore of count 0 does *not* block and returns false
/// if block is false.
namespace ThreadsSemaphoreNonBlock
{
   static AutoPtr<Semaphore> semaphore;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      bool result = semaphore->acquire(false) == Threading::Status_Busy;
      EXPECT_TRUE(result);//, "Succeeded in non-blocking acquire of semaphore with count 0");
      return result ? 0 : -1; 
   }

   TEST(Semaphore, NonBlock)
   {
      semaphore = new Semaphore(0);
      Thread t(MakeDelegate(&work));
      t.start();
      t.finish();
      semaphore->release();
      EXPECT_TRUE(t.getReturnCode() == 0);//, "Semaphore was blocking");
   }
};

TEST(Semaphore, Release)
{
   AutoPtr<Semaphore> semaphore(new Semaphore(1, 3));
   EXPECT_TRUE(semaphore->release() == Threading::Status_NoError);
   EXPECT_TRUE(semaphore->release() == Threading::Status_NoError);
   EXPECT_TRUE(semaphore->release() == Threading::Status_Resources);//, 
   //"Expected that releasing semaphore beyond maxCount would return Status_Resources");
};