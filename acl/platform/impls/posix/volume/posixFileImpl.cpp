//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/posix/volume/posixFileImpl.h"
#include <errno.h>

using namespace ACLib::FS;

namespace ACLib
{
   namespace Platform
   {
      namespace Internal_
      {
         PosixFileImpl::PosixFileImpl()
         {
            mHandle = 0;
         }

         void PosixFileImpl::updateStatus()
         {
            switch (errno)
            {
            case EACCES:   mStatus = FileNode::AccessDenied;    break;
            case ENOSPC:   mStatus = FileNode::FileSystemFull;  break;
            case ENOTDIR:  mStatus = FileNode::NoSuchFile;      break;
            case ENOENT:   mStatus = FileNode::NoSuchFile;      break;
            case EISDIR:   mStatus = FileNode::AccessDenied;    break;
            case EROFS:    mStatus = FileNode::AccessDenied;    break;
            default:       mStatus = FileNode::UnknownError;    break;
            }
         }

         U32 PosixFileImpl::getPosition()
         {
            return ftell(mHandle);
         }

         U32 PosixFileImpl::setPosition(U32 pos, File::SeekMode mode)
         {
            S32 fmode = 0;
            switch (mode)
            {
            case File::Begin:    fmode = SEEK_SET; break;
            case File::Current:  fmode = SEEK_CUR; break;
            case File::End:      fmode = SEEK_END; break;
            default:       break;
            }

            if (fseek(mHandle, pos, fmode))
            {
               mStatus = FileNode::UnknownError;
               return 0;
            }

            return ftell(mHandle);
         }

         bool PosixFileImpl::open(const Path& path, File::AccessMode mode)
         {
            const char* fmode = "r";
            switch (mode)
            {
            case File::Read:        fmode = "r"; break;
            case File::Write:       fmode = "w"; break;
            case File::ReadWrite:   
               {
                  fmode = "r+";
                  // Make sure the file exists
                  FILE* temp = fopen(path.getFullPath().c_str(), "a+");
                  fclose(temp);
                  break;
               }
            case File::WriteAppend: fmode = "a"; break;
            default:          break;
            }

            if(!(mHandle = fopen(path.getFullPath().c_str(), fmode)))
               return false;

            return true;
         }

         void PosixFileImpl::close()
         {
            if(mHandle)
            {
               fflush(mHandle);
               fclose(mHandle);
               mHandle = 0;
            }
         }

         U32 PosixFileImpl::read(void* dst, U32 size)
         {
            U32 bytesRead = fread(dst, 1, size, mHandle);

            if (bytesRead != size)
            {
               if(feof(mHandle))
                  mStatus = FileNode::EndOfFile;
               else
                  updateStatus();
            }

            return bytesRead;
         }

         U32 PosixFileImpl::write(const void* src, U32 size)
         {
            return fwrite(src, 1, size, mHandle);
         }
      }
   }
}