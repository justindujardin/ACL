//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "platform2/platform.h"
#include "core/assert.h"
#include "core/util/autoPtr.h"

#include "platform2/threads/thread.h"
#include "platform2/threads/mutex.h"
#include "platform2/threads/semaphore.h"
#include "platform2/dlibrary/dlibrary.h"
#include "platform2/volume/platform2Volume.h"

#include "platform2/test/threads/testThreadImpl.h"
#include "platform2/test/threads/testMutexImpl.h"
#include "platform2/test/threads/testSemaphoreImpl.h"
#include "platform2/test/threads/testThreadLocalImpl.h"
#include "platform2/test/dlibrary/testDLibraryImpl.h"
#include "platform2/test/volume/testFileSystemImpl.h"
#include "platform2/test/volume/testFileImpl.h"
#include "platform2/test/volume/testDirectoryImpl.h"
#include "platform2/test/volume/testFileSystemChangeNotifierImpl.h"

using fastdelegate::MakeDelegate;

using namespace Platform2;
using namespace Platform2::Internal_;
using namespace Torque;
using namespace Torque::FS;

   class TestCreator
   {
   public:
      TestCreator()
      {
         threadImplCt = mutexImplCt = semaphoreImplCt = threadLocalImplCt = fontImplCt = dlibraryImplCt = fileSystemImplCt = fileImplCt = directoryImplCt = 
         windowImplCt = 0;
         threadImpl = NULL;
         mutexImpl = NULL;
         semaphoreImpl = NULL;
         threadLocalImpl[0] = NULL;
         threadLocalImpl[1] = NULL;
         fileSystemImpl = NULL;
         fileImpl = NULL;
         directoryImpl = NULL;
         dlibraryImpl = NULL;
         mutexValid = true;
         semaphoreValid = true;
      }
      
      ThreadImpl* createThread() { threadImplCt++; if(!threadImpl) threadImpl = new TestThreadImpl; return threadImpl; }
      MutexImpl* createMutex() { mutexImplCt++; if(!mutexImpl) mutexImpl = new TestMutexImpl(mutexValid); return mutexImpl; }
      SemaphoreImpl* createSemaphore() { semaphoreImplCt++; if(!semaphoreImpl) semaphoreImpl = new TestSemaphoreImpl(semaphoreValid); return semaphoreImpl; }
      ThreadLocalImpl* createThreadLocal() 
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
      FileSystemImpl* createFileSystem() { fileSystemImplCt++; if(!fileSystemImpl) fileSystemImpl = new TestFileSystemImpl; return fileSystemImpl; }
      FileImpl* createFile() { fileImplCt++; if(!fileImpl) { fileImpl = new TestFileImpl; return fileImpl; } return new TestFileImpl; }
      DirectoryImpl* createDirectory() { directoryImplCt++; if(!directoryImpl) { directoryImpl = new TestDirectoryImpl; return directoryImpl; } return new TestDirectoryImpl; }
      DLibraryImpl* createDLibrary() { dlibraryImplCt++; if(!dlibraryImpl) dlibraryImpl = new TestDLibraryImpl; return dlibraryImpl; }
      
      TestThreadImpl* threadImpl;
      TestMutexImpl* mutexImpl;
      TestSemaphoreImpl* semaphoreImpl;
      TestThreadLocalImpl* threadLocalImpl[2];
      
      TestDLibraryImpl* dlibraryImpl;
      TestFileSystemImpl* fileSystemImpl;
      TestFileImpl* fileImpl;
      TestDirectoryImpl* directoryImpl;
      
      U32 threadImplCt;
      U32 mutexImplCt;
      U32 semaphoreImplCt;
      U32 threadLocalImplCt;
      U32 fontImplCt;
      U32 fileSystemImplCt;
      U32 fileImplCt;
      U32 directoryImplCt;
      U32 dlibraryImplCt;
      U32 windowImplCt;
      
      bool mutexValid;
      bool semaphoreValid;
   };
   
   class BehaviorTestFixture
   {
   public:
      TestCreator* creator;
      AssertImpl* oldImpl;
      BehaviorTestFixture(UnitTest* test)
      {
         creator = new TestCreator;
         GetPlatform()->pushFactory();
         TypeRebind& factory = GetPlatform()->getProtectedFactory();
         factory.bind<ThreadImpl>().to(Delegate<ThreadImpl*()>(creator, &TestCreator::createThread));
         factory.bind<MutexImpl>().to(Delegate<MutexImpl*()>(creator, &TestCreator::createMutex));
         factory.bind<SemaphoreImpl>().to(Delegate<SemaphoreImpl*()>(creator, &TestCreator::createSemaphore));
         factory.bind<ThreadLocalImpl>().to(Delegate<ThreadLocalImpl*()>(creator, &TestCreator::createThreadLocal));
         factory.bind<FileSystemImpl>().to(Delegate<FileSystemImpl*()>(creator, &TestCreator::createFileSystem));
         factory.bind<FileImpl>().to(Delegate<FileImpl*()>(creator, &TestCreator::createFile));
         factory.bind<DirectoryImpl>().to(Delegate<DirectoryImpl*()>(creator, &TestCreator::createDirectory));
         factory.bind<DLibraryImpl>().to(Delegate<DLibraryImpl*()>(creator, &TestCreator::createDLibrary));
         factory.bind<FileSystemChangeNotifierImpl>().to(Delegate<FileSystemChangeNotifierImpl*()>(&TestFileSystemChangeNotifierImpl::create));
         
         assertImpl->currentTest = test;
         Assert::Get().pushImpl(assertImpl);
      }
      
      ~BehaviorTestFixture()
      {
         delete creator;
         GetPlatform()->popFactory();
         Assert::Get().popImpl();
      }
   };

