//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_TEST_THREADIMPL_H_
#define TORQUE_PLATFORM2_TEST_THREADIMPL_H_

#include "platform2/impls/base/threads/threadImpl.h"

namespace Platform2
{
namespace Internal_
{
   class TestThreadImpl : public ThreadImpl
   {
   public:
      U32 startCt;
      Param* param;
      Threading::Status startSuccess;
      
      TestThreadImpl()
      {
         startCt = 0;
         startSuccess = Threading::Status_PlatformError;
      }
      
      virtual Threading::Status start(Param* p)
      {
         param = p;
         startCt++;
         return startSuccess;
      }
      
      void run()
      {
         // Delegate just returns, so this should immediately post
         CommonThreadEntry(param);
      }
   };
}
}

#endif