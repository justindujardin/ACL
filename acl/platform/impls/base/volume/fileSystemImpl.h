//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_FILESYSTEMIMPL_H_

#include "core/util/refBase.h"
#include "core/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
   namespace Internal_
   {
      class FileSystemImpl : public StrongRefBase, private Noncopyable
      {
      public:
         virtual ~FileSystemImpl() {}
         virtual bool getAttributesForPath(ACLib::FS::FileNode::Attributes& attributes, const ACLib::Path& path) = 0;
         virtual bool createDirectory(const ACLib::Path& path) = 0;
         virtual bool removeDirectory(const ACLib::Path& path) = 0;
         virtual bool removeFile(const ACLib::Path& path) = 0;
         virtual bool move(const ACLib::Path& from, const ACLib::Path& to) = 0;
      };
   }
}

#endif