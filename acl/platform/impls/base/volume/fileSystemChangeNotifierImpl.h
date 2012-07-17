//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform/volume/volume.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
   namespace Internal_
   {
      class FileSystemChangeNotifierImpl : private Noncopyable
      {
      public:
         virtual ~FileSystemChangeNotifierImpl() {}
         virtual bool addNotification(const ACLib::Path& dir) = 0;
         virtual bool removeNotification(const ACLib::Path& dir) = 0;
         virtual void process() = 0;

         Delegate<void(const ACLib::Path& dir)> onDirChanged;
      };
   }
}

#endif

