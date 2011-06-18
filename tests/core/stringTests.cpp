//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "acl.h"
#include "core/util/str.h"
#include "core/containers/tVector.h"
#include "core/strings/stringFunctions.h"

#include <gtest/gtest.h>
#include <conio.h>

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