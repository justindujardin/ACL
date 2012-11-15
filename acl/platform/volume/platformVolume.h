//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_ACL_PLATFORMVOLUME_H_
#define ACL_PLATFORM_ACL_PLATFORMVOLUME_H_

#include "platform/volume/volume.h"
#include "core/util/scopedPtr.h"
#include "core/util/noncopyable.h"

namespace ACLib
{
   namespace Platform
   {
      class PlatformFileSystem : public FS::FileSystem, private Noncopyable
      {
      public:
         PlatformFileSystem(const String& volume);
         ~PlatformFileSystem();

         String getTypeStr() const { return "Platform"; }

         FS::FileNodeRef resolve(const Path& path);
         FS::FileNodeRef create(const Path& path, FS::FileNode::Mode mode);
         bool remove(const Path& path);
         bool rename(const Path& from, const Path& to);
         Path mapTo(const Path& path);
         Path mapFrom(const Path& path);

      private:
         struct Internal;
         ScopedPtr<Internal> mImpl;
      };

      class PlatformDirectory : public FS::Directory, private Noncopyable
      {
      public:
         ~PlatformDirectory();

         Path getName() const;
         Status getStatus() const;
         bool getAttributes(Attributes*);

         bool open();
         bool close();
         bool read(Attributes*);

      private:
         friend class PlatformFileSystem;

         struct Config;

         PlatformDirectory(const Config&);
         U32 calculateChecksum();

         struct Internal;
         ScopedPtr<Internal> mImpl;
      };

      class PlatformFile : public FS::File, private Noncopyable
      {
      public:
         ~PlatformFile();

         Path getName() const;
         Status getStatus() const;
         bool getAttributes(Attributes*);

         U32 getPosition();
         U32 setPosition(U32,SeekMode);

         bool open(AccessMode);
         bool close();

         U32 read(void* dst, U32 size);
         U32 write(const void* src, U32 size);

      private:
         friend class PlatformFileSystem;

         struct Config;

         PlatformFile(const Config&);
         U32 calculateChecksum();

         struct Internal;
         ScopedPtr<Internal> mImpl;
      };

      class PlatformFileSystemChangeNotifier : public FS::FileSystemChangeNotifier, private Noncopyable
      {
         friend class PlatformFileSystem;

         PlatformFileSystemChangeNotifier(FS::FileSystem* fs);
         ~PlatformFileSystemChangeNotifier();
      protected:
         void internalProcessOnce();
         bool internalAddNotification(const Path& dir);
         bool internalRemoveNotification(const Path& dir);
         void onDirChanged(const Path& dir);

      private:
         struct Internal;
         ScopedPtr<Internal> mImpl;
      };
   }
}
#endif

