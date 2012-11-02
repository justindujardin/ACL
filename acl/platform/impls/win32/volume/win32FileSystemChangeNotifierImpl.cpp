//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <Windows.h>
#include "platform/impls/win32/volume/win32FileSystemChangeNotifierImpl.h"
#include "core/util/path.h"

using namespace ACLib;
using namespace ACLib::FS;

namespace Platform2
{
   namespace Internal_
   {
      bool Win32FileSystemChangeNotifierImpl::addNotification(const Path& dir)
      {
         HANDLE   changeHandle = ::FindFirstChangeNotificationW( 
            (LPCWSTR)dir.getFullPath().utf16(),      // directory to watch 
            FALSE,                           // do not watch subtree 
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_ATTRIBUTES);  // watch file write changes

         if (changeHandle == INVALID_HANDLE_VALUE || changeHandle == NULL) 
         {
            // TODO:Console
            //Con::errorf("[Win32FileSystemChangeNotifier::internalAddNotification] : failed on [%s] [%d]", dir.getFullPath().c_str(), GetLastError());
            return false; 
         }

         mPaths.push_back( dir );
         mHandles.push_back( changeHandle );

         return true;
      }

      bool Win32FileSystemChangeNotifierImpl::removeNotification(const Path& dir)
      {
         for ( S32 i = 0; i < mPaths.size(); ++i )
         {
            if ( mPaths[i] == dir )
            {
               mPaths.erase( i );
               mHandles.erase( i );
               return true;
            }
         }
         return false;
      }

      void Win32FileSystemChangeNotifierImpl::process()
      {
         U32 numHandles = mHandles.size();
         if ( numHandles == 0 )
            return;

         DWORD dwWaitStatus = WaitForMultipleObjects(numHandles, mHandles.address(), FALSE, 0);
         if ( dwWaitStatus == WAIT_FAILED || dwWaitStatus == WAIT_TIMEOUT )
            return;

         if (dwWaitStatus <= (WAIT_OBJECT_0 + numHandles - 1))
         {
            U32 index = dwWaitStatus;

            // reset our notification
            // NOTE: we do this before letting the volume system check mod times so we don't miss any.
            //    It is going to loop over the files and check their mod time vs. the saved time.
            //    This may result in extra calls to internalNotifyDirChanged(), but it will simpley check mod times again.
            ::FindNextChangeNotification( mHandles[index] );

            onDirChanged( mPaths[index] );
         }
      }
   }
}
