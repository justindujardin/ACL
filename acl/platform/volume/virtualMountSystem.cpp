//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/volume/virtualMountSystem.h"
#include "platform/platform.h"
//#include "console/console.h"

namespace ACLib {
namespace FS {

bool gVMSVerboseLog = false;

bool VirtualMountSystem::mount(String root, FileSystemRef fs) {
  bool ok = Parent::mount(root, fs);
  if (!ok)
    return false;

  PathFSMap *rootDict = NULL;
  root = String::ToLower(root);

  if (!mMountMap.tryGetValue(root, rootDict)) {
    rootDict = new PathFSMap();
    mMountMap[root] = rootDict;
  }

  U32 start = Platform::GetPlatform()->getRealMilliseconds();

  // get the paths from the fs and add them to the rootDict
  Vector<String> paths;

  // we'll use the mount system's findByPattern function to build the path list.
  // but, we want to override its default behavior so that it searches only the
  // desired fs.
  _setFindByPatternOverrideFS(fs);

  Path basePath;
  // we use an empty root so that the resulting filenames don't have the root
  // filename in them. we don't want to include the root in the dict has
  // entries.  we can omit the root because we have specified an override FS;
  // the search would fail otherwise.
  basePath.setRoot(root);
  basePath.setPath("/");
  mUseParentFind = true;
  if (findByPattern(basePath, "*.*", true, paths, true) == -1) {
    // this is probably a problem
    _log("Unable to get paths from filesystem for virtual mount");
    _setFindByPatternOverrideFS(NULL);
    mUseParentFind = false;
    return false;
  }

  _setFindByPatternOverrideFS(NULL);
  mUseParentFind = false;

  for (S32 i = 0; i < paths.size(); ++i) {
    String path = String::ToLower(paths[i]);

    // is it a directory? if so remove dir prefix
    String dirPrefix = "DIR:";
    String::SizeType dIdx = path.find(dirPrefix, 0, String::NoCase);
    if (dIdx == 0)
      path = path.substr(dirPrefix.length());
    // omit leading /
    if (path[(String::SizeType)0] == '/')
      path = path.substr(1);

    // warn about duplicate files (not directories)
    // JMQ: disabled this, it false alarms at startup because the mount doc
    // always mounts the root before other processing mounts.  still, would be
    // useful, maybe change the mount doc to not mount root?
    if (dIdx != 0 && (*rootDict)[path].size() > 0)
      _log(String::ToString("Duplicate file path detected, first volume "
                            "containing file will be used: %s",
                            path.c_str()));

    (*rootDict)[path].push_back(fs);
  }

  if (gVMSVerboseLog)
    _log(String::ToString("Indexed virtual file system in %ums",
                          Platform::GetPlatform()->getRealMilliseconds() -
                              start));

  return true;
}

bool VirtualMountSystem::mount(String root, const Path &path) {
  AssertFatal(false, "This function not supported in virtual mount system");
  return false;
}

FileSystemRef VirtualMountSystem::unmount(String root) {
  FileSystemRef ret = Parent::unmount(root);

  // clear all filesystem lists for root.
  PathFSMap *rootDict = NULL;
  root = String::ToLower(root);
  if (!mMountMap.tryGetValue(root, rootDict))
    return ret;

  // buh bye
  mMountMap.erase(root);
  delete rootDict;

  return ret;
}

bool VirtualMountSystem::unmount(FileSystemRef fs) {
  bool unmounted = Parent::unmount(fs);
  if (!unmounted)
    return false;

  // this is a linear time operation, because we have to search every path in
  // all roots to remove references to the fs. contant time operation can be
  // achieved be using the unmount(string) version, which unmounts all
  // filesystems for a given root and so doesn't need to do any searching.
  U32 start = Platform::GetPlatform()->getRealMilliseconds();
  for (RootToPathFSMap::Iterator riter = mMountMap.begin();
       riter != mMountMap.end(); ++riter) {
    PathFSMap *rootDict = (*riter).value;
    for (PathFSMap::Iterator piter = rootDict->begin();
         piter != rootDict->end(); ++piter) {
      Vector<FileSystemRef> &plist = (*piter).value;
      for (S32 i = plist.size() - 1; i >= 0; i--) {
        if (plist[i].getPointer() == fs.getPointer())
          plist.erase(i);
      }
    }
  }

  if (gVMSVerboseLog)
    _log(String::ToString("Unmounted virtual file system in %ums",
                          Platform::GetPlatform()->getRealMilliseconds() -
                              start));

  return true;
}

S32 VirtualMountSystem::findByPattern(const Path &inBasePath,
                                      const String &inFilePattern,
                                      bool inRecursive, Vector<String> &outList,
                                      bool includeDirs) {
  if (mUseParentFind)
    // use parent version
    return Parent::findByPattern(inBasePath, inFilePattern, inRecursive,
                                 outList, includeDirs);

  // don't want to re-enter this version
  mUseParentFind = true;

  // kind of cheesy, just call find by pattern on each File system mounted on
  // the root
  for (Vector<MountFS>::const_iterator itr = mMountList.begin();
       itr != mMountList.end(); itr++) {
    if (itr->root.equal(inBasePath.getRoot(), String::NoCase)) {
      FileSystemRef fsref = itr->fileSystem;
      _setFindByPatternOverrideFS(fsref);
      Parent::findByPattern(inBasePath, inFilePattern, inRecursive, outList,
                            includeDirs);
      _setFindByPatternOverrideFS(NULL);
    }
  }

  mUseParentFind = false;

  return outList.size();
}

bool VirtualMountSystem::createPath(const Path &path) {
  bool ret = Parent::createPath(path);

  if (ret) {
    // make sure the filesystem that owns the path has the path elements
    // in its search table (so that we can open the file, if it is new)
    String root = String::ToLower(path.getRoot());
    FileSystemRef fsRef = getFileSystem(path);

    PathFSMap *rootDict = mMountMap[root];
    if (rootDict) {
      // add all directories in the path
      // add the filename

      // Start from the top and work our way down
      Path sub, dir;
      dir.setPath("");
      for (U32 i = 0; i < path.getDirectoryCount(); i++) {
        sub.setPath(path.getDirectory(i));
        dir.appendPath(sub);

        Vector<FileSystemRef> &fsList =
            (*rootDict)[String::ToLower(dir.getPath())];
        Vector<FileSystemRef>::iterator iter =
            Find(fsList.begin(), fsList.end(), fsRef);

        if (iter == fsList.end())
          fsList.push_back(fsRef);
      }

      // add full file path
      Vector<FileSystemRef> &fsList =
          (*rootDict)[String::ToLower(path.getFullPath(false))];
      Vector<FileSystemRef>::iterator iter =
          Find(fsList.begin(), fsList.end(), fsRef);

      if (iter == fsList.end())
        fsList.push_back(fsRef);
    }
  }

  return ret;
}

void VirtualMountSystem::_log(const String &msg) {
  String newMsg = "VirtualMountSystem: " + msg;
  // TODO: Logging
  AssertFatal(false, "Not Implemented");
  // Con::warnf("%s", newMsg.c_str());
}

FileSystemRef VirtualMountSystem::_removeMountFromList(String root) {
  return Parent::_removeMountFromList(root);
}

FileSystemRef
VirtualMountSystem::_getFileSystemFromList(const Path &fullpath) const {
  String root = String::ToLower(fullpath.getRoot());
  String path = fullpath.getFullPath(false);
  // eat leading slash
  if (path[(String::SizeType)0] == '/')
    path = path.substr(1);
  // lowercase it
  path = String::ToLower(path);

  // find the dictionary for root
  PathFSMap *rootDict = NULL;
  if (!mMountMap.tryGetValue(root, rootDict))
    return NULL;

  // see if we have a FS list for this path
  Vector<FileSystemRef> &fsList = (*rootDict)[path];
  if (fsList.size() == 0) {
    // no exact match for path, defer to parent
    return Parent::_getFileSystemFromList(fullpath);
  } else {
    // return first filesystem owning the path
    return fsList[0];
  }
}

} // namespace FS
} // namespace ACLib
