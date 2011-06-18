//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef TORQUE_PLATFORM2_TEST_DLIBRARYIMPL_H_
#define TORQUE_PLATFORM2_TEST_DLIBRARYIMPL_H_

#include "platform2/impls/base/dlibrary/dlibraryImpl.h"

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