//-----------------------------------------------------------------------------
// GarageGames Library
// Copyright (c) GarageGames, All Rights Reserved
//-----------------------------------------------------------------------------
#ifndef _MEMVOLUME_H_
#define _MEMVOLUME_H_

#ifndef _VOLUME_H_
#include "core/volume.h"
#endif

#ifndef _TDICTIONARY_H_
#include "core/containers/tDictionary.h"
#endif 

namespace Torque
{
   namespace Mem
   {

      struct MemFileData;
      struct MemDirectoryData;
      class MemDirectory;

      //-----------------------------------------------------------------------------
      class MemFileSystem: public Torque::FS::FileSystem
      {
      public:
         MemFileSystem(String volume);
         ~MemFileSystem();

         String   getTypeStr() const { return "Mem"; }

         Torque::FS::FileNodeRef resolve(const Torque::Path& path);
         Torque::FS::FileNodeRef create(const Torque::Path& path,Torque::FS::FileNode::Mode);
         bool remove(const Torque::Path& path);
         bool rename(const Torque::Path& from,const Torque::Path& to);
         Torque::Path mapTo(const Torque::Path& path);
         Torque::Path mapFrom(const Torque::Path& path);

      private:
         String mVolume;
         MemDirectoryData* mRootDir;

         MemDirectory* getParentDir(const Torque::Path& path, Torque::FS::FileNodeRef& parentRef);
      };

      //-----------------------------------------------------------------------------
      /// Mem stdio file access.
      /// This class makes use the fopen, fread and fwrite for buffered io.
      class MemFile: public Torque::FS::File
      {
      public:
         MemFile(MemFileSystem* fs, MemFileData* fileData);
         virtual ~MemFile();

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
         U32 calculateChecksum();

         MemFileSystem* mFileSystem;
         MemFileData* mFileData;
         Status   mStatus;
         U32 mCurrentPos;

         bool _updateInfo();
         void _updateStatus();
      };


      //-----------------------------------------------------------------------------

      class MemDirectory: public Torque::FS::Directory
      {
      public:
         MemDirectory(MemFileSystem* fs, MemDirectoryData* dir);
         ~MemDirectory();

         Torque::Path getName() const;
         Status getStatus() const;
         bool getAttributes(Attributes*);

         bool open();
         bool close();
         bool read(Attributes*);

      private:
         friend class MemFileSystem;
         MemFileSystem* mFileSystem;
         MemDirectoryData* mDirectoryData;

         U32 calculateChecksum();         
         
         Status   mStatus;
         U32 mSearchIndex;         
      };

   } // Namespace
} // Namespace

#endif