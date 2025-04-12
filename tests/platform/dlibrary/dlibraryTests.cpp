//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/autoPtr.h"
#include "core/util/delegate.h"
#include "platform/dlibrary/dlibrary.h"
#include "platform/platform.h"

#include <gtest/gtest.h>

#include "../fixtures/platformFixture.h"
#include "../fixtures/testAssertImpl.h"

using namespace ACLib;
using namespace ACLib::Platform;
using namespace ACLib::TestPlatform;

TEST(DLibraryBehavior, Valid) {
  PlatformFixture fixture;
  TestAssertImpl *assertImpl = fixture.assert;
  DLibrary d;

  // Specify that creating a DLibrary will create a DLibraryImpl
  TestDLibraryImpl *testImpl = fixture.dlibraryImpl;
  EXPECT_TRUE(
      fixture.dlibraryImplCt ==
      1); //, "Expected that DLibrary::DLibrary() would create a DLibraryImpl");

  // Specify that a DLibrary starts in an unloaded state
  EXPECT_TRUE(
      d.isLoaded() ==
      false); //, "Expected that a DLibrary would start in an unloaded state");

  TestAssertImpl::FailOnAssert = false;
  // Specify that it is invalid to call DLibrary::load with an empty string
  EXPECT_TRUE(d.load(String("")) ==
              false); //, "Expected that call to DLibrary::load with empty
                      // string would return false");
  EXPECT_TRUE(
      assertImpl->checkAndReset(1)); //, "Expected that call to DLibrary::load
                                     // with empty string would assert");
  EXPECT_TRUE(testImpl->initCt ==
              0); //, "Expected that call to DLibrary::load with empty string
                  // would not call DLibraryImpl::init");
  EXPECT_TRUE(d.isLoaded() ==
              false); //, "Expected that a call to DLibrary::load with empty
                      // string would not cause DLibrary to be loaded");

  // Specify that it is invalid to call DLibrary::bind before a successfull init
  EXPECT_TRUE(d.bind(String("someFn")) ==
              NULL); //, "Expected that call to DLibrary::bind before init would
                     // return NULL");
  EXPECT_TRUE(assertImpl->checkAndReset(
      1)); //, "Expected that call to DLibrary::bind before init would assert");
  EXPECT_TRUE(testImpl->bindCt ==
              0); //, "Expected that call to DLibrary::bind before init would
                  // not call DLibraryImpl::bind");

  // Do a successful load
  TestAssertImpl::FailOnAssert = true;
  testImpl->initSuccess = true;
  EXPECT_TRUE(d.load(String("SomeLib.dll")) ==
              true); //, "Expected that valid call to DLibrary::load would
                     // return true");
  EXPECT_TRUE(testImpl->initCt ==
              1); //, "Expected that valid call to DLibrary::load would call
                  // DLibraryImpl::init");
  EXPECT_TRUE(d.isLoaded() ==
              true); //, "Expected that valid call to DLibrary::load would cause
                     // DLibrary to be in a loaded state");

  // Specify that it is invalid to load a library more than once
  TestAssertImpl::FailOnAssert = false;
  EXPECT_TRUE(d.load(String("SomeOtherLib.dll")) ==
              false); //, "Expected that attempt to load DLibrary a second time
                      // would return false");
  EXPECT_TRUE(
      assertImpl->checkAndReset(1)); //, "Expected that attempt to load DLibrary
                                     // a second time would assert");
  EXPECT_TRUE(testImpl->initCt ==
              1); //, "Expected that attemp tto load DLibrary a second time
                  // would not call DLibraryImpl::init");

  // Specify that it is invalid to call bind with an empty string
  EXPECT_TRUE(d.bind(String("")) ==
              NULL); //, "Expected that call to DLibrary::bind with empty string
                     // would return NULL");
  EXPECT_TRUE(
      assertImpl->checkAndReset(1)); //, "Expected that call to DLibrary::bind
                                     // with empty string would assert");
  EXPECT_TRUE(testImpl->bindCt ==
              0); //, "Expected that call to DLibrary::bind with empty string
                  // would not call DLibraryImpl::bind");

  // Specify valid call behavior
  TestAssertImpl::FailOnAssert = true;
  EXPECT_TRUE(
      d.bind(String("SomeFn")) ==
      testImpl->voidRet); //, "Expected that valid call to DLibrary::bind would
                          // return a reasonable value");
  EXPECT_TRUE(testImpl->bindCt ==
              1); //, "Expected that a valid call to DLibrary::bind would call
                  // DLibraryImpl::bind");

  // Specify that ~DLibrary will call close (TestDLibraryImpl will call
  // EXPECT_TRUE)
};

TEST(DLibraryBehavior, NoInit) {
  PlatformFixture fixture;
  // Specify that a DLibrary which was never loaded shouldn't assert when it is
  // destroyed.
  DLibrary d;
};
