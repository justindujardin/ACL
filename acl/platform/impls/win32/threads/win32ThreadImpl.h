//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_THREADIMPL_H_
#define ACL_PLATFORM_WIN32_THREADIMPL_H_

#include "platform/impls/base/threads/threadImpl.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class Win32ThreadImpl : public ThreadImpl
         {
         public:
            Win32ThreadImpl();
            ~Win32ThreadImpl();
            virtual Threading::Status start(Param* p);

         private:
            HANDLE mThread;
         };
      }
   }
}
#endif
