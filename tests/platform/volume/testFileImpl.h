//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_FILEIMPL_H_
#define ACL_PLATFORM_TEST_FILEIMPL_H_

#include "platform/impls/base/volume/fileImpl.h"

namespace Platform2
{
namespace Internal_
{
   class TestFileImpl : public FileImpl
   {
   public:
      
      virtual void updateStatus()
      {
      }
      
      virtual U32 getPosition()
      {
         return 0;
      }
      
      virtual U32 setPosition(U32 pos, Torque::FS::File::SeekMode mode)
      {
         return 0;
      }
      
      virtual bool open(const Torque::Path& path, Torque::FS::File::AccessMode mode)
      {
         return false;
      }
      
      virtual void close()
      {
      }
      
      virtual U32 read(void* dst, U32 size)
      {
         return 0;
      }
      
      virtual U32 write(const void* src, U32 size)
      {
         return 0;
      }
   };
}
}

#endif