CreateUnitTest(TestThreadStart0, "Platform2/Behavior/Thread/StartCallsImplStart")
{
   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }
   
   void run()
   {
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d(this, &TestThreadStart0::dummy);
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      test(testImpl->startCt == 0, "Expected that thread constructor would not call ThreadImpl::start");
      t.start();
      test(testImpl->startCt == 1, "Expected that Thread::start() would call ThreadImpl::start()");
   }
};

CreateUnitTest(TestThreadStart1, "Platform2/Behavior/Thread/StartParrotsImplStart")
{
   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }
   
   void run()
   {
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d(this, &TestThreadStart1::dummy);
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      testImpl->startSuccess = Threading::Status_PlatformError;
      test(t.start() == Threading::Status_PlatformError, "Expected that Thread::start() would fail if ThreadImpl::start() fails");
      testImpl->startSuccess = Threading::Status_NoError;
      test(t.start() == Threading::Status_NoError, "Expected that Thread::start() would succeed if ThreadImpl::start() succeeds");
   }
};

CreateUnitTest(TestThreadStart2, "Platform2/Behavior/Thread/IsRunningParrotsImplStart")
{
   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }
   
   void run()
   {
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d(this, &TestThreadStart2::dummy);
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      testImpl->startSuccess = Threading::Status_PlatformError;
      t.start();
      test(t.isRunning() == false, "Expected that Thread::isRunning() would be false if ThreadImpl::start fails");
      
      testImpl->startSuccess = Threading::Status_NoError;
      t.start();
      // Pretend that we've acquired the gateway semaphore
      TestSemaphoreImpl* testSem = fixture.creator->semaphoreImpl;
      testSem->acquireSuccess = Threading::Status_Busy;
      test(t.isRunning() == true, "Expected that Thread::isRunning() would be true if ThreadImpl::start succeeds");
   }                 
};

CreateUnitTest(TestThreadStart3, "Platform2/Behavior/Thread/StartOnce")
{
   S32 dummy(Thread::MessageQueue&)
   {
      return 0;
   }
   
   void run()
   { 
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d(this, &TestThreadStart3::dummy);
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      testImpl->startSuccess = Threading::Status_NoError;
      test(t.start() == Threading::Status_NoError, "Expected that Thread::start() would succeed if ThreadImpl::start succeds");
      test(t.start() == Threading::Status_Again, "Expected that Thread::start() would fail if called a second time");
   }
};

CreateUnitTest(TestThreadStart4, "Platform2/Behavior/Thread/StartDelegate")
{
   S32 someFunc(Thread::MessageQueue&)
   {
      return 0;
   }
   
   void run()
   {
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d(this, &TestThreadStart4::someFunc);
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      testImpl->startSuccess = Threading::Status_NoError;
      t.start();
      
      test(testImpl->param->d == MakeDelegate(this, &TestThreadStart4::someFunc), 
         "Expected that delegate passed to ThreadImpl::start would be same as delegate passed to Thread::Thread()");
   }
};

CreateUnitTest(TestThreadStart5, "Platform2/Behavior/Thread/InvalidStartDelegate")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d;
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      testImpl->startSuccess = Threading::Status_NoError;
      test(t.start() == Threading::Status_DelegateInvalid, "Expected that thread with empty startdelegate would return DelegateInvalid on start()");
      test(testImpl->startCt == 0, "Expected that thread with invalid startdelegate would not call ThreadImpl::start()");
   }
};

CreateUnitTest(TestThreadReturnCode, "Platform2/Behavior/Thread/ReturnCode")
{
   static const S32 MAGIC_NUMBER = 42;
   S32 someFunc(Thread::MessageQueue&)
   {
      return MAGIC_NUMBER;
   }
   
   void run()
   {
      BehaviorTestFixture fixture(this);
      Thread::StartDelegate d(this, &TestThreadReturnCode::someFunc);
      Thread t(d);
      
      TestThreadImpl* testImpl = fixture.creator->threadImpl;
      testImpl->startSuccess = Threading::Status_NoError;
      t.start();
      testImpl->run();
      test(t.getReturnCode() == MAGIC_NUMBER, "Expected Thread::getReturnCode() would return the same value returned by the delegate");
   }
};

