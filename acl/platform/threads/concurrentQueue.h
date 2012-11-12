//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_CONCURRENTQUEUE_H_
#define ACL_PLATFORM_CONCURRENTQUEUE_H_

#include "core/util/noncopyable.h"
#include "core/containers/tVector.h"
#include "platform/threads/mutex.h"
#include "platform/threads/waitObject.h"

namespace Platform2
{

   /// @ingroup p2thread
   /// ConcurrentQueue implements a thread-safe queue.
   template<typename T> class ConcurrentQueue : private Noncopyable
   {
   public:
      ConcurrentQueue() : mMutex(new Mutex), mWaitObject(new WaitObject) {}
      ~ConcurrentQueue()
      {
         mMutex->lock();
         delete mMutex;
         delete mWaitObject;
      }
      void push(T const& data)
      {
         mMutex->lock();
         mQueue.push_back(data);
         mMutex->unlock();
         mWaitObject->signalOne();
      }
      void pop(T& msg)
      {
         if(empty())
            mWaitObject->wait();        
         Mutex::ScopedLock lock(*mMutex);
         msg = mQueue.front();
         mQueue.pop_front();
      }
      bool pop_if_present(T& msg)
      {
         Mutex::ScopedLock lock(*mMutex);
         if(mQueue.empty())
            return false;
         msg = mQueue.front();
         mQueue.pop_front();
         return true;         
      }
      bool empty() const
      {
         Mutex::ScopedLock lock(*mMutex);
         return mQueue.empty();
      }
   private:
      Vector<T> mQueue;
      Mutex *mMutex;
      WaitObject *mWaitObject;
   };
}


#endif
