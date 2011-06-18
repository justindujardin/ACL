//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#include <sys/time.h>

#include "platform2/impls/mac/macPlatform.h"
#include "platform2/impls/base/platformCreator.h"

#include "platform2/windows/display.h"
#include "console/console.h"
#include "platform2/impls/mac/console/macConsole.h"
#include "platform2/impls/mac/cocoaUtils.h"
#include "platform2/impls/mac/fonts/macFontImpl.h"
#include "platform2/impls/posix/dlibrary/posixDLibraryImpl.h"
#include "platform2/impls/mac/threads/macThreadImpl.h"
#include "platform2/impls/mac/threads/macMutexImpl.h"
#include "platform2/impls/mac/threads/macSemaphoreImpl.h"
#include "platform2/impls/mac/threads/macThreadLocalImpl.h"
#include "platform2/impls/mac/windows/macWindowImpl.h"
#include "platform2/impls/mac/windows/macCursorControllerImpl.h"
#include "platform2/impls/mac/windows/macMessageBoxImpl.h"
#include "platform2/impls/mac/windows/macFileDialogImpl.h"
#include "platform2/impls/posix/threads/posixThreadImpl.h"
#include "platform2/impls/posix/threads/posixMutexImpl.h"
#include "platform2/impls/posix/threads/posixThreadLocalImpl.h"
#include "platform2/impls/posix/volume/posixFileSystemImpl.h"
#include "platform2/impls/posix/volume/posixFileImpl.h"
#include "platform2/impls/posix/volume/posixDirectoryImpl.h"
#include "platform2/test/volume/testFileSystemChangeNotifierImpl.h"
#include "platform2/impls/mac/input/macInputDeviceManagerImpl.h"
#include "core/util/journal/process.h"

extern void mInstallLibrary_C();

using namespace Torque;

namespace Platform2
{
namespace Internal_
{
   void InitCreator()
   {
      RegisterPlatform<MacPlatformObject>();
   }
   
   struct MacPlatformObject::Internal
   {
      Internal() : fullscreenDisplay(0), defaultDisplayMode(nil)
      {
      }
      
      CGDirectDisplayID fullscreenDisplay;
      NSDictionary* defaultDisplayMode;
   };
   
   MacPlatformObject::MacPlatformObject() :
      mInternal(new Internal),
      mIsFullscreen(false)
   {      
      TypeRebind& factory = getProtectedFactory();
      factory.bind<WindowImpl>().to<MacWindowImpl>();
      factory.bind<FontImpl>().to<MacFontImpl>();
      factory.bind<DLibraryImpl>().to<PosixDLibraryImpl>();
      factory.bind<ThreadImpl>().to<PosixThreadImpl>();
      factory.bind<MutexImpl>().to<PosixMutexImpl>();
      factory.bind<SemaphoreImpl>().to<MacSemaphoreImpl>();
      factory.bind<ThreadLocalImpl>().to<MacThreadLocalImpl>();
      factory.bind<CursorControllerImpl>().to<MacCursorControllerImpl>();
      factory.bind<InputDeviceManagerImpl>().to<MacInputDeviceManagerImpl>();
      factory.bind<MessageBoxImpl>().to<MacMessageBoxImpl>();
      factory.bind<FileDialogImpl>().to<MacFileDialogImpl>();
      
      factory.bind<FileSystemImpl>().to<PosixFileSystemImpl>();
      factory.bind<FileImpl>().to<PosixFileImpl>();
      factory.bind<DirectoryImpl>().to<PosixDirectoryImpl>();
      factory.bind<FileSystemChangeNotifierImpl>().to<TestFileSystemChangeNotifierImpl>();
   }
   
   MacPlatformObject::~MacPlatformObject()
   {
      
   }
   
   void MacPlatformObject::init()
   {
      Con::printf("Math Init:");
      Con::printf("   Installing Standard C extensions");
      mInstallLibrary_C();

      Con::printf(" ");
      // Set the platform variable for the scripts
      Con::setVariable( "$platform", "macos" );

      MacConsole::create();
      MacConsole::enableStatic();
   }
   
