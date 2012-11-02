//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
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

      ACLib::TypeRebind& GetPlatformFactory()
      {
         static ACLib::TypeRebind factory;
         return factory;
      }
   }
}
