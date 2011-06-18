//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_SEMAPHOREIMPL_H_
#define ACL_PLATFORM_TEST_SEMAPHOREIMPL_H_

#include "platform/impls/base/threads/semaphoreImpl.h"

namespace Platform2
{
namespace Internal_
{
   class TestSemaphoreImpl : public SemaphoreImpl
   {
   public:
      U32 initCt;
      U32 acquireCt;
      U32 releaseCt;
      
      U32 initialCount;
      U32 maxCount;
      
      bool initSuccess;
      Threading::Status acquireSuccess;
      Threading::Status releaseSuccess;
      
      TestSemaphoreImpl(bool semaphoreValid)
      {
         maxCount = initialCount = 0;
         initCt = acquireCt = releaseCt = 0;
         acquireSuccess = Threading::Status_PlatformError;
         releaseSuccess = Threading::Status_NoError;
         initSuccess = semaphoreValid;
      }
      
      virtual bool init(U32 _initCount, U32 _maxCount)
      {
         initialCount = _initCount;
         maxCount = _maxCount;
         initCt++;
         return initSuccess;
      }
      
      virtual Threading::Status acquire(bool block)
      {
         acquireCt++;
         return acquireSuccess;
      }
      
      virtual Threading::Status release()
      {
         releaseCt++;
         return releaseSuccess;
      }
   };
}
}

#endif