   void MacPlatformObject::shutdown()
   {
      MacConsole::destroy();
   }
   
   void MacPlatformObject::abort(U32 code)
   {
      ::abort();
   }
   
   void MacPlatformObject::debugBreak()
   {
      DebugStr("\pDEBUG_BREAK!");
   }
   
   U32 MacPlatformObject::getRealMilliseconds()
   {
      Nanoseconds durTime = AbsoluteToNanoseconds(UpTime());
      return *(U64*)&durTime/1000000;
   }
   
   void MacPlatformObject::sleep(U32 ms)
   {
      usleep(ms * 1000);
   }
   
   const Vector<Display>& MacPlatformObject::getDisplays()
   {
      static Vector<Display> _displays;
      _displays.clear();
      
      NSArray* displays = [NSScreen screens];      
      NSEnumerator* enumerator = [displays objectEnumerator];
      NSScreen* s;
      while((s = [enumerator nextObject]))
      {
         NSDictionary* d = [s deviceDescription];
         _displays.push_back(Display(CocoaUtils::NSRectToRectI([s frame]), CocoaUtils::NSRectToRectI([s visibleFrame]), NSBitsPerPixelFromDepth([s depth]), (void*)[[d valueForKey:@"NSScreenNumber"] intValue]));
      }
      
      return _displays;
   }
   
   void MacPlatformObject::enterFullscreen(void* display)
   {
      mIsFullscreen = true;
      mInternal->fullscreenDisplay = (CGDirectDisplayID)display;
      mInternal->defaultDisplayMode = (NSDictionary*)CGDisplayCurrentMode(mInternal->fullscreenDisplay);
      CGDisplayCapture(mInternal->fullscreenDisplay);
   }
   
   void MacPlatformObject::setFullscreenResolution(const Point2I& res)
   {
      if(!mIsFullscreen)
         return;
      
      CFDictionaryRef mode = CGDisplayBestModeForParameters(mInternal->fullscreenDisplay, 32, res.x, res.y, NULL);
      CGDisplaySwitchToMode(mInternal->fullscreenDisplay, mode);
   }
   
   RectI MacPlatformObject::getFullscreenBounds()
   {
      if(!mIsFullscreen)
         return RectI(0, 0, 0, 0);
         
      
      const Vector<Display>& displays = getDisplays();
      for(U32 i = 0; i < displays.size(); i++)
      {
         const Display& disp = displays[i];
         if(disp.getId() == (void*)mInternal->fullscreenDisplay)
            return disp.getBounds();
      }
      
      return RectI(0, 0, 0, 0);
   }
   
   void MacPlatformObject::exitFullscreen()
   {
      CGDisplaySwitchToMode(mInternal->fullscreenDisplay, (CFDictionaryRef)mInternal->defaultDisplayMode);
      CGDisplayRelease(mInternal->fullscreenDisplay);
      mInternal->fullscreenDisplay = 0;
      mInternal->defaultDisplayMode = nil;
      mIsFullscreen = false;
   }
   
   // helper func for getWorkingDirectory
   static inline bool isMainDotCsPresent(NSString* dir)
   { 
      return [[NSFileManager defaultManager] fileExistsAtPath:[dir stringByAppendingPathComponent:@"main.cs"]] == YES;
   }
   
   Torque::Path MacPlatformObject::getExecutablePath()
   {
      static String cwd;
      if(cwd.isEmpty())
      {
         NSString* string = [[NSBundle mainBundle] pathForResource:@"main" ofType:@"cs"];
         if(!string)
            string = [[NSBundle mainBundle] bundlePath];
         
         string = [string stringByDeletingLastPathComponent];
         AssertISV(isMainDotCsPresent(string), "MacPlatformObject::getExecutablePath - Failed to find main.cs!");
         cwd = (UTF16*)[string cStringUsingEncoding:NSUTF16StringEncoding];
         cwd += "/";
      }
      
      return cwd;
   }
   
