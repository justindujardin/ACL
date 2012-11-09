//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WAITOBJECTIMPL_H_
#define ACL_PLATFORM_WAITOBJECTIMPL_H_

#include "core/types.h"
#include "core/util/noncopyable.h"
#include "platform/threads/mutex.h"

namespace Platform2
{
   namespace Internal_ 
   {
      /// @ingroup p2threadimpl
      /// Base class for your platforms WaitObject implementation.
      class WaitObjectImpl : private Noncopyable
      {
      public:
         virtual ~WaitObjectImpl() {}

         virtual Threading::Status wait(Mutex *mutex, S32 timeout=-1) = 0;
        
         virtual void signalOne() = 0;
         virtual void signalAll() = 0;
      };
   }
}

#endif
