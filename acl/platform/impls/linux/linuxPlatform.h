//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_LINUX_PLATFORM_H_
#define ACL_PLATFORM_LINUX_PLATFORM_H_

#include "platform/platform.h"

namespace Platform2
{
   namespace Internal_
   {
      class LinuxPlatformObject : public PlatformObject
      {
      public:
         LinuxPlatformObject();
         ~LinuxPlatformObject();

         virtual void init();
         virtual void shutdown();
         virtual void abort(U32 code);
         virtual void debugBreak();

         virtual U32 getRealMilliseconds();
         virtual void sleep(U32 ms);

         virtual ACLib::Path getExecutablePath();
         virtual String getExecutableName();

         virtual String getUserDataDirectory();
         virtual String getUserHomeDirectory();

         virtual String getClipboard();
         virtual bool setClipboard(const String& text);

         virtual void restartInstance();
         virtual void postQuitMessage(U32 code);
         virtual void outputDebugString(const String& str);

         virtual bool touchFile(const ACLib::Path& path);

      protected:
         virtual U32 getMathControlState();
         virtual void setMathControlState(U32 state);
         virtual void setMathControlStateKnown();

      private:
         virtual void initSystemInfo_(SystemInfo& info);
      };
   }
}

#endif