//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/threads/posixThreadLocalImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      PosixThreadLocalImpl::PosixThreadLocalImpl()
      {
         pthread_key_create(&mKey, NULL);
      }

      PosixThreadLocalImpl::~PosixThreadLocalImpl()
      {
         pthread_key_delete(mKey);
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
