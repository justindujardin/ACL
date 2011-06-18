//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "math/mRect.h"
#include "core/util/str.h"
#include "platform2/platform.h"
#include "platform2/windows/window.h"
#include "platform2/test/windows/testWindowImpl.h"
#include "platform2/threads/thread.h"
#include "platform2/threads/mutex.h"
#include "platform2/threads/semaphore.h"
#include "platform2/threads/threadLocal.h"
#include "core/assert.h"
#include "platform2/platform.h"
#include "core/util/autoPtr.h"

#include "console/console.h"

using fastdelegate::MakeDelegate;

using namespace UnitTesting;
using namespace Platform2;

/// Basic validation of thread functionality.  Verifies that impl properly reports running state
/// that impl calls the given method, and that impl passes in userData and dataSize, and that imple
/// reports the return value of the thread.
CreateUnitTest(p2_thread_basic, "Platform2/Functional/Thread/Basic")
{
   bool didWork;
   bool passedMagicNumber;
   static const U32 _magicNumber = 42;
   S32 work(Thread::MessageQueue& messageQueue)
   {
      didWork = true;
      return _magicNumber;
   }
   
   void run()
   {
      didWork = passedMagicNumber = false;
      Thread t(MakeDelegate(this, &p2_thread_basic::work));
      t.start();
      t.finish();
      test(t.isRunning() == false, "Thread in a running state after finish()");
      test(didWork, "Thread not executed");
      test(t.getReturnCode(), "Thread did not return magic number");
   }
};

/// Verifies that when a thread is deleted it posts a Terminate message to the message queue and 
/// waits until that message is received and acted upon.
CreateUnitTest(p2_thread_deletion, "Platform2/Functional/Thread/DeleteTerminates")
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
   
   void run()
   {
      AutoPtr<Thread> t(new Thread(MakeDelegate(this, &p2_thread_deletion::work)));
                  
      U32 time = Platform2::GetPlatform()->getRealMilliseconds();
      t->start();
      t = NULL;
      time = Platform2::GetPlatform()->getRealMilliseconds() - time;
      test(time < 1000, "Thread deletion did not immediately terminate the thread");
   }
};

/// Verifies that Thread can post user defined messages and that threads can wait for messages to be received
CreateUnitTest(p2_thread_postmessage, "Platform2/Functional/Thread/PostMessage")
{
   // This will return as soon as a message is posted
   S32 work(Thread::MessageQueue& messageQueue)
   {
      U32 start = Platform2::GetPlatform()->getRealMilliseconds();
      Thread::MessageRef msg;
      messageQueue.waitOnMessageToThread(msg, true);
      return Platform2::GetPlatform()->getRealMilliseconds() - start;
   }
   
   void run()
   {
      Thread t(MakeDelegate(this, &p2_thread_postmessage::work));
      t.start();
      GetPlatform()->sleep(100);
      // We don't care about the type of the message
      Thread::Message* msg = new Thread::TerminateMessage;
      t.getMessageQueue().postToThread(msg);
      t.finish();
      test(t.getReturnCode() >= 90, "Thread terminated early, didn't wait on message");
   }
};

/// Verify that the implementation polls the thread when isRunning is called, and that given
/// a thread that will finish in a finite amount of time isRunning begins to return false in a finite amount of time.
CreateUnitTest(p2_thread_isrunning, "Platform2/Functional/Thread/IsRunning")
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
      worker = new Thread(MakeDelegate(this, &p2_thread_isrunning::work));
      worker->start();
      while(worker->isRunning())
         GetPlatform()->sleep(1);
      return 0;
   }
   
   void run()
   {
      
      watchdog = new Thread(MakeDelegate(this, &p2_thread_isrunning::watch));
      passed = true;
      
      watchdog->start();
      GetPlatform()->sleep(200);
      test(!watchdog->isRunning(), "Thread polled never finished");
      watchdog = NULL;
      worker = NULL;
   }
};

/// Verifies that locking a mutex twice results in the second lock blocking the calling
/// thread until the first thread unlocks the mutex.
CreateUnitTest(p2_thread_mutex_block, "Platform2/Functional/Thread/Mutex/Block")
{
   Mutex m;
   
   S32 lock(Thread::MessageQueue& messageQueue)
   {
      U32 start = Platform2::GetPlatform()->getRealMilliseconds();
      test(m.lock(true) == Threading::Status_NoError, "Failed a blocking lock");
      return Platform2::GetPlatform()->getRealMilliseconds() - start;
   }
   
   void run()
   {
      Thread t(MakeDelegate(this, &p2_thread_mutex_block::lock));
      test(m.lock(true) == Threading::Status_NoError, "Failed to lock unlocked mutex");
      t.start();
      Platform2::GetPlatform()->sleep(100);
      m.unlock();
      t.finish();
      test(t.getReturnCode() >= 90, "Blocking lock didn't block!");
   }
};

