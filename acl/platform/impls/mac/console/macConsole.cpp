//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/impls/mac/console/macConsole.h"
#include "core/strings/stringFunctions.h"
#include "platform/platform.h"

#include <stdio.h>

using fastdelegate::MakeDelegate;

// TODO: convert this to use ncurses.

MacConsole *gConsole = NULL;

static void macConsoleConsumer(ConsoleLogEntry::Level, const String &line)
{
   if (gConsole)
      gConsole->processConsoleLine(line);
}

void MacConsole::create()
{
   gConsole = new MacConsole();
}

void MacConsole::destroy()
{
   if (gConsole)
   {
      gConsole->enable(false);
      delete gConsole;
   }
   gConsole = NULL;
}

void MacConsole::enableStatic(bool shouldEnable)
{
   if (gConsole)
      gConsole->enable(shouldEnable);
}

void MacConsole::enable(bool enabled)
{
   if (gConsole == NULL) return;

   consoleEnabled = enabled;
}

bool MacConsole::isEnabled()
{
   if ( !gConsole )
      return false;

   return gConsole->consoleEnabled;
}


MacConsole::MacConsole()
{
   consoleEnabled = false;
   clearInBuf();

   Con::getConsumerSignal().notify(&macConsoleConsumer);
}

MacConsole::~MacConsole()
{
   Con::getConsumerSignal().remove(&macConsoleConsumer);
}

void MacConsole::processConsoleLine(const char *consoleLine)
{
   if(consoleEnabled)
   {
      ::printf("%s\n", consoleLine);
      ::fflush(stdout);
   }
}

void MacConsole::clearInBuf()
{
   dMemset(inBuf, 0, Con::MaxLineLength);
   inBufPos=0;
}