CreateUnitTest(TestSemaphoreConstruct0, "Platform2/Behavior/Semaphore/ConstructorMaxCount")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      TestAssertImpl* assertImpl = fixture.assertImpl;
      assertImpl->failOnAssert = false;
      
      // Specify that it is valid to create a semaphore with max count > 0
      AutoPtr<Semaphore> s(new Semaphore(1, 1));
      TestSemaphoreImpl* testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(0), "Expected that a semaphore created with max count > 0 would not assert");
      
      // Specify that maxcount goes directly to the impl
      test(testImpl->maxCount == 1, "Expected that maxcount > 0 would go directly to impl");
      
      // Reset impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
      
      // Specify that creating a semaphore with max count 0 will assert (boundary)
      s = new Semaphore(0, 0);
      testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(1), "Expected that a semaphore created with max count == 0 would assert");
      
      // Specify that we recover from the assert by setting maxCount to S32_MAX
      test(testImpl->maxCount == S32_MAX, "Expected that maxcount == 0 would be set to S32_MAX");

      // Reset impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
      
      // Specifiy that creating a sempahore with max count < 0 will assert (error)
      s = new Semaphore(0, -15);
      testImpl = fixture.creator->semaphoreImpl;
      // Will also assert because initialCount > max count
      test(assertImpl->checkAndReset(2), "Expected that a semaphore created with max count < 0 would assert");
      
      // Specify that we can recover from an assertion by providing maxCount = S32_MAX if maxCount was given as 0
      test(testImpl->maxCount == S32_MAX, "Expected that maxCount < 0 would be set to S32_MAX");
      
      // Rest impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
   }
};

CreateUnitTest(TestSemaphoreCreator1, "Platform2/Behavior/Semaphore/ConstructorInitialCountVsMaxCount")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      TestAssertImpl* assertImpl = fixture.assertImpl;
      assertImpl->failOnAssert = false;
      
      // Specify that it is valid to create a semaphore with initial count < maxcount
      AutoPtr<Semaphore> s(new Semaphore(1, 2));
      TestSemaphoreImpl* testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(0), "Expected that a semaphore created with initialCount < maxCount would not assert");
      
      // Specify that initialcount goes directly to the impl
      test(testImpl->initialCount == 1, "Expected that initial count < maxCount would go directly to impl");
      
      // Reset impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
      
      // Specify that creating a semaphore with initial count == maxcount is valid (boundary)
      s = new Semaphore(2, 2);
      testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(0), "Expected that a semphoare with initialCount == maxCount would be valid");
      
      // Specify that initialCount goes directly to impl
      test(testImpl->initialCount == 2, "Expected that initial count == maxCount would go directly to impl");

      // Reset impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
      
      // Specifiy that creating a sempahore with initialCount > maxCount will fail
      s = new Semaphore(3, 2);
      testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(1), "Expected that a semaphore created with initialCount > maxCount would assert");
      
      // Specify that we can recover from an assertion by providing initialCount = maxCount
      test(testImpl->initialCount == testImpl->maxCount, "Expected that initialCount == maxCount when given as > maxCount");
      
      // Rest impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
   }
};

CreateUnitTest(TestSemaphoreConstruct2, "Platform2/Behavior/Semaphore/ConstructorInitialCountVs0")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      TestAssertImpl* assertImpl = fixture.assertImpl;
      assertImpl->failOnAssert = false;
      
      // Specify that it is valid to create a semaphore with initial count > 0
      AutoPtr<Semaphore> s(new Semaphore(1, 2));
      TestSemaphoreImpl* testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(0), "Expected that a semaphore created with initialCount > 0 would not assert");
      test(testImpl->initialCount == 1, "Expected that initial count > 0 would go directly to impl");
      
      // Reset impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
      
      // Specify that creating a semaphore with initial count == 0 is valid (boundary)
      s = new Semaphore(0, 2);
      testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(0), "Expected that a semphoare with initialCount == 0 would be valid");
      
      // Specify that initialCount goes directly to impl
      test(testImpl->initialCount == 0, "Expected that initial count == 0 would go directly to impl");
      
      // Reset impl
      s = NULL;
      fixture.creator->semaphoreImpl = new TestSemaphoreImpl(true);
      fixture.creator->threadLocalImpl[0] = NULL;
      
      // Specify that creating a semaphore with initial count < 0 is invalid (error)
      s = new Semaphore(-1, 2);
      testImpl = fixture.creator->semaphoreImpl;
      test(assertImpl->checkAndReset(1), "Expected that a semphoare with initialCount < 0 would assert");
      
      // Specify that we recover by setting initialCount = maxCount
      test(testImpl->initialCount == testImpl->maxCount, "Expected that initial count < 0 would be set to maxCount");
   }
};

