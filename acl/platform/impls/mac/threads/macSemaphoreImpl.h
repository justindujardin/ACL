//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_MAC_SEMAPHOREIMPL_H_
#define TORQUE_PLATFORM2_MAC_SEMAPHOREIMPL_H_

#include "platform2/impls/base/threads/semaphoreImpl.h"

namespace Platform2
{
namespace Internal_
{
   class MacSemaphoreImpl : public SemaphoreImpl
   {
   public:      
      MacSemaphoreImpl();
      virtual ~MacSemaphoreImpl();
      virtual bool init(U32 initialCount, U32 maxCount);
      virtual Threading::Status acquire(bool block);
      virtual Threading::Status release();
      
   private:
      MPSemaphoreID mSemaphore;
      OSStatus mCreationErr;
   };
}
}

#endif