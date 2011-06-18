//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_MUTEXIMPL_H_
#define ACL_PLATFORM_WIN32_MUTEXIMPL_H_

#include <Windows.h>
#include "platform/impls/base/threads/mutexImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class Win32MutexImpl : public MutexImpl
      {
      public:
         Win32MutexImpl();
         virtual ~Win32MutexImpl();
         virtual bool init();
         virtual Threading::Status lock(bool block);
         virtual Threading::Status unlock();

      private:
         HANDLE mMutex;
      };
   }
}

#endif
