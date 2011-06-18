//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_DIRECTORYIMPL_H_
#define ACL_PLATFORM_DIRECTORYIMPL_H_

#include "core/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
   namespace Internal_
   {
      class DirectoryImpl : private Noncopyable
      {
      public:
         virtual ~DirectoryImpl() {}
         virtual void updateStatus() = 0;
         virtual bool open(const ACLib::Path& path) = 0;
         virtual void close() = 0;
         virtual bool read(const ACLib::Path& path, ACLib::FS::FileNode::Attributes& attributes) = 0;

         ACLib::FS::FileNode::Status mStatus;
      };
   }
}

#endif