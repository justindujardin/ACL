//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_CONSOLE_H_
#define ACL_PLATFORM_MAC_CONSOLE_H_

#ifndef _CONSOLE_H_
#include "console/console.h"
#endif
#ifndef _EVENT_H_
#include "platform/event.h"
#endif

class MacConsole
{
private:
   bool consoleEnabled;

   U32   inBufPos;
   char  inBuf[Con::MaxLineLength];

   void clearInBuf();

public:
   static void create();
   static void destroy();
   static bool isEnabled();
   static void enableStatic(bool shouldEnable = true);

   MacConsole();
   ~MacConsole();
   void enable(bool);

   void processConsoleLine(const char *consoleLine);
};

extern MacConsole *gConsole;

#endif
