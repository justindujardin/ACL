//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_MAC_THREADIMPL_H_
#define ACL_PLATFORM_MAC_THREADIMPL_H_

#include "platform/impls/base/threads/threadImpl.h"

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
