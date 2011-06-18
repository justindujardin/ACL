//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <stdio.h>
#include <sys/stat.h>

#include "platform2/impls/posix/volume/posixFileSystemImpl.h"

#ifndef NGROUPS_UMAX
   #define NGROUPS_UMAX 32
#endif

using namespace Torque;
using namespace Torque::FS;

namespace Platform2
{
namespace Internal_
{
namespace Posix
{
   static uid_t _Uid;                     // Current user id
   static int _GroupCount;                // Number of groups in the table
   static gid_t _Groups[NGROUPS_UMAX+1];  // Table of all the user groups

   void copyStatAttributes(const struct stat& info, FileNode::Attributes* attr)
   {
      // We need to user and group id's in order to determin file
      // read-only access permission. This information is only retrieved
      // once per execution.
      if (!_Uid)
      {
         _Uid = getuid();
         _GroupCount = getgroups(NGROUPS_UMAX,_Groups);
         _Groups[_GroupCount++] = getegid();
      }

      // Fill in the return struct. The read-only flag is determined
      // by comparing file user and group ownership.
      attr->flags = 0;
      if (S_ISDIR(info.st_mode))
         attr->flags |= FileNode::Directory;
         
      if (S_ISREG(info.st_mode))
         attr->flags |= FileNode::File;
         
      if (info.st_uid == _Uid)
      {
         if (!(info.st_mode & S_IWUSR))
            attr->flags |= FileNode::ReadOnly;
      }
      else
      {
         S32 i = 0;
         for (; i < _GroupCount; i++)
         {
            if (_Groups[i] == info.st_gid)
               break;
         }
         if (i != _GroupCount)
         {
            if (!(info.st_mode & S_IWGRP))
               attr->flags |= FileNode::ReadOnly;
         }
         else
         {
            if (!(info.st_mode & S_IWOTH))
               attr->flags |= FileNode::ReadOnly;
         }
      }

      attr->size = info.st_size;
      attr->mtime = UnixTimeToTime(info.st_mtime);
      attr->atime = UnixTimeToTime(info.st_atime);
   }
}
using namespace Posix;

   bool PosixFileSystemImpl::getAttributesForPath(FileNode::Attributes& attributes, const Path& path)
   {
      struct stat info;
      if(stat(path.getFullPath().c_str(), &info) == 0)
      {
         copyStatAttributes(info, &attributes);
         return true;
      }
      
      return false;
   }
   
   bool PosixFileSystemImpl::createDirectory(const Path& path)
   {
      return !mkdir(path.getFullPath().c_str(),S_IRWXU | S_IRWXG | S_IRWXO);     
   }
   
   bool PosixFileSystemImpl::removeDirectory(const Path& path)
   {
      return !rmdir(path.getFullPath().c_str());
   }
   
   bool PosixFileSystemImpl::removeFile(const Path& path)
   {
      return !unlink(path.getFullPath().c_str());
   }
   
   bool PosixFileSystemImpl::move(const Path& from, const Path& to)
   {
      return !rename(from.getFullPath().c_str(), to.getFullPath().c_str());
   }
}
}
