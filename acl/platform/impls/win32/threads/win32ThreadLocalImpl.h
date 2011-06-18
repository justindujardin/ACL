//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_THREADLOCALIMPL_H_
#define ACL_PLATFORM_WIN32_THREADLOCALIMPL_H_

#include <Windows.h>
#include "platform/impls/base/threads/threadLocalImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class Win32ThreadLocalImpl : public ThreadLocalImpl
      {
      public:
         Win32ThreadLocalImpl();
         virtual ~Win32ThreadLocalImpl();
         virtual void* get();
         virtual void set(void* value);

      private:
         DWORD mTLSIndex;
      };
   }
}

#endif
