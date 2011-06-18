//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FILESYSTEMIMPL_H_
#define ACL_PLATFORM_TEST_FILESYSTEMIMPL_H_

#include "platform/impls/base/volume/fileSystemImpl.h"
#include "core/containers/tDictionary.h"

namespace Platform2
{
   namespace Internal_
   {
      class TestFileSystemImpl : public FileSystemImpl
      {
      public:
         UnitTesting::UnitTest* currentTest;

         U32 getAttributesForPathCt;
         U32 removeFileCt;
         U32 removeDirectoryCt;
         U32 createDirectoryCt;

         Map<String, bool> hasAttributesMap;
         Map<String, ACLib::FS::FileNode::Attributes> attributesMap;
         Map<String, bool> createDirectoryMap;
         Map<String, bool> removeDirectoryMap;
         Map<String, bool> removeFileMap;

         TestFileSystemImpl() { currentTest = NULL; getAttributesForPathCt = removeFileCt = removeDirectoryCt = createDirectoryCt = 0; }

         bool getAttributesForPath(ACLib::FS::FileNode::Attributes& attributes, const ACLib::Path& path)
         {
            getAttributesForPathCt++;
            if(!hasAttributesMap.contains(path))
            {
               currentTest->fail(String::ToString("Unexpected path in getAttributesForPath: %s", path.getFullPath().c_str()).c_str());
               return false;
            }
            if(!hasAttributesMap[path])
               return false;

            attributes = attributesMap[path];
            return true;
         }

         bool createDirectory(const ACLib::Path& path)
         {
            createDirectoryCt++;
            if(!createDirectoryMap.contains(path))
            {
               currentTest->fail(String::ToString("Unexpected path in createDirectory: %s", path.getFullPath().c_str()).c_str());
               return false;
            }
            return createDirectoryMap[path];
         }

         bool removeDirectory(const ACLib::Path& path)
         {
            removeDirectoryCt++;
            if(!removeDirectoryMap.contains(path))
            {
               currentTest->fail(String::ToString("Unexpected path in removeDirectory: %s", path.getFullPath().c_str()).c_str());
               return false;
            }
            return true;
         }

         bool removeFile(const ACLib::Path& path)
         {
            removeFileCt++;
            if(!removeFileMap.contains(path))
            {
               currentTest->fail(String::ToString("Unexpected path in removeFile: %s", path.getFullPath().c_str()).c_str());
               return false;
            }
            return true;
         }

         bool move(const ACLib::Path& from, const ACLib::Path& to)
         {
            return false;
         }
      };
   }
}

#endif
