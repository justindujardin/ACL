//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORUE_CORE_ASSERTIMPL_H_
#define TORUE_CORE_ASSERTIMPL_H_

#ifndef TORQUE_CORE_ASSERT_H_
#include "./assert.h"
#endif

class String;

namespace Torque
{
   class AssertImpl
   {
   public:
      virtual ~AssertImpl() {}
      virtual bool shouldDebugBreak(Assert::Type type, const String& message);
      virtual bool notifyUser(Assert::Type type, const String& title, const String& message);
      virtual void forceShutdown(U32 code);
      virtual void printToConsole(Assert::Type type, const String& title, const String& message);
   };
}

#endif