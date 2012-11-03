//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/refBase.h"

#include <gtest/gtest.h>

// Test WeakRefPtr and StrongRefPtr (WeakRefUnion and StrongRefUnion still to come)

// Simple Class for testing WeakRefBase
class TestWeakRefBase : public WeakRefBase
{
public:

};

// Simple Class for testing StrongRefBase
class TestStrongRefBase : public StrongRefBase
{
public:
   ~TestStrongRefBase() { smDestructorCalled++; }

   static S32 smDestructorCalled;
};

S32 TestStrongRefBase::smDestructorCalled = 0;

TEST(WeakRefTest,AssignAndDestroy)
{
   TestWeakRefBase * weakRef = new TestWeakRefBase;

   // Test that weak ptr attains proper value and is cleared when deleting
   WeakRefPtr<TestWeakRefBase> weakPtr = weakRef;
   //"weak pointer not set or null even though assigned"
   EXPECT_TRUE(weakPtr==weakRef);
   delete weakRef;
   //"weak pointer not null even though target was deleted"
   EXPECT_TRUE(!weakPtr);
}

TEST(WeakRefTest,TwoRefsOnePtr)
{
   // Test pointing weak pointer at two different weak ref
   TestWeakRefBase * weakRef = new TestWeakRefBase;
   TestWeakRefBase * weakRef2 = new TestWeakRefBase;
   WeakRefPtr<TestWeakRefBase> weakPtr = weakRef;
   //"weak pointer not set or null even though assigned"
   EXPECT_TRUE(weakPtr==weakRef);
   weakPtr = weakRef2;
   //"weak pointer not set correctly"
   EXPECT_TRUE(weakPtr==weakRef2);
   delete weakRef;
   //"weak pointer corrupted by deleting previously assigned weak ref"
   EXPECT_TRUE(weakPtr==weakRef2);
   delete weakRef2;
   //"weak pointer not null even though target was deleted"
   EXPECT_TRUE(!weakPtr);
}

TEST(WeakRefTest, TwoPtrsOneRef)
{
   // Test two weak pointers at the same weak ref
   TestWeakRefBase * weakRef = new TestWeakRefBase;
   weakRef = new TestWeakRefBase;
   WeakRefPtr<TestWeakRefBase> weakPtr = weakRef;
   //"weak pointer not set or null even though assigned"
   EXPECT_TRUE(weakPtr==weakRef);
   WeakRefPtr<TestWeakRefBase> weakPtr2 = weakRef;
   //"weak pointer not set or null even though assigned"
   EXPECT_TRUE(weakPtr2==weakRef);
   delete weakRef;
   //"weak pointer not null even though target was deleted"
   EXPECT_TRUE(!weakPtr);
   //"weak pointer not null even though target was deleted"
   EXPECT_TRUE(!weakPtr2);
}

TEST(StrongRefTest,AssignAndDestroy)
{
   // Other possible tests: make sure WeakRefBase::WeakReference is cleared when it should be

   // Test that strong ptr assigned properly and destroys target when cleared
   TestStrongRefBase * strongRef = new TestStrongRefBase;
   StrongRefPtr<TestStrongRefBase> strongPtr;
   strongPtr = strongRef;
   //"strong pointer not set properly"
   EXPECT_TRUE(strongPtr == strongRef);
   S32 track = TestStrongRefBase::smDestructorCalled;
   strongPtr = NULL;
   //"strong pointer not cleared properly"
   EXPECT_TRUE(strongPtr == NULL);
   //"strong ref not destroyed even though strong ref ptr cleared"
   EXPECT_TRUE(track + 1 == TestStrongRefBase::smDestructorCalled);

}

TEST(StrongRefTest,OnePtrTwoRefs)
{
   // Test that strong ptr deals with two strong refs ok
   TestStrongRefBase * strongRef = new TestStrongRefBase;
   StrongRefPtr<TestStrongRefBase> strongPtr;
   TestStrongRefBase * strongRef2 = new TestStrongRefBase;
   strongPtr = strongRef;
   //"strong pointer not set properly"
   EXPECT_TRUE(strongPtr == strongRef);
   S32 track = TestStrongRefBase::smDestructorCalled;
   strongPtr = strongRef2;
   // result should be we now point at strongRef2 and strongRef was deleted (note: only check that a deletion occurred)
   //"strong pointer not set properly"
   EXPECT_TRUE(strongPtr == strongRef2);
   //"strong ref not destroyed even though strong ref ptr cleared"
   EXPECT_TRUE(track + 1 == TestStrongRefBase::smDestructorCalled);
   track = TestStrongRefBase::smDestructorCalled;
   strongPtr = NULL;
   //"strong pointer not cleared properly"
   EXPECT_TRUE(strongPtr == NULL);
   //"strong ref not destroyed even though strong ref ptr cleared"
   EXPECT_TRUE(track + 1 == TestStrongRefBase::smDestructorCalled);
}

TEST(StrongRefTest,TwoPtrsOneRef)
{
   // Test that two strong ptr's properly handle a strong ref
   TestStrongRefBase * strongRef = new TestStrongRefBase;
   StrongRefPtr<TestStrongRefBase> strongPtr = strongRef;
   StrongRefPtr<TestStrongRefBase> strongPtr2;
   S32 track = TestStrongRefBase::smDestructorCalled;
   strongPtr2 = strongPtr;
   //"strong pointer not set properly"
   EXPECT_TRUE(strongPtr2 == strongRef);
   //"strong ref destroyed at wrong time"
   EXPECT_TRUE(track == TestStrongRefBase::smDestructorCalled);
   track = TestStrongRefBase::smDestructorCalled;
   TestStrongRefBase * strongRef2 = new TestStrongRefBase;
   strongPtr2 = strongRef2;
   //"strong ref destroyed at wrong time"
   EXPECT_TRUE(track == TestStrongRefBase::smDestructorCalled);
   // ptr=ref, ptr2=ref2...make sure we can assign to our selves...
   track = TestStrongRefBase::smDestructorCalled;
   strongPtr2 = strongRef2;
   //"strong ref destroyed at wrong time"
   EXPECT_TRUE(track == TestStrongRefBase::smDestructorCalled);
   strongPtr2 = strongPtr2;
   //"strong ref destroyed at wrong time"
   EXPECT_TRUE(track == TestStrongRefBase::smDestructorCalled);
   // make sure we properly delete
   track = TestStrongRefBase::smDestructorCalled;
   strongPtr = strongPtr2;
   //"strong ref not destroyed"
   EXPECT_TRUE(track + 1 == TestStrongRefBase::smDestructorCalled);
   track = TestStrongRefBase::smDestructorCalled;
   strongPtr = NULL;
   //"strong ref destroyed at wrong time"
   EXPECT_TRUE(track == TestStrongRefBase::smDestructorCalled);
   track = TestStrongRefBase::smDestructorCalled;
   strongPtr2 = NULL;
   //"strong ref not destroyed"
   EXPECT_TRUE(track + 1 == TestStrongRefBase::smDestructorCalled);
}
