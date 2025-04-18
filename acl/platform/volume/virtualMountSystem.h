//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _CORE_VMS_H_
#define _CORE_VMS_H_

#include "core/containers/tMap.h"
#include "platform/volume/volume.h"

namespace ACLib {
namespace FS {

/// The VirtualMountSystem extend the mount system by allowing you to mount and
/// access multiple filesystems on a single root.  This allows you to mount
/// multiple volume files on a single root without needing to change source
/// paths that reference those volumes to use new roots.  For instance, you
/// could mount "missions1.zip" and "missions2.zip", on the same root "", and
/// you could access files from either one.
///
/// If you need to mount a writeable filesystem on a VMS, you should mount it as
/// the first filesystem on a root. File writes are handled as follows: 1) If
/// you try to open a file for write, and it exists on one of the virtual
/// mounts, that is the mount that will be used for writing.  But if the
/// filesystem is read-only then the writes will fail (and you will get
/// appropriate error codes from your FileStream or whatever). 2) If you try to
/// open a file for write that doesn't exist, the VMS falls back to the default
/// MountSystem behavior, which is to use the first filesystem mounted on the
/// root.  If this filesystem happens to be writable, then you will be able to
/// write to the file. 3) Nothing special is done for the WriteAppend case; that
/// is, it follows the same logic as if you were just trying to Write the file.
///
/// Because of rule 1) above, you should take care that any files you need to
/// write (such as prefs.cs), are not included in read-only zip archive files.
class VirtualMountSystem : public MountSystem {
  typedef MountSystem Parent;

public:
  VirtualMountSystem() : mUseParentFind(false) {}

  virtual ~VirtualMountSystem() {}
  virtual bool mount(String root, FileSystemRef fs);
  virtual bool mount(String root, const Path &path);
  virtual FileSystemRef unmount(String root);
  virtual bool unmount(FileSystemRef fs);
  virtual S32 findByPattern(const Path &inBasePath, const String &inFilePattern,
                            bool inRecursive, Vector<String> &outList,
                            bool includeDirs = false);
  virtual bool createPath(const Path &path);

protected:
  virtual void _log(const String &msg);
  virtual FileSystemRef _removeMountFromList(String root);
  virtual FileSystemRef _getFileSystemFromList(const Path &path) const;
  // map of path to list of file systems containing path
  typedef Map<String, Vector<FileSystemRef>> PathFSMap;
  // map of root to PathFSMap for that root
  typedef Map<String, PathFSMap *> RootToPathFSMap;

  RootToPathFSMap mMountMap;
  bool mUseParentFind; // this is needed because findByParent calls itself
                       // recursively and in some cases we don't want it to
                       // re-enter our version
};

} // namespace FS
} // namespace ACLib

#endif
