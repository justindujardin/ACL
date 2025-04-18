//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FILEIMPL_H_
#define ACL_PLATFORM_TEST_FILEIMPL_H_

#include "platform/impls/base/volume/fileImpl.h"

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib {
namespace TestPlatform {
class TestFileImpl : public FileImpl {
public:
  virtual void updateStatus() {}

  virtual U32 getPosition() { return 0; }

  virtual U32 setPosition(U32 pos, FS::File::SeekMode mode) { return 0; }

  virtual bool open(const Path &path, FS::File::AccessMode mode) {
    return false;
  }

  virtual void close() {}

  virtual U32 read(void *dst, U32 size) { return 0; }

  virtual U32 write(const void *src, U32 size) { return 0; }
};
} // namespace TestPlatform
} // namespace ACLib
#endif
