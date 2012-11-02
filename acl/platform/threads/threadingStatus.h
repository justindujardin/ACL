//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_THREADINGSTATUS_H_
#define ACL_PLATFORM_THREADINGSTATUS_H_

namespace Platform2
{
   namespace Threading
   {
      /// @ingroup p2thread
      /// These are the various status codes that may be returned by functions/methods
      /// in the @ref p2thread module.
      enum Status
      {
         /// Indicates that the operation completed successfully with no errors.
         Status_NoError = 0,

         /// Indicates that the lock/acquire operation was not completed because
         /// the Mutex/Semaphore was already blocked.
         Status_Busy, 

         /// Indicates that the delegate is empty, thus the thread cannot be started.
         Status_DelegateInvalid, 

         /// Indicates that the platform object (e.g. Mutex or Semaphore) is invalid.
         Status_ObjectInvalid,

         /// Indicates that the thread has already been successfully started, so it 
         /// cannot be started again.
         Status_Again, 

         /// Indicates that a deadlock condition was detected.  
         /// This will probably only be returned when ACL_DEBUG_THREADING is defined.
         Status_Deadlock, 

         /// Indicates that the thread/process does not have permission to do the operation.
         /// e.g. a thread which does not own a mutex attempting to unlock it
         Status_Permission, 

         /// Indicates that necessary resources were not able to be allocated.
         /// This may also indicate other general failure, depending on the implementation.
         Status_Resources, 

         /// Indicates that some unknown platform specific error occurred.  
         /// There is probably no recovery.
         Status_PlatformError = 0xFF
      };
   }
}

#endif
