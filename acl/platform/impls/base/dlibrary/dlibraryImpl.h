//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_DLIBRARYIMPL_H_
#define ACL_PLATFORM_DLIBRARYIMPL_H_

#include "core/util/noncopyable.h"

namespace ACLib {
class String;

namespace Platform {
namespace Internal_ {
class DLibraryImpl : private Noncopyable {
public:
  virtual ~DLibraryImpl() {}
  virtual bool init(const String &file) = 0;
  virtual void *bind(const String &name) = 0;
  virtual void close() = 0;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
