//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_MUTEXIMPL_H_
#define TORQUE_PLATFORM2_POSIX_MUTEXIMPL_H_

#include "platform2/impls/base/threads/mutexImpl.h"
#include "core/types/types.h"
#include <pthread.h>

namespace Platform2
{
namespace Internal_
{
   class PosixMutexImpl : public MutexImpl
   {
   public:      
      PosixMutexImpl();
      virtual ~PosixMutexImpl();
      virtual bool init();
      virtual Threading::Status lock(bool block);
      virtual Threading::Status unlock();
      
   private:
      pthread_mutex_t mMutex;
   };
}
}

#endif