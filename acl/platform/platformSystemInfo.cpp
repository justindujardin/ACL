//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
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
      PlatformObject * plat = Platform2::GetPlatform();
      plat->outputDebugString("System Information:");
      plat->outputDebugString(String::ToString("  OS: %s", fullOsName.c_str()));
      plat->outputDebugString(String::ToString("  Major: %i Minor: %i Bugfix: %i", majorVersion, minorVersion, bugfixVersion));
      plat->outputDebugString(String::ToString("  Memory: %i MB", memoryInMB));
      plat->outputDebugString("");
      plat->outputDebugString("Processor Information:");
      plat->outputDebugString(String::ToString("  Vendor: %s", Processor.vendor.c_str()));
      plat->outputDebugString(String::ToString("  Family: %s", Processor.family.c_str()));
      plat->outputDebugString(String::ToString("  Number of logical processors: %i", Processor.numLogicalProcessors));
      plat->outputDebugString(String::ToString("  Speed: %i MHz", Processor.speedInMhz));
      plat->outputDebugString(String::ToString("  SSE: %i", Processor.sseLevel));
      plat->outputDebugString("");
   }
}

