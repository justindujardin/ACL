//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#include <sys/time.h>

#include "platform/impls/mac/macPlatform.h"
#include "platform/impls/base/platformCreator.h"

#include "platform/impls/mac/cocoaUtils.h"
#include "platform/impls/posix/dlibrary/posixDLibraryImpl.h"
#include "platform/impls/mac/threads/macThreadImpl.h"
#include "platform/impls/mac/threads/macMutexImpl.h"
#include "platform/impls/mac/threads/macSemaphoreImpl.h"
#include "platform/impls/mac/threads/macThreadLocalImpl.h"
#include "platform/impls/posix/threads/posixThreadImpl.h"
#include "platform/impls/posix/threads/posixMutexImpl.h"
#include "platform/impls/posix/threads/posixThreadLocalImpl.h"
#include "platform/impls/posix/volume/posixFileSystemImpl.h"
#include "platform/impls/posix/volume/posixFileImpl.h"
#include "platform/impls/posix/volume/posixDirectoryImpl.h"

using namespace ACLib;

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
      mInternal(new Internal)
   {      
      TypeRebind& factory = getProtectedFactory();
      factory.bind<DLibraryImpl>().to<PosixDLibraryImpl>();
      factory.bind<ThreadImpl>().to<PosixThreadImpl>();
      factory.bind<MutexImpl>().to<PosixMutexImpl>();
      factory.bind<SemaphoreImpl>().to<MacSemaphoreImpl>();
      factory.bind<ThreadLocalImpl>().to<MacThreadLocalImpl>();
      
      factory.bind<FileSystemImpl>().to<PosixFileSystemImpl>();
      factory.bind<FileImpl>().to<PosixFileImpl>();
      factory.bind<DirectoryImpl>().to<PosixDirectoryImpl>();
   }
   
   MacPlatformObject::~MacPlatformObject()
   {
      
   }
   
   void MacPlatformObject::init()
   {
//      Con::printf("Math Init:");
//      Con::printf("   Installing Standard C extensions");
//      mInstallLibrary_C();

//      Con::printf(" ");
//      // Set the platform variable for the scripts
//      Con::setVariable( "$platform", "macos" );
//
//      MacConsole::create();
//      MacConsole::enableStatic();
   }
   
   void MacPlatformObject::shutdown()
   {
//      MacConsole::destroy();
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
   
   
   // helper func for getWorkingDirectory
   static inline bool isMainDotCsPresent(NSString* dir)
   { 
      return [[NSFileManager defaultManager] fileExistsAtPath:[dir stringByAppendingPathComponent:@"main.cs"]] == YES;
   }
   
   Path MacPlatformObject::getExecutablePath()
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
      
//      Con::printf("---- %s -----",execCString);
      system(execCString);
      delete[] execCString;
   }
   
   void MacPlatformObject::postQuitMessage(U32 code)
   {
      // We need a window installed for this to properly route through the 
      // standard quit sequence.  In the unit test or dedicated server cases
      // we don't have a window.
      //[NSApp terminate:nil];
//      Process::requestShutdown();
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
   
   bool MacPlatformObject::touchFile(const Path& path)
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
