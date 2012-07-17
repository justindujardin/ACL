//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/types.h"
#include "core/containers/tVector.h"
#include "core/containers/tList.h"
#include "core/containers/tPair.h"
#include "core/strings/stringFunctions.h"

#include <iostream>
#include <gtest/gtest.h>


// Vector insertion and expect values
TEST(VectorTest,Insertion) {
   // Vector
   Vector<S32> intVec;
   intVec.push_back(1337);
   intVec.increment();
   intVec.last() = 12;
   intVec.push_front(141);
   EXPECT_TRUE(intVec[0]==141);
   EXPECT_TRUE(intVec.last()==12);
   EXPECT_TRUE(intVec[1]==1337);
}

TEST(ListTest,Insertion) {
   // List
   ACLib::List<const char *> nameList;
   static const char *names[] = { "bill", "patricia", "rodney", "colbert", "robert", NULL };
   U32 i = 0;
   const char *nameIter = names[i];
   while(nameIter != NULL)
   {
      nameList.pushBack(nameIter);
      nameIter = names[++i];
   }
   for(i = 0; i < nameList.getSize(); i++)
   {
      EXPECT_STRCASEEQ(nameList[i],names[i]);
   }
}

TEST(PairTest,Creation) {
   // Pair
   Pair<S32,F32> intFloat = MakePair<S32,F32>(1337,1.337f);
   EXPECT_EQ(intFloat.first,1337);
   EXPECT_FLOAT_EQ(intFloat.second,1.337f);
}