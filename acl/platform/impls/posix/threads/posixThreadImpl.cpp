//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixThreadImpl.h"
#include <errno.h>

static void* threadEntry(void* p)
{
   ACLib::Platform::Internal_::ThreadImpl::CommonThreadEntry(p);
   return 0;
}

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         PosixThreadImpl::~PosixThreadImpl()
         {
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
}