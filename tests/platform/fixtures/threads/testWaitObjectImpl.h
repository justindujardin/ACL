//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_WAITOBJECTLIMPL_H_
#define ACL_PLATFORM_TEST_WAITOBJECTLIMPL_H_

#include "platform/impls/base/threads/waitObjectImpl.h"

using namespace Platform2;
using namespace Platform2::Internal_;

namespace TestPlatform
{
   class TestWaitObjectImpl : public WaitObjectImpl
   {
   public:
      bool timeout;
      TestWaitObjectImpl(): timeout(false)
      {
      }
      virtual Threading::Status wait(S32 timeout) 
      {
         return timeout ? Threading::Status_WaitTimeout : Threading::Status_WaitSignaled;
      }
      virtual void signalOne()
      {
      }
      virtual void signalAll()
      {
      }
   };
}

#endif
