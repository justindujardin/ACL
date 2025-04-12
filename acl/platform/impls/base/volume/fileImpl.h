//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_FILEIMPL_H_
#define ACL_PLATFORM_FILEIMPL_H_

#include "core/util/noncopyable.h"
#include "platform/volume/volume.h"

namespace ACLib {
namespace Platform {
namespace Internal_ {
class FileImpl : private Noncopyable {
public:
  virtual ~FileImpl() {}

  virtual void updateStatus() = 0;
  virtual U32 getPosition() = 0;
  virtual U32 setPosition(U32 pos, FS::File::SeekMode mode) = 0;
  virtual bool open(const Path &path, FS::File::AccessMode mode) = 0;
  virtual void close() = 0;
  virtual U32 read(void *dst, U32 size) = 0;
  virtual U32 write(const void *src, U32 size) = 0;

  FS::FileNode::Status mStatus;
};
} // namespace Internal_
} // namespace Platform
} // namespace ACLib
#endif
