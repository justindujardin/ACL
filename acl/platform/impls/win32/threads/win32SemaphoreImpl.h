//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_SEMAPHOREIMPL_H_
#define ACL_PLATFORM_WIN32_SEMAPHOREIMPL_H_

#include <Windows.h>
#include "platform/impls/base/threads/semaphoreImpl.h"

namespace Platform2
{
namespace Internal_
{
   class Win32SemaphoreImpl : public SemaphoreImpl
   {
   public:
      Win32SemaphoreImpl();
      virtual ~Win32SemaphoreImpl();
      virtual bool init(U32 initialCount, U32 maxCount);
      virtual Threading::Status acquire(bool block);
      virtual Threading::Status release();

   private:
      HANDLE mSemaphore;
   };
}
}

#endif