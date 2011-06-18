//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_FILEIMPL_H_
#define ACL_PLATFORM_POSIX_FILEIMPL_H_

#include "platform/impls/base/volume/fileImpl.h"
#include <stdio.h>

namespace Platform2
{
   namespace Internal_
   {
      class PosixFileImpl : public FileImpl
      {
      public:            
         PosixFileImpl();
         virtual void updateStatus();
         virtual U32 getPosition();
         virtual U32 setPosition(U32 pos, ACLib::FS::File::SeekMode mode);
         virtual bool open(const ACLib::Path& path, ACLib::FS::File::AccessMode mode);
         virtual void close();
         virtual U32 read(void* dst, U32 size);
         virtual U32 write(const void* src, U32 size);

      private:
         FILE* mHandle;
      };
   }
}

#endif