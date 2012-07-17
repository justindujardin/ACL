//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "acl.h"
#include "core/strings/str.h"
#include "core/containers/tVector.h"
#include "core/strings/stringFunctions.h"

#include <gtest/gtest.h>

// Unicode Strings
#ifdef ACL_OS_WIN32
#include <windows.h>
TEST(StringTest,UnicodeStrings) {
   HWND deskWnd = ::GetForegroundWindow();  

   // get text in UTF16 buffer
   UTF16 unicodeWindowText[256];
   ::GetWindowTextW(deskWnd,(LPWSTR)&unicodeWindowText,256);
   String unicodeWindowTextStr = unicodeWindowText;

   UTF8 windowText[256];
   ::GetWindowTextA(deskWnd,(LPSTR)&windowText,256);
   String windowTextStr = windowText;

   EXPECT_TRUE(windowTextStr==unicodeWindowTextStr);
}
#endif

// Test string constructor
TEST(StringTest,Construct) {
   String str1("Hello World");
   EXPECT_TRUE(str1.c_str() != NULL);
}

// Test string compares
TEST(StringTest,Compare) {
   String str1("Hello World");
   String str2 = String::ToLower(str1);
   // Case sensitive
   EXPECT_FALSE(str1 == str2);
   EXPECT_TRUE(str1 == str1);
   // Case insensitive
   EXPECT_TRUE(str1.equal(str2, String::NoCase));
}

// Make sure strings work with Vector
TEST(StringTest,VectorUsage) {
   // construct/destruct in place
   Vector<String> strVec;
   strVec.push_back("test");
   EXPECT_TRUE(strVec.size() == 1);
   EXPECT_TRUE(strVec[0] == String("test"));

   // increment insert   
   String fox = "Big brown box?";
   strVec.increment();
   strVec.last() = fox;
   EXPECT_TRUE(strVec.last() == fox);
   EXPECT_TRUE(strVec[strVec.size()-1] == fox);
}


TEST(StringTest,EmptyString)
{
  String a;
  String b("");
  String c(" ");
  c = c.substr(0,0);

  EXPECT_TRUE(a.equal(b));
  EXPECT_TRUE(b.equal(a));
  EXPECT_TRUE(b.equal(c));
  EXPECT_TRUE(c.equal(b));
  EXPECT_TRUE(a.equal(c));
  EXPECT_TRUE(c.equal(a));

}

TEST(StringTest,SubStr)
{
  String a("Boo Ya Ya!");

  // bad input cases: asserts in debug, empty string in release
  EXPECT_FALSE(a.substr(0,a.length()+1).equal(""));
  EXPECT_FALSE(a.substr(5,a.length()).equal(""));
  EXPECT_FALSE(a.substr(a.length(),1).equal(""));

  EXPECT_TRUE(a.substr(0,a.length()).equal(a));
  EXPECT_TRUE(a.substr(a.length()-1,0).equal(""));
  EXPECT_TRUE(a.substr(a.length(), 0).equal("")); // index out of range but len is 0, ok
  EXPECT_TRUE(a.substr(3, 3).equal(" Ya"));

  EXPECT_TRUE(a.substr(0).equal(a));
  EXPECT_TRUE(a.substr(a.length()-1).equal("!"));
  EXPECT_TRUE(a.substr(3).equal(" Ya Ya!"));
}

TEST(StringTest,EndsWith)
{
  String a("Boo Ya Ya!");

  EXPECT_TRUE(a.endsWith("Boo Ya Ya!"));
  EXPECT_FALSE(a.endsWith("Ya"));
  EXPECT_TRUE(a.endsWith(""));
  EXPECT_TRUE(a.endsWith("Ya!"));
  EXPECT_TRUE(a.endsWith("ya!"));
  
  String b("");

  EXPECT_FALSE(b.endsWith("Boo Ya Ya!"));
  EXPECT_FALSE(b.endsWith("Ya"));
  EXPECT_TRUE(b.endsWith(""));
  EXPECT_FALSE(b.endsWith("Ya!"));

  String c("C");

  EXPECT_FALSE(c.endsWith("Boo Ya Ya!"));
  EXPECT_TRUE(c.endsWith(""));
  EXPECT_TRUE(c.endsWith("C"));
}

