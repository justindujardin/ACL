//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "unit/testAssertImpl.h"
#include "core/containers/tIntrusiveList.h"

using namespace UnitTesting;
using namespace ACLib;

class Thing : public IntrusiveListNode<Thing>
{
public:
   Thing(U32 id) : mId(id)
   {
   }
   
   U32 mId;
   
   bool operator==(const Thing& rhs) const
   {
      return mId == rhs.mId;
   }
   
   bool operator!=(const Thing& rhs) const
   {
      return !operator==(rhs);
   }
};

CreateUnitTest(TestIntrusiveList, "Core/Util/IntrusiveList")
{
   Thing t0;
   Thing t1;
   public:
   TestIntrusiveList() : t0(0), t1(1)
   {
   }
   
   void testConstIteration(const IntrusiveList<Thing>& thingList)
   {
      // Forward const iteration
      IntrusiveList<Thing>::ConstIterator ci = thingList.begin();
      test(*ci == t1, "Expected that first item in the list would be equal to the item pushed to the front");
      ++ci;
      test(*ci == t0, "Expected that second item in the list would be equal to the item pushed to the back");
      ++ci;
      test(ci == thingList.end(), "Expected that the list would end after the only two items");
      ++ci;
      test(*ci == t1, "Expected that list would be circular");
      
      // Reverse const iteration
      IntrusiveList<Thing>::ConstIterator cri = thingList.rbegin();
      test(*cri == t0, "Expected that first item in reversed list would be equal to the item pushed to the back");
      --cri;
      test(*cri == t1, "Expected that the second item in reversed list would be equal to the item pushed to the front");
      --cri;
      test(cri == thingList.rend(), "Expected that the reversed list would end after the only two items");
      --cri;
      test(*cri == t0, "Expected that reversed list would be circular");
   }
   
   void testIteration(IntrusiveList<Thing>& thingList)
   {
      // Forward iteration
      IntrusiveList<Thing>::Iterator i = thingList.begin();
      test(*i == t1, "Expected that first item in the list would be equal to the item pushed to the front");
      ++i;
      test(*i == t0, "Expected that second item in the list would be equal to the item pushed to the back");
      ++i;
      test(i == thingList.end(), "Expected that the list would end after the only two items");
      ++i;
      test(*i == t1, "Expected that list would be circular");
      
      // Reverse iteration
      IntrusiveList<Thing>::Iterator ri = thingList.rbegin();
      test(*ri == t0, "Expected that first item in reversed list would be equal to the item pushed to the back");
      --ri;
      test(*ri == t1, "Expected that the second item in reversed list would be equal to the item pushed to the front");
      --ri;
      test(ri == thingList.rend(), "Expected that the reversed list would end after the only two items");
      --ri;
      test(*ri == t0, "Expected that reversed list would be circular");
   }
   
   void run()
   {
      // Specify that a newly created list is empty with size 0
      IntrusiveList<Thing> thingList;
      test(thingList.empty(), "Expected that a newly created list would be empty");
      test(thingList.size() == 0, "Expected that an empty list would have size 0");
      
      thingList.pushBack(t0);
      test(!thingList.empty(), "Expected that a list with >= 1 thing in it would not be empty");
      test(thingList.size() == 1, "Expected that a list with one thing in it would have size 1");
      
      thingList.pushFront(t1);
      test(thingList.size() == 2, "Expected that a list with two things in it would have size 2");
      
      testIteration(thingList);
      testConstIteration(thingList);
      
      thingList.popFront();
      test(t1.next == &t1, "Expected that thing at front of list would be removed after popFront");
      test(t1.prev == &t1, "Expected that thing at front of list would be removed after popFront");
      test(thingList.size() == 1, "Expected that list size would be 1 after an object was removed");
      
      thingList.popBack();
      test(t0.next == &t0, "Expected that thing at back of list would be removed after popBack");
      test(t0.prev == &t0, "Expected that thing at back of list would be removed after popBack");
      test(thingList.size() == 0, "Expected that list size would be 0 after both objects are removed");
      test(thingList.empty(), "Expected that list would be empty after both objects are removed");
   }
};
