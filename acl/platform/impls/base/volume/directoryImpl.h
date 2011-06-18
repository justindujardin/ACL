//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_DIRECTORYIMPL_H_
#define ACL_PLATFORM_DIRECTORYIMPL_H_

#include "core/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
namespace Internal_
{
   class DirectoryImpl : private Noncopyable
   {
   public:
      virtual ~DirectoryImpl() {}
      virtual void updateStatus() = 0;
      virtual bool open(const Torque::Path& path) = 0;
      virtual void close() = 0;
      virtual bool read(const Torque::Path& path, Torque::FS::FileNode::Attributes& attributes) = 0;
      
      Torque::FS::FileNode::Status mStatus;
   };
}
}

#endif