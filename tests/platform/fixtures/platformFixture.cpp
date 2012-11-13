//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "core/assert.h"
#include "core/util/autoPtr.h"
#include "./platformFixture.h"

using fastdelegate::MakeDelegate;

using namespace Platform2;
using namespace Platform2::Internal_;
using namespace ACLib;
using namespace ACLib::FS;
using namespace TestPlatform;

PlatformFixture::PlatformFixture()
{
   threadImplCt = mutexImplCt = semaphoreImplCt = threadLocalImplCt = dlibraryImplCt = fileSystemImplCt = fileImplCt = directoryImplCt = 
      waitObjectImplCt = 0;
   threadImpl = NULL;
   mutexImpl = NULL;
   semaphoreImpl = NULL;
   waitObjectImpl = NULL;
   threadLocalImpl[0] = NULL;
   threadLocalImpl[1] = NULL;
   fileSystemImpl = NULL;
   fileImpl = NULL;
   directoryImpl = NULL;
   dlibraryImpl = NULL;
   mutexValid = true;
   semaphoreValid = true;
   GetPlatform()->pushFactory();
   TypeRebind& factory = GetPlatform()->getProtectedFactory();
   factory.bind<ThreadImpl>().to(MakeDelegate(this, &PlatformFixture::createThread));
   factory.bind<MutexImpl>().to(MakeDelegate(this, &PlatformFixture::createMutex));
   factory.bind<SemaphoreImpl>().to(MakeDelegate(this, &PlatformFixture::createSemaphore));
   factory.bind<ThreadLocalImpl>().to(MakeDelegate(this, &PlatformFixture::createThreadLocal));
   factory.bind<FileSystemImpl>().to(MakeDelegate(this, &PlatformFixture::createFileSystem));
   factory.bind<FileImpl>().to(MakeDelegate(this, &PlatformFixture::createFile));
   factory.bind<DirectoryImpl>().to(MakeDelegate(this, &PlatformFixture::createDirectory));
   factory.bind<DLibraryImpl>().to(MakeDelegate(this, &PlatformFixture::createDLibrary));
   factory.bind<WaitObjectImpl>().to(MakeDelegate(this, &PlatformFixture::createWaitObject));
   factory.bind<FileSystemChangeNotifierImpl>().to(MakeDelegate(&TestFileSystemChangeNotifierImpl::create));
   assert = new TestAssertImpl;
   Assert::Get().pushImpl(assert);

}

PlatformFixture::~PlatformFixture()
{
   GetPlatform()->popFactory();
   Assert::Get().popImpl();
   assert = NULL;
}

WaitObjectImpl* PlatformFixture::createWaitObject() 
{ 
   waitObjectImplCt++; if(!waitObjectImpl) waitObjectImpl = new TestWaitObjectImpl; return waitObjectImpl; 
}

ThreadImpl* PlatformFixture::createThread() 
{ 
   threadImplCt++; if(!threadImpl) threadImpl = new TestThreadImpl; return threadImpl; 
}
MutexImpl* PlatformFixture::createMutex() 
{ 
   mutexImplCt++; if(!mutexImpl) mutexImpl = new TestMutexImpl(mutexValid); return mutexImpl; 
}
SemaphoreImpl* PlatformFixture::createSemaphore() 
{ 
   semaphoreImplCt++; if(!semaphoreImpl) semaphoreImpl = new TestSemaphoreImpl(semaphoreValid); return semaphoreImpl; 
}
ThreadLocalImpl* PlatformFixture::createThreadLocal() 
{ 
   threadLocalImplCt++; 
   if(!threadLocalImpl[0]) 
   {
      threadLocalImpl[0] = new TestThreadLocalImpl; 
      return threadLocalImpl[0]; 
   }
   else if(!threadLocalImpl[1])
   {
      threadLocalImpl[1] = new TestThreadLocalImpl;
      return threadLocalImpl[1];
   }
   return NULL;
}
FileSystemImpl* PlatformFixture::createFileSystem() 
{ 
   fileSystemImplCt++; if(!fileSystemImpl) fileSystemImpl = new TestFileSystemImpl; return fileSystemImpl; 
}
FileImpl* PlatformFixture::createFile() 
{ 
   fileImplCt++; if(!fileImpl) { fileImpl = new TestFileImpl; return fileImpl; } return new TestFileImpl; 
}
DirectoryImpl* PlatformFixture::createDirectory() 
{ 
   directoryImplCt++; if(!directoryImpl) { directoryImpl = new TestDirectoryImpl; return directoryImpl; } return new TestDirectoryImpl; 
}
DLibraryImpl* PlatformFixture::createDLibrary() { 
   dlibraryImplCt++; if(!dlibraryImpl) dlibraryImpl = new TestDLibraryImpl; return dlibraryImpl; 
}
