//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#ifndef _PLATFORM_PATHS_DIRECTORY_H_
#define _PLATFORM_PATHS_DIRECTORY_H_

#include "../core/types/types.h"
#include "../core/util/str.h"
#include "../core/util/delegate.h"

//----------------------------------------------------------------------
namespace Torque
{
   class Path;
}

 
typedef Delegate<void (const String&)> FileNameDelegate;

namespace Directory
{
  // check if a directory exists
  bool exists(const String & path);

  // get current working directory
  String getCWD();

  // get path of current executable
  String getExecutablePath();

  // set current working directories
  void setCWD(const String & path);

  // Create directory 'path' including all parent directories if missing
  void create(const String & path);

  // callback enumerate files in a path
  void enumFiles(const String & path, FileNameDelegate fileCallback);

};



#endif // _PLATFORM_PATHS_DIRECTORY_H_


