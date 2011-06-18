//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_ACL_PLATFORMVOLUME_H_
#define ACL_PLATFORM_ACL_PLATFORMVOLUME_H_

#include "core/volume.h"
#include "core/util/scopedPtr.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
   class PlatformFileSystem : public ACLib::FS::FileSystem, private Noncopyable
   {
   public:
      PlatformFileSystem(const String& volume);
      ~PlatformFileSystem();

      String getTypeStr() const { return "Platform"; }

      ACLib::FS::FileNodeRef resolve(const ACLib::Path& path);
      ACLib::FS::FileNodeRef create(const ACLib::Path& path, ACLib::FS::FileNode::Mode mode);
      bool remove(const ACLib::Path& path);
      bool rename(const ACLib::Path& from, const ACLib::Path& to);
      ACLib::Path mapTo(const ACLib::Path& path);
      ACLib::Path mapFrom(const ACLib::Path& path);

   private:
      struct Internal;
      ACLib::ScopedPtr<Internal> mImpl;
   };

   class PlatformDirectory : public ACLib::FS::Directory, private Noncopyable
   {
   public:
      ~PlatformDirectory();

      ACLib::Path getName() const;
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
      ACLib::ScopedPtr<Internal> mImpl;
   };

   class PlatformFile : public ACLib::FS::File, private Noncopyable
   {
   public:
      ~PlatformFile();

      ACLib::Path getName() const;
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
      ACLib::ScopedPtr<Internal> mImpl;
   };

   class PlatformFileSystemChangeNotifier : public ACLib::FS::FileSystemChangeNotifier, private Noncopyable
   {
      friend class PlatformFileSystem;

      PlatformFileSystemChangeNotifier(ACLib::FS::FileSystem* fs);
      ~PlatformFileSystemChangeNotifier();
   protected:
      void internalProcessOnce();
      bool internalAddNotification(const ACLib::Path& dir);
      bool internalRemoveNotification(const ACLib::Path& dir);
      void onDirChanged(const ACLib::Path& dir);

   private:
      struct Internal;
      ACLib::ScopedPtr<Internal> mImpl;
   };
}

#endif

