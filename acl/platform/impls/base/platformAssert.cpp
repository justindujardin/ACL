//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/impls/base/platformAssert.h"
#include "platform/platform.h"

namespace Platform2
{
   namespace Internal_
   {
      bool PlatformAssertImpl::askToEnterDebugger(const char* message )
      {
         static bool haveAsked = false;
         static bool useDebugger = true;
         if(!haveAsked )
         {
            String messageStr = String::ToString("ACL has encountered an assertion with message\n\n"
               "%s\n\n"
               "Would you like to use the debugger? If you cancel, you won't be asked again.", 
               message);

            //TODO: This
            //Messagebox msg("Use debugger?", messageStr, Messagebox::MBOkCancel, Messagebox::MIWarning);
            //useDebugger = msg.run() == Messagebox::MROk;
            haveAsked = true;
         }
         return useDebugger;
      }

      bool PlatformAssertImpl::shouldDebugBreak(ACLib::Assert::Type type, const String& message)
      {
         if(type != ACLib::Assert::Warning)
            return askToEnterDebugger(message.c_str());

         return false;
      }

      void PlatformAssertImpl::forceShutdown(U32 code)
      {
         GetPlatform()->abort(code);
      }

      bool PlatformAssertImpl::notifyUser(ACLib::Assert::Type type, const String& title, const String& message)
      {
         bool ret = true;
         printToConsole(type, title, message);
         // if not a WARNING pop-up a dialog box
         //      if (type != ACLib::Assert::Warning)
         //      {
         //#ifdef ACL_DEBUG
         //         // In debug versions, allow a retry even for ISVs...
         //         Messagebox msg(title, message, Messagebox::MBRetryCancel, Messagebox::MIQuestion);
         //         ret = msg.run() == Messagebox::MRRetry;
         //#else
         //         Messagebox msg(title, message, Messagebox::MBOk, Messagebox::MIInformation);
         //         msg.run();
         //         ret = false;
         //#endif
         //      }

         return ret;
      }
   }
}
