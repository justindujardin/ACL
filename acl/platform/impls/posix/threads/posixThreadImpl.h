//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_THREADIMPL_H_
#define TORQUE_PLATFORM2_POSIX_THREADIMPL_H_

#include "platform2/impls/base/threads/threadImpl.h"
#include <pthread.h>

namespace Platform2
{
namespace Internal_
{
   class PosixThreadImpl : public ThreadImpl
   {
   public:      
      ~PosixThreadImpl();
      virtual Threading::Status start(Param* p);
      
   private:
      pthread_t mThread;
      S32 mCreationErr;
   };
}
}

#endif