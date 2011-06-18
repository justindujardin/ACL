//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include <Windows.h>
#include "platform/impls/win32/volume/win32FileImpl.h"

using namespace ACLib;
using namespace ACLib::FS;

namespace Platform2
{
   namespace Internal_
   {
      Win32FileImpl::Win32FileImpl() : mHandle(0)
      {
      }

      void Win32FileImpl::updateStatus()
      {
         switch (::GetLastError())
         {
         case ERROR_INVALID_ACCESS:       mStatus = FileNode::AccessDenied;     break;
         case ERROR_TOO_MANY_OPEN_FILES:  mStatus = FileNode::UnknownError;     break;
         case ERROR_PATH_NOT_FOUND:       mStatus = FileNode::NoSuchFile;       break;
         case ERROR_FILE_NOT_FOUND:       mStatus = FileNode::NoSuchFile;       break;
         case ERROR_SHARING_VIOLATION:    mStatus = FileNode::SharingViolation; break;
         case ERROR_HANDLE_DISK_FULL:     mStatus = FileNode::FileSystemFull;   break;
         case ERROR_ACCESS_DENIED:        mStatus = FileNode::AccessDenied;     break;
         default:                         mStatus = FileNode::UnknownError;     break;
         }
      }

      U32 Win32FileImpl::getPosition()
      {
         return ::SetFilePointer(mHandle,0,0,FILE_CURRENT);
      }

      U32 Win32FileImpl::setPosition( U32 pos, File::SeekMode mode )
      {
         DWORD fmode;
         switch (mode)
         {
         case File::Begin:    fmode = FILE_BEGIN; break;
         case File::Current:  fmode = FILE_CURRENT; break;
         case File::End:      fmode = FILE_END; break;
         default:       fmode = 0; break;
         }

         DWORD realPos = ::SetFilePointer(mHandle,pos,0,fmode);
         if (realPos == INVALID_SET_FILE_POINTER)
         {
            mStatus = FileNode::UnknownError;
            return 0;
         }

         return realPos;
      }

      bool Win32FileImpl::open( const Path& path, File::AccessMode mode )
      {
         struct Mode
         {
            DWORD mode,share,open;
         } Modes[] =
         {
            { GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING }, // Read
            { GENERIC_WRITE,0,CREATE_ALWAYS },              // Write
            { GENERIC_WRITE | GENERIC_READ,0,OPEN_ALWAYS }, // ReadWrite
            { GENERIC_WRITE,0,OPEN_ALWAYS }                 // WriteAppend
         };

         Mode& m = (mode == File::Read)? Modes[0]: (mode == File::Write)? Modes[1]:
            (mode == File::ReadWrite)? Modes[2]: Modes[3];

         mHandle = ::CreateFileW((LPCWSTR)path.getFullPath().utf16(),
            m.mode, m.share,
            NULL, m.open,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
            NULL);

         if(S32(mHandle) == -1)
            mHandle = NULL;

         return mHandle != NULL;
      }

      void Win32FileImpl::close()
      {
         if (mHandle)
         {
            ::CloseHandle(mHandle);
            mHandle = 0;
         }
      }

      U32 Win32FileImpl::read( void* dst, U32 size )
      {
         DWORD bytesRead;
         if (!::ReadFile((HANDLE)mHandle,dst,size,&bytesRead,0))
            updateStatus();
         else if (bytesRead != size)
            mStatus = FileNode::EndOfFile;

         return bytesRead;
      }

      U32 Win32FileImpl::write( const void* src, U32 size )
      {
         DWORD bytesWritten;
         ::WriteFile((HANDLE)mHandle,src,size,&bytesWritten,0);
         return bytesWritten;
      }
   }
}