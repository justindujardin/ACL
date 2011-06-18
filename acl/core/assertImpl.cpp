//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/assertImpl.h"
#include "core/util/str.h"

#ifdef WIN32
#include <Windows.h>
#endif

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
      {
#ifdef WIN32
         ::MessageBox( NULL, (LPCWSTR)message.utf16(), (LPCWSTR)title.utf16(), MB_ICONERROR | MB_RETRYCANCEL );
#endif
         return true;
      }

      return true;
   }

   void AssertImpl::forceShutdown(U32 code)
   {
      // Default behavior is to force a crash
      U32* p = NULL;
      *p = 0xdead;
   }

   void AssertImpl::printToConsole(Assert::Type type, const String& title, const String& message)
   {
      // Default behavior is to error unless we're dealing with a warning
      //if (Con::isActive())
      //{
      //   if (type == Assert::Warning)
      //      Con::warnf(ConsoleLogEntry::Assert, "%s: %s", title.c_str(), message.c_str());
      //   else
      //      Con::errorf(ConsoleLogEntry::Assert, "%s: %s", title.c_str(), message.c_str());
      //}
   }
}