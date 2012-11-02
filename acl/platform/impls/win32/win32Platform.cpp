//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/win32Platform.h"
#include "platform/impls/base/platformCreator.h"

#include "platform/impls/win32/threads/win32ThreadImpl.h"
#include "platform/impls/win32/threads/win32MutexImpl.h"
#include "platform/impls/win32/threads/win32ThreadLocalImpl.h"
#include "platform/impls/win32/threads/win32SemaphoreImpl.h"

#include "platform/impls/win32/dlibrary/win32DLibraryImpl.h"

#include "platform/impls/win32/volume/win32FileSystemImpl.h"
#include "platform/impls/win32/volume/win32FileImpl.h"
#include "platform/impls/win32/volume/win32DirectoryImpl.h"
#include "platform/impls/win32/volume/win32FileSystemChangeNotifierImpl.h"

#include "platform/platform.h"

#include "core/strings/unicode.h"
#include "platform/platformSystemInfo.h"

// Grrr... system header is issuing warnings
#pragma warning(push)
#pragma warning(disable:4201)
#include <MMSystem.h>
#pragma warning(pop)

#include <lmcons.h>
#include <shlobj.h>
#include <sys/utime.h>
#include <ShellAPI.h>

#ifndef UNICODE
#  error "ERROR: You must have UNICODE defined in your preprocessor settings (ie, /DUNICODE)!"
#endif

namespace Platform2
{
   namespace Internal_
   {
      void InitCreator()
      {
         RegisterPlatform<Win32PlatformObject>();
      }

      Win32PlatformObject::Win32PlatformObject()
      {
         mUseQPC = QueryPerformanceFrequency((LARGE_INTEGER*)&mQPCFrequency);

         ACLib::TypeRebind& factory = getProtectedFactory();
         factory.bind<ThreadImpl>().to<Win32ThreadImpl>();
         factory.bind<MutexImpl>().to<Win32MutexImpl>();
         factory.bind<ThreadLocalImpl>().to<Win32ThreadLocalImpl>();
         factory.bind<SemaphoreImpl>().to<Win32SemaphoreImpl>();

         factory.bind<DLibraryImpl>().to<Win32DLibraryImpl>();

         factory.bind<FileSystemImpl>().to<Win32FileSystemImpl>();
         factory.bind<FileImpl>().to<Win32FileImpl>();
         factory.bind<DirectoryImpl>().to<Win32DirectoryImpl>();
         factory.bind<FileSystemChangeNotifierImpl>().to<Win32FileSystemChangeNotifierImpl>();
      }

      Win32PlatformObject::~Win32PlatformObject()
      {

      }

      void Win32PlatformObject::init()
      {
         //Con::printf(" ");
         //Con::printf("Initializing platform...");

         // Set the platform variable for the scripts
         //Con::setVariable( "$platform", "windows" );
         //Con::printf("Done");
      }

      void Win32PlatformObject::shutdown()
      {

      }

      void Win32PlatformObject::abort(U32 code)
      {
         ExitProcess(code);
      }

      void Win32PlatformObject::debugBreak()
      {
         DebugBreak();
      }

      U32 Win32PlatformObject::getRealMilliseconds()
      {
         if(mUseQPC)
         {
            U64 qpcCount;
            QueryPerformanceCounter((LARGE_INTEGER*)&qpcCount);
            return (qpcCount * 1000)/mQPCFrequency;
         }

         return (U32)timeGetTime();
      }

      void Win32PlatformObject::sleep(U32 ms)
      {
         Sleep(ms);
      }

      ACLib::Path Win32PlatformObject::getExecutablePath()
      {
         static String cen;
         if (cen.isEmpty())
         {
            WCHAR cen_buf[2048];
            GetModuleFileNameW( NULL, cen_buf, 2047);
            cen = ACLib::PathToPlatform((UTF16*)cen_buf);
            cen = cen.substr(0, cen.find('/', 0, String::Case|String::Right));
         }
         return cen;
      }

