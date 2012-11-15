//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_FILESYSTEMIMPL_H_

#include "core/util/refBase.h"
#include "platform/volume/volume.h"
#include "core/util/noncopyable.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class FileSystemImpl : public StrongRefBase, private Noncopyable
         {
         public:
            virtual ~FileSystemImpl() {}
            virtual bool getAttributesForPath(FS::FileNode::Attributes& attributes, const Path& path) = 0;
            virtual bool createDirectory(const Path& path) = 0;
            virtual bool removeDirectory(const Path& path) = 0;
            virtual bool removeFile(const Path& path) = 0;
            virtual bool move(const Path& from, const Path& to) = 0;
         };
      }
   }
}
#endif
