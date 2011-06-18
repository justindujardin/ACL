//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "core/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
namespace Internal_
{
   class FileSystemChangeNotifierImpl : private Noncopyable
   {
   public:
      virtual ~FileSystemChangeNotifierImpl() {}
      virtual bool addNotification(const Torque::Path& dir) = 0;
      virtual bool removeNotification(const Torque::Path& dir) = 0;
      virtual void process() = 0;

      Delegate<void(const Torque::Path& dir)> onDirChanged;
   };
}
}

#endif

