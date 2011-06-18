//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "platform/platformSystemInfo.h"
#include "core/assert.h"

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
      AssertFatal(false,"Platform system information: Not Implemented");
      //Con::printf("System Information:");
      //Con::printf("  OS: %s", fullOsName.c_str());
      //Con::printf("  Major: %i Minor: %i Bugfix: %i", majorVersion, minorVersion, bugfixVersion);
      //Con::printf("  Memory: %i MB", memoryInMB);
      //Con::printf("");
      //Con::printf("Processor Information:");
      //Con::printf("  Vendor: %s", Processor.vendor.c_str());
      //Con::printf("  Family: %s", Processor.family.c_str());
      //Con::printf("  Number of logical processors: %i", Processor.numLogicalProcessors);
      //Con::printf("  Speed: %i MHz", Processor.speedInMhz);
      //Con::printf("  SSE: %i", Processor.sseLevel);
      //Con::printf("");
   }
}

