//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_THREADIMPL_H_
#define ACL_PLATFORM_WIN32_THREADIMPL_H_

#include "platform/impls/base/threads/threadImpl.h"

namespace Platform2
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

#endif