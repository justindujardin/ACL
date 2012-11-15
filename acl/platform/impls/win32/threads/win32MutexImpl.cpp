//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/threads/win32MutexImpl.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         Win32MutexImpl::Win32MutexImpl() : mMutex(0)
         {
         }

         Win32MutexImpl::~Win32MutexImpl()
         {
            CloseHandle(mMutex);
         }

         bool Win32MutexImpl::init()
         {
            mMutex = CreateMutex(NULL, FALSE, NULL);
            return mMutex != 0;
         }

         Threading::Status Win32MutexImpl::lock(bool block)
         {
            DWORD status = WaitForSingleObject(mMutex, block ? INFINITE : 0);
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

         Threading::Status Win32MutexImpl::unlock()
         {
            if(ReleaseMutex(mMutex))
               return Threading::Status_NoError;
            return Threading::Status_PlatformError;
         }
      }
   }
}