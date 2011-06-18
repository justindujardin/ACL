//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/base/platformCreator.h"
#include "platform/impls/base/threads/threadImpl.h"

namespace Platform2
{
namespace Internal_
{
   void InitializePlatform()
   {
      ThreadImpl::RegisterCurrentThreadAsMain();
   }
   
   Torque::TypeRebind& GetPlatformFactory()
   {
      static Torque::TypeRebind factory;
      return factory;
   }
}
}