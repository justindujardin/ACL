//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_TEST_FILESYSTEMIMPL_H_
#define TORQUE_PLATFORM2_TEST_FILESYSTEMIMPL_H_

#include "platform2/impls/base/volume/fileSystemImpl.h"
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
      Map<String, Torque::FS::FileNode::Attributes> attributesMap;
      Map<String, bool> createDirectoryMap;
      Map<String, bool> removeDirectoryMap;
      Map<String, bool> removeFileMap;
      
      TestFileSystemImpl() { currentTest = NULL; getAttributesForPathCt = removeFileCt = removeDirectoryCt = createDirectoryCt = 0; }
      
      bool getAttributesForPath(Torque::FS::FileNode::Attributes& attributes, const Torque::Path& path)
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
      
      bool createDirectory(const Torque::Path& path)
      {
         createDirectoryCt++;
         if(!createDirectoryMap.contains(path))
         {
            currentTest->fail(String::ToString("Unexpected path in createDirectory: %s", path.getFullPath().c_str()).c_str());
            return false;
         }
         return createDirectoryMap[path];
      }
      
      bool removeDirectory(const Torque::Path& path)
      {
         removeDirectoryCt++;
         if(!removeDirectoryMap.contains(path))
         {
            currentTest->fail(String::ToString("Unexpected path in removeDirectory: %s", path.getFullPath().c_str()).c_str());
            return false;
         }
         return true;
      }
      
      bool removeFile(const Torque::Path& path)
      {
         removeFileCt++;
         if(!removeFileMap.contains(path))
         {
            currentTest->fail(String::ToString("Unexpected path in removeFile: %s", path.getFullPath().c_str()).c_str());
            return false;
         }
         return true;
      }
      
      bool move(const Torque::Path& from, const Torque::Path& to)
      {
         return false;
      }
   };
}
}

#endif
