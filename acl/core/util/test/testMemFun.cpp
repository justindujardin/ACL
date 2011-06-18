//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "unit/testAssertImpl.h"
#include "core/util/tMemFunc.h"
#include "core/util/scopedPtr.h"

using namespace UnitTesting;
using namespace Torque;

namespace 
{
   class SomeClass
   {
   public:
      SomeClass() : fnCall(0) {}
   
      void someFunction()
      {
         fnCall++;
      }
      
      U32 otherFunction()
      {
         fnCall++;
         return fnCall;
      }
      
      void ohSoConst() const
      {
      }
      
      U32 fnCall;
   };
   
   CreateUnitTest(TestMemFun, "Core/Util/TestMemFun")
   {
      void run()
      {
         MemFuncT<void, SomeClass> memfun(&SomeClass::someFunction);
         ScopedPtr<SomeClass> p(new SomeClass);
         memfun(p.get());
         test(p->fnCall == 1, "Should've called someFunction");
         MemFuncT<U32, SomeClass> othermemfun(&SomeClass::otherFunction);
         test(othermemfun(p.get()) == 2, "Should've called otherFunction");
         test(p->fnCall == 2, "Should've called otherFunctoin");
         
         ConstMemFuncT<void, SomeClass> constmemfun(&SomeClass::ohSoConst);
      }
   };
}
