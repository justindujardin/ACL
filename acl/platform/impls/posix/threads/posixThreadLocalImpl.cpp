//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixThreadLocalImpl.h"
#include "core/assert.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         PosixThreadLocalImpl::PosixThreadLocalImpl()
         {
            int rc = pthread_key_create(&mKey, NULL);
            AssertFatal(rc == 0, "Failed to create TLS key");
         }

         PosixThreadLocalImpl::~PosixThreadLocalImpl()
         {
            pthread_key_delete(mKey);
            mKey = -1;
         }

         void* PosixThreadLocalImpl::get()
         {
            return pthread_getspecific(mKey);
         }

         void PosixThreadLocalImpl::set(void* value)
         {
            pthread_setspecific(mKey, value);
         }
      }
   }
}