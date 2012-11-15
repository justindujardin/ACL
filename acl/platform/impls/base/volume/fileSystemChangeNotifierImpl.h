//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform/volume/volume.h"
#include "core/util/noncopyable.h"

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         class FileSystemChangeNotifierImpl : private Noncopyable
         {
         public:
            virtual ~FileSystemChangeNotifierImpl() {}
            virtual bool addNotification(const Path& dir) = 0;
            virtual bool removeNotification(const Path& dir) = 0;
            virtual void process() = 0;

            Delegate<void(const Path& dir)> onDirChanged;
         };
      }
   }
}
#endif

