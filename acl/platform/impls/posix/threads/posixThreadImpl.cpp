//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixThreadImpl.h"
#include <errno.h>

static void* threadEntry(void* p)
{
   Platform2::Internal_::ThreadImpl::CommonThreadEntry(p);
   return 0;
}

namespace Platform2
{
   namespace Internal_
   {
      PosixThreadImpl::~PosixThreadImpl()
      {
         // Errors if we never actually created a thread, but we don't care.
         pthread_join(mThread, NULL);
      }

      Threading::Status PosixThreadImpl::start(Param* p)
      {
         mCreationErr = pthread_create(&mThread, NULL, threadEntry, p);
         switch(mCreationErr)
         {
         case 0:
            return Threading::Status_NoError;
         case EAGAIN:
            return Threading::Status_Resources;
         default:
            return Threading::Status_PlatformError;
         }
      }
   }
}