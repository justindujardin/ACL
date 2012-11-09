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
#include "platform/threads/waitObject.h"
#include "platform/threads/concurrentQueue.h"
#include "core/assert.h"
#include "platform/platform.h"
#include "core/util/autoPtr.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;

/// WaitObject signalAll test.
namespace WaitObjectSignalAll
{
   static const S32   _numThreads = 32;
   static Thread * _threads[_numThreads];
   static WaitObject      _wait;
   static Mutex           _mutex;

   // Wait on the signal from the main thread, and return 0 if it is properly received.
   S32 work(Thread::MessageQueue& messageQueue)
   {
      Mutex::ScopedLock lock(_mutex);
      Threading::Status ret = _wait.wait(&_mutex,1500);
      return (ret == Threading::Status_WaitTimeout) ? -1 : 0;
   }
   TEST(WaitObject,SignalAll) {
      // Create numThreads Thread objects, and have them all wait on the same WaitObject.
      for (S32 i = 0; i < _numThreads; i++)
      {
         _threads[i] = new Thread(MakeDelegate(&work));
         _threads[i]->start();
      }

      // Once the threads are created and waiting, call signalAll to cause them to exit.
      _wait.signalAll();

      // Check to make sure all return codes are 0 (did not timeout)
      for (S32 i = 0; i < _numThreads; i++)
      {
         Thread *thread = _threads[i];
         thread->finish();
         S32 ret = thread->getReturnCode();
         EXPECT_TRUE(ret == 0);
         delete thread;
      }
   }
}


/// Basic validation of WaitObject signalOne functionality.  
/// The WaitObject must be called with a locked Mutex, and it will return with the same Mutex locked.
namespace WaitObjectSignalOne 
{
   static WaitObject *_wait      = NULL;
   static Mutex *_mutex          = NULL;
   static const U32 _magicNumber = 42;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      _wait->wait(_mutex);
      return GetPlatform()->getRealMilliseconds();
   }  

   TEST(WaitObject,SignalOne) {
      _mutex = new Mutex();
      _wait = new WaitObject();
      Thread t(MakeDelegate(&work));
      S32 start = GetPlatform()->getRealMilliseconds();
      t.start();
      GetPlatform()->sleep(150);
      _wait->signalOne();      
      GetPlatform()->sleep(1);
      EXPECT_FALSE(t.isRunning()); // Wait signal should cause thread to exit.
      EXPECT_TRUE(t.getReturnCode() - start >= 150);
      //, "Thread took at least sleep time before returning from wait object");
   }
}

/// Basic validation of WaitObject timeout functionality.  
namespace WaitObjectTimeout
{
   static WaitObject *_wait      = NULL;
   static Mutex *_mutex          = NULL;

   S32 work(Thread::MessageQueue& messageQueue)
   {
      Threading::Status ret = _wait->wait(_mutex,10);
      return (ret == Threading::Status_WaitTimeout) ? 0 : -1;
   }

   TEST(WaitObject,Timeout) {
      _mutex = new Mutex();
      _wait = new WaitObject();
      Thread t(MakeDelegate(&work));
      S32 start = GetPlatform()->getRealMilliseconds();
      t.start();
      GetPlatform()->sleep(150);
      EXPECT_FALSE(t.isRunning()); // Wait signal should cause thread to exit.
      EXPECT_TRUE(t.getReturnCode() == 0); // Thread wait should have timed out
   }
}