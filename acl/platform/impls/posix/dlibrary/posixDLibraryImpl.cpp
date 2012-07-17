//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include <dlfcn.h>

#include "platform/impls/posix/dlibrary/posixDLibraryImpl.h"
#include "core/types.h"
#include "core/strings/str.h"

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