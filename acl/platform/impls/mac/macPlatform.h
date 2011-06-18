//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_MAC_PLATFORM_H_
#define TORQUE_PLATFORM2_MAC_PLATFORM_H_

#include "platform2/platform.h"

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
      
      virtual Torque::Path getExecutablePath();
      virtual String getExecutableName();
      
      virtual String getUserDataDirectory();
      virtual String getUserHomeDirectory();
      
      virtual String getClipboard();
      virtual bool setClipboard(const String& text);
      
      virtual void restartInstance();
      virtual void postQuitMessage(U32 code);
      virtual void outputDebugString(const String& str);
      virtual bool openWebBrowser(const String& address);
      
      virtual bool touchFile(const Torque::Path& path);
      
   protected:
      virtual U32 getMathControlState();
      virtual void setMathControlState(U32 state);
      virtual void setMathControlStateKnown();
      virtual void initSystemInfo_(SystemInfo& systemInfo);
      
   private:
      struct Internal;
      Torque::ScopedPtr<Internal> mInternal;
      bool mIsFullscreen;
   };
}
}

#endif