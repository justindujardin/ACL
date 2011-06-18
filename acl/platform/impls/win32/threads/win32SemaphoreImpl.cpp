//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/threads/win32SemaphoreImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      Win32SemaphoreImpl::Win32SemaphoreImpl() : mSemaphore(0)
      {
      }

      Win32SemaphoreImpl::~Win32SemaphoreImpl()
      {
         if(mSemaphore)
            CloseHandle(mSemaphore);
      }

      bool Win32SemaphoreImpl::init(U32 initialCount, U32 maxCount)
      {
         mSemaphore = CreateSemaphore(NULL, initialCount, maxCount, NULL);
         return mSemaphore != 0;
      }

      Threading::Status Win32SemaphoreImpl::acquire(bool block)
      {
         DWORD status = WaitForSingleObject(mSemaphore, block ? INFINITE : 0);
         switch(status)
         {
         case WAIT_OBJECT_0:
            return Threading::Status_NoError;
         case WAIT_TIMEOUT:
            return Threading::Status_Busy;
         default:
            return Threading::Status_PlatformError;
         }
      }

      Threading::Status Win32SemaphoreImpl::release()
      {
         if(ReleaseSemaphore(mSemaphore, 1, NULL))
            return Threading::Status_NoError;
         if(GetLastError() == ERROR_TOO_MANY_POSTS)
            return Threading::Status_Resources;
         return Threading::Status_PlatformError;
      }
   }
}