CreateUnitTest(TestSemaphoreValid, "Platform2/Behavior/Semaphore/Valid")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      TestAssertImpl* assertImpl = fixture.assertImpl;
      assertImpl->failOnAssert = true;
      
      AutoPtr<Semaphore> s(new Semaphore(1, 1));
      TestSemaphoreImpl* testImpl = fixture.creator->semaphoreImpl;
      
      // Specify that creating a semaphore will immediately call SemaphoreImpl::init
      test(testImpl->initCt == 1, "Expected that Semaphore::Semaphore() would call SemaphoreImpl::init");
      
      // Specify that release should parrot the impl (note that it is the implementation's responsibility to enforce maxCount)
      testImpl->releaseSuccess = Threading::Status_PlatformError;
      test(s->release() == Threading::Status_PlatformError, "Expected that Semaphore::release would parrot the impl");
      test(testImpl->releaseCt == 1, "Expected that Semaphore::release would call SemaphoreImpl::release");
      
      testImpl->releaseSuccess = Threading::Status_NoError;
      test(s->release() == Threading::Status_NoError, "Expected that Semaphore::release would parrot the impl");
      test(testImpl->releaseCt == 2, "Expected that Semaphore::release would call SemaphoreImpl::release");
      
      // Specify that acquire should parrot the impl
      testImpl->acquireSuccess = Threading::Status_PlatformError;
      test(s->acquire() == Threading::Status_PlatformError, "Expected that Semaphore::acquire would parrot the impl");
      test(testImpl->acquireCt == 1, "Expected that Semaphore::acquire would call SemaphoreImpl::acquire");
      
      testImpl->acquireSuccess = Threading::Status_NoError;
      test(s->acquire() == Threading::Status_NoError, "Expected that Semaphore::acquire would parrot the impl");
      test(testImpl->acquireCt == 2, "Expected that Semaphore::acquire would call SemaphoreImpl::acquire");
   }
};

CreateUnitTest(TestSemaphoreInvalid, "Platform2/Behavior/Semaphore/Invalid")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      TestAssertImpl* assertImpl = fixture.assertImpl;
      TestCreator* creator = fixture.creator;
      
      assertImpl->failOnAssert = false;
      creator->semaphoreValid = false;
      
      // Specify that construction asserts if SemaphoreImpl is invalid post-construction
      Semaphore fail(1);
      test(assertImpl->checkAndReset(1), "Expected that Semaphore constructor would assert if SemaphoreImpl failed to init properly");
      
      TestSemaphoreImpl* testImpl = creator->semaphoreImpl;
      testImpl->acquireSuccess = Threading::Status_NoError;
      // Specify that a blocking acquire fails and asserts if SemaphoreImpl is invalid
      test(fail.acquire() == Threading::Status_ObjectInvalid, "Expected that Semaphore::acquire would fail if SemaphoreImpl failed to init properly");
      test(assertImpl->checkAndReset(1), "Expected that Semaphore::acquire would assert if SemaphoreImpl failed to init properly");
      test(testImpl->acquireCt == 0, "Expected that SemaphoreImpl::acquire would not be called if SemaphoreImpl failed to init properly");
      
      // Specify that a non-blocking acquire fails and asserts if SemaphoreImpl is invalid
      test(fail.acquire(false) == Threading::Status_ObjectInvalid, "Expected that non-blocking Semaphore::acquire would fail if SemaphoreImpl failed to init properly");
      test(assertImpl->checkAndReset(1), "Expected that non-blocking Semaphore::acquire would assert if SemaphoreImpl failed to init properly");
      test(testImpl->acquireCt == 0, "Expected that SemaphoreImpl::acquire would not be called if SemaphoreImpl failed to init properly");
      
      // Specify that release asserts if SemaphoreImpl is invalid
      test(fail.release() == Threading::Status_ObjectInvalid, "Expected that Semaphore::release would fail if SemaphoreImpl failed to init properly");
      test(assertImpl->checkAndReset(1), "Expected that Semaphore::release would assert if SemaphoreImpl failed to init properly");
      test(testImpl->releaseCt == 0, "Expected that SemaphoreImpl::release would not be called if SemaphoreImpl failed to init properly");
   }
};

