//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
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

         virtual bool addNotification(const ACLib::Path& dir)
         {
            return false;
         }

         virtual bool removeNotification(const ACLib::Path& dir)
         {
            return false;
         }

         virtual void process() {}
      };
   }
}

#endif
