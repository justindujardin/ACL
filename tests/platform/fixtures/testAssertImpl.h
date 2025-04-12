//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_TEST_ASSERTIMPL_H_
#define ACL_TEST_ASSERTIMPL_H_

#include "core/assertImpl.h"

namespace ACLib {
class TestAssertImpl : public AssertImpl {
public:
  static bool FailOnAssert;

  U32 assertCt;

  TestAssertImpl();
  bool shouldDebugBreak(Assert::Type type, const String &message);
  bool notifyUser(Assert::Type type, const String &title,
                  const String &message);
  bool checkAndReset(U32 expected);
};
} // namespace ACLib
#endif