//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_TEST_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform/impls/base/volume/fileSystemChangeNotifierImpl.h"

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib
{
   namespace TestPlatform
   {
      class TestFileSystemChangeNotifierImpl : public FileSystemChangeNotifierImpl
      {
      public:
         static FileSystemChangeNotifierImpl* create() { return new TestFileSystemChangeNotifierImpl; }

         virtual bool addNotification(const Path& dir)
         {
            return false;
         }

         virtual bool removeNotification(const Path& dir)
         {
            return false;
         }

         virtual void process() {}
      };
   }
}
#endif
