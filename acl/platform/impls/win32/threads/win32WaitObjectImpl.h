//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_WAITOBJECTIMPL_H_
#define ACL_PLATFORM_WIN32_WAITOBJECTIMPL_H_

#include "platform/impls/base/threads/waitObjectImpl.h"
#include <Windows.h>

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class Win32WaitObjectImpl : public WaitObjectImpl
         {
         public:
            Win32WaitObjectImpl();
            virtual ~Win32WaitObjectImpl();
            virtual Threading::Status wait(S32 timeout = -1);
            virtual void signalOne();
            virtual void signalAll();
         private:
            S32 mWaitQueue;
            /// The semaphore to wait on.
            HANDLE mSemaphore;
         };
      }
   }
}
#endif
