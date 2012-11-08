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



namespace Platform2
{
   namespace Internal_
   {
      PosixWaitObjectImpl::PosixWaitObjectImpl()
      {
         pthread_cond_init( &mCondition, NULL );
        AssertFatal(&mCondition != NULL, "Failed to initialize condition variable");
      }

      PosixWaitObjectImpl::~PosixWaitObjectImpl()
      {
         pthread_cond_destroy( &mCondition );
      }

      Threading::Status PosixWaitObjectImpl::wait(Mutex *mutex, S32 timeout /*= -1*/)
      {
        int result = 0;
        if(timeout != -1 && timeout != 0)
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
          ts.tv_nsec += timeout * 1000000; // convert milliseconds to nanoseconds
          result = pthread_cond_timedwait(&mCondition, (pthread_mutex_t*)mutex->getNative(), &ts);
        }
        else
        {
          result = pthread_cond_wait(&mCondition,(pthread_mutex_t*)mutex->getNative());
        }
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
