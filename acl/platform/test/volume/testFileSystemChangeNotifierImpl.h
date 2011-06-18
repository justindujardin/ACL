//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_TEST_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define TORQUE_PLATFORM2_TEST_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform2/impls/base/volume/fileSystemChangeNotifierImpl.h"

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