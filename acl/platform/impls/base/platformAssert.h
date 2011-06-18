//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_ASSERT_H_
#define ACL_PLATFORM_ASSERT_H_  

#include "core/assertImpl.h"

namespace Platform2
{
namespace Internal_
{
   class PlatformAssertImpl : public Torque::AssertImpl
   {
   public:
      virtual ~PlatformAssertImpl() {}
      virtual void forceShutdown(U32 code);
      virtual bool shouldDebugBreak(Torque::Assert::Type type, const String& message);
      virtual bool notifyUser(Torque::Assert::Type type, const String& title, const String& message);
      static bool askToEnterDebugger(const char* message);
   };
}
}

#endif