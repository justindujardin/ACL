//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include <sys/sysctl.h>
#include <CoreServices/CoreServices.h>

#include "core/util/str.h"
#include "platform/impls/mac/macPlatform.h"
#include "platform/platformSystemInfo.h"

// These didn't show up until 10.5, but they're nice to have
#ifndef CPUFAMILY_INTEL_6_23
#define CPUFAMILY_INTEL_6_23 0x78ea4fbc  /* Penryn */
#endif
#ifndef CPUFAMILY_INTEL_6_26
#define CPUFAMILY_INTEL_6_26 0x6b5a4cd2  /* Nehalem */
#endif

namespace Platform2
{
   namespace Internal_
   {   
      U32 getSSELevel_()
      {
         size_t theSize = sizeof( int );
         int extension = 0;

         sysctlbyname( "hw.optional.sse3", &extension, &theSize, NULL, 0 );
         if(extension)
            return 3;

         sysctlbyname( "hw.optional.sse2", &extension, &theSize, NULL, 0 );
         if(extension)
            return 2;

         sysctlbyname( "hw.optional.sse", &extension, &theSize, NULL, 0 );
         if(extension)
            return 1;

         return 0;
      }

      String getCPUFamilyString_(int familyId)
      {
         switch(familyId)
         {
         case CPUFAMILY_POWERPC_G3:
            return "PowerPC G3";
         case CPUFAMILY_POWERPC_G4:
            return "PowerPC G4";
         case CPUFAMILY_POWERPC_G5:
            return "PowerPC G5";
         case CPUFAMILY_INTEL_6_14:
            return "Intel Core";
         case CPUFAMILY_INTEL_6_15:
            return "Intel Core 2";
         case CPUFAMILY_INTEL_6_23:
            return "Intel Penryn";
         case CPUFAMILY_INTEL_6_26:
            return "Intel Nehalem";
         case CPUFAMILY_UNKNOWN:
         default:
            return "Unknown";
         }
      }

      void MacPlatformObject::initSystemInfo_(SystemInfo& systemInfo)
      {
         OSErr err = noErr;

         long  major;
         long  minor;
         long  bugFix;

         err = Gestalt(gestaltSystemVersionMajor, &major);
         err |= Gestalt(gestaltSystemVersionMinor, &minor);
         err |= Gestalt(gestaltSystemVersionBugFix, &bugFix);

         if(err == noErr)
         {
            systemInfo.majorVersion = major;
            systemInfo.minorVersion = minor;
            systemInfo.bugfixVersion = bugFix;
            systemInfo.fullOsName = String::ToString("Mac OS X %i.%i.%i", systemInfo.majorVersion, systemInfo.minorVersion, systemInfo.bugfixVersion);
         }
         else
         {
            systemInfo.majorVersion = systemInfo.minorVersion = systemInfo.bugfixVersion = -1;
            systemInfo.fullOsName = String("MacOS X ERROR.ERROR.ERROR");
         }


         U64 physicalRAM = 0;
         size_t theSize = sizeof(U64);

         sysctlbyname( "hw.physmem", &physicalRAM, &theSize, NULL, 0 );
         systemInfo.memoryInMB = physicalRAM / (1024 * 1024);

         U64 procSpeed;
         theSize = sizeof(U64);
         sysctlbyname( "hw.cpufrequency", &procSpeed, &theSize, NULL, 0 );
         procSpeed = procSpeed / 1000000.0f;

         systemInfo.Processor.speedInMhz = procSpeed;

         systemInfo.Processor.sseLevel = getSSELevel_();

         theSize = sizeof(systemInfo.Processor.numLogicalProcessors);
         sysctlbyname("hw.ncpu", &systemInfo.Processor.numLogicalProcessors, &theSize, NULL, 0);

         char vendor[256] = { '\0' };
         theSize = sizeof(vendor);
         err = sysctlbyname("machdep.cpu.vendor", vendor, &theSize, NULL, 0);
         if(err == noErr)
            systemInfo.Processor.vendor = String(vendor);
         else
            systemInfo.Processor.vendor = String("Unknown");

         int cpufamily;
         theSize = sizeof(int);
         sysctlbyname("hw.cpufamily", &cpufamily, &theSize, NULL, 0);

         systemInfo.Processor.family = getCPUFamilyString_(cpufamily);
      }
   }
}

