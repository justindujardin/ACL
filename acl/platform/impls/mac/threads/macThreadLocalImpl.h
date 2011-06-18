//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_THREADLOCALIMPL_H_
#define ACL_PLATFORM_MAC_THREADLOCALIMPL_H_

#include "platform/impls/base/threads/threadLocalImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class MacThreadLocalImpl : public ThreadLocalImpl
      {
      public:      
         MacThreadLocalImpl();
         virtual ~MacThreadLocalImpl();
         virtual void* get();
         virtual void set(void* value);

      private:
         TaskStorageIndex mTaskIndex;
      };
   }
}

#endif