CreateUnitTest(p2_behavior_mutex, "Platform2/Behavior/Mutex")
{
   TestAssertImpl* assertImpl;
   TestMutexImpl* testImpl;
   
   void reset()
   {
      testImpl->lockCt = testImpl->unlockCt = 0;
   }
   
   void run()
   {
      BehaviorTestFixture fixture(this);
      assertImpl = fixture.assertImpl;
         
      Mutex m;
      
      // A mutex impl should be created
      test(fixture.creator->mutexImplCt == 1, "Expected one mutex impl to be created");
      
      testImpl = fixture.creator->mutexImpl;
      
      // init should be called on it
      test(testImpl->initCt == 1, "Expected that init would be called on MutexImpl");
      
      testImpl->lockSuccess = Threading::Status_NoError;
      
      // Basic coverage
      test(m.lock() == Threading::Status_NoError, "Expected that mutex would return true when impl returns true");
      test(testImpl->lockCt == 1, "Expected that mutex would call MutexImpl::lock() once");
      m.unlock();
      test(testImpl->unlockCt == 1, "Expected that mutex would call MutexImpl::unlock() once");
      
      reset();
      
      testImpl->lockSuccess = Threading::Status_PlatformError;
      test(m.lock() == Threading::Status_PlatformError, "Expected that mutex would return false when impl returns false");
      reset();
      
#ifndef TORQUE_DEBUG_THREADING
      // Nothing more to cover
      return;
#else
      
      testImpl->lockSuccess = Threading::Status_NoError;
      // Specify that we assert when attempting to lock more than once on the same thread
      assertImpl->failOnAssert = false;
      m.lock();
      test(m.lock() == Threading::Status_Deadlock, "Expected that lock() would return false when invalidly called");
      test(testImpl->lockCt == 1, "Expected that lock() would not call MutexImpl::lock() when invalidly called");
      test(assertImpl->checkAndReset(1), "Expected that lock() would assert when called invalidly");
      
      reset();
      
      m.unlock();
      test(testImpl->unlockCt == 1, "Expected that unlock() would call MutexImpl::unlock()");
      assertImpl->failOnAssert = true;
      // Specify that after unlocking it is valid to call lock() again
      test(m.lock() == Threading::Status_NoError, "Expected that lock() would return true when validly called");
      test(testImpl->lockCt == 1, "Expected that lock() would call MutexImpl::lock() when called validly");
      m.unlock();
      
      reset();
      
      // Specify that unlocking a mutex when it is not locked is invalid
      assertImpl->failOnAssert = false;
      m.unlock();
      test(testImpl->unlockCt == 0, "Expected that unlock() would not call MutexImpl::unlock() when called invalidly");
      test(assertImpl->checkAndReset(1), "Expected that unlock() would assert when called invalidly");
      
      reset();
      assertImpl->failOnAssert = true;
      // Specify that a non-blocking attempt to lock will not assert, even if a blocking attempt should
      testImpl->lockSuccess = Threading::Status_NoError;
      test(m.lock() == Threading::Status_NoError, "Expected that lock() would succeed when called validly");
      test(testImpl->lockCt == 1, "Expected that lock() would call MutexImpl::lock when called validly");
      reset();
      test(m.lock(false) == Threading::Status_NoError, "Expected that non-blocking lock would succeed");
      test(testImpl->lockCt == 1, "Expected that non-blocking lock would call MutexImpl::lock");
      m.unlock();
#endif
   }
};

CreateUnitTest(p2_behavior_mutex_invalid, "Platform2/Behavior/Mutex/Invalid")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      TestAssertImpl* assertImpl = fixture.assertImpl;
      TestCreator* creator = fixture.creator;
      
      assertImpl->failOnAssert = false;
      creator->mutexValid = false;
      
      // Specify that construction asserts if MutexImpl is invalid post-construction
      Mutex fail;
      test(assertImpl->checkAndReset(1), "Expected that Mutex constructor would assert if MutexImpl failed to construct properly");
      
      TestMutexImpl* testImpl = creator->mutexImpl;
      
      // Specify that a blocking lock fails and asserts if MutexImpl is invalid
      test(fail.lock() == Threading::Status_ObjectInvalid, "Expected that Mutex::lock would fail if MutexImpl failed to construct properly");
      test(assertImpl->checkAndReset(1), "Expected that Mutex::lock would assert if MutexImpl failed to construct properly");
      test(testImpl->lockCt == 0, "Expected that MutexImpl::lock would not be called if MutexImpl failed to construct properly");
      
      // Specify that a non-blocking lock fails and asserts if MutexImpl is invalid
      test(fail.lock(false) == Threading::Status_ObjectInvalid, "Expected that non-blocking Mutex::lock would fail if MutexImpl failed to construct properly");
      test(assertImpl->checkAndReset(1), "Expected that non-blocking Mutex::lock would assert if MutexImpl failed to construct properly");
      test(testImpl->lockCt == 0, "Expected that MutexImpl::lock would not be called if MutexImpl failed to construct properly");
      
      // Specify that unlock asserts if MutexImpl is invalid
      test(fail.unlock() == Threading::Status_ObjectInvalid, "Expected that Mutex::unlock would fail if MutexImpl failed to construct properly");
      test(assertImpl->checkAndReset(1), "Expected that Mutex::unlock would assert if MutexImpl failed to construct properly");
      test(testImpl->unlockCt == 0, "Expected that MutexImpl::unlock would not be called if MutexImpl failed to construct properly");
   }
};

