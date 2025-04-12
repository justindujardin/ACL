//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_POSIX_DIRECTORYIMPL_H_
#define ACL_PLATFORM_POSIX_DIRECTORYIMPL_H_

#include "platform/impls/base/volume/directoryImpl.h"
#include <dirent.h>

namespace ACLib {
namespace Platform {
namespace Internal_ {
class PosixDirectoryImpl : public DirectoryImpl {
public:
  PosixDirectoryImpl();
  virtual void updateStatus();
  virtual bool open(const Path &path);
  virtual void close();
  virtual bool read(const Path &path, FS::FileNode::Attributes &attributes);

private:
  DIR *mHandle;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
