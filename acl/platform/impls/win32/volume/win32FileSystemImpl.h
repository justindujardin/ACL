//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_WIN32_FILESYSTEMIMPL_H_

#include "platform/impls/base/volume/fileSystemImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class Win32FileSystemImpl : public FileSystemImpl
      {
      public:
         virtual bool getAttributesForPath(ACLib::FS::FileNode::Attributes& attributes, const ACLib::Path& path);
         virtual bool createDirectory(const ACLib::Path& path);
         virtual bool removeDirectory(const ACLib::Path& path);
         virtual bool removeFile(const ACLib::Path& path);
         virtual bool move(const ACLib::Path& from, const ACLib::Path& to);
      };
   }
}

#endif
