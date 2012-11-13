//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/delegate.h"
#include "platform/platform.h"
#include "platform/threads/thread.h"
#include "platform/threads/mutex.h"
#include "platform/threads/waitObject.h"
#include "core/util/autoPtr.h"

#include <gtest/gtest.h>

#include "./fixtures/platformFixture.h"

using fastdelegate::MakeDelegate;
using namespace Platform2;
using namespace TestPlatform;

/// Verifies that locking a mutex twice results in the second lock blocking the calling
/// thread until the first thread unlocks the mutex.
namespace MutexBlock
{
   AutoPtr<Mutex> m;
   AutoPtr<WaitObject> wait;

   S32 lock(Thread::MessageQueue& messageQueue)
   {
      EXPECT_TRUE(m->lock(true) == Threading::Status_NoError);
      return 0;
   }
   TEST(Mutex, Blocking)
   {
      m = new Mutex();
      wait = new WaitObject();
      EXPECT_TRUE(m->lock(true) == Threading::Status_NoError);//, "Failed to lock unlocked mutex");
      Thread t(MakeDelegate(&lock));
      t.start();
      while(!t.isRunning())
         GetPlatform()->sleep(10);
      EXPECT_TRUE(m->unlock() == Threading::Status_NoError);//, "Failed to lock unlocked mutex");
      t.finish();
      EXPECT_TRUE(t.getReturnCode() == 0);
   }
};

/// Verifies that an attempt to lock a locked mutex returns false if block is false.
namespace MutexNonBlock
{
   AutoPtr<Mutex> m;

   S32 lock(Thread::MessageQueue& messageQueue)
   {
      // Cannot non-blocking lock the mutex
      EXPECT_TRUE(m->lock(false) == Threading::Status_Busy);
      return 0;
   }

   TEST(Mutex, NonBlocking)
   {
      m = new Mutex();
      EXPECT_TRUE(m->lock(true) == Threading::Status_NoError);
      Thread t(MakeDelegate(&lock));
      t.start();
      t.finish();
      m->unlock();
   }
};

// Behavioral Specifications


namespace MutexBehvaiorValid
{
   TestMutexImpl* testImpl;

   void reset()
   {
      testImpl->lockCt = testImpl->unlockCt = 0;
   }

   TEST(MutexBehavior,Valid)
   {
      ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
      Mutex m;

      // A mutex impl should be created
      EXPECT_TRUE(fixture->mutexImplCt == 1);//, "Expected one mutex impl to be created");

      testImpl = fixture->mutexImpl;

      // init should be called on it
      EXPECT_TRUE(testImpl->initCt == 1);//, "Expected that init would be called on MutexImpl");

      testImpl->lockSuccess = Threading::Status_NoError;

      // Basic coverage
      EXPECT_TRUE(m.lock() == Threading::Status_NoError);//, "Expected that mutex would return true when impl returns true");
      EXPECT_TRUE(testImpl->lockCt == 1);//, "Expected that mutex would call MutexImpl::lock() once");
      m.unlock();
      EXPECT_TRUE(testImpl->unlockCt == 1);//, "Expected that mutex would call MutexImpl::unlock() once");

      reset();

      testImpl->lockSuccess = Threading::Status_PlatformError;
      EXPECT_TRUE(m.lock() == Threading::Status_PlatformError);//, "Expected that mutex would return false when impl returns false");
      reset();

#ifndef ACL_DEBUG_THREADING
      // Nothing more to cover
      return;
#else

      testImpl->lockSuccess = Threading::Status_NoError;
      // Specify that we assert when attempting to lock more than once on the same thread
      assertImpl->failOnAssert = false;
      m.lock();
      EXPECT_TRUE(m.lock() == Threading::Status_Deadlock);//, "Expected that lock() would return false when invalidly called");
      EXPECT_TRUE(testImpl->lockCt == 1);//, "Expected that lock() would not call MutexImpl::lock() when invalidly called");
      EXPECT_TRUE(assertImpl->checkAndReset(1));//, "Expected that lock() would assert when called invalidly");

      reset();

      m.unlock();
      EXPECT_TRUE(testImpl->unlockCt == 1);//, "Expected that unlock() would call MutexImpl::unlock()");
      assertImpl->failOnAssert = true;
      // Specify that after unlocking it is valid to call lock() again
      EXPECT_TRUE(m.lock() == Threading::Status_NoError);//, "Expected that lock() would return true when validly called");
      EXPECT_TRUE(testImpl->lockCt == 1);//, "Expected that lock() would call MutexImpl::lock() when called validly");
      m.unlock();

      reset();

      // Specify that unlocking a mutex when it is not locked is invalid
      assertImpl->failOnAssert = false;
      m.unlock();
      EXPECT_TRUE(testImpl->unlockCt == 0);//, "Expected that unlock() would not call MutexImpl::unlock() when called invalidly");
      EXPECT_TRUE(assertImpl->checkAndReset(1));//, "Expected that unlock() would assert when called invalidly");

      reset();
      assertImpl->failOnAssert = true;
      // Specify that a non-blocking attempt to lock will not assert, even if a blocking attempt should
      testImpl->lockSuccess = Threading::Status_NoError;
      EXPECT_TRUE(m.lock() == Threading::Status_NoError);//, "Expected that lock() would succeed when called validly");
      EXPECT_TRUE(testImpl->lockCt == 1);//, "Expected that lock() would call MutexImpl::lock when called validly");
      reset();
      EXPECT_TRUE(m.lock(false) == Threading::Status_NoError);//, "Expected that non-blocking lock would succeed");
      EXPECT_TRUE(testImpl->lockCt == 1);//, "Expected that non-blocking lock would call MutexImpl::lock");
      m.unlock();
#endif
   }
};

