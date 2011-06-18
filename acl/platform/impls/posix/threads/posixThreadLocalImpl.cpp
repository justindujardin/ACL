//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "platform2/impls/posix/threads/posixThreadLocalImpl.h"

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