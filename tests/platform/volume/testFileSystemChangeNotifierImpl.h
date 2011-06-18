//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_TEST_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform/impls/base/volume/fileSystemChangeNotifierImpl.h"

namespace Platform2
{
namespace Internal_
{
   class TestFileSystemChangeNotifierImpl : public FileSystemChangeNotifierImpl
   {
   public:
      static FileSystemChangeNotifierImpl* create() { return new TestFileSystemChangeNotifierImpl; }

      virtual bool addNotification(const Torque::Path& dir)
      {
         return false;
      }

      virtual bool removeNotification(const Torque::Path& dir)
      {
         return false;
      }

      virtual void process() {}
   };
}
}

#endif