   String MacPlatformObject::getExecutableName()
   { 
      static String name;
      if(name.isEmpty())
         name = [[[[NSBundle mainBundle] bundlePath] lastPathComponent] cStringUsingEncoding:NSUTF16StringEncoding];
      
      return name;
   }
   
   String MacPlatformObject::getUserDataDirectory()
   {
      // application support directory is most in line with the current usages of this function.
      // this may change with later usage
      // perhaps the user data directory should be pref-controlled?
      return (UTF16*)[[@"~/Library/Application Support/" stringByStandardizingPath] cStringUsingEncoding:NSUTF16StringEncoding];
   }
   
   String MacPlatformObject::getUserHomeDirectory()
   {
      return (UTF16*)[[@"~/" stringByStandardizingPath] cStringUsingEncoding:NSUTF16StringEncoding];
   }
   
   String MacPlatformObject::getClipboard()
   {
      NSString* data = [[NSPasteboard generalPasteboard] stringForType:NSStringPboardType];
      if(!data)
         return "";
      
      return (UTF16*)[data cStringUsingEncoding:NSUTF16StringEncoding];
   }
   
   bool MacPlatformObject::setClipboard(const String& text)
   {
      [[NSPasteboard generalPasteboard] declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
      BOOL set = [[NSPasteboard generalPasteboard] setString:CocoaUtils::StringToNSString(text) forType:NSStringPboardType];

      return set == YES;
   }

   void MacPlatformObject::restartInstance()
   {
      // execl() leaves open file descriptors open, that's the main reason it's not
      // used here. We want to start fresh.
      
      // get the path to the torque executable
      CFBundleRef mainBundle =  CFBundleGetMainBundle();
      CFURLRef execURL = CFBundleCopyExecutableURL(mainBundle);
      CFStringRef execString = CFURLCopyFileSystemPath(execURL, kCFURLPOSIXPathStyle);

      // append ampersand so that we can launch without blocking.
      // encase in quotes so that spaces in the path are accepted.
      CFMutableStringRef mut = CFStringCreateMutableCopy(NULL, 0, execString);
      CFStringInsert(mut, 0, CFSTR("\""));
      CFStringAppend(mut, CFSTR("\" & "));
      
      U32 len = CFStringGetMaximumSizeForEncoding(CFStringGetLength(mut), kCFStringEncodingUTF8);
      char *execCString = new char[len+1];
      CFStringGetCString(mut, execCString, len, kCFStringEncodingUTF8);
      execCString[len] = '\0';
      
      Con::printf("---- %s -----",execCString);
      system(execCString);
      delete[] execCString;
   }
   
   void MacPlatformObject::postQuitMessage(U32 code)
   {
      // We need a window installed for this to properly route through the 
      // standard quit sequence.  In the unit test or dedicated server cases
      // we don't have a window.
      //[NSApp terminate:nil];
      Process::requestShutdown();
   }
   
   void MacPlatformObject::outputDebugString(const String& str)
   {
      fprintf(stderr, str );
      fprintf(stderr, "\n" );
      fflush(stderr);
   }
   
   bool MacPlatformObject::openWebBrowser(const String& address)
   {
      return [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:CocoaUtils::StringToNSString(address)]];
   }
   
   bool MacPlatformObject::touchFile(const Torque::Path& path)
   {
      return utimes(path.getFullPath().c_str(), NULL) == 0;
   }
 
   U32 MacPlatformObject::getMathControlState()
   {
      U16 cw;

      __asm__ __volatile__ (
         "fstcw %0\n"
         : "=m" (cw)
         : 
         );
      return cw;
   }
   
   void MacPlatformObject::setMathControlState(U32 state)
   {
      U16 cw = state;

      __asm__ __volatile__ ( 
             "fldcw %0\n"
         : 
         : "m" (cw)
         );
   }
   
   void MacPlatformObject::setMathControlStateKnown()
   {
      U16 cw = 0x27F;
      __asm__ __volatile__ ( 
             "fldcw %0\n"
         : 
         : "m" (cw)
         );
   }
}
}
