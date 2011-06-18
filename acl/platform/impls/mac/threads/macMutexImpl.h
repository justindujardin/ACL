//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_MAC_MUTEXIMPL_H_
#define TORQUE_PLATFORM2_MAC_MUTEXIMPL_H_

#include "platform2/impls/base/threads/mutexImpl.h"

namespace Platform2
{
namespace Internal_
{
   class MacMutexImpl : public MutexImpl
   {
   public:      
      MacMutexImpl();
      virtual ~MacMutexImpl();
      virtual bool init();
      virtual Threading::Status lock(bool block);
      virtual Threading::Status unlock();
   
   private:
      MPSemaphoreID mSemaphore;
   };
}
}

#endif