//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/volume/platformVolume.h"
#include "platform/impls/base/volume/fileSystemImpl.h"
#include "platform/impls/base/volume/fileImpl.h"
#include "platform/impls/base/volume/directoryImpl.h"
#include "platform/impls/base/volume/fileSystemChangeNotifierImpl.h"

#include "platform/platform.h"

#include "core/frameAllocator.h"
#include "core/crc.h"
#include "core/stream/stream.h"

using namespace ACLib;
using namespace ACLib::FS;

namespace Platform2
{
   // utility class for passing a PlatformFile object to functions that require a Stream interface
   class PlatformFileStream : public Stream
   {
   public:
      PlatformFile* mFile;
      PlatformFileStream(PlatformFile* file)
      {
         mFile = file;
      };

      virtual bool _read(const U32 in_numBytes,  void* out_pBuffer)
      {
         return mFile->read(out_pBuffer, in_numBytes) >= in_numBytes;
      }

      virtual bool _write(const U32 in_numBytes, const void* in_pBuffer)
      {
         return mFile->write(in_pBuffer, in_numBytes) >= in_numBytes;
      }
      virtual bool hasCapability(const Capability) const
      {
         return true;
      }
      /// Gets the position in the stream
      virtual U32  getPosition() const
      {
         return mFile->getPosition();
      }
      /// Sets the position of the stream.  Returns if the new position is valid or not
      virtual bool setPosition(const U32 in_newPosition)
      {
         return mFile->setPosition(in_newPosition, ACLib::FS::File::Begin);
      }
      /// Gets the size of the stream
      virtual U32  getStreamSize()
      {
         return mFile->getSize();
      }
   };

   static String buildFileName(const String& prefix, const Path& path)
   {
      // Need to join the path (minus the root) with our
      // internal path name.
      String file = prefix;
      file = Path::Join(file,'/',path.getPath());
      file = Path::Join(file,'/',path.getFileName());
      file = Path::Join(file,'.',path.getExtension());
      return file;
   }

   struct PlatformDirectory::Config
   {
      const Path& path;
      const Path& fullPath;
      const StrongRefPtr<Internal_::FileSystemImpl>& fileSystemImpl;

      Config(const Path& path_, const Path& fullPath_, const StrongRefPtr<Internal_::FileSystemImpl>& fileSystemImpl_) :
      path(path_), fullPath(fullPath_), fileSystemImpl(fileSystemImpl_)
      {
      }
   };

   struct PlatformFile::Config
   {
      const Path& path;
      const Path& fullPath;
      const StrongRefPtr<Internal_::FileSystemImpl>& fileSystemImpl;

      Config(const Path& path_, const Path& fullPath_, const StrongRefPtr<Internal_::FileSystemImpl>& fileSystemImpl_) :
      path(path_), fullPath(fullPath_), fileSystemImpl(fileSystemImpl_)
      {
      }
   };

   struct PlatformFileSystem::Internal
   {
      String volume;
      StrongRefPtr<Internal_::FileSystemImpl> impl;

      Internal(const String& volume_) :
      volume(volume_),
         impl(GetPlatform()->getFactory().create<Internal_::FileSystemImpl>())
      {
      }
   };

   PlatformFileSystem::PlatformFileSystem(const String& volume) : 
   mImpl(new Internal(volume))
   {
      mChangeNotifier = new PlatformFileSystemChangeNotifier(this);
   }

   PlatformFileSystem::~PlatformFileSystem()
   {
   }

   FileNodeRef PlatformFileSystem::resolve(const Path& path)
   {
      String file = buildFileName(mImpl->volume, path);
      FileNode::Attributes attributes;
      if(!mImpl->impl->getAttributesForPath(attributes, PathToOS(file)))
         return 0;

      if(attributes.flags & FileNode::Directory)
         return new PlatformDirectory(PlatformDirectory::Config(path, file, mImpl->impl));
      else if(attributes.flags & FileNode::File)
         return new PlatformFile(PlatformFile::Config(path, file, mImpl->impl));

      return 0;
   }

   FileNodeRef PlatformFileSystem::create(const Path& path, FileNode::Mode mode)
   {
      if(mode & FileNode::File)
         return new PlatformFile(PlatformFile::Config(path, buildFileName(mImpl->volume, path), mImpl->impl));

      if(mode & FileNode::Directory)
      {
         String file = buildFileName(mImpl->volume, path);
         if(mImpl->impl->createDirectory(PathToOS(file)))
            return new PlatformDirectory(PlatformDirectory::Config(path, file, mImpl->impl));
      }
      return 0;
   }

