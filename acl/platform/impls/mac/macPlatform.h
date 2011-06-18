//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_PLATFORM_H_
#define ACL_PLATFORM_MAC_PLATFORM_H_

#include "platform/platform.h"

namespace Platform2
{
   namespace Internal_
   {
      class MacPlatformObject : public PlatformObject
      {
      public:
         MacPlatformObject();
         ~MacPlatformObject();

         virtual void init();
         virtual void shutdown();
         virtual void abort(U32 code);
         virtual void debugBreak();

         virtual U32 getRealMilliseconds();
         virtual void sleep(U32 ms);

         virtual const Vector<Display>& getDisplays();

         virtual void enterFullscreen(void* display);
         virtual bool isFullscreen() { return mIsFullscreen; }
         virtual void setFullscreenResolution(const Point2I& res);
         virtual RectI getFullscreenBounds();
         virtual void exitFullscreen();

         virtual ACLib::Path getExecutablePath();
         virtual String getExecutableName();

         virtual String getUserDataDirectory();
         virtual String getUserHomeDirectory();

         virtual String getClipboard();
         virtual bool setClipboard(const String& text);

         virtual void restartInstance();
         virtual void postQuitMessage(U32 code);
         virtual void outputDebugString(const String& str);
         virtual bool openWebBrowser(const String& address);

         virtual bool touchFile(const ACLib::Path& path);

      protected:
         virtual U32 getMathControlState();
         virtual void setMathControlState(U32 state);
         virtual void setMathControlStateKnown();
         virtual void initSystemInfo_(SystemInfo& systemInfo);

      private:
         struct Internal;
         ACLib::ScopedPtr<Internal> mInternal;
         bool mIsFullscreen;
      };
   }
}

#endif