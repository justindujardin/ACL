//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_DLIBRARYIMPL_H_
#define ACL_PLATFORM_TEST_DLIBRARYIMPL_H_

#include "platform/impls/base/dlibrary/dlibraryImpl.h"
#include <gtest/gtest.h>

namespace Platform2
{
   namespace Internal_
   {
      class TestDLibraryImpl : public DLibraryImpl
      {
      public:
         U32 initCt;
         U32 bindCt;
         U32 closeCt;
         bool initSuccess;
         void* voidRet;

        testing::Test* currentTest;

         TestDLibraryImpl()
         {
            initCt = bindCt = closeCt = 0;
            initSuccess = true;
            voidRet = NULL;
            currentTest = NULL;
         }

         ~TestDLibraryImpl()
         {
            if(currentTest)
              //"expected DLibrary destructor to call close"
               EXPECT_TRUE(initCt == closeCt);
         }

         virtual bool init(const String& file)
         {
            initCt++;
            return initSuccess;
         }

         virtual void* bind(const String& name)
         {  
            bindCt++;
            return voidRet;
         }

         virtual void close()
         {
            closeCt++;
         }
      };
   }
}

#endif
