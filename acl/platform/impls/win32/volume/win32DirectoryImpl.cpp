//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <Windows.h>
#include "platform/impls/win32/volume/win32DirectoryImpl.h"

using namespace Torque;
using namespace Torque::FS;

namespace Platform2
{
namespace Internal_
{
   namespace Win32 { extern void _CopyStatAttributes(const WIN32_FIND_DATAW& info, FileNode::Attributes* attr); }
   using namespace Win32;

   Win32DirectoryImpl::Win32DirectoryImpl() : mHandle(0)
   {
   }

   void Win32DirectoryImpl::updateStatus()
   {
      switch (::GetLastError())
      {
      case ERROR_NO_MORE_FILES:     mStatus = FileNode::EndOfFile;        break;
      case ERROR_INVALID_ACCESS:    mStatus = FileNode::AccessDenied;     break;
      case ERROR_PATH_NOT_FOUND:    mStatus = FileNode::NoSuchFile;       break;
      case ERROR_SHARING_VIOLATION: mStatus = FileNode::SharingViolation; break;
      case ERROR_ACCESS_DENIED:     mStatus = FileNode::AccessDenied;     break;
      default:                      mStatus = FileNode::UnknownError;     break;
      }
   }

   bool Win32DirectoryImpl::open( const Path& path )
   {
      return true;
   }

   void Win32DirectoryImpl::close()
   {
      if (mHandle)
      {
         ::FindClose((HANDLE)mHandle);
         mHandle = 0;
      }
   }

   bool Win32DirectoryImpl::read( const Path& path, FileNode::Attributes& attributes )
   {
      WIN32_FIND_DATA info;
      if (!mHandle)
      {
         mHandle = ::FindFirstFileW((LPCWSTR)(path.getFullPath() + "\\*").utf16(), &info);

         if (mHandle == NULL)
            return false;
      }
      else
         if (!::FindNextFileW(mHandle, &info))
            return false;

         // Skip "." and ".." entries
         if (info.cFileName[0] == '.' && (info.cFileName[1] == '\0' ||
            (info.cFileName[1] == '.' && info.cFileName[2] == '\0')))
            return read(path, attributes);

         _CopyStatAttributes(info,&attributes);
         attributes.name = String((UTF16*)info.cFileName);
         return true;
   }
}
}