   bool PlatformFileSystem::remove(const Path& path)
   {
      String file = PathToOS(buildFileName(mImpl->volume, path));
      FileNode::Attributes attributes;
      if(!mImpl->impl->getAttributesForPath(attributes, file))
         return false;

      if(attributes.flags & FileNode::Directory)
         return mImpl->impl->removeDirectory(file);
      else if(attributes.flags & FileNode::File)
         return mImpl->impl->removeFile(file);

      return false;
   }

   bool PlatformFileSystem::rename(const Path& from, const Path& to)
   {
      String fa = PathToOS(buildFileName(mImpl->volume, from));
      String fb = PathToOS(buildFileName(mImpl->volume, to));

      return mImpl->impl->move(fa, fb);
   }

   Path PlatformFileSystem::mapTo(const Path& path)
   {
      return buildFileName(mImpl->volume, path);
   }

   Path PlatformFileSystem::mapFrom(const Path& path)
   {
      String::SizeType volumePathLen = mImpl->volume.length();
      String pathStr = path.getFullPath();

      if ( mImpl->volume.compare( pathStr, volumePathLen, String::NoCase ))
         return Path();
      return pathStr.substr( volumePathLen, pathStr.length() - volumePathLen );
   }

   struct PlatformDirectory::Internal
   {
      ACLib::ScopedPtr<Internal_::DirectoryImpl> impl;
      StrongRefPtr<Internal_::FileSystemImpl> fileSystemImpl;
      ACLib::Path path;
      ACLib::Path fullPath;

      Internal(const PlatformDirectory::Config& config) :
      impl(GetPlatform()->getFactory().create<Internal_::DirectoryImpl>()),
         fileSystemImpl(config.fileSystemImpl), path(config.path), fullPath(config.fullPath)
      {
      }
   };

   PlatformDirectory::PlatformDirectory(const PlatformDirectory::Config& config) :
   mImpl(new Internal(config))
   {
      mImpl->impl->mStatus = FileNode::Closed;
   }

   PlatformDirectory::~PlatformDirectory() 
   {
      mImpl->impl->close();
   }

   ACLib::Path PlatformDirectory::getName() const
   {
      return mImpl->path;
   }

   FileNode::Status PlatformDirectory::getStatus() const
   {
      return mImpl->impl->mStatus;
   }

   bool PlatformDirectory::getAttributes(Attributes* attributes)
   {
      if(!mImpl->fileSystemImpl->getAttributesForPath(*attributes, mImpl->fullPath))
      {
         mImpl->impl->updateStatus();
         return false;
      }

      return true;
   }

   bool PlatformDirectory::open()
   {
      FileNode::Attributes attrs;
      if(!getAttributes(&attrs))
         return false;

      if(!(attrs.flags & FileNode::Directory))
      {
         mImpl->impl->mStatus = NoSuchFile;
         return false;
      }

      if(!mImpl->impl->open(mImpl->fullPath))
      {
         mImpl->impl->updateStatus();
         return false;
      }
      mImpl->impl->mStatus = FileNode::Open;
      return true;
   }

   bool PlatformDirectory::close()
   {
      mImpl->impl->close();
      mImpl->impl->mStatus = FileNode::Closed;
      return true;
   }

   bool PlatformDirectory::read(Attributes* attributes)
   {
      if(mImpl->impl->mStatus != FileNode::Open)
         return false;

      if(!mImpl->impl->read(mImpl->fullPath, *attributes))
      {
         mImpl->impl->updateStatus();
         return false;
      }

      return true;
   }

   U32 PlatformDirectory::calculateChecksum()
   {
      return 0;
   }

   struct PlatformFile::Internal
   {
      ACLib::ScopedPtr<Internal_::FileImpl> impl;
      StrongRefPtr<Internal_::FileSystemImpl> fileSystemImpl;
      ACLib::Path path;
      ACLib::Path fullPath;

      Internal(const PlatformFile::Config& config) : 
      impl(GetPlatform()->getFactory().create<Internal_::FileImpl>()),
         fileSystemImpl(config.fileSystemImpl), path(config.path), fullPath(config.fullPath)
      {
      }
   };

   PlatformFile::PlatformFile(const PlatformFile::Config& config) : 
   mImpl(new Internal(config))
   {
      mImpl->impl->mStatus = FileNode::Closed;
   }

   PlatformFile::~PlatformFile()
   {
      mImpl->impl->close();
   }

   ACLib::Path PlatformFile::getName() const
   {
      return mImpl->path;
   }

   FileNode::Status PlatformFile::getStatus() const
   {
      return mImpl->impl->mStatus;
   }