/// Verifies that an attempt to lock a locked mutex returns false if block is false.
CreateUnitTest(p2_thread_mutex_nonblock, "Platform2/Functional/Thread/Mutex/NonBlock")
{
   Mutex m;
   
   S32 lock(Thread::MessageQueue& messageQueue)
   {
      U32 start = Platform2::GetPlatform()->getRealMilliseconds();
      test(m.lock(false) == Threading::Status_Busy, "Non blocking call succeeded in locking already locked mutex");
      return Platform2::GetPlatform()->getRealMilliseconds() - start; 
   }
   
   void run()
   {
      Thread t(MakeDelegate(this, &p2_thread_mutex_nonblock::lock));
      test(m.lock(false) == Threading::Status_NoError, "Failed to lock unlocked mutex");
      t.start();
      Platform2::GetPlatform()->sleep(100);
      m.unlock();
      t.finish();
      test(t.getReturnCode() < 110, "Non-blocking lock blocked!");
   }
};


/// Verifies that when a semaphore of count 0 is acquired the calling thread is blocked
/// until the semaphores count is > 0.
CreateUnitTest(p2_thread_semaphore_block, "Platform2/Functional/Thread/Semaphore/Block")
{
   AutoPtr<Semaphore> s;
   
   S32 work(Thread::MessageQueue& messageQueue)
   {
      U32 start = Platform2::GetPlatform()->getRealMilliseconds();
      test(s->acquire(true) == Threading::Status_NoError, "Failed to acquire blocking semaphore with count > 0");
      test(s->acquire(true) == Threading::Status_NoError, "Failed to acquire blocking semaphore");
      return Platform2::GetPlatform()->getRealMilliseconds() - start; 
   }
   
   void run()
   {
      s = new Semaphore(2);
      Thread t(MakeDelegate(this, &p2_thread_semaphore_block::work));
      s->acquire(true);
      t.start();
      GetPlatform()->sleep(100);
      s->release();
      t.finish();
      test(t.getReturnCode() >= 90, "Semaphore wasn't blocking");
   }
};

/// Verifies that an attempt to acquire a semaphore of count 0 does *not* block and returns false
/// if block is false.
CreateUnitTest(p2_thread_semaphore_nonblock, "Platform2/Functional/Thread/Semaphore/NonBlock")
{
   AutoPtr<Semaphore> s;
   
   S32 work(Thread::MessageQueue& messageQueue)
   {
      U32 start = Platform2::GetPlatform()->getRealMilliseconds();
      test(s->acquire(true) == Threading::Status_NoError, "Failed to acquire blocking semaphore with count > 0");
      test(s->acquire(false) == Threading::Status_Busy, "Succeeded in non-blocking acquire of semaphore with count 0");
      return Platform2::GetPlatform()->getRealMilliseconds() - start; 
   }
   
   void run()
   {
      s = new Semaphore(2);
      Thread t(MakeDelegate(this, &p2_thread_semaphore_nonblock::work));
      s->acquire(true);
      t.start();
      GetPlatform()->sleep(100);
      s->release();
      t.finish();
      test(t.getReturnCode() < 110, "Semaphore was blocking");
   }
};

CreateUnitTest(TestSemaphoreRelease, "Platform2/Functional/Thread/Semaphore/Release")
{
   void run()
   {
      AutoPtr<Semaphore> s(new Semaphore(1, 1));
      test(s->release() == Threading::Status_Resources, 
         "Expected that releasing semaphore beyond maxCount would return Status_Resources");
   }
};

/// Verifies that a thread local stores different values for each accessing thread.
CreateUnitTest(p2_thread_threadlocal, "Platform2/Functional/Thread/ThreadLocal")
{
   ThreadLocal l;
   
   S32 work(Thread::MessageQueue& messageQueue)
   {
      l.set(reinterpret_cast<void*>(7));
      GetPlatform()->sleep(100);
      test(reinterpret_cast<U32>(l.get()) == 7, "Thread local value incorrect on thread");
      return 0;
   }
   
   void run()
   {
      Thread t(MakeDelegate(this, &p2_thread_threadlocal::work));
      l.set(reinterpret_cast<void*>(42));
      t.start();
      t.finish();
      test(reinterpret_cast<U32>(l.get()) == 42, "Thread local value incorrect");
   }
};
