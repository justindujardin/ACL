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
      TestWaitObjectImpl()
      {
      }
      virtual Threading::Status wait(S32 timeout = -1) 
      {
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
