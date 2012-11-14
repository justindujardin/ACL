//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/assertImpl.h"
#include "core/strings/str.h"
#include "core/strings/stringFunctions.h"

namespace ACLib
{
   bool AssertImpl::shouldDebugBreak(Assert::Type type, const String&)
   {
      return type != Assert::Warning;
   }

   bool AssertImpl::notifyUser(Assert::Type type, const String& title, const String& message)
   {
      printToConsole(type, title, message);
      if(type != Assert::Warning)
         return true;
      return false;
   }

   void AssertImpl::forceShutdown(U32 code)
   {
      // Default behavior is to force a crash
      U32* p = NULL;
      *p = 0xdead;
   }

   void AssertImpl::printToConsole(Assert::Type type, const String& title, const String& message)
   {
      dPrintf("\nASSERTION FAILED: %s: %s\x00", title.c_str(), message.c_str());
   }
}