CreateUnitTest(p2_behavior_mutex_scopedlock, "Platform2/Behavior/MutexScopedLock")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      Mutex m;
      
      TestMutexImpl* testImpl;
      testImpl = fixture.creator->mutexImpl;
      testImpl->lockSuccess = Threading::Status_NoError;
      // Specify that Mutex::ScopedLock construction locks the mutex, and destruction unlocks the mutex
      {
         Mutex::ScopedLock s(m);
         test(testImpl->lockCt == 1, "Expected that Mutex::ScopedLock constructor would lock mutex");
      }
      test(testImpl->unlockCt == 1, "Expected that Mutex::ScopedLock destructor would unlock mutex");
   }
};

CreateUnitTest(p2_behavior_semaphore_scopedlock, "Platform2/Behavior/SemaphoreScopedAcquire")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      Semaphore s(1);
      
      TestSemaphoreImpl* testImpl;
      testImpl = fixture.creator->semaphoreImpl;
      testImpl->acquireSuccess = Threading::Status_NoError;
      // Specify that Semaphore::ScopedLock construction locks the mutex, and destruction unlocks the mutex
      {
         Semaphore::ScopedAcquire l(s);
         test(testImpl->acquireCt == 1, "Expected that Semaphore::ScopedAcquire constructor would acquire semaphore");
      }
      test(testImpl->releaseCt == 1, "Expected that Semaphore::ScopedAcquire destructor would release semaphore");
   }
};

CreateUnitTest(p2_behavior_font, "Platform2/Behavior/Font")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      Font f;
      
      // Specify that creating a Font will create a FontImpl
      TestFontImpl* testImpl = fixture.creator->fontImpl;
      TestAssertImpl* assertImpl = fixture.assertImpl;
      test(fixture.creator->fontImplCt == 1, "Expected that Font::Font() would create a FontImpl");
      
      // Specify that it is invalid to call methods befor init has been called
      assertImpl->failOnAssert = false;
      test(f.isValidChar(UTF16('a')) == false, "Expected that call to isValidChar before init would return false");
      test(assertImpl->checkAndReset(1), "Expected that call to isValidChar before init would assert");
      test(testImpl->isValidCharCt == 0, "Expected that call to isValidChar before init would not call FontImpl::isValidChar");
      
      f.getCharInfo('a');
      test(assertImpl->checkAndReset(1), "Expected that call to getCharInfo before init would assert");
      test(testImpl->getCharInfoCt == 0, "Expected that call to getCharInfo before init would not call FontImpl::getCharInfo");
      
      test(f.getHeight() == 0, "Expected that call to getHeight before init would return 0");
      test(assertImpl->checkAndReset(1), "Expected that call to getHeight before init would assert");
      test(testImpl->getHeightCt == 0, "Expected that call to getHeight before init would not call FontImpl::getHeight");
      
      test(f.getAscent() == 0, "Expected that call to getAscent before init would return 0");
      test(assertImpl->checkAndReset(1), "Expected that call to getAscent before init would assert");
      test(testImpl->getAscentCt == 0, "Expected that call to getAscent before init would not call FontImpl::getAscent");
      
      // Specify that it is invalid to create an unnamed font
      test(f.init(String(""), 12, FontCharset::ANSI) == false, "Expected that creation of an unnamed font would fail");
      test(assertImpl->checkAndReset(1), "Expected that creation of an unnamed font would assert");
      test(testImpl->initCt == 0, "Expected that creation of an unnamed font would not call FontImpl::init");
      
      // Specify that it is invalid to create a font with size < 9
      test(f.init(String("Arial"), 8, FontCharset::ANSI) == false, "Expected that creation of a font with size < 9 would fail");
      test(assertImpl->checkAndReset(1), "Expected that creation of a font with size < 9 would assert");
      test(testImpl->initCt == 0, "Expected that creation of a font with size < 9 would not call FontImpl::init");
      
      // Specify that if the FontImpl::init fails Font::init should not assert, return false, and allow subsequent attempts
      assertImpl->failOnAssert = true;
      testImpl->initSuccess = false;
      test(f.init(String("Arial"), 9, FontCharset::ANSI) == false, "Expected that Font::init would return false if FontImpl::init returns false");
      test(testImpl->initCt == 1, "Expected that Font::init would call FontImpl::init when given valid parameters");
      testImpl->initCt = 0;
      testImpl->initSuccess = true;
      
      // Specify that a font with valid parameters should call FontImpl::init (and succeed)
      test(f.init(String("Arial"), 9, FontCharset::ANSI), "Expected that a font would init when given valid parameters");
      test(testImpl->initCt == 1, "Expected that creation of a font with valid parameters would call FontImpl::init");
      
      // Specify that it is not valid to init a font more than once
      assertImpl->failOnAssert = false;
      test(f.init(String("Arial"), 9, FontCharset::ANSI) == false, "Expected that a font would fail to be init'd more than once");
      test(assertImpl->checkAndReset(1), "Expected that init'ing a font more than once would assert");
      test(testImpl->initCt == 1, "Expected that init'ing a font more than once would not call FontImpl::assert");
      
      // Specify that it is valid to call methods after init has been called
      assertImpl->failOnAssert = true;
      test(f.isValidChar(UTF16('a')) == true, "Expected that call to isValidChar after init would return true");
      test(testImpl->isValidCharCt == 1, "Expected that call to isValidChar after init would call FontImpl::isValidChar");
      
      f.getCharInfo('a');
      test(testImpl->getCharInfoCt == 1, "Expected that call to getCharInfo after init would call FontImpl::getCharInfo");
      
      test(f.getHeight() == testImpl->u32ret, "Expected that call to getHeight after init would return a valid value");
      test(testImpl->getHeightCt == 1, "Expected that call to getHeight after init would call FontImpl::getHeight");
      
      test(f.getAscent() == testImpl->u32ret, "Expected that call to getAscent after init would return a valid value");
      test(testImpl->getAscentCt == 1, "Expected that call to getAscent after init would call FontImpl::getAscent");
   }
};

