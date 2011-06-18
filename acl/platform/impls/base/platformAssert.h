//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_ASSERT_H_
#define ACL_PLATFORM_ASSERT_H_  

#include "core/assertImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class PlatformAssertImpl : public ACLib::AssertImpl
      {
      public:
         virtual ~PlatformAssertImpl() {}
         virtual void forceShutdown(U32 code);
         virtual bool shouldDebugBreak(ACLib::Assert::Type type, const String& message);
         virtual bool notifyUser(ACLib::Assert::Type type, const String& title, const String& message);
         static bool askToEnterDebugger(const char* message);
      };
   }
}

#endif