   bool PlatformFile::getAttributes(Attributes* attributes)
   {
      if(!mImpl->fileSystemImpl->getAttributesForPath(*attributes, mImpl->fullPath))
      {
         mImpl->impl->updateStatus();
         return false;
      }

      return true;
   }

   U32 PlatformFile::getPosition()
   {
      if(mImpl->impl->mStatus == FileNode::Open || mImpl->impl->mStatus == FileNode::EndOfFile)
         return mImpl->impl->getPosition();

      return 0;
   }

   U32 PlatformFile::setPosition(U32 pos, SeekMode mode)
   {
      if(mImpl->impl->mStatus != FileNode::Open && mImpl->impl->mStatus != FileNode::EndOfFile)
         return 0;

      mImpl->impl->mStatus = FileNode::Open;
      return mImpl->impl->setPosition(pos, mode);
   }

   bool PlatformFile::open(AccessMode mode)
   {
      close();
      if(mImpl->fullPath.getFullPath().isEmpty())
         return false;

      if(!mImpl->impl->open(mImpl->fullPath, mode))
      {
         mImpl->impl->updateStatus();
         return false;
      }
      mImpl->impl->mStatus = FileNode::Open;
      return true;
   }

   bool PlatformFile::close()
   {
      mImpl->impl->close();
      mImpl->impl->mStatus = FileNode::Closed;
      return true;
   }

   U32 PlatformFile::read(void* dst, U32 size)
   {
      if(mImpl->impl->mStatus != FileNode::Open && mImpl->impl->mStatus != FileNode::EndOfFile)
         return 0;

      U32 bytesRead = mImpl->impl->read(dst, size);
      if(bytesRead != size && mImpl->impl->mStatus != FileNode::EndOfFile)
         mImpl->impl->updateStatus();

      return bytesRead;
   }

   U32 PlatformFile::write(const void* src, U32 size)
   {
      if(mImpl->impl->mStatus != FileNode::Open && mImpl->impl->mStatus != FileNode::EndOfFile)
         return 0;

      U32 bytesWritten = mImpl->impl->write(src, size);
      if(bytesWritten != size)
         mImpl->impl->updateStatus();

      return bytesWritten;
   }

   U32 PlatformFile::calculateChecksum()
   {
      if (!open( Read ))
         return 0;

      U64   fileSize = getSize();

      bool useFA = fileSize < FrameAllocator::getHighWaterMark();

      FrameTemp<U8>   fBuf( useFA ? fileSize : 1);

      U32 crc = CRC::INVALID_CRC;

      if (useFA)
      {
         U32 bytesRead = read(fBuf, fileSize);

         close();

         if ( bytesRead != fileSize )
            return 0;

         crc = CRC::calculateCRC(fBuf, fileSize);
      }
      else
      {
         PlatformFileStream fStream(this);
         crc = CRC::calculateCRCStream(&fStream);
         close();
      }

      return crc;
   }

   struct PlatformFileSystemChangeNotifier::Internal
   {
      ACLib::ScopedPtr<Internal_::FileSystemChangeNotifierImpl> impl;
      Vector<ACLib::Path> paths;

      Internal() :
      impl(GetPlatform()->getFactory().create<Internal_::FileSystemChangeNotifierImpl>())
      {
      }
   };

   PlatformFileSystemChangeNotifier::PlatformFileSystemChangeNotifier(FileSystem* fs) :
   FileSystemChangeNotifier(fs), 
      mImpl(new Internal)
   {
      mImpl->impl->onDirChanged.bind(this, &PlatformFileSystemChangeNotifier::onDirChanged);
   }

   PlatformFileSystemChangeNotifier::~PlatformFileSystemChangeNotifier()
   {
   }

   void PlatformFileSystemChangeNotifier::internalProcessOnce()
   {
      mImpl->impl->process();
   }

   bool PlatformFileSystemChangeNotifier::internalAddNotification(const Path& dir)
   {
      for(U32 i = 0; i < mImpl->paths.size(); i++)
         if(mImpl->paths[i] == dir)
            return false;

      String fullPath = PathToOS(mFS->mapTo(dir));
      return mImpl->impl->addNotification(fullPath);
   }

   bool PlatformFileSystemChangeNotifier::internalRemoveNotification(const Path& dir)
   {
      for(U32 i = 0; i < mImpl->paths.size(); i++)
      {
         if(mImpl->paths[i] == dir)
         {
            String fullPath = PathToOS(mFS->mapTo(dir));
            return mImpl->impl->removeNotification(dir);
         }
      }
      return false;
   }

   void PlatformFileSystemChangeNotifier::onDirChanged(const Path& dir)
   {
      Path platformDir = mFS->mapFrom(PathToPlatform(dir));
      internalNotifyDirChanged(platformDir);
   }
}
