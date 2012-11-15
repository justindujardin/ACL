//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/assert.h"
#include "platform/impls/posix/threads/posixWaitObjectImpl.h"
#include "platform/threads/threadingStatus.h"
#include <time.h>
#include <sys/time.h>
#include <errno.h>


namespace ACLib
{
  namespace Platform
  {
     namespace Internal_
     {
        const S64 NANOSEC_PER_MILLISEC = 1000000;
        const S64 NANOSEC_PER_SEC = 1000000000;
       
        PosixWaitObjectImpl::PosixWaitObjectImpl()
        {
          pthread_mutexattr_init( &mMutexAttr );
          pthread_mutexattr_settype( &mMutexAttr, PTHREAD_MUTEX_RECURSIVE );        
          pthread_mutex_init( &mMutex, &mMutexAttr );

          pthread_cond_init( &mCondition, NULL );
          AssertFatal(&mCondition != NULL, "Failed to initialize condition variable");
        }

        PosixWaitObjectImpl::~PosixWaitObjectImpl()
        {
          pthread_cond_destroy( &mCondition );
        }

        Threading::Status PosixWaitObjectImpl::wait(S32 timeout /*= -1*/)
        {

           if(pthread_mutex_lock(&mMutex) != 0)
              return Threading::Status_PlatformError;

          S32 result = 0;
          if(timeout > 0)
          {
            struct timespec ts;
  #ifdef ACL_OS_MAC
            struct timeval now;
            gettimeofday(&now, NULL);
            ts.tv_sec  = now.tv_sec;
            ts.tv_nsec = now.tv_usec * 1000;
  #else
            clock_gettime(CLOCK_REALTIME, &ts);
  #endif
            S32 milliSeconds = timeout % 1000;
            S32 seconds = (timeout - milliSeconds) / 1000;
            ts.tv_nsec += milliSeconds * NANOSEC_PER_MILLISEC;
            ts.tv_sec += seconds;
            result = pthread_cond_timedwait(&mCondition, &mMutex, &ts);
          }
          else
          {
            result = pthread_cond_wait(&mCondition, &mMutex);
          }

          pthread_mutex_unlock(&mMutex);

          if(result == ETIMEDOUT)
            return Threading::Status_WaitTimeout;
          else if(result == EINVAL)
            return Threading::Status_ObjectInvalid;

          return Threading::Status_WaitSignaled;
        }
      
       void PosixWaitObjectImpl::signalAll()
       {
         pthread_cond_broadcast(&mCondition);
       }
       
       void PosixWaitObjectImpl::signalOne()
       {
         pthread_cond_signal(&mCondition);
       }
     }
  }
}