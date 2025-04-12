//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/assert.h"
#include "core/strings/str.h"
#include "core/util/autoPtr.h"
#include "core/util/delegate.h"
#include "platform/platform.h"
#include "platform/threads/mutex.h"
#include "platform/threads/semaphore.h"
#include "platform/threads/thread.h"
#include "platform/threads/waitObject.h"

#include "../fixtures/platformFixture.h"

#include <gtest/gtest.h>

using fastdelegate::MakeDelegate;
using namespace ACLib;
using namespace ACLib::Platform;
using namespace ACLib::TestPlatform;

/// Verifies that when a semaphore of count 0 is acquired the calling thread is
/// blocked until the semaphores count is > 0.
namespace ThreadSemaphoreBlock {
AutoPtr<Semaphore> semaphore;
AutoPtr<WaitObject> wait;

S32 work(Thread::MessageQueue &messageQueue) {
  wait->signalOne();
  U32 start = GetPlatform()->getRealMilliseconds();
  EXPECT_TRUE(semaphore->acquire(true) ==
              Threading::Status_NoError); //, "Failed to acquire blocking
                                          // semaphore with count > 0");
  EXPECT_TRUE(
      semaphore->acquire(true) ==
      Threading::Status_NoError); //, "Failed to acquire blocking semaphore");
  return GetPlatform()->getRealMilliseconds() - start;
}

TEST(Semaphore, Blocking) {
  wait = new WaitObject();
  semaphore = new Semaphore(2);
  Thread t(MakeDelegate(&work));
  semaphore->acquire(true);
  t.start();
  wait->wait();
  GetPlatform()->sleep(210);
  semaphore->release();
  t.finish();
  EXPECT_TRUE(t.getReturnCode() >= 200); //, "Semaphore wasn't blocking");
}
}; // namespace ThreadSemaphoreBlock

/// Verifies that an attempt to acquire a semaphore of count 0 does *not* block
/// and returns false if block is false.
namespace ThreadsSemaphoreNonBlock {
AutoPtr<Semaphore> semaphore;

S32 work(Thread::MessageQueue &messageQueue) {
  bool result = semaphore->acquire(false) == Threading::Status_Busy;
  EXPECT_TRUE(result); //, "Succeeded in non-blocking acquire of semaphore with
                       // count 0");
  return result ? 0 : -1;
}

TEST(Semaphore, NonBlock) {
  semaphore = new Semaphore(0);
  Thread t(MakeDelegate(&work));
  t.start();
  t.finish();
  semaphore->release();
  EXPECT_TRUE(t.getReturnCode() == 0); //, "Semaphore was blocking");
}
}; // namespace ThreadsSemaphoreNonBlock

TEST(Semaphore, Release) {
  AutoPtr<Semaphore> semaphore(new Semaphore(1, 3));
  EXPECT_TRUE(semaphore->release() == Threading::Status_NoError);
  EXPECT_TRUE(semaphore->release() == Threading::Status_NoError);
  EXPECT_TRUE(semaphore->release() == Threading::Status_Resources); //,
  //"Expected that releasing semaphore beyond maxCount would return
  // Status_Resources");
};

// Behavioral Specifications

TEST(SemaphoreBehavior, ConstructorMaxCount) {
  ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
  TestAssertImpl::FailOnAssert = false;
  TestAssertImpl *assertImpl = fixture->assert;

  // Specify that it is valid to create a semaphore with max count > 0
  AutoPtr<Semaphore> s(new Semaphore(1, 1));
  TestSemaphoreImpl *testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(
      assertImpl->checkAndReset(0)); //, "Expected that a semaphore created with
                                     // max count > 0 would not assert");

  // Specify that maxcount goes directly to the impl
  EXPECT_TRUE(testImpl->maxCount ==
              1); //, "Expected that maxcount > 0 would go directly to impl");

  // Reset impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;

  // Specify that creating a semaphore with max count 0 will assert (boundary)
  s = new Semaphore(0, 0);
  testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(
      assertImpl->checkAndReset(1)); //, "Expected that a semaphore created with
                                     // max count == 0 would assert");

  // Specify that we recover from the assert by setting maxCount to S32_MAX
  EXPECT_TRUE(
      testImpl->maxCount ==
      S32_MAX); //, "Expected that maxcount == 0 would be set to S32_MAX");

  // Reset impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;

  // Specifiy that creating a sempahore with max count < 0 will assert (error)
  s = new Semaphore(0, -15);
  testImpl = fixture->semaphoreImpl;
  // Will also assert because initialCount > max count
  EXPECT_TRUE(
      assertImpl->checkAndReset(2)); //, "Expected that a semaphore created with
                                     // max count < 0 would assert");

  // Specify that we can recover from an assertion by providing maxCount =
  // S32_MAX if maxCount was given as 0
  EXPECT_TRUE(
      testImpl->maxCount ==
      S32_MAX); //, "Expected that maxCount < 0 would be set to S32_MAX");

  // Rest impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;
};