CreateUnitTest(p2_behavior_dlibrary, "Platform2/Behavior/DLibrary")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      DLibrary d;
      
      // Specify that creating a DLibrary will create a DLibraryImpl
      TestDLibraryImpl* testImpl = fixture.creator->dlibraryImpl;
      testImpl->currentTest = this;
      TestAssertImpl* assertImpl = fixture.assertImpl;
      test(fixture.creator->dlibraryImplCt == 1, "Expected that DLibrary::DLibrary() would create a DLibraryImpl");
      
      // Specify that a DLibrary starts in an unloaded state
      test(d.isLoaded() == false, "Expected that a DLibrary would start in an unloaded state");
      
      assertImpl->failOnAssert = false;
      // Specify that it is invalid to call DLibrary::load with an empty string
      test(d.load(String("")) == false, "Expected that call to DLibrary::load with empty string would return false");
      test(assertImpl->checkAndReset(1), "Expected that call to DLibrary::load with empty string would assert");
      test(testImpl->initCt == 0, "Expected that call to DLibrary::load with empty string would not call DLibraryImpl::init");
      test(d.isLoaded() == false, "Expected that a call to DLibrary::load with empty string would not cause DLibrary to be loaded");
      
      // Specify that it is invalid to call DLibrary::bind before a successfull init
      test(d.bind(String("someFn")) == NULL, "Expected that call to DLibrary::bind before init would return NULL");
      test(assertImpl->checkAndReset(1), "Expected that call to DLibrary::bind before init would assert");
      test(testImpl->bindCt == 0, "Expected that call to DLibrary::bind before init would not call DLibraryImpl::bind");
      
      // Do a successful load
      assertImpl->failOnAssert = true;
      testImpl->initSuccess = true;
      test(d.load(String("SomeLib.dll")) == true, "Expected that valid call to DLibrary::load would return true");
      test(testImpl->initCt == 1, "Expected that valid call to DLibrary::load would call DLibraryImpl::init");
      test(d.isLoaded() == true, "Expected that valid call to DLibrary::load would cause DLibrary to be in a loaded state");
      
      // Specify that it is invalid to load a library more than once
      assertImpl->failOnAssert = false;
      test(d.load(String("SomeOtherLib.dll")) == false, "Expected that attempt to load DLibrary a second time would return false");
      test(assertImpl->checkAndReset(1), "Expected that attempt to load DLibrary a second time would assert");
      test(testImpl->initCt == 1, "Expected that attemp tto load DLibrary a second time would not call DLibraryImpl::init");
      
      // Specify that it is invalid to call bind with an empty string
      test(d.bind(String("")) == NULL, "Expected that call to DLibrary::bind with empty string would return NULL");
      test(assertImpl->checkAndReset(1), "Expected that call to DLibrary::bind with empty string would assert");
      test(testImpl->bindCt == 0, "Expected that call to DLibrary::bind with empty string would not call DLibraryImpl::bind");
      
      // Specify valid call behavior
      assertImpl->failOnAssert = true;
      test(d.bind(String("SomeFn")) == testImpl->voidRet, "Expected that valid call to DLibrary::bind would return a reasonable value");
      test(testImpl->bindCt == 1, "Expected that a valid call to DLibrary::bind would call DLibraryImpl::bind");
      
      // Specify that ~DLibrary will call close (TestDLibraryImpl will do the test)
   }
};

CreateUnitTest(p2_behavior_dlibrarynoinit, "Platform2/Behavior/DLibrary/NoInit")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      // Specify that a DLibrary which was never loaded shouldn't assert when it is destroyed.
      DLibrary d;
   }
};
   
