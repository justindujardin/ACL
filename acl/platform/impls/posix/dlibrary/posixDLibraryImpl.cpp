//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include <dlfcn.h>

#include "core/strings/str.h"
#include "core/types.h"
#include "platform/impls/posix/dlibrary/posixDLibraryImpl.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
PosixDLibraryImpl::PosixDLibraryImpl() : mHandle(NULL) {}

PosixDLibraryImpl::~PosixDLibraryImpl() {}

bool PosixDLibraryImpl::init(const String &file) {
  mHandle = dlopen(file.c_str(), RTLD_LAZY | RTLD_LOCAL);
  return mHandle != NULL;
}

void *PosixDLibraryImpl::bind(const String &name) {
  return mHandle ? dlsym(mHandle, name.c_str()) : NULL;
}

void PosixDLibraryImpl::close() {
  dlclose(mHandle);
  mHandle = NULL;
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib