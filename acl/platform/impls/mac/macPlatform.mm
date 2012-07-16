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
      Internal()
      {
      }
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
      DebugStr((const unsigned char*)"DEBUG_BREAK!");
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
   
   
   void MacPlatformObject::outputDebugString(const String& str)
   {
      fprintf(stderr, "%s", str.c_str() );
      fprintf(stderr, "\n" );
      fflush(stderr);
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
