//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_DLIBRARYIMPL_H_
#define ACL_PLATFORM_POSIX_DLIBRARYIMPL_H_

#include "platform/impls/base/dlibrary/dlibraryImpl.h"

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
