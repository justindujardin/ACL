//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_TEST_DLIBRARYIMPL_H_
#define ACL_PLATFORM_TEST_DLIBRARYIMPL_H_

#include "platform/impls/base/dlibrary/dlibraryImpl.h"
#include "unit/test.h"

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
      
      UnitTesting::UnitTest* currentTest;
      
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
            currentTest->test(initCt == closeCt, "expected DLibrary destructor to call close");
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