using namespace Torque;
using namespace Torque::FS;
CreateUnitTest(p2_behavior_filesystem, "Platform2/Behavior/FileSystem")
{
   void run()
   {
      BehaviorTestFixture fixture(this);
      PlatformFileSystem fs("/testing/simpletest");
      
      TestFileSystemImpl* testImpl = fixture.creator->fileSystemImpl;
      testImpl->currentTest = this;
      
      // Set up the impl
      testImpl->hasAttributesMap["/testing/simpletest/dne"] = false;
      testImpl->hasAttributesMap["/testing/simpletest/file"] = true;
      testImpl->hasAttributesMap["/testing/simpletest/directory"] = true;
      
      testImpl->removeFileMap["/testing/simpletest/file"] = true;
      testImpl->removeDirectoryMap["/testing/simpletest/directory"] = true;
      
      testImpl->createDirectoryMap["/testing/simpletest/fail"] = false;
      testImpl->createDirectoryMap["/testing/simpletest/newdirectory"] = true;
      
      FileNode::Attributes attrs;
      attrs.flags = FileNode::File;
      testImpl->attributesMap["/testing/simpletest/file"] = attrs;
      
      attrs.flags = FileNode::Directory;
      testImpl->attributesMap["/testing/simpletest/directory"] = attrs;
      
      // Specify that mapTo maps a relative path to a full path, and mapFrom maps a full path to a relative path
      test(fs.mapTo("file").getFullPath() == String("/testing/simpletest/file"), "Expected that mapTo would map the given path to a full system path");
      test(fs.mapFrom("/testing/simpletest/file").getFullPath() == String("/file"), "Expected that mapFrom would map the full system path to a relative path");
      
      // Specify that attempting to resolve a non-existant file will return NULL
      test(fs.resolve("dne") == 0, "Expected that a non-existant file would not be resolveable");
      test(testImpl->getAttributesForPathCt == 1, "Expected that getAttributesForPath would be called");
      
      // Specify that attempting to resolve an existing file will return a non-NULL pointer to a File subclass
      FileNodeRef file = fs.resolve("file");
      test(file != 0, "Expected that an existing file would be resolveable");
      test(dynamic_cast<File*>(file.getPointer()) != NULL, "Expected that a file would resolve to a File subclass");
      test(testImpl->getAttributesForPathCt == 2, "Expected that getAttributesForPath would be called");
      
      // Specify that attempting to resolve an existing directory will return a non-NULL pointer to a Directory subclass
      FileNodeRef dir = fs.resolve("directory");
      test(dir != 0, "Expected that an existing directory would be resolveable");
      test(dynamic_cast<Directory*>(dir.getPointer()) != NULL, "Expected that a directory would resolve to a Directory sublcass");
      test(testImpl->getAttributesForPathCt == 3, "Expected that getAttributesForPath would be called");
      
      // Specify that attempting to remove a non-existant file will fail and not call into the platform
      test(fs.remove("dne") == false, "Expected that attempt to remove non-existant file would fail");
      test(testImpl->removeFileCt == 0, "Expected that attempt to remove non-existant file would not call removeFile");
      test(testImpl->removeDirectoryCt == 0, "Expected that attempt to remove non-existant file would not call removeDirectory");
      
      // Specify that attempting to remove an existing file will succeed and call FileSystemImpl::removeFile
      test(fs.remove("file") == true, "Expected that attempt to remove existing file would succeed");
      test(testImpl->removeFileCt == 1, "Expected that attempt to remove existing file would call removeFile");
      test(testImpl->removeDirectoryCt == 0, "Expected that attemp to remove existing file would not call removeDirectory");
      
      // Specify that attempting to remove an existing directory will succeed and call FileSystemImpl::removeDirectory
      test(fs.remove("directory") == true, "Expected that attempt to remove existing directory would succeed");
      test(testImpl->removeFileCt == 1, "Expected that attempt to remove existing directory would not call removeFile");
      test(testImpl->removeDirectoryCt == 1, "Expected that attempt to remove existing directory would call removeDirectory");
      
      // Specify that attempting to create a new file will return a non-NULL pointer to a File subclass
      FileNodeRef newFile = fs.create("newfile", FileNode::File);
      test(newFile != 0, "Expected that creating a new file would return non-NULL");
      test(dynamic_cast<File*>(newFile.getPointer()) != NULL, "Expected that returned object would be a File subclass");
      
      // Specify that failure to create a new directory will return a NULL pointer
      FileNodeRef newDir = fs.create("fail", FileNode::Directory);
      test(newDir == 0, "Expected that failing to create a new directory would return a NULL pointer");
      test(testImpl->createDirectoryCt == 1, "Expected that attempting to create a new directory would call createDirectory");
      
      // Specify that successfully creating a new directory will return a non-NULL pointer to a Directory subclass
      newDir = fs.create("newdirectory", FileNode::Directory);
      test(newDir != 0, "Expected that succesfully creating a new directory would return a non-NULL pointer");
      test(testImpl->createDirectoryCt == 2, "Expected that attempting to create a new directory would call createDirectory");
   }
};
