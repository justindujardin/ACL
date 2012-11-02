//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _MEMVOLUME_H_
#define _MEMVOLUME_H_

#ifndef _VOLUME_H_
#include "platform/volume/volume.h"
#endif

#ifndef _TDICTIONARY_H_
#include "core/containers/tDictionary.h"
#endif 

namespace ACLib
{
   namespace Mem
   {

      struct MemFileData;
      struct MemDirectoryData;
      class MemDirectory;

      //-----------------------------------------------------------------------------
      class MemFileSystem: public ACLib::FS::FileSystem
      {
      public:
         MemFileSystem(String volume);
         ~MemFileSystem();

         String   getTypeStr() const { return "Mem"; }

         ACLib::FS::FileNodeRef resolve(const ACLib::Path& path);
         ACLib::FS::FileNodeRef create(const ACLib::Path& path,ACLib::FS::FileNode::Mode);
         bool remove(const ACLib::Path& path);
         bool rename(const ACLib::Path& from,const ACLib::Path& to);
         ACLib::Path mapTo(const ACLib::Path& path);
         ACLib::Path mapFrom(const ACLib::Path& path);

      private:
         String mVolume;
         MemDirectoryData* mRootDir;

         MemDirectory* getParentDir(const ACLib::Path& path, ACLib::FS::FileNodeRef& parentRef);
      };

      //-----------------------------------------------------------------------------
      /// Mem stdio file access.
      /// This class makes use the fopen, fread and fwrite for buffered io.
      class MemFile: public ACLib::FS::File
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

      class MemDirectory: public ACLib::FS::Directory
      {
      public:
         MemDirectory(MemFileSystem* fs, MemDirectoryData* dir);
         ~MemDirectory();

         ACLib::Path getName() const;
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
