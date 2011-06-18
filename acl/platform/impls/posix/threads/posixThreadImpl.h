//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_THREADIMPL_H_
#define ACL_PLATFORM_POSIX_THREADIMPL_H_

#include "platform/impls/base/threads/threadImpl.h"
#include <pthread.h>

namespace Platform2
{
   namespace Internal_
   {
      class PosixThreadImpl : public ThreadImpl
      {
      public:      
         ~PosixThreadImpl();
         virtual Threading::Status start(Param* p);

      private:
         pthread_t mThread;
         S32 mCreationErr;
      };
   }
}

#endif