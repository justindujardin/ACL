//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/containers/tVector.h"
#include "platform/threads/thread.h"
#include "platform/impls/base/threads/threadImpl.h"
#include "platform/threads/mutex.h"
#include "platform/threads/waitObject.h"
#include "platform/platform.h"
#include "platform/threads/concurrentQueue.h"


namespace ACLib
{
   namespace Platform
   {
      const String Thread::TerminateMessage::Type("Terminate");

      /// @cond
      struct Thread::MessageQueue::Internal
      {
         ConcurrentQueue<MessageRef> toThread;
      };
      /// @endcond

      Thread::MessageQueue::MessageQueue() : mImpl(new Internal)
      {
      }

      Thread::MessageQueue::~MessageQueue()
      {
      }

      void Thread::MessageQueue::postToThread(MessageRef msg)
      {
         mImpl->toThread.push(msg);
      }

      bool Thread::MessageQueue::waitOnMessageToThread(MessageRef& msg, bool block)
      {
         if(!block)
            return mImpl->toThread.pop_if_present(msg);
         else
            mImpl->toThread.pop(msg);
         return true;
      }

      /// @cond
      struct Thread::Internal
      {
         ScopedPtr<Internal_::ThreadImpl> impl;
         Thread::StartDelegate delegate;
         Thread::MessageQueue messageQueue;
         bool running;
         bool hasStarted;
         Mutex isRunningBlocker;

         static bool ThreadStarted;

         Internal(const Thread::StartDelegate& d) :
         impl(GetPlatform()->getFactory().create<Internal_::ThreadImpl>()),
            delegate(d), messageQueue(), running(false), hasStarted(false), isRunningBlocker()
         {
         }

         ~Internal()
         {
         }
      };
      /// @endcond

      bool Thread::Internal::ThreadStarted = false;

      Thread::Thread(const StartDelegate& delegate) : mImpl(new Internal(delegate))
      {
      }

      Thread::~Thread()
      {
         mImpl->messageQueue.postToThread(new TerminateMessage);
         finish();
      }

      Threading::Status Thread::start()
      {
         // We need a valid delgate...
         if(!mImpl->delegate)
            return Threading::Status_DelegateInvalid;

         // Don't allow thread to be run more than once.
         if(mImpl->hasStarted)
            return Threading::Status_Again;

         // Prevent issues with calling start() on multiple threads simultaneously
         // Also prevent issues with calling isRunning() during thread setup.
         Mutex::ScopedLock startLock(mImpl->isRunningBlocker);
         // Check if another thread succeeded while we were blocked
         if(mImpl->hasStarted)
            return Threading::Status_Again;

         Internal_::ThreadImpl::Param* p = 
            new Internal_::ThreadImpl::Param(mImpl->delegate, mImpl->messageQueue,
            mImpl->impl.get(), this);

         mImpl->impl->gateway.acquire();
         Threading::Status perror = mImpl->impl->start(p);
         if(perror != Threading::Status_NoError)
         {
            mImpl->running = false;
            mImpl->impl->gateway.release();
            delete p;
            return perror;
         }
         else
         {
            mImpl->running = true;
            mImpl->hasStarted = true;
            Thread::Internal::ThreadStarted = true;
         }
         return Threading::Status_NoError;
      }

      bool Thread::isRunning()
      {
         if(mImpl->running)
         {
            // Prevents race condition.  Thread A could acquire the semaphore and
            // before updating isRunning, thread B could enter this section and also
            // attempt to acquire the semaphore.  Thread B would fail to acquire the
            // semaphore, causing it to erroneously say that we're still running.
            Mutex::ScopedLock s(mImpl->isRunningBlocker);
            mImpl->running = mImpl->impl->gateway.acquire(false) == Threading::Status_Busy;
            if(!mImpl->running)
               mImpl->impl->gateway.release();
         }

         return mImpl->running;
      }

      void Thread::finish()
      {
         if(mImpl->running)
         {
            // The running thread holds on to the gateway, such that by blocking here
            // until we can acquire the gateway, we know the thread has exited.
            mImpl->impl->gateway.acquire();
            mImpl->impl->gateway.release();
            mImpl->running = false;
         }
      }

      Thread* Thread::GetCurrentThread()
      {
         return Internal_::ThreadImpl::GetCurrentThread();
      }

      bool Thread::IsMainThread()
      {
         // If no thread has been started then we must be the main thread.  This is
         // done to work around certain static init problems.
         if(!Thread::Internal::ThreadStarted)
            return true;

         return GetCurrentThread() == Internal_::ThreadImpl::GetMainThread();
      }

      S32 Thread::getReturnCode() const
      {
         return mImpl->impl->getReturnCode();
      }

      Thread::MessageQueue& Thread::getMessageQueue()
      {
         return mImpl->messageQueue;
      }
   }
}