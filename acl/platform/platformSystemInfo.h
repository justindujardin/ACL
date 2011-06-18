//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_SYSTEMINFO_H_
#define ACL_PLATFORM_SYSTEMINFO_H_

#include "core/types/types.h"
#include "core/util/str.h"

namespace Platform2
{
   /// @ingroup platform
   /// Contains general information about the user's computer.
   struct SystemInfo
   {
      /// Contains general information about the processor(s) in the user's computer.
      struct ProcessorInfo
      {
         S32 speedInMhz; ///< Defaults to -1
         S32 sseLevel; ///< Does not differentiate between SSE 4.1 or 4.2, both reported as '4'.  Does not indicate SSSE3. Defaults to -1
         S32 numLogicalProcessors; ///< This will include Hyper-Thread 'processors'. Defaults to -1
         String vendor; ///< e.g. GenuineIntel, AuthenticAMD, defaults to "Uninitialized"
         String family; ///< e.g. Intel Core 2, AMD Athlon(tm) 64 X2 Dual Core Processor 4200+, defaults to "Uninitialized"
         ProcessorInfo();
      } Processor;

      S32 memoryInMB; ///< Defaults to -1
      S32 majorVersion; ///< e.g. 5 for Windows 2000/XP, 10 for all instances of Mac OS X, defaults to -1
      S32 minorVersion; ///< e.g. 1 for Windows XP (ver 5.1), 4 for Mac OS X 10.4, defaults to -1
      S32 bugfixVersion; ///< ServicePack version on Windows (So, XP ServicePack 2 would put a 2 here), 
      ///< bugfix on OS X (10.4.11 would put an 11 here), defaults to -1

      String fullOsName; ///< e.g. Windows 5.1.3 or Mac OS X 10.5.6, defaults to "Uninitialized"

      void dumpToConsole() const;

      SystemInfo();
   };
}

#endif