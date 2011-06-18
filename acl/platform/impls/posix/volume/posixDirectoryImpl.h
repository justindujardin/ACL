//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_DIRECTORYIMPL_H_
#define ACL_PLATFORM_POSIX_DIRECTORYIMPL_H_

#include "platform/impls/base/volume/directoryImpl.h"
#include <dirent.h>

namespace Platform2
{
   namespace Internal_
   {
      class PosixDirectoryImpl : public DirectoryImpl
      {
      public:   
         PosixDirectoryImpl();
         virtual void updateStatus();
         virtual bool open(const ACLib::Path& path);
         virtual void close();
         virtual bool read(const ACLib::Path& path, ACLib::FS::FileNode::Attributes& attributes);

      private:
         DIR* mHandle;
      };
   }
}

#endif