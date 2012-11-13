//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "core/assert.h"
#include "core/util/autoPtr.h"

#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "platform/threads/semaphore.h"
#include "platform/dlibrary/dlibrary.h"
#include "platform/volume/platformVolume.h"

#include "./threads/testThreadImpl.h"
#include "./threads/testMutexImpl.h"
#include "./threads/testSemaphoreImpl.h"
#include "./threads/testThreadLocalImpl.h"
#include "./dlibrary/testDLibraryImpl.h"
#include "./volume/testFileSystemImpl.h"
#include "./volume/testFileImpl.h"
#include "./volume/testDirectoryImpl.h"
#include "./volume/testFileSystemChangeNotifierImpl.h"

using fastdelegate::MakeDelegate;

using namespace Platform2;
using namespace Platform2::Internal_;
using namespace ACLib;
using namespace ACLib::FS;



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

#ifndef ACL_DEBUG_THREADING
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