TEST(SemaphoreBehavior, ConstructorInitialCountVsMaxCount) {
  ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
  TestAssertImpl::FailOnAssert = false;
  TestAssertImpl *assertImpl = fixture->assert;

  // Specify that it is valid to create a semaphore with initial count <
  // maxcount
  AutoPtr<Semaphore> s(new Semaphore(1, 2));
  TestSemaphoreImpl *testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(assertImpl->checkAndReset(
      0)); //, "Expected that a semaphore created with initialCount < maxCount
           // would not assert");

  // Specify that initialcount goes directly to the impl
  EXPECT_TRUE(testImpl->initialCount ==
              1); //, "Expected that initial count < maxCount would go directly
                  // to impl");

  // Reset impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;

  // Specify that creating a semaphore with initial count == maxcount is valid
  // (boundary)
  s = new Semaphore(2, 2);
  testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(assertImpl->checkAndReset(
      0)); //, "Expected that a semphoare with initialCount == maxCount would be
           // valid");

  // Specify that initialCount goes directly to impl
  EXPECT_TRUE(testImpl->initialCount ==
              2); //, "Expected that initial count == maxCount would go directly
                  // to impl");

  // Reset impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;

  // Specifiy that creating a sempahore with initialCount > maxCount will fail
  s = new Semaphore(3, 2);
  testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(
      assertImpl->checkAndReset(1)); //, "Expected that a semaphore created with
                                     // initialCount > maxCount would assert");

  // Specify that we can recover from an assertion by providing initialCount =
  // maxCount
  EXPECT_TRUE(testImpl->initialCount ==
              testImpl->maxCount); //, "Expected that initialCount == maxCount
                                   // when given as > maxCount");

  // Rest impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;
};

TEST(SemaphoreBehavior, ConstructorInitialCountVs0) {
  ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
  TestAssertImpl::FailOnAssert = false;
  TestAssertImpl *assertImpl = fixture->assert;

  // Specify that it is valid to create a semaphore with initial count > 0
  AutoPtr<Semaphore> s(new Semaphore(1, 2));
  TestSemaphoreImpl *testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(
      assertImpl->checkAndReset(0)); //, "Expected that a semaphore created with
                                     // initialCount > 0 would not assert");
  EXPECT_TRUE(
      testImpl->initialCount ==
      1); //, "Expected that initial count > 0 would go directly to impl");

  // Reset impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;

  // Specify that creating a semaphore with initial count == 0 is valid
  // (boundary)
  s = new Semaphore(0, 2);
  testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(
      assertImpl->checkAndReset(0)); //, "Expected that a semphoare with
                                     // initialCount == 0 would be valid");

  // Specify that initialCount goes directly to impl
  EXPECT_TRUE(
      testImpl->initialCount ==
      0); //, "Expected that initial count == 0 would go directly to impl");

  // Reset impl
  s = NULL;
  fixture->semaphoreImpl = new TestSemaphoreImpl(true);
  fixture->threadLocalImpl[0] = NULL;

  // Specify that creating a semaphore with initial count < 0 is invalid (error)
  s = new Semaphore(-1, 2);
  testImpl = fixture->semaphoreImpl;
  EXPECT_TRUE(assertImpl->checkAndReset(
      1)); //, "Expected that a semphoare with initialCount < 0 would assert");

  // Specify that we recover by setting initialCount = maxCount
  EXPECT_TRUE(testImpl->initialCount ==
              testImpl->maxCount); //, "Expected that initial count < 0 would be
                                   // set to maxCount");
};

