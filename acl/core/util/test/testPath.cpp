//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "unit/testAssertImpl.h"
#include "core/util/path.h"
#include "core/util/str.h"

using namespace UnitTesting;
using namespace Torque;

#define FILELINE String(__FILE__) + ":" + String::ToString(__LINE__) + ": "

CreateUnitTest(CoreUtilPath, "Core/Util/Path")
{
   TestAssertImpl *assertImpl;

   void testStuff()
   {
      // String must end with / otherwise Path will treat Dir2 as a file name
      Path foo("/Dir1/Dir2/");
      test(foo.getFullPath().equal("/Dir1/Dir2"), FILELINE + "getFullPath failed");

      // String must end with / otherwise Path will treat Garbage as a filename and the append path will have
      // no effect
      foo.appendPath("Garbage/");
      test(foo.getFullPath().equal("/Dir1/Dir2/Garbage"), FILELINE + "appendPath failed");      
   }

   void testCompressPath()
   {
      test(Path::CompressPath("foo/smith/../../bar").equal("bar"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/./././.././../bar").equal("bar"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/../bar").equal("foo/bar"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/./bar").equal("foo/smith/bar"), FILELINE + "compress path failed");

      test(Path::CompressPath("foo/smith/../../bar/").equal("bar/"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/./././.././../bar/").equal("bar/"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/../bar/").equal("foo/bar/"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/./bar/").equal("foo/smith/bar/"), FILELINE + "compress path failed");

      test(Path::CompressPath("foo/bar/..").equal("foo"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/bar/../").equal("foo/"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/../../bar/.").equal("bar"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/smith/../../bar/./").equal("bar/"), FILELINE + "compress path failed");

      test(Path::CompressPath("foo//bar/////..").equal("foo"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo\\bar\\..").equal("foo"), FILELINE + "compress path failed");
      
      test(Path::CompressPath("foo\\bar//..").equal("foo"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/./././///\\//./bar/.././///.//\\///./\\/.").equal("foo"), FILELINE + "compress path failed");
      test(Path::CompressPath("foo/\\//\\///\\././.\\.\\/\\//./bar////././.\\.././///.//\\///./\\/.").equal("foo"), FILELINE + "compress path failed");

   }

   void run()
   {
      UseTestAsserts u(this);
      assertImpl = u.getImpl();

      testStuff();

      testCompressPath();
   }
};

