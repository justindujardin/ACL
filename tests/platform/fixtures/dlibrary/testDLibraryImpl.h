//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_DLIBRARYIMPL_H_
#define ACL_PLATFORM_TEST_DLIBRARYIMPL_H_

#include "platform/impls/base/dlibrary/dlibraryImpl.h"
#include <gtest/gtest.h>

using namespace ACLib::Platform;
using namespace ACLib::Platform::Internal_;

namespace ACLib {
namespace TestPlatform {
class TestDLibraryImpl : public DLibraryImpl {
public:
  U32 initCt;
  U32 bindCt;
  U32 closeCt;
  bool initSuccess;
  void *voidRet;

  TestDLibraryImpl() {
    initCt = bindCt = closeCt = 0;
    initSuccess = true;
    voidRet = NULL;
  }

  ~TestDLibraryImpl() {
    //"expected DLibrary destructor to call close"
    EXPECT_TRUE(initCt == closeCt);
  }

  virtual bool init(const String &file) {
    initCt++;
    return initSuccess;
  }

  virtual void *bind(const String &name) {
    bindCt++;
    return voidRet;
  }

  virtual void close() { closeCt++; }
};
} // namespace TestPlatform
} // namespace ACLib
#endif
