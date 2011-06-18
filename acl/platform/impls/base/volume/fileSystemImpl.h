//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_FILESYSTEMIMPL_H_

#include "core/util/refBase.h"
#include "core/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
namespace Internal_
{
   class FileSystemImpl : public StrongRefBase, private Noncopyable
   {
   public:
      virtual ~FileSystemImpl() {}
      virtual bool getAttributesForPath(Torque::FS::FileNode::Attributes& attributes, const Torque::Path& path) = 0;
      virtual bool createDirectory(const Torque::Path& path) = 0;
      virtual bool removeDirectory(const Torque::Path& path) = 0;
      virtual bool removeFile(const Torque::Path& path) = 0;
      virtual bool move(const Torque::Path& from, const Torque::Path& to) = 0;
   };
}
}

#endif