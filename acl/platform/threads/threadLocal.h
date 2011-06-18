//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_THREADLOCAL_H_
#define ACL_PLATFORM_THREADLOCAL_H_

#include "core/util/scopedPtr.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
   /// @ingroup p2thread
   /// ThreadLocal can store unique data for each thread.
   /// ThreadLocal does not directly depend on any other class in the @ref p2thread
   /// module.
   class ThreadLocal : private Noncopyable
   {
   public:
      ThreadLocal();
      ~ThreadLocal();
      
      /// Returns the value of the thread local on the current thread.
      /// Undefined if called before set() has been called on the current thread.
      void* get();
      
      /// Sets the value for the current thread.
      void set(void* value);
   private:
      /// @cond
      struct Internal;
      Torque::ScopedPtr<Internal> mImpl;
      /// @endcond
   };
}

#endif