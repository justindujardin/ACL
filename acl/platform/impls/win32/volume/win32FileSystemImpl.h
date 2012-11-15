//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_WIN32_FILESYSTEMIMPL_H_

#include "platform/impls/base/volume/fileSystemImpl.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class Win32FileSystemImpl : public FileSystemImpl
         {
         public:
            virtual bool getAttributesForPath(FS::FileNode::Attributes& attributes, const Path& path);
            virtual bool createDirectory(const Path& path);
            virtual bool removeDirectory(const Path& path);
            virtual bool removeFile(const Path& path);
            virtual bool move(const Path& from, const Path& to);
         };
      }
   }
}
#endif
