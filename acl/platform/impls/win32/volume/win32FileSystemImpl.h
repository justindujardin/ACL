//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
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
      virtual bool getAttributesForPath(Torque::FS::FileNode::Attributes& attributes, const Torque::Path& path);
      virtual bool createDirectory(const Torque::Path& path);
      virtual bool removeDirectory(const Torque::Path& path);
      virtual bool removeFile(const Torque::Path& path);
      virtual bool move(const Torque::Path& from, const Torque::Path& to);
   };
}
}

#endif