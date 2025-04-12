//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_SEMAPHORE_H_
#define ACL_PLATFORM_SEMAPHORE_H_

#include "core/types.h"
#include "core/util/noncopyable.h"
#include "core/util/scopedPtr.h"
#include "platform/threads/threadingStatus.h"

namespace ACLib {
namespace Platform {
/// @ingroup p2thread
/// A wrapper around a Semaphore. Unlike a mutex, a semaphore has a count
/// associated with it, and can be released by a thread which did not
/// previously acquire it.
class Semaphore : private Noncopyable {
public:
  /// @brief Creates a counting semaphore with the given initial and max counts.
  /// @param initialCount The initial count of the semaphore.
  /// Must be >= 0 and <= maxCount
  /// @param maxCount The maximum count of the semaphore.  The semaphore
  /// cannot be released above this.  Must be > 0 and <= S32_MAX.
  Semaphore(S32 initialCount, S32 maxCount = S32_MAX);

  /// @brief Destroys the semaphore.
  ~Semaphore();

  /// @brief Decrements count by 1.
  /// @param block If true, blocks the calling thread until the semaphore
  /// can be acquired.  If false, does not block the calling thread and
  /// immediately returns Status_Busy if the semaphore could not be acquired.
  /// @returns Threading::Status_NoError if the semaphore was acquired
  /// @returns Threading::Status_Busy if the semaphore could not be acquired
  /// because its count is already 0 and block was false.
  /// @returns Threading::Status_ObjectInvalid if the semaphore was not
  /// successfully created. This error is probably non-recoverable.
  /// @returns Threading::Status_PlatformError in implementation defined
  /// circumstances. This error is probably non-recoverable.
  Threading::Status acquire(bool block = true);

  /// @brief Increments count by 1.
  /// @returns Threading::Status_NoError if the semaphore's count was
  /// succesfully increased.
  /// @returns Threading::Status_Resources if releasing the semaphore would push
  /// its count above maxCount.
  /// @returns Threading::Status_ObjectInvalid if the semaphore was not
  /// successfully created. This error is probably non-recoverable.
  /// @returns Threading::Status_PlatformError in implementation defined
  /// circumstances. This error is probably non-recoverable.
  Threading::Status release();

  /// Acquires the given semaphore for the scope its in.
  /// @warning ScopedAcquire does not perform any error checking.
  class ScopedAcquire : private Noncopyable {
  public:
    ScopedAcquire(Semaphore &s) : mSempahore(s) { mSempahore.acquire(); }

    ~ScopedAcquire() { mSempahore.release(); }

  private:
    Semaphore &mSempahore;
  };

private:
  /// @cond
  struct Internal;
  ScopedPtr<Internal> mImpl;
  /// @endcond
};
} // namespace Platform
} // namespace ACLib
#endif
