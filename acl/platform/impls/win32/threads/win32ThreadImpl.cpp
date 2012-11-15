//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <process.h>
#include <errno.h>
#include "platform/impls/win32/threads/win32ThreadImpl.h"

unsigned int WINAPI threadEntry(void* p)
{
   ACLib::Platform::Internal_::ThreadImpl::CommonThreadEntry(p);
   return 0;
}

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         Win32ThreadImpl::Win32ThreadImpl() : mThread(0)
         {
         }

         Win32ThreadImpl::~Win32ThreadImpl()
         {
            if(mThread)
               CloseHandle(mThread);
         }

         Threading::Status Win32ThreadImpl::start(Param* p)
         {
            // Thread is not inherited by child process, uses default stack size, 
            // and is immediately started.
            mThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &threadEntry, p, 0, NULL));
            if(mThread != NULL)
               return Threading::Status_NoError;
            errno_t err;
            // If we fail to get the error...
            if(_get_errno(&err))
               return Threading::Status_PlatformError;
            switch(err)
            {
            case EAGAIN:
            case EACCES:
               return Threading::Status_Resources;
            default:
               return Threading::Status_PlatformError;

            }
         }
      }
   }
}