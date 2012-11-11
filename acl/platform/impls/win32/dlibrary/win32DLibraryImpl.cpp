//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/dlibrary/win32DLibraryImpl.h"
#include "core/strings/str.h"

namespace Platform2
{
   namespace Internal_
   {
      Win32DLibraryImpl::Win32DLibraryImpl() : mHandle(0)
      {
      }

      Win32DLibraryImpl::~Win32DLibraryImpl()
      {
      }

      bool Win32DLibraryImpl::init(const String& file)
      {
         mHandle = LoadLibraryW((LPCWSTR)file.utf16());
         return mHandle != 0;
      }

      void* Win32DLibraryImpl::bind(const String& name)
      {
         return GetProcAddress(mHandle, name.c_str());
      }

      void Win32DLibraryImpl::close()
      {
         FreeLibrary(mHandle);
         mHandle = 0;
      }
   }
}
