//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_PLATFORM_H_
#define ACL_PLATFORM_MAC_PLATFORM_H_

#include "platform/platform.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
class MacPlatformObject : public PlatformObject {
public:
  MacPlatformObject();
  ~MacPlatformObject();

  virtual void init();
  virtual void shutdown();
  virtual void abort(U32 code);
  virtual void debugBreak();

  virtual U32 getRealMilliseconds();
  virtual void sleep(U32 ms);

  virtual Path getExecutablePath();
  virtual String getExecutableName();

  virtual String getClipboard();
  virtual bool setClipboard(const String &text);
  virtual void restartInstance();
  virtual void postQuitMessage(U32 code);
  virtual void outputDebugString(const String &str);
  virtual bool openWebBrowser(const String &address);
  virtual bool touchFile(const Path &path);

protected:
  virtual U32 getMathControlState();
  virtual void setMathControlState(U32 state);
  virtual void setMathControlStateKnown();
  virtual void initSystemInfo_(SystemInfo &systemInfo);

private:
  struct Internal;
  ScopedPtr<Internal> mInternal;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
