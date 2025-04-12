//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "platform/impls/win32/volume/win32FileSystemImpl.h"
#include <Windows.h>

using namespace ACLib::FS;

namespace ACLib {
namespace Platform {
namespace Internal_ {
namespace Win32 {
// If the file is a Directory, Offline, System or Temporary then FALSE
#define S_ISREG(Flags)                                                         \
  !((Flags) & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_OFFLINE |             \
               FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY))

#define S_ISDIR(Flags) ((Flags)&FILE_ATTRIBUTE_DIRECTORY)

void _CopyStatAttributes(const WIN32_FIND_DATAW &info,
                         FileNode::Attributes *attr) {
  // Fill in the return struct.
  attr->flags = 0;
  if (S_ISDIR(info.dwFileAttributes))
    attr->flags |= FileNode::Directory;
  if (S_ISREG(info.dwFileAttributes))
    attr->flags |= FileNode::File;

  if (!(info.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
    attr->flags |= FileNode::ReadOnly;

  attr->size = info.nFileSizeLow;
  attr->mtime = Win32FileTimeToTime(info.ftLastWriteTime.dwLowDateTime,
                                    info.ftLastWriteTime.dwHighDateTime);

  attr->atime = Win32FileTimeToTime(info.ftLastAccessTime.dwLowDateTime,
                                    info.ftLastAccessTime.dwHighDateTime);
}
} // namespace Win32
using namespace Win32;
bool Win32FileSystemImpl::getAttributesForPath(FileNode::Attributes &attributes,
                                               const Path &path) {
  WIN32_FIND_DATAW info;
  HANDLE handle = ::FindFirstFileW((LPCWSTR)path.getFullPath().utf16(), &info);
  FindClose(handle);
  if (handle == INVALID_HANDLE_VALUE)
    return false;

  _CopyStatAttributes(info, &attributes);
  return true;
}

bool Win32FileSystemImpl::createDirectory(const Path &path) {
  return ::CreateDirectoryW((LPCWSTR)path.getFullPath().utf16(), 0);
}

bool Win32FileSystemImpl::removeDirectory(const Path &path) {
  return ::RemoveDirectoryW((LPCWSTR)path.getFullPath().utf16());
}

bool Win32FileSystemImpl::removeFile(const Path &path) {
  return ::DeleteFileW((LPCWSTR)path.getFullPath().utf16());
}

bool Win32FileSystemImpl::move(const Path &from, const Path &to) {
  return MoveFile((LPCWSTR)from.getFullPath().utf16(),
                  (LPCWSTR)to.getFullPath().utf16());
}
} // namespace Internal_
} // namespace Platform
} // namespace ACLib