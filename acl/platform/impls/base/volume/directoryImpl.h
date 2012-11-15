//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_DIRECTORYIMPL_H_
#define ACL_PLATFORM_DIRECTORYIMPL_H_

#include "platform/volume/volume.h"
#include "core/util/noncopyable.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class DirectoryImpl : private Noncopyable
         {
         public:
            virtual ~DirectoryImpl() {}
            virtual void updateStatus() = 0;
            virtual bool open(const Path& path) = 0;
            virtual void close() = 0;
            virtual bool read(const Path& path, FS::FileNode::Attributes& attributes) = 0;

            FS::FileNode::Status mStatus;
         };
      }
   }
}
#endif
