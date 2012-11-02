//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_MUTEXIMPL_H_
#define ACL_PLATFORM_MAC_MUTEXIMPL_H_

#include "platform/impls/base/threads/mutexImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class MacMutexImpl : public MutexImpl
      {
      public:      
         MacMutexImpl();
         virtual ~MacMutexImpl();
         virtual bool init();
         virtual Threading::Status lock(bool block);
         virtual Threading::Status unlock();

      private:
         MPSemaphoreID mSemaphore;
      };
   }
}

#endif
