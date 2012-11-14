//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/platformSystemInfo.h"
#include "core/assert.h"
#include "platform/platform.h"

namespace Platform2
{
   SystemInfo::SystemInfo() : Processor(), memoryInMB(-1), majorVersion(-1), 
      minorVersion(-1), bugfixVersion(-1), fullOsName("Uninitialized")
   {
   }

   SystemInfo::ProcessorInfo::ProcessorInfo() : speedInMhz(-1), sseLevel(-1), 
      numLogicalProcessors(-1), vendor("Uninitialized"), family("Uninitialized")
   {
   }

   void SystemInfo::dumpToConsole() const
   {
      dPrintf("System Information:\n");
      dPrintf(String::ToString("  OS: %s\n", fullOsName.c_str()));
      dPrintf(String::ToString("  Major: %i Minor: %i Bugfix: %i\n", majorVersion, minorVersion, bugfixVersion));
      dPrintf(String::ToString("  Memory: %i MB\n", memoryInMB));
      dPrintf("\n");
      dPrintf("Processor Information:\n");
      dPrintf(String::ToString("  Vendor: %s\n", Processor.vendor.c_str()));
      dPrintf(String::ToString("  Family: %s\n", Processor.family.c_str()));
      dPrintf(String::ToString("  Number of logical processors: %i\n", Processor.numLogicalProcessors));
      dPrintf(String::ToString("  Speed: %i MHz\n", Processor.speedInMhz));
      dPrintf(String::ToString("  SSE: %i\n", Processor.sseLevel));
      dPrintf("");
   }
}

