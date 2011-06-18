//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_MAC_THREADIMPL_H_
#define TORQUE_PLATFORM2_MAC_THREADIMPL_H_

#include "platform2/impls/base/threads/threadImpl.h"

namespace Platform2
{
namespace Internal_
{
   class MacThreadImpl : public ThreadImpl
   {
   public:
      static const U32 cStackSize = 512 * 1024;
      static const U32 cMagic = 0xdecafbad;
      
      MacThreadImpl();
      virtual ~MacThreadImpl();
      virtual Threading::Status start(Param* p);
   private:
      MPTaskID mTask;
      MPQueueID mQueue;
   };
}
}

#endif