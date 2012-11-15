//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------


#include "acl.h"

#include <iostream>
#include <gtest/gtest.h>

using namespace ACLib;

// Determine if a block of memory is aligned
#define IS_ALIGNED(ptr) EXPECT_TRUE(((((unsigned long)ptr) & 15) == 0))

// Test to be sure the memory allocations for this platform are aligned.
TEST(MemoryAllocation,Aligned) {

   const S32 loops = 100;
   void *allocArray[loops];
   for(S32 i = 0; i < loops; i++)
   {
      allocArray[i] = dMalloc(sizeof(U8));
      IS_ALIGNED(allocArray[i]);
   }
   for(S32 i = 0; i < loops; i++)
      dFree(allocArray[i]);
}

// 
TEST(MemoryReallocation,Aligned) {

   const S32 loops = 100;
   void *allocArray[loops];
   for(S32 i = 0; i < loops; i++)
   {
      allocArray[i] = dMalloc(sizeof(U8));
      IS_ALIGNED(allocArray[i]);
   }
   for(S32 i = 0; i < loops; i++)
   {
      allocArray[i] = dRealloc(allocArray[i],sizeof(S64));
      IS_ALIGNED(allocArray[i]);
   }
   for(S32 i = 0; i < loops; i++)
      dFree(allocArray[i]);
}
