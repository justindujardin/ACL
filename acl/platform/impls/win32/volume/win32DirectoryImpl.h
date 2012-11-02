//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_DIRECTORYIMPL_H_
#define ACL_PLATFORM_WIN32_DIRECTORYIMPL_H_

#include "platform/impls/base/volume/directoryImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class Win32DirectoryImpl : public DirectoryImpl
      {
      public:
         Win32DirectoryImpl();
         virtual void updateStatus();
         virtual bool open(const ACLib::Path& path);
         virtual void close();
         virtual bool read(const ACLib::Path& path, ACLib::FS::FileNode::Attributes& attributes);

      private:
         HANDLE mHandle;
      };
   }
}

#endif
