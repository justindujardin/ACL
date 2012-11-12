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
#include "platform/threads/threadLocal.h"
#include "platform/threads/waitObject.h"
#include "core/assert.h"
#include "platform/platform.h"
#include "core/util/autoPtr.h"

#include "./fixtures/platformFixture.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;
using namespace TestPlatform;

//-----------------------------------------------------------------------------
// Functional Tests
//-----------------------------------------------------------------------------

/// Basic validation of thread functionality.  Verifies that impl properly reports running state
/// that impl calls the given method, and that impl passes in userData and dataSize, and that imple
/// reports the return value of the thread.

namespace BasicUsage 
{
   bool didWork;
   static const U32 _magicNumber = 42;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      didWork = true;
      return _magicNumber;
   }  

   TEST(Thread,BasicUsage) {
      didWork = false;
      Thread t(MakeDelegate(&work));
      t.start();
      t.finish();
      GetPlatform()->sleep(10); // Give the OS a moment to start/end the tread.  We shouldn't assume this is sync.
      EXPECT_FALSE(t.isRunning()); //"Thread in a running state after finish()");
      EXPECT_TRUE(didWork); //"Thread not executed");
      EXPECT_TRUE(t.getReturnCode() == _magicNumber);//, "Thread did not return magic number");
   }
}

/// Verifies that when a thread is deleted it posts a Terminate message to the message queue and 
/// waits until that message is received and acted upon.
namespace DeleteTermination 
{
   S32 work(Thread::MessageQueue& messageQueue)
   {
      // Sleep in a tight loop.  
      // Certain implementations can only cancel at entry to certain API calls, such as sleep.
      for(U32 i = 0; i < 1000; i++)
      {
         Thread::MessageRef msg;
         if(messageQueue.waitOnMessageToThread(msg, false))
         {
            if(msg->getType() == Thread::TerminateMessage::Type)
               return 0;
         }
         GetPlatform()->sleep(1);
      }
      return 0;
   }

   TEST(Thread, DeleteTermination)
   {
      AutoPtr<Thread> t(new Thread(MakeDelegate(&work)));

      U32 time = Platform2::GetPlatform()->getRealMilliseconds();
      t->start();
      t = NULL;
      time = Platform2::GetPlatform()->getRealMilliseconds() - time;
      //, "Thread deletion did not immediately terminate the thread");
      EXPECT_TRUE(time < 1000);
   }
}

/// Verifies that Thread can post user defined messages and that threads can wait for messages to be received
namespace MessageQueuePost
{
   // This will return as soon as a message is posted
   S32 work(Thread::MessageQueue& messageQueue)
   {
      U32 start = GetPlatform()->getRealMilliseconds();
      Thread::MessageRef msg;
      messageQueue.waitOnMessageToThread(msg, true);
      return GetPlatform()->getRealMilliseconds() - start;
   }

   TEST(Thread, PostMessage)
   {
      Thread t(MakeDelegate(&work));
      t.start();
      GetPlatform()->sleep(100);
      // We don't care about the type of the message
      Thread::Message* msg = new Thread::TerminateMessage;
      t.getMessageQueue().postToThread(msg);
      t.finish();
      //, "Thread terminated early, didn't wait on message");
      EXPECT_TRUE(t.getReturnCode() >= 90);
   }
}

/// Verify that the implementation polls the thread when isRunning is called, and that given
/// a thread that will finish in a finite amount of time isRunning begins to return false in a finite amount of time.
namespace ThreadIsRunning
{
   AutoPtr<Thread> worker;
   AutoPtr<Thread> watchdog;
   bool passed;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      GetPlatform()->sleep(100);
      return 0;
   }

   S32 watch(Thread::MessageQueue& messageQueue)
   {
      worker = new Thread(MakeDelegate(&work));
      worker->start();
      while(worker->isRunning())
         GetPlatform()->sleep(1);
      return 0;
   }

   TEST(Thread, IsRunning)
   {

      watchdog = new Thread(MakeDelegate(&watch));
      passed = true;

      watchdog->start();
      GetPlatform()->sleep(200);
      //, "Thread polled never finished");
      EXPECT_FALSE(watchdog->isRunning());
      watchdog = NULL;
      worker = NULL;
   }
}

//-----------------------------------------------------------------------------
// Behavioral Tests
//-----------------------------------------------------------------------------
// These are commented out, because they have been ported, but are still failing.
#if 0
namespace ThreadBehaviorStartCallsImplStart
{
   AutoPtr<PlatformFixture> fixture;   

   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }

   TEST(ThreadBehavior, StartCallsImplStart)
   {
      fixture = new PlatformFixture;
      Thread t(MakeDelegate(&dummy));

      // Expect that thread constructor won't call ThreadImpl::start
      EXPECT_TRUE(fixture->threadImpl->startCt == 0);
      t.start();
      // Expect that Thread::start() will call ThreadImpl::start()
      EXPECT_TRUE(fixture->threadImpl->startCt == 1);
   }
};

