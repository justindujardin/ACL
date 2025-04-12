//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/volume/posixDirectoryImpl.h"
#include <errno.h>
#include <sys/stat.h>

using namespace ACLib::FS;

namespace ACLib {
namespace Platform {
namespace Internal_ {
namespace Posix {
extern void copyStatAttributes(const struct stat &info,
                               FileNode::Attributes *attr);
}
using namespace Posix;
PosixDirectoryImpl::PosixDirectoryImpl() : mHandle(0) {}

void PosixDirectoryImpl::updateStatus() {
  switch (errno) {
  case EACCES:
    mStatus = FileNode::AccessDenied;
    break;
  case ENOTDIR:
    mStatus = FileNode::NoSuchFile;
    break;
  case ENOENT:
    mStatus = FileNode::NoSuchFile;
    break;
  default:
    mStatus = FileNode::UnknownError;
    break;
  }
}

bool PosixDirectoryImpl::open(const Path &path) {
  mHandle = opendir(path.getFullPath().c_str());
  return mHandle != 0;
}

void PosixDirectoryImpl::close() {
  if (mHandle) {
    closedir(mHandle);
    mHandle = 0;
  }
}

bool PosixDirectoryImpl::read(const Path &path,
                              FileNode::Attributes &attributes) {
  struct dirent *de = readdir(mHandle);

  if (!de) {
    mStatus = FileNode::EndOfFile;
    return false;
  }

  // Skip "." and ".." entries
  if (de->d_name[0] == '.' && (de->d_name[1] == '\0' ||
                               (de->d_name[1] == '.' && de->d_name[2] == '\0')))
    return read(path, attributes);

  // The dirent structure doesn't actually return much beside
  // the name, so we must call stat for more info.
  struct stat info;
  String file = path.getFullPath() + "/" + de->d_name;

  int error = stat(file.c_str(), &info);

  if (error < 0) {
    updateStatus();
    return false;
  }
  copyStatAttributes(info, &attributes);
  attributes.name = de->d_name;
  return true;
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib