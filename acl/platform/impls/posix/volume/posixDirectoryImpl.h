//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_DIRECTORYIMPL_H_
#define TORQUE_PLATFORM2_POSIX_DIRECTORYIMPL_H_

#include "platform2/impls/base/volume/directoryImpl.h"
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
      virtual bool open(const Torque::Path& path);
      virtual void close();
      virtual bool read(const Torque::Path& path, Torque::FS::FileNode::Attributes& attributes);
   
   private:
      DIR* mHandle;
   };
}
}

#endif