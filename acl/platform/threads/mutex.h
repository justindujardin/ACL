//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MUTEX_H_
#define ACL_PLATFORM_MUTEX_H_

#include "core/util/scopedPtr.h"
#include "core/util/noncopyable.h"
#include "platform/threads/threadingStatus.h"

namespace Platform2
{
   /// @ingroup p2thread
   /// A simple thread synchronization primitive that provides exclusive access
   /// while locked.
   class Mutex : private Noncopyable
   {
   public:
      /// Constructs the mutex.  
      /// @post A mutex which is not locked on any thread.
      Mutex();
      
      /// Destroys the mutex.
      /// @pre The mutex must be @e unlocked.
      ~Mutex();
      
      /// @brief Locks the mutex.
      /// @remarks If TORQUE_DEBUG_THREADING is defined this method will assert if it is
      /// called more than once from the same thread (with block = true).
      /// @param block If true, blocks the calling thread until the mutex can be 
      /// locked.  If false, does not block the calling thread and 
      /// immediately returns Status_Busy if the mutex could not be locked.
      /// @returns Threading::Status_NoError if the mutex was locked.
      /// @returns Threading::Status_Busy if block was false and the mutex was already locked by another thread.
      /// @returns Threading::Status_Deadlock if a deadlock condition was detected 
      /// (detection enabled if TORQUE_DEBUG_THREADING is defined)
      /// @returns Threading::Status_ObjectInvalid if the mutex was not successfully created.
      /// This error is probably non-recoverable.
      /// @returns Threading::Status_PlatformError under implementation defined circumstances.
      /// This error is probably non-recoverable.
      Threading::Status lock(bool block = true);
      
      /// @brief Unlocks the mutex.
      /// @pre The mutex must have been previously locked on the calling thread.
      /// @post The mutex will no longer be locked on the calling thread.
      /// @remarks If TORQUE_DEBUG_THREADING is defined this method will assert if it is
      /// called from a thread which did not lock the mutex.
      /// @returns Threading::Status_NoError if the mutex was unlocked
      /// @returns Threading::Status_Permission if the calling thread does not own the mutex.
      /// (detection enabled if TORQUE_DEBUG_THREADING is defined)
      /// @returns Threading::Status_ObjectInvalid if the mutex was not successfully created.
      /// This error is probably non-recoverable.
      /// @returns Threading::Status_PlatformError under implementation defined circumstances.
      /// This error is probably non-recoverable.
      Threading::Status unlock();
      
      /// @brief Locks the mutex when constructed and unlocks when destructed.  
      /// @details Useful for locking a mutex for a block of code without 
      /// worrying about possible exit points.
      /// @warning ScopedLock does not perform any error checking.
      class ScopedLock : private Noncopyable
      {
      public:
         ScopedLock(Mutex& m) : mMutex(m)
         {
            mMutex.lock();
         }
         
         ~ScopedLock()
         {
            mMutex.unlock();
         }
         
      private:
         Mutex& mMutex;
      };
      
   private:
      /// @cond
      struct Internal;
      Torque::ScopedPtr<Internal> mImpl;
      /// @endcond
   };
}

#endif