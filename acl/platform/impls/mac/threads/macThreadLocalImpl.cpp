//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <CoreServices/CoreServices.h>

#include "platform2/impls/mac/threads/macThreadLocalImpl.h"

namespace Platform2
{
namespace Internal_
{
   MacThreadLocalImpl::MacThreadLocalImpl() : mTaskIndex(0)
   {
      MPAllocateTaskStorageIndex(&mTaskIndex);
   }
   
   MacThreadLocalImpl::~MacThreadLocalImpl()
   {
      if(mTaskIndex)
         MPDeallocateTaskStorageIndex(mTaskIndex);
   }
   
   void* MacThreadLocalImpl::get()
   {
      return MPGetTaskStorageValue(mTaskIndex);
   }
   
   void MacThreadLocalImpl::set(void* value)
   {
      MPSetTaskStorageValue(mTaskIndex, value);
   }
}
}