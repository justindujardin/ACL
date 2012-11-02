//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_DIRECTORYIMPL_H_
#define ACL_PLATFORM_TEST_DIRECTORYIMPL_H_

#include "platform/impls/base/volume/directoryImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class TestDirectoryImpl : public DirectoryImpl
      {
      public:
         virtual void updateStatus()
         {
         }

         virtual bool open(const ACLib::Path& path)
         {
            return false;
         }

         virtual void close()
         {
         }

         virtual bool read(const ACLib::Path& path, ACLib::FS::FileNode::Attributes& attributes)
         {
            return false;
         }
      };
   }
}

#endif
