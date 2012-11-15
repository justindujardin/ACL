//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_TEST_FILESYSTEMIMPL_H_

#include "platform/impls/base/volume/fileSystemImpl.h"
#include "core/containers/tMap.h"

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib
{
   namespace TestPlatform
   {
      class TestFileSystemImpl : public FileSystemImpl
      {
      public:
         U32 getAttributesForPathCt;
         U32 removeFileCt;
         U32 removeDirectoryCt;
         U32 createDirectoryCt;

         Map<String, bool> hasAttributesMap;
         Map<String, FS::FileNode::Attributes> attributesMap;
         Map<String, bool> createDirectoryMap;
         Map<String, bool> removeDirectoryMap;
         Map<String, bool> removeFileMap;

         TestFileSystemImpl() { getAttributesForPathCt = removeFileCt = removeDirectoryCt = createDirectoryCt = 0; }

         bool getAttributesForPath(FS::FileNode::Attributes& attributes, const Path& path)
         {
            getAttributesForPathCt++;
            if(!hasAttributesMap.contains(path))
            {
               //"Unexpected path in getAttributesForPath"
               EXPECT_TRUE(false); 
               return false;
            }
            if(!hasAttributesMap[path])
               return false;

            attributes = attributesMap[path];
            return true;
         }

         bool createDirectory(const Path& path)
         {
            createDirectoryCt++;
            if(!createDirectoryMap.contains(path))
            {
               //"Unexpected path in createDirectory"
               EXPECT_TRUE(false); 
               return false;
            }
            return createDirectoryMap[path];
         }

         bool removeDirectory(const Path& path)
         {
            removeDirectoryCt++;
            if(!removeDirectoryMap.contains(path))
            {
               //"Unexpected path in removeDirectory"
               EXPECT_TRUE(false);
               return false;
            }
            return true;
         }

         bool removeFile(const Path& path)
         {
            removeFileCt++;
            if(!removeFileMap.contains(path))
            {
               //"Unexpected path in removeFile"
               EXPECT_TRUE(false);              
               return false;
            }
            return true;
         }

         bool move(const Path& from, const Path& to)
         {
            return false;
         }
      };
   }
}
#endif
