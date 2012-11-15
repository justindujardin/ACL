//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "./testAssertImpl.h"
#include <gtest/gtest.h>

namespace ACLib
{
  bool TestAssertImpl::FailOnAssert = true;

  TestAssertImpl::TestAssertImpl() : assertCt(0) 
  {
  }

  bool TestAssertImpl::shouldDebugBreak(ACLib::Assert::Type type, const String& message)
  {
    return false;
  }
  bool TestAssertImpl::notifyUser(ACLib::Assert::Type type, const String& title, const String& message)
  {
    if(TestAssertImpl::FailOnAssert)
      EXPECT_TRUE("Unexpected Assertion in test" == "");
    assertCt++;
    return true;
  }
  bool TestAssertImpl::checkAndReset(U32 expected)
  {
  #ifdef ACL_ENABLE_ASSERTS
        bool ret = assertCt == expected;
  #else
        bool ret = true;
  #endif
        assertCt = 0;
        return ret;
  }
}