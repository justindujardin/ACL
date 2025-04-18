//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/volume/volume.h"
#include "core/strings/findMatch.h"
#include "platform/platform.h"
#include "platform/volume/virtualMountSystem.h"

namespace ACLib {
using namespace FS;

//-----------------------------------------------------------------------------

bool FileSystemChangeNotifier::addNotification(const Path &path) {
  // strip the filename and extension - we notify on dirs
  Path dir(path.getFullPath());

  dir.setFileName(String());
  dir.setExtension(String());

  // first lookup the dir to see if we already have an entry for it...
  DirMap::Iterator itr = mDirMap.find(dir);

  FileInfoList *fileList = NULL;
  bool addedDir = false;

  if (itr != mDirMap.end()) {
    fileList = &(itr->value);

    // look for the file and return if we find it
    for (U32 i = 0; i < fileList->getSize(); i++) {
      if ((*fileList)[i].filePath == path)
        return true;
    }
  } else {
    // otherwise we need to add the dir to our map and let the inherited class
    // add it
    itr = mDirMap.insert(dir, FileInfoList());

    fileList = &(itr->value);

    addedDir = true;
  }

  FileNode::Attributes attr;

  GetFileAttributes(path, &attr);

  FileInfo newInfo;

  newInfo.filePath = path;
  newInfo.savedLastModTime = attr.mtime;

  fileList->pushBack(newInfo);

  return addedDir ? internalAddNotification(dir) : true;
}

bool FileSystemChangeNotifier::removeNotification(const Path &path) {
  // strip the filename and extension - we notify on dirs
  Path dir(path.getFullPath());

  dir.setFileName(String());
  dir.setExtension(String());

  DirMap::Iterator itr = mDirMap.find(dir);

  if (itr == mDirMap.end())
    return true;

  FileInfoList &fileList = itr->value;

  // look for the file and return if we find it
  for (U32 i = 0; i < fileList.getSize(); i++) {
    if (fileList[i].filePath == path) {
      fileList.erase(i);
      break;
    }
  }

  // IF we removed the last file
  //    THEN get rid of the dir from our map and notify inherited classes
  if (fileList.getSize() == 0) {
    mDirMap.erase(dir);

    return internalRemoveNotification(dir);
  }

  return true;
}

void FileSystemChangeNotifier::startNotifier() {
  // update the timestamps of all the files we are managing

  DirMap::Iterator itr = mDirMap.begin();

  for (; itr != mDirMap.end(); ++itr) {
    FileInfoList &fileList = itr->value;

    for (U32 i = 0; i < fileList.getSize(); i++) {
      FileInfo &fInfo = fileList[i];

      FileNode::Attributes attr;

      bool success = GetFileAttributes(fInfo.filePath, &attr);

      AssertWarn(success, "[FileSystemChangeNotifier::startNotifier] failed to "
                          "get attributes on file");

      fInfo.savedLastModTime = attr.mtime;
    }
  }

  mNotifying = true;

  // TODO: Process List
  AssertFatal(false, "Process list not implemented");
  // Process::notify( this, &FileSystemChangeNotifier::process,
  // PROCESS_LAST_ORDER );
}

void FileSystemChangeNotifier::process() { internalProcessOnce(); }

void FileSystemChangeNotifier::stopNotifier() {
  mNotifying = false;

  // TODO: Process List
  AssertFatal(false, "Process list not implemented");
  // Process::remove( this, &FileSystemChangeNotifier::process );
}

void FileSystemChangeNotifier::internalNotifyDirChanged(const Path &dir) {
  // TODO: Logging
  // Con::warnf("[FileSystemChangeNotifier::internalNotifyDirChanged] : [%s]",
  // dir.getFullPath().c_str());

  DirMap::Iterator itr = mDirMap.find(dir);

  if (itr == mDirMap.end())
    return;

  // look for any files which have been modified
  FileInfoList &fileList = itr->value;

  Vector<Path> changedFiles;

  for (U32 i = 0; i < fileList.getSize(); i++) {
    FileInfo &fInfo = fileList[i];

    FileNode::Attributes attr;

    bool success = GetFileAttributes(fInfo.filePath, &attr);

    AssertFatal(success, "[FileSystemChangeNotifier::internalNotifyDirChanged] "
                         "failed to get attributes on file");

    if (attr.mtime > fInfo.savedLastModTime) {
      fInfo.savedLastModTime = attr.mtime;

      // TODO: Logging
      // Con::warnf("    : file changed [%s]",
      // fInfo.filePath.getFullPath().c_str());

      // Queue up notifications.  If a file is removed from the monitored list,
      // inside of a signal, bad things can happen.
      changedFiles.push_back(fInfo.filePath);
    }
  }

  for (S32 i = 0; i < changedFiles.size(); i++) {
    notifyFileChanged(changedFiles[i]);
  }
}

void FileSystemChangeNotifier::notifyFileChanged(const Path &path) {
  mChangeSignal.trigger(path);
}

//-----------------------------------------------------------------------------

FileSystem::FileSystem() : mChangeNotifier(NULL), mReadOnly(false) {}

FileSystem::~FileSystem() {
  delete mChangeNotifier;
  mChangeNotifier = NULL;
}

File::File() {}
File::~File() {}
Directory::Directory() {}
Directory::~Directory() {}

FileNode::FileNode() : mChecksum(0) {}

Time FileNode::getModifiedTime() {
  Attributes attrs;

  bool success = getAttributes(&attrs);

  if (!success)
    return Time();

  return attrs.mtime;
}

U64 FileNode::getSize() {
  Attributes attrs;

  bool success = getAttributes(&attrs);

  if (!success)
    return 0;

  return attrs.size;
}

U32 FileNode::getChecksum() {
  bool calculateCRC = (mLastChecksum == Time());

  if (!calculateCRC) {
    Time modTime = getModifiedTime();

    calculateCRC = (modTime > mLastChecksum);
  }

  if (calculateCRC)
    mChecksum = calculateChecksum();

  if (mChecksum)
    mLastChecksum = Time::getCurrentTime();

  return mChecksum;
}
//-----------------------------------------------------------------------------

class FileSystemRedirect : public FileSystem {
public:
  FileSystemRedirect(MountSystem *mfs, const Path &path);

