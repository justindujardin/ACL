//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <CoreServices/CoreServices.h>

#include "platform/impls/mac/threads/macThreadLocalImpl.h"

namespace ACLib
{
   namespace Platform
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
}