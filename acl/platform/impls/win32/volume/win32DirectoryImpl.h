//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
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
      virtual bool open(const Torque::Path& path);
      virtual void close();
      virtual bool read(const Torque::Path& path, Torque::FS::FileNode::Attributes& attributes);

   private:
      HANDLE mHandle;
   };
}
}

#endif