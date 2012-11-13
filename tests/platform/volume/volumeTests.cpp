//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------


#include "core/strings/str.h"
#include "core/util/delegate.h"
#include "core/util/scopedPtr.h"
#include "platform/platform.h"
#include "core/assert.h"
#include "platform/platform.h"
#include "core/util/autoPtr.h"
#include "platform/volume/platformVolume.h"

#include "../fixtures/platformFixture.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace Platform2;
using namespace TestPlatform;
using namespace ACLib;
using namespace ACLib::FS;

TEST(VolumeBehavior,FileSystem)
{
   ScopedPtr<PlatformFixture> fixture(new PlatformFixture());

   PlatformFileSystem fs("/testing/simpletest");

   // Set up the impl
   fixture->fileSystemImpl->hasAttributesMap["/testing/simpletest/dne"] = false;
   fixture->fileSystemImpl->hasAttributesMap["/testing/simpletest/file"] = true;
   fixture->fileSystemImpl->hasAttributesMap["/testing/simpletest/directory"] = true;

   fixture->fileSystemImpl->removeFileMap["/testing/simpletest/file"] = true;
   fixture->fileSystemImpl->removeDirectoryMap["/testing/simpletest/directory"] = true;

   fixture->fileSystemImpl->createDirectoryMap["/testing/simpletest/fail"] = false;
   fixture->fileSystemImpl->createDirectoryMap["/testing/simpletest/newdirectory"] = true;

   FileNode::Attributes attrs;
   attrs.flags = FileNode::File;
   fixture->fileSystemImpl->attributesMap["/testing/simpletest/file"] = attrs;

   attrs.flags = FileNode::Directory;
   fixture->fileSystemImpl->attributesMap["/testing/simpletest/directory"] = attrs;

   // Specify that mapTo maps a relative path to a full path, and mapFrom maps a full path to a relative path
   EXPECT_TRUE(fs.mapTo("file").getFullPath() == String("/testing/simpletest/file"));//, "Expected that mapTo would map the given path to a full system path");
   EXPECT_TRUE(fs.mapFrom("/testing/simpletest/file").getFullPath() == String("/file"));//, "Expected that mapFrom would map the full system path to a relative path");

   // Specify that attempting to resolve a non-existant file will return NULL
   EXPECT_TRUE(fs.resolve("dne") == 0);//, "Expected that a non-existant file would not be resolveable");
   EXPECT_TRUE(fixture->fileSystemImpl->getAttributesForPathCt == 1);//, "Expected that getAttributesForPath would be called");

   // Specify that attempting to resolve an existing file will return a non-NULL pointer to a File subclass
   FileNodeRef file = fs.resolve("file");
   EXPECT_TRUE(file != 0);//, "Expected that an existing file would be resolveable");
   EXPECT_TRUE(dynamic_cast<File*>(file.getPointer()) != NULL);//, "Expected that a file would resolve to a File subclass");
   EXPECT_TRUE(fixture->fileSystemImpl->getAttributesForPathCt == 2);//, "Expected that getAttributesForPath would be called");

   // Specify that attempting to resolve an existing directory will return a non-NULL pointer to a Directory subclass
   FileNodeRef dir = fs.resolve("directory");
   EXPECT_TRUE(dir != 0);// "Expected that an existing directory would be resolveable");
   EXPECT_TRUE(dynamic_cast<Directory*>(dir.getPointer()) != NULL);//, "Expected that a directory would resolve to a Directory sublcass");
   EXPECT_TRUE(fixture->fileSystemImpl->getAttributesForPathCt == 3);//, "Expected that getAttributesForPath would be called");

   // Specify that attempting to remove a non-existant file will fail and not call into the platform
   EXPECT_TRUE(fs.remove("dne") == false);//, "Expected that attempt to remove non-existant file would fail");
   EXPECT_TRUE(fixture->fileSystemImpl->removeFileCt == 0);//, "Expected that attempt to remove non-existant file would not call removeFile");
   EXPECT_TRUE(fixture->fileSystemImpl->removeDirectoryCt == 0);//, "Expected that attempt to remove non-existant file would not call removeDirectory");

   // Specify that attempting to remove an existing file will succeed and call FileSystemImpl::removeFile
   EXPECT_TRUE(fs.remove("file") == true);//, "Expected that attempt to remove existing file would succeed");
   EXPECT_TRUE(fixture->fileSystemImpl->removeFileCt == 1);//, "Expected that attempt to remove existing file would call removeFile");
   EXPECT_TRUE(fixture->fileSystemImpl->removeDirectoryCt == 0);//, "Expected that attemp to remove existing file would not call removeDirectory");

   // Specify that attempting to remove an existing directory will succeed and call FileSystemImpl::removeDirectory
   EXPECT_TRUE(fs.remove("directory") == true);//, "Expected that attempt to remove existing directory would succeed");
   EXPECT_TRUE(fixture->fileSystemImpl->removeFileCt == 1);//, "Expected that attempt to remove existing directory would not call removeFile");
   EXPECT_TRUE(fixture->fileSystemImpl->removeDirectoryCt == 1);//, "Expected that attempt to remove existing directory would call removeDirectory");

   // Specify that attempting to create a new file will return a non-NULL pointer to a File subclass
   FileNodeRef newFile = fs.create("newfile", FileNode::File);
   EXPECT_TRUE(newFile != 0);//, "Expected that creating a new file would return non-NULL");
   EXPECT_TRUE(dynamic_cast<File*>(newFile.getPointer()) != NULL);//, "Expected that returned object would be a File subclass");

   // Specify that failure to create a new directory will return a NULL pointer
   FileNodeRef newDir = fs.create("fail", FileNode::Directory);
   EXPECT_TRUE(newDir == 0);//, "Expected that failing to create a new directory would return a NULL pointer");
   EXPECT_TRUE(fixture->fileSystemImpl->createDirectoryCt == 1);//, "Expected that attempting to create a new directory would call createDirectory");

   // Specify that successfully creating a new directory will return a non-NULL pointer to a Directory subclass
   newDir = fs.create("newdirectory", FileNode::Directory);
   EXPECT_TRUE(newDir != 0);//, "Expected that succesfully creating a new directory would return a non-NULL pointer");
   EXPECT_TRUE(fixture->fileSystemImpl->createDirectoryCt == 2);//, "Expected that attempting to create a new directory would call createDirectory");
};
