//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILEIMPL_H_
#define ACL_PLATFORM_FILEIMPL_H_

#include "core/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
namespace Internal_
{
   class FileImpl : private Noncopyable
   {
   public:
      virtual ~FileImpl() {}
      
      virtual void updateStatus() = 0;
      virtual U32 getPosition() = 0;
      virtual U32 setPosition(U32 pos, Torque::FS::File::SeekMode mode) = 0;
      virtual bool open(const Torque::Path& path, Torque::FS::File::AccessMode mode) = 0;
      virtual void close() = 0;
      virtual U32 read(void* dst, U32 size) = 0;
      virtual U32 write(const void* src, U32 size) = 0;
      
      Torque::FS::FileNode::Status mStatus;
   };
}
}

#endif