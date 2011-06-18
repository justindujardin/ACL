//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_ACL_PLATFORMVOLUME_H_
#define ACL_PLATFORM_ACL_PLATFORMVOLUME_H_

#include "core/volume.h"
#include "core/util/scopedPtr.h"
#include "core/util/noncopyable.h"

namespace Platform2
{
   class PlatformFileSystem : public Torque::FS::FileSystem, private Noncopyable
   {
   public:
      PlatformFileSystem(const String& volume);
      ~PlatformFileSystem();
      
      String getTypeStr() const { return "Platform"; }
      
      Torque::FS::FileNodeRef resolve(const Torque::Path& path);
      Torque::FS::FileNodeRef create(const Torque::Path& path, Torque::FS::FileNode::Mode mode);
      bool remove(const Torque::Path& path);
      bool rename(const Torque::Path& from, const Torque::Path& to);
      Torque::Path mapTo(const Torque::Path& path);
      Torque::Path mapFrom(const Torque::Path& path);
      
   private:
      struct Internal;
      Torque::ScopedPtr<Internal> mImpl;
   };
   
   class PlatformDirectory : public Torque::FS::Directory, private Noncopyable
   {
   public:
      ~PlatformDirectory();
      
      Torque::Path getName() const;
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
      Torque::ScopedPtr<Internal> mImpl;
   };
   
   class PlatformFile : public Torque::FS::File, private Noncopyable
   {
   public:
      ~PlatformFile();
      
      Torque::Path getName() const;
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
      Torque::ScopedPtr<Internal> mImpl;
   };
   
   class PlatformFileSystemChangeNotifier : public Torque::FS::FileSystemChangeNotifier, private Noncopyable
   {
      friend class PlatformFileSystem;

      PlatformFileSystemChangeNotifier(Torque::FS::FileSystem* fs);
      ~PlatformFileSystemChangeNotifier();
   protected:
      void internalProcessOnce();
      bool internalAddNotification(const Torque::Path& dir);
      bool internalRemoveNotification(const Torque::Path& dir);
      void onDirChanged(const Torque::Path& dir);
      
   private:
      struct Internal;
      Torque::ScopedPtr<Internal> mImpl;
   };
}

#endif

