//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_DLIBRARYIMPL_H_
#define TORQUE_PLATFORM2_POSIX_DLIBRARYIMPL_H_

#include "platform2/impls/base/dlibrary/dlibraryImpl.h"

namespace Platform2
{
   namespace Internal_
   {
      class PosixDLibraryImpl : public DLibraryImpl
      {
      public:         
         PosixDLibraryImpl();
         virtual ~PosixDLibraryImpl();
         virtual bool init(const String& file);
         virtual void* bind(const String& name);
         virtual void close();
      private:
         void* mHandle;
      };
   }
}

#endif