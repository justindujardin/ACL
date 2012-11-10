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

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;

/// Basic validation of thread functionality.  Verifies that impl properly reports running state
/// that impl calls the given method, and that impl passes in userData and dataSize, and that imple
/// reports the return value of the thread.

namespace BasicUsage 
{
   static bool didWork;
   static bool passedMagicNumber;
   static const U32 _magicNumber = 42;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      didWork = true;
      return _magicNumber;
   }  

   TEST(Threads,BasicUsage) {
      didWork = passedMagicNumber = false;
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

   TEST(Threads, DeleteTermination)
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

   TEST(Threads, PostMessage)
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

   TEST(Threads, IsRunning)
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

/// Verifies that locking a mutex twice results in the second lock blocking the calling
/// thread until the first thread unlocks the mutex.
namespace ThreadMutexBlock
{
   static Mutex m;
   static WaitObject wait;
   S32 lock(Thread::MessageQueue& messageQueue)
   {
      wait.signalOne();
      EXPECT_TRUE(m.lock(true) == Threading::Status_NoError);//, "Failed a blocking lock");
      return 0;
   }
   TEST(Threads, MutexBlock)
   {
      Thread t(MakeDelegate(&lock));
      EXPECT_TRUE(m.lock(true) == Threading::Status_NoError);//, "Failed to lock unlocked mutex");
      t.start();
      wait.wait(&m);
      GetPlatform()->sleep(100);
      m.unlock();
      t.finish();
      EXPECT_TRUE(t.getReturnCode() == 0);
   }
};

/// Verifies that an attempt to lock a locked mutex returns false if block is false.
namespace ThreadMutexNonBlock
{
   static Mutex m;
   static WaitObject w;

   S32 lock(Thread::MessageQueue& messageQueue)
   {
      // Cannot non-blocking lock the mutex
      EXPECT_TRUE(m.lock(false) == Threading::Status_Busy);
      return 0;
   }

   TEST(Threads, MutexNonBlock)
   {
      EXPECT_TRUE(m.lock(true) == Threading::Status_NoError);
      Thread t(MakeDelegate(&lock));
      t.start();
      t.finish();
      m.unlock();
   }
};


/// Verifies that a thread local stores different values for each accessing thread.
namespace ThreadLocalData
{
   static ThreadLocal l;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      l.set(reinterpret_cast<void*>(7));
      GetPlatform()->sleep(100);
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
