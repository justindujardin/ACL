//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_WIN32_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform/impls/base/volume/fileSystemChangeNotifierImpl.h"
#include "core/containers/tVector.h"

namespace Platform2
{
namespace Internal_
{
   class Win32FileSystemChangeNotifierImpl : public FileSystemChangeNotifierImpl
   {
   public:
      virtual bool addNotification(const Torque::Path& dir);
      virtual bool removeNotification(const Torque::Path& dir);
      virtual void process();

   private:
      Vector<Torque::Path> mPaths;
      Vector<HANDLE> mHandles;
   };
}
}

#endif
