//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_DIRECTORYIMPL_H_
#define ACL_PLATFORM_TEST_DIRECTORYIMPL_H_

#include "platform/impls/base/volume/directoryImpl.h"

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib {
namespace TestPlatform {
class TestDirectoryImpl : public DirectoryImpl {
public:
  virtual void updateStatus() {}

  virtual bool open(const Path &path) { return false; }

  virtual void close() {}

  virtual bool read(const Path &path, FS::FileNode::Attributes &attributes) {
    return false;
  }
};
} // namespace TestPlatform
} // namespace ACLib

#endif
