//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------


#include "core/strings/str.h"
#include "core/util/delegate.h"
#include "platform/platform.h"
#include "platform/threads/mutex.h"
#include "platform/threads/waitObject.h"
#include "platform/threads/thread.h"
#include "platform/threads/concurrentQueue.h"
#include "platform/platform.h"
#include "core/util/autoPtr.h"

#include <gtest/gtest.h>
#include <stdlib.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;


/// ConcurrentQueue sanity test.  Create a bunch of threads, and have them randomly
/// insert and remove from the queue.
namespace ConcurrentQueueRace
{
   typedef ConcurrentQueue<S32> QueueType;
   static const S32       _numThreads = 32;
   Thread *               _threads[_numThreads];
   QueueType  *_queue;
   WaitObject *_wait;
   bool blocking = false;

   // Wait on the signal from the main thread, and return 0 if it is properly received.
   S32 work(Thread::MessageQueue& messageQueue)
   {
      S32 sleepMS = rand() % 2;
      S32 iterations = rand() % 100;
      Threading::Status ret = _wait->wait(1500);
      if (ret == Threading::Status_WaitTimeout)
         return -1;
      // The race has begun, iterate over the list, and insert/remove items.
      for (S32 i = 0; i < iterations; i++)
      {
         _queue->push((S32)(rand() % 256));
         S32 result;
         GetPlatform()->sleep(sleepMS);
         if(blocking)
            _queue->pop(result);
         else
            _queue->pop_if_present(result);
      }
      return 0;
   }

   void testThreadWorkers(bool blockingPop) 
   {
      _queue = new QueueType();
      _wait = new WaitObject;
      blocking = blockingPop;

      // Create the threads, which will sit idle until we signal the start of the race.
      for (S32 i = 0; i < _numThreads; i++)
      {
         _threads[i] = new Thread(MakeDelegate(&work));
         _threads[i]->start();
      }

      GetPlatform()->sleep(500);

      // Once the threads are created and waiting, call signalAll to cause them to exit.
      _wait->signalAll();

      // Check to make sure all return codes are 0
      for (S32 i = 0; i < _numThreads; i++)
      {
         Thread *thread = _threads[i];
         thread->finish();
         S32 ret = thread->getReturnCode();
         EXPECT_TRUE(ret == 0);
         delete thread;
      }

      delete _queue;
      delete _wait;
   }

   TEST(ConcurrentQueue,PopIfPresent) {
      srand(1337);
      testThreadWorkers(false);
   }
   TEST(ConcurrentQueue,Pop) {
      srand(1337);
      testThreadWorkers(true);
   }


}



