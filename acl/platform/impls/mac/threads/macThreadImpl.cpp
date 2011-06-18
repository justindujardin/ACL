//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <CoreServices/CoreServices.h>

// TODO: Error handling
#include "platform2/impls/mac/threads/macThreadImpl.h"

static OSStatus threadEntry(void* p)
{
   Platform2::Internal_::ThreadImpl::CommonThreadEntry(p);
   return noErr;
}

namespace Platform2
{
namespace Internal_
{
   MacThreadImpl::MacThreadImpl()
   {
      mTask = 0;
      mQueue = 0;
   }
   
   MacThreadImpl::~MacThreadImpl()
   {
      MPDeleteQueue(mQueue);
   }
   
   Threading::Status MacThreadImpl::start(Param* p)
   {
      OSStatus err = MPCreateQueue(&mQueue);
      if(err != noErr)
      {
         if(err == kMPInsufficientResourcesErr)
            return Threading::Status_Resources;
         else
            return Threading::Status_PlatformError;
      }
      
      err = MPCreateTask(threadEntry, p, cStackSize, mQueue, reinterpret_cast<void*>(cMagic), 0, 0, &mTask);
      switch(err)
      {
      case noErr:
         return Threading::Status_NoError;
      case kMPInsufficientResourcesErr:
         return Threading::Status_Resources;
      default:
         return Threading::Status_PlatformError;
      }
   }
}
}