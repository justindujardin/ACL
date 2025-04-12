//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/base/platformCreator.h"

#include "platform/impls/linux/linuxPlatform.h"

#include "platform/impls/posix/dlibrary/posixDLibraryImpl.h"
#include "platform/impls/posix/threads/posixMutexImpl.h"
#include "platform/impls/posix/threads/posixSemaphoreImpl.h"
#include "platform/impls/posix/threads/posixThreadImpl.h"
#include "platform/impls/posix/threads/posixThreadLocalImpl.h"
#include "platform/impls/posix/threads/posixWaitObjectImpl.h"
#include "platform/impls/posix/volume/posixDirectoryImpl.h"
#include "platform/impls/posix/volume/posixFileImpl.h"
#include "platform/impls/posix/volume/posixFileSystemImpl.h"

#include "platform/platform.h"

#include "core/strings/unicode.h"
#include "platform/platformSystemInfo.h"

#include <sys/time.h>
#include <unistd.h>

namespace ACLib {
namespace Platform {
namespace Internal_ {
void InitCreator() { RegisterPlatform<LinuxPlatformObject>(); }

LinuxPlatformObject::LinuxPlatformObject() {
  TypeRebind &factory = getProtectedFactory();
  factory.bind<SemaphoreImpl>().to<PosixSemaphoreImpl>();
  factory.bind<ThreadImpl>().to<PosixThreadImpl>();
  factory.bind<MutexImpl>().to<PosixMutexImpl>();
  factory.bind<WaitObjectImpl>().to<PosixWaitObjectImpl>();
  factory.bind<ThreadLocalImpl>().to<PosixThreadLocalImpl>();

  factory.bind<DLibraryImpl>().to<PosixDLibraryImpl>();

  factory.bind<FileSystemImpl>().to<PosixFileSystemImpl>();
  factory.bind<FileImpl>().to<PosixFileImpl>();
  factory.bind<DirectoryImpl>().to<PosixDirectoryImpl>();
}

LinuxPlatformObject::~LinuxPlatformObject() {}

void LinuxPlatformObject::init() {}

void LinuxPlatformObject::shutdown() {}

void LinuxPlatformObject::abort(U32 code) {}

void LinuxPlatformObject::debugBreak() {}

static U32 sgTimeOffset = 0;
static bool sgTimeInitialized = false;
U32 LinuxPlatformObject::getRealMilliseconds() {
  timeval t;
  if (sgTimeInitialized == false) {
    sgTimeInitialized = true;
    gettimeofday(&t, NULL);
    sgTimeOffset = t.tv_sec;
  }
  gettimeofday(&t, NULL);
  U32 secs = t.tv_sec - sgTimeOffset;
  U32 uSecs = t.tv_usec;
  // Make granularity 1 ms
  return (secs * 1000) + (uSecs / 1000);
}

void LinuxPlatformObject::sleep(U32 ms) { usleep(ms * 1000); }

Path LinuxPlatformObject::getExecutablePath() { return Path(""); }

String LinuxPlatformObject::getExecutableName() { return String(""); }

String LinuxPlatformObject::getUserDataDirectory() { return String(""); }

String LinuxPlatformObject::getUserHomeDirectory() { return String(""); }

String LinuxPlatformObject::getClipboard() { return String(""); }

bool LinuxPlatformObject::setClipboard(const String &text) { return false; }

void LinuxPlatformObject::restartInstance() {}

void LinuxPlatformObject::postQuitMessage(U32 code) {}

void LinuxPlatformObject::outputDebugString(const String &str) {}

bool LinuxPlatformObject::touchFile(const Path &path) { return false; }

U32 LinuxPlatformObject::getMathControlState() { return 0; }

void LinuxPlatformObject::setMathControlState(U32 state) {}

void LinuxPlatformObject::setMathControlStateKnown() {}

void LinuxPlatformObject::initSystemInfo_(SystemInfo &info) {}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib