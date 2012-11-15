//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_CORE_ASSERTIMPL_H_
#define ACL_CORE_ASSERTIMPL_H_

#ifndef ACL_CORE_ASSERT_H_
#include "core/assert.h"
#endif

namespace ACLib
{
   class String;
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
