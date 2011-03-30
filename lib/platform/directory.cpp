//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "../core/assert.h"
#include "../core/util/str.h"
#include "../core/util/path.h"
#include "./directory.h"

#ifdef TORQUE_OS_WIN32
#include <windows.h>
#include <tchar.h>
#endif

#define PATH_MAX 1024

namespace Directory
{
   String getCWD()
   {
      UTF16 cwd[PATH_MAX];
#ifdef TORQUE_OS_WIN32
      DWORD res = ::GetCurrentDirectoryW(PATH_MAX, (LPWSTR)cwd);
#endif
      return String(cwd);
   }

   void setCWD(const String & path)
   {
      int res = 0;
#ifdef TORQUE_OS_WIN32
      res = ::SetCurrentDirectoryW((LPCWSTR)path.utf16()) ? 0 : -1;
#endif
   }

   String getExecutablePath()
   {
      String result;
#ifdef TORQUE_OS_WIN32
      WCHAR modFileName[PATH_MAX];
      ZeroMemory(modFileName,PATH_MAX);

      ::GetModuleFileNameW(NULL,modFileName,PATH_MAX);

      WCHAR *tmp;
      if((tmp = wcsrchr(modFileName,'\\')) != NULL)
      {
         tmp++;
         *tmp = 0x00;
      }
      result = String((UTF16*)modFileName);
#endif

      return result;
   }

   void create(const String &path)
   {
      bool success;
#ifdef TORQUE_OS_WIN32
      success = (::CreateDirectoryW((LPCWSTR)path.utf16(), NULL) != FALSE);
#endif
      AssertWarn(success,"Failed to create directory");
   }


   bool exists(const String & path)
   {
#ifdef TORQUE_OS_WIN32
      bool dirExists = false;
      WIN32_FIND_DATA findData;
      HANDLE dirHandle = ::FindFirstFileW((LPCWSTR)path.utf16(),&findData);
      if(!dirHandle)
         return false;

      ::FindClose(dirHandle);
      if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         return true;
#endif
      return false;
   }

   void enumFiles( const String & path, FileNameDelegate fileCallback )
   {
#ifdef TORQUE_OS_WIN32
      // format is "path*.*" path class should ensure proper trailing \ on passed in path
      Torque::Path fullPath(path);
      String searchString( fullPath.getFullPath() + String("/*") );

      WIN32_FIND_DATAW findData;
      HANDLE findFile = FindFirstFileW((LPCWSTR)searchString.utf16(),&findData);
      while(findFile)
      {
         String enumFileName((UTF16*)findData.cFileName);

         if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
         {
            static const String dotFile(".");
            static const String dotDotFile("..");
            if(enumFileName != dotFile && enumFileName != dotDotFile)
            {
               const String outFile(fullPath.getFullPath() + String("/") + enumFileName);
               fileCallback(outFile);
            }
         }
         if(!FindNextFileW(findFile,&findData))
            break;
      }

      FindClose(findFile);
#endif
   }
}

