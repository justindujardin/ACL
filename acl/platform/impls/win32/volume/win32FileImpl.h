//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_FILEIMPL_H_
#define ACL_PLATFORM_WIN32_FILEIMPL_H_

#include "platform/impls/base/volume/fileImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class Win32FileImpl : public FileImpl
      {
      public:
         Win32FileImpl();
         virtual void updateStatus();
         virtual U32 getPosition();
         virtual U32 setPosition(U32 pos, ACLib::FS::File::SeekMode mode);
         virtual bool open(const ACLib::Path& path, ACLib::FS::File::AccessMode mode);
         virtual void close();
         virtual U32 read(void* dst, U32 size);
         virtual U32 write(const void* src, U32 size);

      private:
         HANDLE mHandle;
      };
   }
}

#endif