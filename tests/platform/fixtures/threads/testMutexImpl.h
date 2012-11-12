//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_MUTEXIMPL_H_
#define ACL_PLATFORM_TEST_MUTEXIMPL_H_

#include "platform/impls/base/threads/mutexImpl.h"

using namespace Platform2;
using namespace Platform2::Internal_;

namespace TestPlatform
{
   class TestMutexImpl : public MutexImpl
   {
   public:
      Threading::Status lockSuccess;
      Threading::Status unlockSuccess;
      U32 lockCt;
      U32 unlockCt;
      U32 initCt;
      bool valid;
      TestMutexImpl(bool isValid) : lockSuccess(Threading::Status_NoError), 
         unlockSuccess(Threading::Status_NoError), lockCt(0), unlockCt(0), initCt(0)
         , valid(isValid)
      {
      }

      virtual bool init()
      {
         ++initCt;
         return valid;
      }

      virtual Threading::Status lock(bool block)
      {
         lockCt++;
         return lockSuccess;
      }

      virtual Threading::Status unlock()
      {
         unlockCt++;
         return unlockSuccess;
      }
   };
}

#endif
