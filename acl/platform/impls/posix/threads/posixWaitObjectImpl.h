//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_WAITOBJECTIMPL_H_
#define ACL_PLATFORM_POSIX_WAITOBJECTIMPL_H_

#include "platform/impls/base/threads/waitObjectImpl.h"
#include <pthread.h>

namespace Platform2
{
   namespace Internal_
   {
      class PosixWaitObjectImpl : public WaitObjectImpl
      {
      public:
         PosixWaitObjectImpl();
         virtual ~PosixWaitObjectImpl();
         virtual Threading::Status wait(S32 timeout = -1);
         virtual void signalOne();
         virtual void signalAll();
      private:
         /// The mutex attribute that is used to make the Mutex recursively lockable.
         pthread_mutexattr_t mMutexAttr;

         /// The posix condition variables implementation requires a Mutex to function, and it must
         /// obey strict locking rules during invocation, so this class manages the mutex itself.
         pthread_mutex_t mMutex;         

         /// The condition object to wait on.
         pthread_cond_t mCondition;
      };
   }
}

#endif
