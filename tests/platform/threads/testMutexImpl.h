//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_MUTEXIMPL_H_
#define ACL_PLATFORM_TEST_MUTEXIMPL_H_

#include "platform/impls/base/threads/mutexImpl.h"

namespace Platform2
{
namespace Internal_
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
}

#endif