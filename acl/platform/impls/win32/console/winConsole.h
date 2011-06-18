//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_WINCONSOLE_H_
#define ACL_PLATFORM_WINCONSOLE_H_

#ifndef _ACL_TYPES_H_
#include "core/types/types.h"
#endif

#include <windows.h>

#include "core/util/refBase.h"

#define MAX_CMDS 10


class WinConsole
{
   bool winConsoleEnabled;

   HANDLE stdOut;
   HANDLE stdIn;
   HANDLE stdErr;
   char inbuf[512];
   S32  inpos;
   bool lineOutput;
   char curTabComplete[512];
   S32  tabCompleteStart;
   char rgCmds[MAX_CMDS][512];
   S32  iCmdIndex;

protected:
   void printf(const char *s, ...);

   WinConsole();
   ~WinConsole();

public:

   void process();
   void enable(bool);
   bool isEnabled() { return winConsoleEnabled; }
   void processConsoleLine(const char *consoleLine);

   static WinConsole * get();
};

#endif
