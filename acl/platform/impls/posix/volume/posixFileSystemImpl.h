//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_FILESYSTEMIMPL_H_
#define TORQUE_PLATFORM2_POSIX_FILESYSTEMIMPL_H_

#include "platform2/impls/base/volume/fileSystemImpl.h"

namespace Platform2
{
namespace Internal_
{
   class PosixFileSystemImpl : public FileSystemImpl
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