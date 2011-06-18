//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "unit/testAssertImpl.h"
#include "core/util/str.h"

using namespace UnitTesting;
using namespace ACLib;

#define FILELINE String(__FILE__) + ":" + String::ToString(__LINE__) + ": "

CreateUnitTest(CoreUtilString, "Core/Util/String")
{
   void testEmpty()
   {
      String a;
      String b("");
      String c(" ");
      c = c.substr(0,0);

      test(a.equal(b), FILELINE + "borked");
      test(b.equal(a), FILELINE + "borked");
      test(b.equal(c), FILELINE + "borked");
      test(c.equal(b), FILELINE + "borked");
      test(a.equal(c), FILELINE + "borked");
      test(c.equal(a), FILELINE + "borked");

   }

   void testSubStr()
   {
      String a("Boo Ya Ya!");

      // bad input cases: asserts in debug, empty string in release
      assertImpl->failOnAssert = false;
      test(a.substr(0,a.length()+1).equal(""), FILELINE + "borked");
      test(a.substr(5,a.length()).equal(""), FILELINE + "borked");
      test(a.substr(a.length(),1).equal(""), FILELINE + "borked");
      assertImpl->failOnAssert = true;

      test(a.substr(0,a.length()).equal(a), FILELINE + "borked");
      test(a.substr(a.length()-1,0).equal(""), FILELINE + "borked");
      test(a.substr(a.length(), 0).equal(""), FILELINE + "borked"); // index out of range but len is 0, ok
      test(a.substr(3, 3).equal(" Ya"), FILELINE + "borked");

      test(a.substr(0).equal(a), FILELINE + "borked");
      test(a.substr(a.length()-1).equal("!"), FILELINE + "borked");
      test(a.substr(3).equal(" Ya Ya!"), FILELINE + "borked");
   }

   void testEndsWith()
   {
      String a("Boo Ya Ya!");

      test(String::EndsWith(a, "Boo Ya Ya!") != String::NPos, FILELINE + "borked");
      test(String::EndsWith(a, "Ya") == String::NPos, FILELINE + "borked");
      test(String::EndsWith(a, "") != String::NPos, FILELINE + "borked");
      test(String::EndsWith(a, "Ya!") != String::NPos, "borked");
      test(String::EndsWith(a, "ya!", String::NoCase) != String::NPos, FILELINE + "borked");
      test(String::EndsWith(a, "ya!", String::Case) == String::NPos, FILELINE + "borked");
      
      String b("");

      test(String::EndsWith(b, "Boo Ya Ya!") == String::NPos, FILELINE + "borked");
      test(String::EndsWith(b, "Ya") == String::NPos, FILELINE + "borked");
      test(String::EndsWith(b, "") != String::NPos, FILELINE + "borked");
      test(String::EndsWith(b, "Ya!") == String::NPos, FILELINE + "borked");
      test(String::EndsWith(b, "ya!", String::NoCase) == String::NPos, FILELINE + "borked");

      String c("C");

      test(String::EndsWith(c, "Boo Ya Ya!") == String::NPos, FILELINE + "borked");
      test(String::EndsWith(c, "") != String::NPos, FILELINE + "borked");
      test(String::EndsWith(c, "C") != String::NPos, FILELINE + "borked");
      test(String::EndsWith(c, "c", String::NoCase) != String::NPos, FILELINE + "borked");
   }

   void testChompEnd()
   {
      String a("Boo Ya Ya!");

      test (String::ChopEnd(a, "Ya!").equal("Boo Ya "), FILELINE + "borked");
      test (String::ChopEnd(a, "Ya").equal("Boo Ya Ya!"), FILELINE + "borked");
      test (String::ChopEnd(a, "ya!", String::NoCase).equal("Boo Ya "), FILELINE + "borked");
      test (String::ChopEnd(a, "Boo Ya ya!", String::NoCase).equal(""), FILELINE + "borked");
      test (String::ChopEnd(a, "").equal("Boo Ya Ya!"), FILELINE + " borked");
   }

   TestAssertImpl *assertImpl;

   void run()
   {
      UseTestAsserts u(this);
      assertImpl = u.getImpl();

      testEmpty();

      testSubStr();

      testEndsWith();    

      testChompEnd();
   }
};

