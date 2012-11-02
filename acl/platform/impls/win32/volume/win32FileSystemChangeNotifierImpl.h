//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WIN32_FILESYSTEMCHANGENOTIFIERIMPL_H_
#define ACL_PLATFORM_WIN32_FILESYSTEMCHANGENOTIFIERIMPL_H_

#include "platform/impls/base/volume/fileSystemChangeNotifierImpl.h"
#include "core/containers/tVector.h"

namespace Platform2
{
   namespace Internal_
   {
      class Win32FileSystemChangeNotifierImpl : public FileSystemChangeNotifierImpl
      {
      public:
         virtual bool addNotification(const ACLib::Path& dir);
         virtual bool removeNotification(const ACLib::Path& dir);
         virtual void process();

      private:
         Vector<ACLib::Path> mPaths;
         Vector<HANDLE> mHandles;
      };
   }
}

#endif
