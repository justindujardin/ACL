//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/types/types.h"
#include "core/util/delegate.h"
#include "core/util/tSignal.h"

#include <iostream>
#include <gtest/gtest.h>


//-----------------------------------------------------------------------------
// Delegate Example
//-----------------------------------------------------------------------------

// Take an S32 and return S32
typedef Delegate<S32 (S32)> SomeIntDelegate;

// Simple Function
S32 invertDelegate(S32 input) { return (input * -1); }

// Method on class
class MethodDelegateUsage
{
public:
   MethodDelegateUsage() {};
   S32 invertMethod(S32 input) { return invertDelegate(input); }
};


TEST(DelegateTest,SimpleFunction) { 
   SomeIntDelegate theDelegate;
   theDelegate.bind(&invertDelegate);
   EXPECT_EQ(theDelegate(1337),-1337);
}

TEST(DelegateTest,MethodDelegate) {
   SomeIntDelegate theDelegate;
   MethodDelegateUsage mduObject;
   theDelegate.bind(&mduObject, &MethodDelegateUsage::invertMethod);
   EXPECT_EQ(theDelegate(1337),-1337);
}


//-----------------------------------------------------------------------------
// Signal Example
//-----------------------------------------------------------------------------

// Double the input 
void doubleNumber(S32 *input) { *input *= 2; }
// Halve the input
void halfNumber(S32 *input) { *input /= 2; }
// Signal of S32 *
typedef Signal<void(S32*)> SomeIntSignal;

TEST(SignalTest,Callback) {
   SomeIntSignal theSignal;
   theSignal.notify(&doubleNumber);
   S32 value = 2048;
   // Double the number
   theSignal.trigger(&value);
   EXPECT_EQ(value,4096);
   // Double, then half, remain the same
   theSignal.notify(&halfNumber);
   theSignal.trigger(&value);
   EXPECT_EQ(value,4096);
}