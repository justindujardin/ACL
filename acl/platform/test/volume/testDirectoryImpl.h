//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_TEST_DIRECTORYIMPL_H_
#define TORQUE_PLATFORM2_TEST_DIRECTORYIMPL_H_

#include "platform2/impls/base/volume/directoryImpl.h"

namespace Platform2
{
namespace Internal_
{
   class TestDirectoryImpl : public DirectoryImpl
   {
   public:
      virtual void updateStatus()
      {
      }
      
      virtual bool open(const Torque::Path& path)
      {
         return false;
      }
      
      virtual void close()
      {
      }
      
      virtual bool read(const Torque::Path& path, Torque::FS::FileNode::Attributes& attributes)
      {
         return false;
      }
   };
}
}

#endif