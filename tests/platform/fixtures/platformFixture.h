//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FIXTURE_H_
#define ACL_PLATFORM_TEST_FIXTURE_H_


#include "platform/platform.h"

#include "./testAssertImpl.h"

#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "platform/threads/semaphore.h"
#include "platform/dlibrary/dlibrary.h"
#include "platform/volume/platformVolume.h"

#include "./threads/testThreadImpl.h"
#include "./threads/testMutexImpl.h"
#include "./threads/testSemaphoreImpl.h"
#include "./threads/testWaitObjectImpl.h"
#include "./threads/testThreadLocalImpl.h"
#include "./dlibrary/testDLibraryImpl.h"
#include "./volume/testFileSystemImpl.h"
#include "./volume/testFileImpl.h"
#include "./volume/testDirectoryImpl.h"
#include "./volume/testFileSystemChangeNotifierImpl.h"

using namespace Platform2;
using namespace Platform2::Internal_;
using namespace ACLib;
using namespace ACLib::FS;

namespace TestPlatform 
{
   class PlatformFixture
   {
   public:
      PlatformFixture();
      ~PlatformFixture();

      ThreadImpl* createThread();
      MutexImpl* createMutex();
      SemaphoreImpl* createSemaphore();
      ThreadLocalImpl* createThreadLocal(); 
      FileSystemImpl* createFileSystem();
      FileImpl* createFile();
      DirectoryImpl* createDirectory();
      DLibraryImpl* createDLibrary();
      WaitObjectImpl* createWaitObject();

      TestThreadImpl* threadImpl;
      TestMutexImpl* mutexImpl;
      TestSemaphoreImpl* semaphoreImpl;
      TestThreadLocalImpl* threadLocalImpl[2];
      TestWaitObjectImpl* waitObjectImpl;

      TestAssertImpl* assert;

      TestDLibraryImpl* dlibraryImpl;
      TestFileSystemImpl* fileSystemImpl;
      TestFileImpl* fileImpl;
      TestDirectoryImpl* directoryImpl;

      U32 threadImplCt;
      U32 mutexImplCt;
      U32 semaphoreImplCt;
      U32 threadLocalImplCt;
      U32 waitObjectImplCt;
      U32 fileSystemImplCt;
      U32 fileImplCt;
      U32 directoryImplCt;
      U32 dlibraryImplCt;

      bool mutexValid;
      bool semaphoreValid;
   };

}

#endif