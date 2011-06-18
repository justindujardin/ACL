//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/assert.h"
#include "core/assertImpl.h"
#include "core/containers/tVector.h"
#include "core/strings/stringFunctions.h"
#include "core/util/str.h"
#include <stdarg.h>

namespace ACLib
{
   struct Assert::Internal
   {
      Vector<AssertImpl*> implStack;
      bool processing;

      static const char *smTypeName[3];

      Internal() : processing(false)
      {
      }

      ~Internal()
      {
         for(U32 i = 0; i < (U32)implStack.size(); i++)
            delete implStack[i];

         implStack.clear();
      }
   };

   const char* Assert::Internal::smTypeName[3] = { "Fatal-ISV", "Fatal", "Warning" };
   Assert::Assert() : mInternal(new Internal)
   {
      mInternal->implStack.push_back(new AssertImpl);
   }

   Assert::~Assert()
   {
      delete mInternal;
   }

   bool Assert::processAssert(Type type, const char* filename, U32 lineNumber, const char* function, const char* message)
   {
      // Erm...
      if(mInternal->implStack.empty())
         return true;

      AssertImpl* impl = mInternal->implStack.last();

      // Assert while processing assert = bail
      if(isProcessingAssert())
         impl->forceShutdown(1);

      mInternal->processing = true;
      String title = String::ToString("%s: (%s @ %ld)", Internal::smTypeName[type], filename, lineNumber);
      String fullMessage = String::ToString("In function %s:\n\n%s", function, message);
      if(!impl->notifyUser(type, title, fullMessage))
         impl->forceShutdown(1);

      bool debugBreak = impl->shouldDebugBreak(type, fullMessage);

      mInternal->processing = false;

      return debugBreak;
   }

   bool Assert::isProcessingAssert() const
   {
      return mInternal->processing;
   }

   void Assert::pushImpl(AssertImpl* impl)
   {
      mInternal->implStack.push_back(impl);
   }

   void Assert::popImpl()
   {
      // We delay deletion until after decrement so that if decrement asserts
      // we still have a valid AssertImpl*.
      AssertImpl* toDelete = mInternal->implStack.last();
      mInternal->implStack.decrement();
      delete toDelete;
   }

   Assert& Assert::Get()
   {
      static Assert staticAsserter;
      return staticAsserter;
   }
}

const char* avar(const char *message, ...)
{
   static char buffer[4096];
   va_list args;
   va_start(args, message);
   dVsprintf(buffer, sizeof(buffer), message, args);
   return( buffer );
}