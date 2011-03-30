//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "../core/torque.h"
#include "./dynamicLibrary.h"

#ifdef TORQUE_OS_WIN32
  #include <Windows.h>
#endif

DynamicLibrary::DynamicLibrary(void * handle) : mHandle(handle) {}
DynamicLibrary::~DynamicLibrary()
{
  if (mHandle)
  {
  #ifdef TORQUE_OS_WIN32
    ::FreeLibrary((HMODULE)mHandle);
  #endif
  }
}

DynamicLibrary * DynamicLibrary::load(const String & name)
{
  if (name.isEmpty()) 
    return NULL;
  
  void * handle = NULL;

  #ifdef TORQUE_OS_WIN32
    handle = ::LoadLibraryW((LPCWSTR)name.utf16());
  #endif

   if (handle == NULL)
      return NULL;

  return new DynamicLibrary(handle);
}

void * DynamicLibrary::getSymbol(const String & symbol)
{
  if (!mHandle)
    return NULL;
  
  #ifdef TORQUE_OS_WIN32
    return ::GetProcAddress((HMODULE)mHandle, symbol.c_str());
  #endif
}

