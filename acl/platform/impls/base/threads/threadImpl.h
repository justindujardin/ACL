//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_THREADIMPL_H_
#define ACL_PLATFORM_THREADIMPL_H_

#include "platform/platform.h"
#include "platform/threads/thread.h"
#include "platform/threads/threadLocal.h"
#include "platform/threads/semaphore.h"
#include "core/util/noncopyable.h"

/// @ingroup p2thread
/// @defgroup p2threadimpl Implementation Details
/// @brief The following are internal classes which you should subclass to implement
/// threading on your platform.

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         /// @ingroup p2threadimpl
         /// Base class for your Platform's Thread implementation.
         /// This base class is the most complex base class in Platform.  Ironically
         /// this complexity is geared to reduce the complexity of your implementation.
         /// While you are encouraged to read the rest of the documentation, the important
         /// thing to remember is that you only need to implement the start() method.
         class ThreadImpl : private Noncopyable
         {
            friend class Platform::Thread;
         protected:
            struct Param;
         public:
            virtual ~ThreadImpl() {}

            /// @brief This will be called when the platform layer when a request to
            /// begin a thread is submitted.
            /// @details You should pass p to CommonThreadEntry on the new platform
            /// thread which is started.
            /// @returns Threading::Status_NoError if the new thread was successfully started.
            /// @returns Threading::Status_Resources if the thread could not be created
            /// due to a lack of resources or too many active threads.
            /// @returns Threading::Status_PlatformError if the thread could not be created
            /// for any other reason.
            virtual Threading::Status start(Param* p) = 0;

            /// Call this with the Param given to you in start() as soon as you've
            /// spawned a new thread.
            static void CommonThreadEntry(void* pv)
            {
               Param* p = static_cast<Param*>(pv);
               // Work around unittest case, where we will be called on the main thread.
               if(GetCurrentThread() != GetMainThread())
                  GetCurrentThreadLocal_().set(p->thread);
               S32 ret = p->d(p->messageQueue);
               p->impl->setReturnCode(ret);
               p->impl->gateway.release();
               delete p;
            }

            /// This is public to support setting the new thread 
            static void RegisterCurrentThreadAsMain()
            {
               GetCurrentThreadLocal_().set(static_cast<void*>(GetMainThread()));
            }

         protected:
            struct Param
            {
               Thread::StartDelegate& d;
               Thread::MessageQueue& messageQueue;
               ThreadImpl* impl;
               Thread* thread;

               Param(Thread::StartDelegate& d_, Thread::MessageQueue& m, ThreadImpl* i,
                  Thread* t) : d(d_), messageQueue(m), impl(i), thread(t)
               {
               }
            };

            ThreadImpl() : gateway(1, 1) {}

         private:
            S32 getReturnCode() const
            {
               return mReturnCode;
            }

            void setReturnCode(S32 ret)
            {
               mReturnCode = ret;
            }

            static ThreadLocal& GetCurrentThreadLocal_()
            {
               static ThreadLocal smCurrentThread;
               return smCurrentThread;
            }

            static Thread* GetCurrentThread()
            {
               // Works around some static-init issues.  Uncertain if this is still
               // necessary.
               if(!GetPlatform())
                  return GetMainThread();
               return static_cast<Thread*>(GetCurrentThreadLocal_().get());
            }

            static Thread* GetMainThread()
            {
               static Thread::StartDelegate d;
               static Thread mainThread(d);
               return &mainThread;
            }

            Semaphore gateway;
            S32 mReturnCode;
         };
      }
   }
}
#endif
