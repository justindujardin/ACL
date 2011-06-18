//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_THREADLOCALIMPL_H_
#define ACL_PLATFORM_POSIX_THREADLOCALIMPL_H_

#include "platform/impls/base/threads/threadLocalImpl.h"
#include <pthread.h>

namespace Platform2
{
   namespace Internal_
   {
      class PosixThreadLocalImpl : public ThreadLocalImpl
      {
      public:      
         PosixThreadLocalImpl();
         virtual ~PosixThreadLocalImpl();
         virtual void* get();
         virtual void set(void* value);

      private:
         pthread_key_t mKey;
      };
   }
}

#endif
