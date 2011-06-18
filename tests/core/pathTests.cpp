//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/util/path.h"
#include "core/util/str.h"

#include <gtest/gtest.h>

using namespace ACLib;

#define FILELINE String(__FILE__) + ":" + String::ToString(__LINE__) + ": "

TEST(PathTest,BasicStuff)
{
   // String must end with / otherwise Path will treat Dir2 as a file name
   Path foo("/Dir1/Dir2/");
   EXPECT_TRUE(foo.getFullPath().equal("/Dir1/Dir2"));

   // String must end with / otherwise Path will treat Garbage as a filename and the append path will have
   // no effect
   foo.appendPath("Garbage/");
   EXPECT_TRUE(foo.getFullPath().equal("/Dir1/Dir2/Garbage"));
}

TEST(PathTest,CompressPath)
{
   EXPECT_TRUE(Path::CompressPath("foo/smith/../../bar").equal("bar"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/./././.././../bar").equal("bar"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/../bar").equal("foo/bar"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/./bar").equal("foo/smith/bar"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/../../bar/").equal("bar/"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/./././.././../bar/").equal("bar/"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/../bar/").equal("foo/bar/"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/./bar/").equal("foo/smith/bar/"));
   EXPECT_TRUE(Path::CompressPath("foo/bar/..").equal("foo"));
   EXPECT_TRUE(Path::CompressPath("foo/bar/../").equal("foo/"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/../../bar/.").equal("bar"));
   EXPECT_TRUE(Path::CompressPath("foo/smith/../../bar/./").equal("bar/"));
   EXPECT_TRUE(Path::CompressPath("foo//bar/////..").equal("foo"));
   EXPECT_TRUE(Path::CompressPath("foo\\bar\\..").equal("foo"));
   EXPECT_TRUE(Path::CompressPath("foo\\bar//..").equal("foo"));
   EXPECT_TRUE(Path::CompressPath("foo/./././///\\//./bar/.././///.//\\///./\\/.").equal("foo"));
   EXPECT_TRUE(Path::CompressPath("foo/\\//\\///\\././.\\.\\/\\//./bar////././.\\.././///.//\\///./\\/.").equal("foo"));
}

