//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/threads/win32ThreadLocalImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      Win32ThreadLocalImpl::Win32ThreadLocalImpl() : mTLSIndex(0)
      {
         mTLSIndex = TlsAlloc();
      }

      Win32ThreadLocalImpl::~Win32ThreadLocalImpl()
      {
         TlsFree(mTLSIndex);
      }

      void* Win32ThreadLocalImpl::get()
      {
         return TlsGetValue(mTLSIndex);
      }

      void Win32ThreadLocalImpl::set(void* value)
      {
         TlsSetValue(mTLSIndex, value);
      }
   }
}