TEST(SemaphoreBehavior, Valid) {
  ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
  TestAssertImpl::FailOnAssert = false;
  TestAssertImpl *assertImpl = fixture->assert;

  AutoPtr<Semaphore> s(new Semaphore(1, 1));
  TestSemaphoreImpl *testImpl = fixture->semaphoreImpl;

  // Specify that creating a semaphore will immediately call SemaphoreImpl::init
  EXPECT_TRUE(testImpl->initCt == 1); //, "Expected that Semaphore::Semaphore()
                                      // would call SemaphoreImpl::init");

  // Specify that release should parrot the impl (note that it is the
  // implementation's responsibility to enforce maxCount)
  testImpl->releaseSuccess = Threading::Status_PlatformError;
  EXPECT_TRUE(
      s->release() ==
      Threading::Status_PlatformError);  //, "Expected that Semaphore::release
                                         // would parrot the impl");
  EXPECT_TRUE(testImpl->releaseCt == 1); //, "Expected that Semaphore::release
                                         // would call SemaphoreImpl::release");

  testImpl->releaseSuccess = Threading::Status_NoError;
  EXPECT_TRUE(s->release() ==
              Threading::Status_NoError); //, "Expected that Semaphore::release
                                          // would parrot the impl");
  EXPECT_TRUE(testImpl->releaseCt == 2);  //, "Expected that Semaphore::release
                                         // would call SemaphoreImpl::release");

  // Specify that acquire should parrot the impl
  testImpl->acquireSuccess = Threading::Status_PlatformError;
  EXPECT_TRUE(
      s->acquire() ==
      Threading::Status_PlatformError);  //, "Expected that Semaphore::acquire
                                         // would parrot the impl");
  EXPECT_TRUE(testImpl->acquireCt == 1); //, "Expected that Semaphore::acquire
                                         // would call SemaphoreImpl::acquire");

  testImpl->acquireSuccess = Threading::Status_NoError;
  EXPECT_TRUE(s->acquire() ==
              Threading::Status_NoError); //, "Expected that Semaphore::acquire
                                          // would parrot the impl");
  EXPECT_TRUE(testImpl->acquireCt == 2);  //, "Expected that Semaphore::acquire
                                         // would call SemaphoreImpl::acquire");
};

TEST(SemaphoreBehavior, Invalid) {
  ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
  TestAssertImpl::FailOnAssert = false;
  TestAssertImpl *assertImpl = fixture->assert;

  fixture->semaphoreValid = false;

  // Specify that construction asserts if SemaphoreImpl is invalid
  // post-construction
  Semaphore fail(1);
  EXPECT_TRUE(assertImpl->checkAndReset(
      1)); //, "Expected that Semaphore constructor would assert if
           // SemaphoreImpl failed to init properly");

  TestSemaphoreImpl *testImpl = fixture->semaphoreImpl;
  testImpl->acquireSuccess = Threading::Status_NoError;
  // Specify that a blocking acquire fails and asserts if SemaphoreImpl is
  // invalid
  EXPECT_TRUE(
      fail.acquire() ==
      Threading::Status_ObjectInvalid); //, "Expected that Semaphore::acquire
                                        // would fail if SemaphoreImpl failed to
                                        // init properly");
  EXPECT_TRUE(assertImpl->checkAndReset(
      1)); //, "Expected that Semaphore::acquire would assert if SemaphoreImpl
           // failed to init properly");
  EXPECT_TRUE(testImpl->acquireCt ==
              0); //, "Expected that SemaphoreImpl::acquire would not be called
                  // if SemaphoreImpl failed to init properly");

  // Specify that a non-blocking acquire fails and asserts if SemaphoreImpl is
  // invalid
  EXPECT_TRUE(fail.acquire(false) ==
              Threading::Status_ObjectInvalid); //, "Expected that non-blocking
                                                // Semaphore::acquire would fail
                                                // if SemaphoreImpl failed to
                                                // init properly");
  EXPECT_TRUE(assertImpl->checkAndReset(
      1)); //, "Expected that non-blocking Semaphore::acquire would assert if
           // SemaphoreImpl failed to init properly");
  EXPECT_TRUE(testImpl->acquireCt ==
              0); //, "Expected that SemaphoreImpl::acquire would not be called
                  // if SemaphoreImpl failed to init properly");

  // Specify that release asserts if SemaphoreImpl is invalid
  EXPECT_TRUE(
      fail.release() ==
      Threading::Status_ObjectInvalid); //, "Expected that Semaphore::release
                                        // would fail if SemaphoreImpl failed to
                                        // init properly");
  EXPECT_TRUE(assertImpl->checkAndReset(
      1)); //, "Expected that Semaphore::release would assert if SemaphoreImpl
           // failed to init properly");
  EXPECT_TRUE(testImpl->releaseCt ==
              0); //, "Expected that SemaphoreImpl::release would not be called
                  // if SemaphoreImpl failed to init properly");
};

TEST(SemaphoreBehavior, ScopedAcquire) {
  ScopedPtr<PlatformFixture> fixture(new PlatformFixture());
  Semaphore s(1);

  TestSemaphoreImpl *testImpl = fixture->semaphoreImpl;
  testImpl->acquireSuccess = Threading::Status_NoError;
  // Specify that Semaphore::ScopedLock construction locks the mutex, and
  // destruction unlocks the mutex
  {
    Semaphore::ScopedAcquire l(s);
    EXPECT_TRUE(testImpl->acquireCt ==
                1); //, "Expected that Semaphore::ScopedAcquire constructor
                    // would acquire semaphore");
  }
  EXPECT_TRUE(testImpl->releaseCt ==
              1); //, "Expected that Semaphore::ScopedAcquire destructor would
                  // release semaphore");
};
