//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_POSIX_FILEIMPL_H_
#define TORQUE_PLATFORM2_POSIX_FILEIMPL_H_

#include "platform2/impls/base/volume/fileImpl.h"
#include <stdio.h>

namespace Platform2
{
namespace Internal_
{
   class PosixFileImpl : public FileImpl
   {
   public:            
      PosixFileImpl();
      virtual void updateStatus();
      virtual U32 getPosition();
      virtual U32 setPosition(U32 pos, Torque::FS::File::SeekMode mode);
      virtual bool open(const Torque::Path& path, Torque::FS::File::AccessMode mode);
      virtual void close();
      virtual U32 read(void* dst, U32 size);
      virtual U32 write(const void* src, U32 size);
      
   private:
      FILE* mHandle;
   };
}
}

#endif