  String getTypeStr() const { return "Redirect"; }

  FileNodeRef resolve(const Path &path);
  FileNodeRef create(const Path &path, FileNode::Mode);
  bool remove(const Path &path);
  bool rename(const Path &a, const Path &b);
  Path mapTo(const Path &path);
  Path mapFrom(const Path &path);

private:
  Path _merge(const Path &path);

  Path mPath;
  MountSystem *mMFS;
};

Path FileSystemRedirect::_merge(const Path &path) {
  Path p = mPath;
  p.setPath(Path::Join(p.getPath(), '/', Path::CompressPath(path.getPath())));
  p.setFileName(path.getFileName());
  p.setExtension(path.getExtension());
  return p;
}

FileSystemRedirect::FileSystemRedirect(MountSystem *mfs, const Path &path) {
  mMFS = mfs;
  mPath.setRoot(path.getRoot());
  mPath.setPath(path.getPath());
}

FileNodeRef FileSystemRedirect::resolve(const Path &path) {
  Path p = _merge(path);
  FileSystemRef fs = mMFS->getFileSystem(p);
  if (fs != NULL)
    return fs->resolve(p);
  return NULL;
}

FileNodeRef FileSystemRedirect::create(const Path &path, FileNode::Mode mode) {
  Path p = _merge(path);
  FileSystemRef fs = mMFS->getFileSystem(p);
  if (fs != NULL)
    return fs->create(p, mode);
  return NULL;
}

bool FileSystemRedirect::remove(const Path &path) {
  Path p = _merge(path);
  FileSystemRef fs = mMFS->getFileSystem(p);
  if (fs != NULL)
    return fs->remove(p);
  return false;
}

bool FileSystemRedirect::rename(const Path &a, const Path &b) {
  Path na = _merge(a);
  Path nb = _merge(b);
  FileSystemRef fsa = mMFS->getFileSystem(na);
  FileSystemRef fsb = mMFS->getFileSystem(nb);
  if (fsa.getPointer() == fsb.getPointer())
    return fsa->rename(na, nb);
  return false;
}

Path FileSystemRedirect::mapTo(const Path &path) {
  Path p = _merge(path);
  FileSystemRef fs = mMFS->getFileSystem(p);
  if (fs != NULL)
    return fs->mapTo(p);
  return NULL;
}

Path FileSystemRedirect::mapFrom(const Path &path) {
  Path p = _merge(path);
  FileSystemRef fs = mMFS->getFileSystem(p);
  if (fs != NULL)
    return fs->mapFrom(p);
  return NULL;
}

//-----------------------------------------------------------------------------
void MountSystem::_log(const String &msg) {
  dPrintf("MountSystem: %s", msg.c_str());
}

FileSystemRef MountSystem::_removeMountFromList(String root) {
  for (Vector<MountFS>::iterator itr = mMountList.begin();
       itr != mMountList.end(); itr++) {
    if (root.equal(itr->root, String::NoCase)) {
      FileSystemRef fs = itr->fileSystem;
      mMountList.erase(itr);
      return fs;
    }
  }
  return NULL;
}

FileSystemRef MountSystem::_getFileSystemFromList(const Path &path) const {
  for (Vector<MountFS>::const_iterator itr = mMountList.begin();
       itr != mMountList.end(); itr++) {
    if (itr->root.equal(path.getRoot(), String::NoCase))
      return itr->fileSystem;
  }

  return NULL;
}

Path MountSystem::_normalize(const Path &path) {
  Path po = path;

  // Assign to cwd root if none is specified.
  if (po.getRoot().isEmpty())
    po.setRoot(mCWD.getRoot());

  // Merge in current working directory if the path is relative to
  // the current cwd.
  if (po.getRoot().equal(mCWD.getRoot(), String::NoCase) && po.isRelative()) {
    po.setPath(
        Path::CompressPath(Path::Join(mCWD.getPath(), '/', po.getPath())));
  }
  return po;
}

FileRef MountSystem::createFile(const Path &path) {
  Path np = _normalize(path);
  FileSystemRef fs = _getFileSystemFromList(np);

  if (fs && fs->isReadOnly()) {
    _log(String::ToString("Cannot create file %s, filesystem is read-only",
                          path.getFullPath().c_str()));
    return NULL;
  }

  if (fs != NULL)
    return static_cast<File *>(fs->create(np, FileNode::File).getPointer());
  return NULL;
}

DirectoryRef MountSystem::createDirectory(const Path &path, FileSystemRef fs) {
  Path np = _normalize(path);
  if (fs.isNull())
    fs = _getFileSystemFromList(np);

  if (fs && fs->isReadOnly()) {
    _log(String::ToString("Cannot create directory %s, filesystem is read-only",
                          path.getFullPath().c_str()));
    return NULL;
  }

  if (fs != NULL)
    return static_cast<Directory *>(
        fs->create(np, FileNode::Directory).getPointer());
  return NULL;
}

FileRef MountSystem::openFile(const Path &path, File::AccessMode mode) {
  FileNodeRef node = getFileNode(path);
  if (node != NULL) {
    FileRef file = dynamic_cast<File *>(node.getPointer());
    if (file != NULL) {
      if (file->open(mode))
        return file;
      else
        return NULL;
    }
  } else {
    if (mode != File::Read) {
      FileRef file = createFile(path);

      if (file != NULL) {
        file->open(mode);
        return file;
      }
    }
  }
  return NULL;
}

DirectoryRef MountSystem::openDirectory(const Path &path) {
  FileNodeRef node = getFileNode(path);

  if (node != NULL) {
    DirectoryRef dir = dynamic_cast<Directory *>(node.getPointer());
    if (dir != NULL) {
      dir->open();
      return dir;
    }
  }
  return NULL;
}

bool MountSystem::remove(const Path &path) {
  Path np = _normalize(path);
  FileSystemRef fs = _getFileSystemFromList(np);
  if (fs && fs->isReadOnly()) {
    _log(String::ToString("Cannot remove path %s, filesystem is read-only",
                          path.getFullPath().c_str()));
    return false;
  }
  if (fs != NULL)
    return fs->remove(np);
  return false;
}

bool MountSystem::rename(const Path &from, const Path &to) {
  // Will only rename files on the same filesystem
  Path pa = _normalize(from);
  Path pb = _normalize(to);
  FileSystemRef fsa = _getFileSystemFromList(pa);
  FileSystemRef fsb = _getFileSystemFromList(pb);
  if (!fsa || !fsb)
    return false;
  if (fsa.getPointer() != fsb.getPointer()) {
    _log(String::ToString("Cannot rename path %s to a different filesystem",
                          from.getFullPath().c_str()));
    return false;
  }
  if (fsa->isReadOnly() || fsb->isReadOnly()) {
    _log(String::ToString(
        "Cannot rename path %s; source or target filesystem is read-only",
        from.getFullPath().c_str()));
    return false;
  }

  return fsa->rename(pa, pb);
}

bool MountSystem::mount(String root, FileSystemRef fs) {
  MountFS mount;
  mount.root = root;
  mount.path = "/";
  mount.fileSystem = fs;
  mMountList.push_back(mount);
  return true;
}

bool MountSystem::mount(String root, const Path &path) {
  return mount(root, new FileSystemRedirect(this, _normalize(path)));
}

FileSystemRef MountSystem::unmount(String root) {
  FileSystemRef first = _removeMountFromList(root);

  // remove remaining FSes on this root
  while (!_removeMountFromList(root).isNull())
    ;

  return first;
}

bool MountSystem::unmount(FileSystemRef fs) {
  if (fs.isNull())
    return false;

  // iterate back to front in case FS is in list multiple times.
  // also check that fs is not null each time since its a strong ref
  // so it could be nulled during removal.
  bool unmounted = false;
  for (int i = mMountList.size() - 1; !fs.isNull() && i >= 0; --i) {
    if (mMountList[i].fileSystem.getPointer() == fs.getPointer()) {
      mMountList.erase(i);
      unmounted = true;
    }
  }
  return unmounted;
}

bool MountSystem::setCwd(const Path &file) {
  if (file.getPath().isEmpty())
    return false;
  mCWD.setRoot(file.getRoot());
  mCWD.setPath(file.getPath());
  return true;
}

const Path &MountSystem::getCwd() const { return mCWD; }

FileSystemRef MountSystem::getFileSystem(const Path &path) {
  return _getFileSystemFromList(_normalize(path));
}

bool MountSystem::getFileAttributes(const Path &path,
                                    FileNode::Attributes *attr) {
  FileNodeRef file = getFileNode(path);

  if (file != NULL) {
    bool result = file->getAttributes(attr);
    return result;
  }

  return false;
}

FileNodeRef MountSystem::getFileNode(const Path &path) {
  Path np = _normalize(path);
  FileSystemRef fs = _getFileSystemFromList(np);
  if (fs != NULL)
    return fs->resolve(np);
  return NULL;
}

bool MountSystem::mapFSPath(const String &inRoot, const Path &inPath,
                            Path &outPath) {
  FileSystemRef fs = _getFileSystemFromList(inRoot);

  if (fs == NULL) {
    outPath = Path();
    return false;
  }

  outPath = fs->mapFrom(inPath);

  return outPath.getFullPath() != String();
}

S32 MountSystem::findByPattern(const Path &inBasePath,
                               const String &inFilePattern, bool inRecursive,
                               Vector<String> &outList, bool includeDirs) {
  if (mFindByPatternOverrideFS.isNull() && !inBasePath.isDirectory())
    return -1;

  DirectoryRef dir = NULL;
  if (mFindByPatternOverrideFS.isNull())
    // open directory using standard mount system search
    dir = openDirectory(inBasePath);
  else {
    // use specified filesystem to open directory
    FileNodeRef fNode = mFindByPatternOverrideFS->resolve(inBasePath);
    if (fNode && (dir = dynamic_cast<Directory *>(fNode.getPointer())) != NULL)
      dir->open();
  }

  if (dir == NULL)
    return -1;

  if (includeDirs) {
    // prepend cheesy "DIR:" annotation for directories
    outList.push_back(String("DIR:") + inBasePath.getPath());
  }

  FileNode::Attributes attrs;

  Vector<String> recurseDirs;

  while (dir->read(&attrs)) {
    // skip hidden files
    if (attrs.name.c_str()[0] == '.')
      continue;

    String name(attrs.name);

    if ((attrs.flags & FileNode::Directory) && inRecursive) {
      name += '/';
      String path = Path::Join(inBasePath, '/', name);
      recurseDirs.push_back(path);
    }

    if (FindMatch::isMatch(inFilePattern, attrs.name, false)) {
      String path = Path::Join(inBasePath, '/', name);
      outList.push_back(path);
    }
  }

  dir->close();

  for (S32 i = 0; i < recurseDirs.size(); i++)
    findByPattern(recurseDirs[i], inFilePattern, true, outList, includeDirs);

  return outList.size();
}

bool MountSystem::isFile(const Path &path) {
  FileNode::Attributes attr;
  if (getFileAttributes(path, &attr))
    return attr.flags & FileNode::File;
  return false;
}

bool MountSystem::isDirectory(const Path &path, FileSystemRef fsRef) {
  FileNode::Attributes attr;

  if (fsRef.isNull()) {
    if (getFileAttributes(path, &attr))
      return attr.flags & FileNode::Directory;
    return false;
  } else {
    FileNodeRef fnRef = fsRef->resolve(path);
    if (fnRef.isNull())
      return false;

    FileNode::Attributes attr;
    if (fnRef->getAttributes(&attr))
      return attr.flags & FileNode::Directory;
    return false;
  }
}

bool MountSystem::isReadOnly(const Path &path) {
  // first check to see if filesystem is read only
  FileSystemRef fs = getFileSystem(path);
  if (fs.isNull())
    // no filesystem owns this file...oh well, return false
    return false;
  if (fs->isReadOnly())
    return true;

  // check the file attributes, note that if the file does not exist,
  // this function returns false.  that should be ok since we know
  // the file system is writable at this point.
  FileNode::Attributes attr;
  if (getFileAttributes(path, &attr))
    return attr.flags & FileNode::ReadOnly;
  return false;
}

void MountSystem::startFileChangeNotifications() {
  for (S32 i = 0; i < mMountList.size(); i++) {
    FileSystemChangeNotifier *notifier =
        mMountList[i].fileSystem->getChangeNotifier();

    if (notifier != NULL && !notifier->amNotifying())
      notifier->startNotifier();
  }
}

void MountSystem::stopFileChangeNotifications() {
  for (S32 i = 0; i < mMountList.size(); i++) {
    FileSystemChangeNotifier *notifier =
        mMountList[i].fileSystem->getChangeNotifier();

    if (notifier != NULL && notifier->amNotifying())
      notifier->startNotifier();
  }
}

bool MountSystem::createPath(const Path &path) {
  if (path.getPath().isEmpty())
    return true;

  // See if the pathectory exists
  Path dir;
  dir.setRoot(path.getRoot());
  dir.setPath(path.getPath());

  // in a virtual mount system, isDirectory may return true if the directory
  // exists in a read only FS, but the directory may not exist on a writeable
  // filesystem that is also mounted. So get the target filesystem that will be
  // used for the full writable path and and make sure the directory exists on
  // it.
  FileSystemRef fsRef = getFileSystem(path);

  if (isDirectory(dir, fsRef))
    return true;

  // Start from the top and work our way down
  Path sub;
  dir.setPath(path.isAbsolute() ? String("/") : String());
  for (U32 i = 0; i < path.getDirectoryCount(); i++) {
    sub.setPath(path.getDirectory(i));
    dir.appendPath(sub);
    if (!isDirectory(dir, fsRef)) {
      if (!createDirectory(dir, fsRef))
        return false;
    }
  }
  return true;
}

//-----------------------------------------------------------------------------

// Default global mount system
static VirtualMountSystem sgMountSystem;

namespace FS {

FileRef CreateFile(const Path &path) { return sgMountSystem.createFile(path); }

DirectoryRef CreateDirectory(const Path &path) {
  return sgMountSystem.createDirectory(path);
}

FileRef OpenFile(const Path &path, File::AccessMode mode) {
  return sgMountSystem.openFile(path, mode);
}

bool ReadFile(const Path &inPath, void *&outData, U32 &outSize,
              bool inNullTerminate) {
  FileRef fileR = OpenFile(inPath, File::Read);

  outData = NULL;
  outSize = 0;

  if (fileR == NULL)
    return false;

  outSize = fileR->getSize();

  if (outSize == 0)
    return false;

  U32 sizeRead = 0;

  if (inNullTerminate) {
    outData = new char[outSize + 1];
    sizeRead = fileR->read(outData, outSize);
    static_cast<char *>(outData)[outSize] = '\0';
  } else {
    outData = new char[outSize];
    sizeRead = fileR->read(outData, outSize);
  }

  if (sizeRead != outSize) {
    delete static_cast<char *>(outData);
    outData = NULL;
    outSize = 0;
    return false;
  }

  return true;
}

DirectoryRef OpenDirectory(const Path &path) {
  return sgMountSystem.openDirectory(path);
}

bool Remove(const Path &path) { return sgMountSystem.remove(path); }

bool Rename(const Path &from, const Path &to) {
  return sgMountSystem.rename(from, to);
}

bool Mount(String root, FileSystemRef fs) {
  return sgMountSystem.mount(root, fs);
}

bool Mount(String root, const Path &path) {
  return sgMountSystem.mount(root, path);
}

FileSystemRef Unmount(String root) { return sgMountSystem.unmount(root); }

bool Unmount(FileSystemRef fs) { return sgMountSystem.unmount(fs); }

bool SetCwd(const Path &file) { return sgMountSystem.setCwd(file); }

const Path &GetCwd() { return sgMountSystem.getCwd(); }

FileSystemRef GetFileSystem(const Path &path) {
  return sgMountSystem.getFileSystem(path);
}

bool GetFileAttributes(const Path &path, FileNode::Attributes *attr) {
  return sgMountSystem.getFileAttributes(path, attr);
}

FileNodeRef GetFileNode(const Path &path) {
  return sgMountSystem.getFileNode(path);
}

bool MapFSPath(const String &inRoot, const Path &inPath, Path &outPath) {
  return sgMountSystem.mapFSPath(inRoot, inPath, outPath);
}

S32 FindByPattern(const Path &inBasePath, const String &inFilePattern,
                  bool inRecursive, Vector<String> &outList) {
  return sgMountSystem.findByPattern(inBasePath, inFilePattern, inRecursive,
                                     outList);
}

bool IsFile(const Path &path) { return sgMountSystem.isFile(path); }

bool IsDirectory(const Path &path) { return sgMountSystem.isDirectory(path); }

bool IsReadOnly(const Path &path) { return sgMountSystem.isReadOnly(path); }

void StartFileChangeNotifications() {
  sgMountSystem.startFileChangeNotifications();
}
void StopFileChangeNotifications() {
  sgMountSystem.stopFileChangeNotifications();
}

S32 GetNumMounts() { return sgMountSystem.getNumMounts(); }
String GetMountRoot(S32 index) { return sgMountSystem.getMountRoot(index); }
String GetMountPath(S32 index) { return sgMountSystem.getMountPath(index); }
String GetMountType(S32 index) { return sgMountSystem.getMountType(index); }

bool CreatePath(const Path &path) { return sgMountSystem.createPath(path); }

} // Namespace FS

} // Namespace ACLib
