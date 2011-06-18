//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_THREADLOCALIMPL_H_
#define ACL_PLATFORM_THREADLOCALIMPL_H_

#include "core/util/noncopyable.h"

namespace Platform2
{
   namespace Internal_ 
   {
      /// @ingroup p2threadimpl
      /// Base class for your platforms ThreadLocal storage functionality.
      /// Due to the guarantee made by the Platform2::ThreadLocal class, your implementation
      /// cannot rely on the presence of any other Platform2 class.
      class ThreadLocalImpl : private Noncopyable
      {
      public:
         virtual ~ThreadLocalImpl() {}

         /// Retrieves the value stored for the calling thread.
         /// @note The return value can be undefined if set() has not be called
         /// on the current thread.
         virtual void* get() = 0;

         /// Sets the value for the calling thread.  After this is called, get()
         /// should return value.
         virtual void set(void* value) = 0;
      };
   }
}

#endif