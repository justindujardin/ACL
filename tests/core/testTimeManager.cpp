//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/simObject/simWorld.h"
#include "core/util/journal/journaledSignal.h"
#include "core/util/journal/process.h"
#include "core/util/timeManager.h"
#include "platform/platform.h"

using namespace ACLib;

CreateUnitTest(Check_advanceTime, "Platform/Time/advanceTime"){
    void run(){U32 time = SimWorld::getRootWorld()->getCurrentTime();
SimWorld::getRootWorld()->advanceTime(10);
U32 newTime = SimWorld::getRootWorld()->getCurrentTime();

test(newTime - time == 10, "Platform::advanceTime is borked, we advanced 10ms "
                           "but didn't get a 10ms delta!");
}
}
;

// Will fail on Win32
CreateUnitTest(Check_platformSleep, "disabled_Platform/Time/Sleep") {
  const static S32 sleepTimeMs = 500;
  void run() {
    U32 start = Platform::GetPlatform()->getRealMilliseconds();
    Platform::GetPlatform()->sleep(sleepTimeMs);
    U32 end = Platform::GetPlatform()->getRealMilliseconds();

    test(end - start >= sleepTimeMs,
         "We didn't sleep at least as long as we requested!");
  }
};

// 1/08/09 - Disabled this test because it runs the main process loop while we
// are in the main process loop when executed from the console.  There is no
// easy way to detect that we are currently in the main process loop so we can't
// skip this test in situations where it could cause issues. -AlexS
CreateUnitTest(Check_timeManager, "disabled_Platform/Time/Manager") {
  void handleTimeEvent(S32 timeDelta) {
    mElapsedTime += timeDelta;
    mNumberCalls++;

    if (mElapsedTime >= 1000)
      Process::requestShutdown();
  }

  S32 mElapsedTime;
  S32 mNumberCalls;

  void run() {
    mElapsedTime = mNumberCalls = 0;

    // Initialize the time manager...
    TimeManager time;
    time.timeEvent.notify(this, &Check_timeManager::handleTimeEvent);

    // Event loop till at least one second has passed.
    const U32 start = Platform::GetPlatform()->getRealMilliseconds();

    while (Process::processEvents()) {
      // If we go too long, kill it off...
      if (Platform::GetPlatform()->getRealMilliseconds() - start > 30 * 1000) {
        test(false, "Terminated process loop due to watchdog, not due to time "
                    "manager event, after 30 seconds.");
        Process::requestShutdown();
      }
    }

    const U32 end = Platform::GetPlatform()->getRealMilliseconds();

    // Now, confirm we have approximately similar elapsed times.
    S32 elapsedRealTime = end - start;
    test(mAbs(elapsedRealTime - mElapsedTime) < 50,
         "Failed to elapse time to within the desired tolerance.");

    test(mNumberCalls > 0, "Somehow got no event callbacks from TimeManager?");

    Con::printf(
        "   Got %d time events, and elapsed %dms from TimeManager, "
        "%dms according to Platform::GetPlatform()->getRealMilliseconds()",
        mNumberCalls, mElapsedTime, elapsedRealTime);
  }
};
