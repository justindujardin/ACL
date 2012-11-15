//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_MUTEXIMPL_H_
#define ACL_PLATFORM_POSIX_MUTEXIMPL_H_

#include "platform/impls/base/threads/mutexImpl.h"
#include "core/types.h"
#include <pthread.h>

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class PosixMutexImpl : public MutexImpl
         {
         public:      
            PosixMutexImpl();
            virtual ~PosixMutexImpl();
            virtual bool init();
            virtual Threading::Status lock(bool block);
            virtual Threading::Status unlock();

         private:
            pthread_mutex_t mMutex;
         };
      }
   }
}
#endif
