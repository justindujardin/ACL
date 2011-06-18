//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "core/util/delegate.h"
#include "core/util/tScopedFunction.h"
#include "unit/test.h"

using namespace UnitTesting;
using namespace Torque;

static U32 someFnCt = 0;
static U32 someOtherFnCt = 0;

static void someFn()
{
   someFnCt++;
}

static void someOtherFn()
{
   someOtherFnCt++;
}

CreateUnitTest(TestScopedFunction, "Util/ScopedFunction")
{
   U32 mFnCt;
   U32 mOtherFnCt;
   
   void mFn()
   {
      mFnCt++;
   }
   
   void mOtherFn()
   {
      mOtherFnCt++;
   }
   
   void run()
   {
      someFnCt = 0;
      someOtherFnCt = 0;
      mFnCt = 0;
      mOtherFnCt = 0;
      
      {
         ScopedFunction<Delegate<void()> > fn(Delegate<void()>(this, &TestScopedFunction::mFn));
         test(mFnCt == 0, "Expected that mFn would not be called yet");
      }
      test(mFnCt == 1, "Expected that mFn would be called when ScopedFunction left scope");
      
      {
         ScopedFunction<Delegate<void()> > fn(Delegate<void()>(this, &TestScopedFunction::mFn));
         ScopedFunction<Delegate<void()> > fn2(fn);
         ScopedFunction<Delegate<void()> > fn3(Delegate<void()>(this, &TestScopedFunction::mOtherFn));
         fn3 = fn;
      }
      
      test(mFnCt == 4, "Expected that mFn would be called thrice due to Copy Constructed and Assignment");
      test(mOtherFnCt == 0, "Expected that mOtherFn would not be called");
      
      {
         ScopedFunction<void(*)()> fn(&someFn);
         test(someFnCt == 0, "Expected that someFn would not be called yet");
      }
      
      test(someFnCt == 1, "Expected that someFn would be called when ScopedFunction left scope");
      
      {
         ScopedFunction<void(*)()> fn(&someFn);
         ScopedFunction<void(*)()> fn2(fn);
         ScopedFunction<void(*)()> fn3(&someOtherFn);
         fn3 = fn;
      }
      
      test(someFnCt == 4, "Expected that someFn would be called thrice due to Copy Constructed and Assignment");
      test(someOtherFnCt == 0, "Expected that someOtherFn would not be called");
   }
};