namespace ThreadBehaviorStartParrotsImplStart
{
   AutoPtr<PlatformFixture> fixture;

   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }

   TEST(ThreadBehavior, StartParrotsImplStart)
   {
      fixture = new PlatformFixture;
      Thread::StartDelegate d(MakeDelegate(&dummy));
      Thread t(d);

      fixture->threadImpl->startSuccess = Threading::Status_PlatformError;
      //"Expected that Thread::start() would fail if ThreadImpl::start() fails");
      EXPECT_TRUE(t.start() == Threading::Status_PlatformError);
      fixture->threadImpl->startSuccess = Threading::Status_NoError;
      //, "Expected that Thread::start() would succeed if ThreadImpl::start() succeeds");
      EXPECT_TRUE(t.start() == Threading::Status_NoError);
   }
};

namespace ThreadBehaviorIsRunningParrotsImplStart
{
   AutoPtr<PlatformFixture> fixture;   

   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }

   TEST(ThreadBehavior, IsRunningParrotsImplStart)
   {
      fixture = new PlatformFixture;
      Thread::StartDelegate d(MakeDelegate(&dummy));
      Thread t(d);

      fixture->threadImpl->startSuccess = Threading::Status_PlatformError;
      t.start();
      //, "Expected that Thread::isRunning() would be false if ThreadImpl::start fails"
      EXPECT_TRUE(t.isRunning() == false);

      fixture->threadImpl->startSuccess = Threading::Status_NoError;
      t.start();
      // Pretend that we've acquired the gateway semaphore
      fixture->semaphoreImpl->acquireSuccess = Threading::Status_Busy;
      //, "Expected that Thread::isRunning() would be true if ThreadImpl::start succeeds"
      EXPECT_TRUE(t.isRunning() == true);
   }                 
};

namespace ThreadBehaviorStartOnce
{
   AutoPtr<PlatformFixture> fixture;   

   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }

   TEST(ThreadBehavior, StartOnce)
   { 
      fixture = new PlatformFixture;
      Thread::StartDelegate d(MakeDelegate(&dummy));
      Thread t(d);

      fixture->threadImpl->startSuccess = Threading::Status_NoError;
      //, "Expected that Thread::start() would succeed if ThreadImpl::start succeds"
      EXPECT_TRUE(t.start() == Threading::Status_NoError);
      //, "Expected that Thread::start() would fail if called a second time"
      EXPECT_TRUE(t.start() == Threading::Status_Again);
   }
};

namespace ThreadBehaviorStartDelegate
{
   AutoPtr<PlatformFixture> fixture;   

   S32 someFunc(Thread::MessageQueue&)
   {
      return 0;
   }

   TEST(ThreadBehavior, StartDelegate)
   {
      fixture = new PlatformFixture;
      Thread::StartDelegate d(MakeDelegate(&someFunc));
      Thread t(d);

      fixture->threadImpl->startSuccess = Threading::Status_NoError;
      t.start();

      // "Expected that delegate passed to ThreadImpl::start would be same as delegate passed to Thread::Thread()"
      EXPECT_TRUE(fixture->threadImpl->param->d == MakeDelegate(&someFunc));
   }
};

TEST(ThreadBehavior, InvalidStartDelegate)
{
   AutoPtr<PlatformFixture> fixture;
   fixture = new PlatformFixture;
   Thread::StartDelegate d;
   Thread t(d);

   fixture->threadImpl->startSuccess = Threading::Status_NoError;
   //, "Expected that thread with empty startdelegate would return DelegateInvalid on start()"
   EXPECT_TRUE(t.start() == Threading::Status_DelegateInvalid);
   //, "Expected that thread with invalid startdelegate would not call ThreadImpl::start()"
   EXPECT_TRUE(fixture->threadImpl->startCt == 0);
}

namespace ThreadBehaviorReturnCode
{
   AutoPtr<PlatformFixture> fixture;   

   static const S32 MAGIC_NUMBER = 42;
   S32 someFunc(Thread::MessageQueue&)
   {
      return MAGIC_NUMBER;
   }

   TEST(ThreadBehavior, ReturnCode)
   {
      fixture = new PlatformFixture;
      Thread::StartDelegate d(MakeDelegate(&someFunc));
      Thread t(d);

      fixture->threadImpl->startSuccess = Threading::Status_NoError;
      t.start();
      fixture->threadImpl->run();
      //, "Expected Thread::getReturnCode() would return the same value returned by the delegate"
      EXPECT_TRUE(t.getReturnCode() == MAGIC_NUMBER);
   }
};

#endif


