//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <Windows.h>
#include "platform/impls/win32/win32Platform.h"
#include "platform/platformSystemInfo.h"

namespace Platform2
{
   namespace Internal_
   {
      static bool checkCpuidSupport_()
      {
         U32 hasSupport = 0;
#if defined(ACL_SUPPORTS_VC_INLINE_X86_ASM)
         __asm
         {  
            push ebx
               pushfd
               pushfd
               pop eax

               mov ebx, eax
               xor eax, 0x200000
               push eax
               popfd
               pushfd
               pop eax
               cmp eax, ebx
               jz EXIT
               mov hasSupport, 1

EXIT:
            popfd
               pop ebx
         }

#endif
         return hasSupport;
      }

      static String getVendor_()
      {
         // From http://www.intel.com/assets/pdf/appnote/241618.pdf
         char vendor[13] = {'\0'};

#if defined(ACL_SUPPORTS_VC_INLINE_X86_ASM)
         __asm
         {
            xor eax, eax
               cpuid
               mov DWORD PTR vendor, ebx
               mov DWORD PTR vendor+4, edx
               mov DWORD PTR vendor+8, ecx
         }
#endif
         return String(vendor);
      }

      static String getBrandName_()
      {
         // From http://www.intel.com/assets/pdf/appnote/241618.pdf
         char name[48] = {'\0'};
#if defined(ACL_SUPPORTS_VC_INLINE_X86_ASM)
         __asm
         {
            mov eax, 80000000h
               cpuid
               cmp eax, 80000004h
               jb END

               mov eax, 80000002h
               cpuid
               mov DWORD PTR name, eax
               mov DWORD PTR name+4, ebx
               mov DWORD PTR name+8, ecx
               mov DWORD PTR name+12, edx

               mov eax, 80000003h
               cpuid
               mov DWORD PTR name+16, eax
               mov DWORD PTR name+20, ebx
               mov DWORD PTR name+24, ecx
               mov DWORD PTR name+28, edx

               mov eax, 80000004h
               cpuid
               mov DWORD PTR name+32, eax
               mov DWORD PTR name+36, ebx
               mov DWORD PTR name+40, ecx
               mov DWORD PTR name+44, edx
END:
         }
#endif
         return String(name);
      }

      static U32 getSSELevel_()
      {
         U32 properties0 = 0;
         U32 properties1 = 0;
#if defined(ACL_SUPPORTS_VC_INLINE_X86_ASM)
         __asm
         {
            mov eax, 1
               cpuid
               mov properties0, edx
               mov properties1, ecx
         }
#endif
         // From http://www.intel.com/assets/pdf/appnote/241618.pdf
         if(properties1 & BIT(20) || properties1 & BIT(19))
            return 4;
         if(properties1 & BIT(0))
            return 3;
         if(properties0 & BIT(26))
            return 2;
         if(properties0 & BIT(25))
            return 1;

         return 0;
      }

      void Win32PlatformObject::initSystemInfo_(SystemInfo& info)
      {
         OSVERSIONINFOEX vInfo;
         vInfo.dwOSVersionInfoSize = sizeof(vInfo);
         BOOL viSuccess = GetVersionEx((LPOSVERSIONINFO)&vInfo);
         if(viSuccess)
         {
            info.majorVersion = vInfo.dwMajorVersion;
            info.minorVersion = vInfo.dwMinorVersion;
            info.bugfixVersion = vInfo.wServicePackMajor;
            info.fullOsName = String::ToString("Windows %u.%u.%u", info.majorVersion, info.minorVersion, info.bugfixVersion);
         }
         else
         {
            info.majorVersion = info.minorVersion = info.bugfixVersion = -1;
            info.fullOsName = String("Windows ERROR.ERROR.ERROR");
         }

         MEMORYSTATUSEX mInfo;
         mInfo.dwLength = sizeof(mInfo);
         BOOL gmsSuccess = GlobalMemoryStatusEx(&mInfo);
         if(gmsSuccess)
            info.memoryInMB = mInfo.ullTotalPhys / (1024 * 1024);
         else
            info.memoryInMB = 0;

         // See http://support.microsoft.com/kb/888282
         HKEY regKey;
         DWORD regMhz = 0;
         DWORD regType = REG_DWORD;
         DWORD regSize = sizeof(regMhz);
         if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\SYSTEM\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &regKey) == ERROR_SUCCESS)
         {
            if(RegQueryValueExA(regKey, "~Mhz", 0, &regType, (LPBYTE)&regMhz, &regSize) == ERROR_SUCCESS)
               info.Processor.speedInMhz = U32(regMhz);
            else
               info.Processor.speedInMhz = 0;
            RegCloseKey(regKey);
         }
         else
            info.Processor.speedInMhz = 0;

         SYSTEM_INFO sInfo;
         GetSystemInfo(&sInfo);
         info.Processor.numLogicalProcessors = sInfo.dwNumberOfProcessors;

         bool hasCpuid = checkCpuidSupport_();
         if(!hasCpuid)
         {
            info.Processor.sseLevel = 0;
            info.Processor.vendor = String("ERROR: No CPUID");
            info.Processor.family = String("ERROR: No CPUID");
            return;
         }
         info.Processor.vendor = getVendor_();
         info.Processor.family = getBrandName_();
         info.Processor.sseLevel = getSSELevel_();
      }
   }
}
