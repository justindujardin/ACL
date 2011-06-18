//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <dlfcn.h>

#include "platform2/impls/posix/dlibrary/posixDLibraryImpl.h"
#include "core/types/types.h"
#include "core/util/str.h"

namespace Platform2
{
   namespace Internal_
   {
      PosixDLibraryImpl::PosixDLibraryImpl() :
         mHandle(NULL)
      {
      }
      
      PosixDLibraryImpl::~PosixDLibraryImpl()
      {
      }
      
      bool PosixDLibraryImpl::init(const String& file)
      {
         mHandle = dlopen(file.c_str(), RTLD_LAZY | RTLD_LOCAL);
         return mHandle != NULL;
      }
      
      void* PosixDLibraryImpl::bind(const String& name)
      {
         return mHandle ? dlsym(mHandle, name.c_str()) : NULL;
      }
      
      void PosixDLibraryImpl::close()
      {
         dlclose(mHandle);
         mHandle = NULL;
      }
   }
}