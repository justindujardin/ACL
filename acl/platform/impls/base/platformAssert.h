//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_ASSERT_H_
#define ACL_PLATFORM_ASSERT_H_

#include "core/assertImpl.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
class PlatformAssertImpl : public AssertImpl {
public:
  virtual ~PlatformAssertImpl() {}
  virtual void forceShutdown(U32 code);
  virtual bool shouldDebugBreak(Assert::Type type, const String &message);
  virtual bool notifyUser(Assert::Type type, const String &title,
                          const String &message);
  static bool askToEnterDebugger(const char *message);
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