      String Win32PlatformObject::getExecutableName()
      {
         static String cen;
         if (cen.isEmpty())
         {
            WCHAR cen_buf[2048];
            GetModuleFileNameW( NULL, cen_buf, 2047);
            cen = ACLib::PathToPlatform((UTF16*)cen_buf);
            String::SizeType nameBegin = cen.find('/', 0, String::Case|String::Right);
            if(nameBegin != String::NPos)
            {
               // Add one to skip the /
               cen = cen.substr(nameBegin + 1);
            }
         }
         return cen;
      }

      String Win32PlatformObject::getUserDataDirectory()
      {
         WCHAR szBuffer[MAX_PATH];
         if(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, szBuffer) != S_OK)
            return "";

         return ACLib::PathToPlatform((UTF16*)szBuffer);
      }

      String Win32PlatformObject::getUserHomeDirectory()
      {
         WCHAR szBuffer[MAX_PATH];
         if(SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szBuffer) != S_OK)
            if(SHGetFolderPathW(NULL, CSIDL_COMMON_DOCUMENTS, NULL, SHGFP_TYPE_CURRENT, szBuffer) != S_OK)
               return "";

         return ACLib::PathToPlatform((UTF16*)szBuffer);
      }

      String Win32PlatformObject::getClipboard()
      {
         HGLOBAL hGlobal;
         LPVOID  pGlobal;

         //make sure we can access the clipboard
         if (!IsClipboardFormatAvailable(CF_TEXT))
            return "";
         if (!OpenClipboard(NULL))
            return "";

         hGlobal = GetClipboardData(CF_TEXT);
         pGlobal = GlobalLock(hGlobal);
         S32 cbLength = strlen((char *)pGlobal);
         String returnBuf((char *)pGlobal);
         GlobalUnlock(hGlobal);
         CloseClipboard();

         //note - this function never returns NULL
         return returnBuf;
      }

      bool Win32PlatformObject::setClipboard( const String& text )
      {
         //make sure we can access the clipboard
         if (!OpenClipboard(NULL))
            return false;

         S32 cbLength = strlen(text);

         HGLOBAL hGlobal;
         LPVOID  pGlobal;

         hGlobal = GlobalAlloc(GHND, cbLength + 1);
         pGlobal = GlobalLock (hGlobal);

         strcpy((char *)pGlobal, text);

         GlobalUnlock(hGlobal);

         EmptyClipboard();
         SetClipboardData(CF_TEXT, hGlobal);
         CloseClipboard();

         return true;
      }

      void Win32PlatformObject::restartInstance()
      {
         STARTUPINFO si;
         PROCESS_INFORMATION pi;

         ZeroMemory( &si, sizeof(si) );
         si.cb = sizeof(si);
         ZeroMemory( &pi, sizeof(pi) );

         const U32 BufSize = 2048;
         TCHAR cen_buf[BufSize] = { '\0' };
         DWORD copiedSize = GetModuleFileName( NULL, cen_buf, BufSize - 1);
         if(copiedSize == BufSize - 1)
         {
            //Con::errorf("Win32PlatformObject::restartInstance - apparently we need more than %d TCHAR's for the file name", BufSize);
            return;
         }

         // Start the child process. 
         if( CreateProcess( cen_buf,
            NULL,            // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi )           // Pointer to PROCESS_INFORMATION structure
            != false )
         {
            WaitForInputIdle( pi.hProcess, 5000 );
            CloseHandle( pi.hProcess );
            CloseHandle( pi.hThread );
         }
      }

      void Win32PlatformObject::postQuitMessage( U32 code )
      {
         PostQuitMessage(code);
      }

      void Win32PlatformObject::outputDebugString( const String& str )
      {
         OutputDebugStringA( str.c_str() );
         OutputDebugStringA( "\n" );
      }

      bool Win32PlatformObject::touchFile(const ACLib::Path& path)
      {
         return utime(path.getFullPath(), 0) != -1;
      }

      U32 Win32PlatformObject::getMathControlState()
      {
         U16 cw;
         _asm
         {
            fstcw cw
         }
         return cw;
      }

      void Win32PlatformObject::setMathControlState( U32 state )
      {
         U16 cw = state;
         _asm
         {
            fldcw cw
         }
      }

      void Win32PlatformObject::setMathControlStateKnown()
      {
         // 0x27F is double-precision rounding
         U16 cw = 0x27F;
         _asm
         {
            fldcw cw
         }
      }
   }

}