TEST(MutexBehavior,Invalid)
{
   ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
   TestAssertImpl::FailOnAssert = false;
   TestAssertImpl* assertImpl = fixture->assert;

   fixture->mutexValid = false;

   // Specify that construction asserts if MutexImpl is invalid post-construction
   Mutex fail;
   EXPECT_TRUE(assertImpl->checkAndReset(1));//, "Expected that Mutex constructor would assert if MutexImpl failed to construct properly");

   TestMutexImpl* testImpl = fixture->mutexImpl;

   // Specify that a blocking lock fails and asserts if MutexImpl is invalid
   EXPECT_TRUE(fail.lock() == Threading::Status_ObjectInvalid);//, "Expected that Mutex::lock would fail if MutexImpl failed to construct properly");
   EXPECT_TRUE(assertImpl->checkAndReset(1));//, "Expected that Mutex::lock would assert if MutexImpl failed to construct properly");
   EXPECT_TRUE(testImpl->lockCt == 0);//, "Expected that MutexImpl::lock would not be called if MutexImpl failed to construct properly");

   // Specify that a non-blocking lock fails and asserts if MutexImpl is invalid
   EXPECT_TRUE(fail.lock(false) == Threading::Status_ObjectInvalid);//, "Expected that non-blocking Mutex::lock would fail if MutexImpl failed to construct properly");
   EXPECT_TRUE(assertImpl->checkAndReset(1));//, "Expected that non-blocking Mutex::lock would assert if MutexImpl failed to construct properly");
   EXPECT_TRUE(testImpl->lockCt == 0);//, "Expected that MutexImpl::lock would not be called if MutexImpl failed to construct properly");

   // Specify that unlock asserts if MutexImpl is invalid
   EXPECT_TRUE(fail.unlock() == Threading::Status_ObjectInvalid);//, "Expected that Mutex::unlock would fail if MutexImpl failed to construct properly");
   EXPECT_TRUE(assertImpl->checkAndReset(1));//, "Expected that Mutex::unlock would assert if MutexImpl failed to construct properly");
   EXPECT_TRUE(testImpl->unlockCt == 0);//, "Expected that MutexImpl::unlock would not be called if MutexImpl failed to construct properly");
};

TEST(MutexBehavior,ScopedLock)
{
   ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
   Mutex m;

   TestMutexImpl* testImpl;
   testImpl = fixture->mutexImpl;
   testImpl->lockSuccess = Threading::Status_NoError;
   // Specify that Mutex::ScopedLock construction locks the mutex, and destruction unlocks the mutex
   {
      Mutex::ScopedLock s(m);
      EXPECT_TRUE(testImpl->lockCt == 1);//, "Expected that Mutex::ScopedLock constructor would lock mutex");
   }
   EXPECT_TRUE(testImpl->unlockCt == 1);//, "Expected that Mutex::ScopedLock destructor would unlock mutex");
};
