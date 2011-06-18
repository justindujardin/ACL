//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "core/util/refBase.h"
#include "unit/test.h"

using namespace UnitTesting;

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

CreateUnitTest(TestRefBase, "Util/RefBase")
{
   void run()
   {
      TestWeakRefBase * weakRef = new TestWeakRefBase;

      // Test that weak ptr attaints proper value and is cleared when deleting
      WeakRefPtr<TestWeakRefBase> weakPtr = weakRef;
      test(weakPtr==weakRef,"weak pointer not set or null even though assigned");
      delete weakRef;
      test(!weakPtr,"weak pointer not null even though target was deleted");

      // Test pointing weak pointer at two different weak ref
      weakRef = new TestWeakRefBase;
      TestWeakRefBase * weakRef2 = new TestWeakRefBase;
      weakPtr = weakRef;
      test(weakPtr==weakRef,"weak pointer not set or null even though assigned");
      weakPtr = weakRef2;
      test(weakPtr==weakRef2,"weak pointer not set correctly");
      delete weakRef;
      test(weakPtr==weakRef2,"weak pointer corrupted by deleting previously assigned weak ref");
      delete weakRef2;
      test(!weakPtr,"weak pointer not null even though target was deleted");

      // Test two weak pointers at the same weak ref
      weakRef = new TestWeakRefBase;
      weakPtr = weakRef;
      test(weakPtr==weakRef,"weak pointer not set or null even though assigned");
      WeakRefPtr<TestWeakRefBase> weakPtr2 = weakRef;
      test(weakPtr2==weakRef,"weak pointer not set or null even though assigned");
      delete weakRef;
      test(!weakPtr,"weak pointer not null even though target was deleted");
      test(!weakPtr2,"weak pointer not null even though target was deleted");

      // Other possible tests: make sure WeakRefBase::WeakReference is cleared when it should be

      // Test that strong ptr assigned properly and destroys target when cleared
      TestStrongRefBase * strongRef = new TestStrongRefBase;
      StrongRefPtr<TestStrongRefBase> strongPtr;
      strongPtr = strongRef;
      test(strongPtr == strongRef,"strong pointer not set properly");
      S32 track = TestStrongRefBase::smDestructorCalled;
      strongPtr = NULL;
      test(strongPtr == NULL,"strong pointer not cleared properly");
      test(track + 1 == TestStrongRefBase::smDestructorCalled,"strong ref not destroyed even though strong ref ptr cleared");

      // Test that strong ptr deals with two strong refs ok
      strongRef = new TestStrongRefBase;
      TestStrongRefBase * strongRef2 = new TestStrongRefBase;
      strongPtr = strongRef;
      test(strongPtr == strongRef,"strong pointer not set properly");
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr = strongRef2;
      // result should be we now point at strongRef2 and strongRef was deleted (note: only check that a deletion occurred)
      test(strongPtr == strongRef2,"strong pointer not set properly");
      test(track + 1 == TestStrongRefBase::smDestructorCalled,"strong ref not destroyed even though strong ref ptr cleared");
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr = NULL;
      test(strongPtr == NULL,"strong pointer not cleared properly");
      test(track + 1 == TestStrongRefBase::smDestructorCalled,"strong ref not destroyed even though strong ref ptr cleared");

      // Test that two strong ptr's properly handle a strong ref
      strongRef = new TestStrongRefBase;
      strongPtr = strongRef;
      StrongRefPtr<TestStrongRefBase> strongPtr2;
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr2 = strongPtr;
      test(strongPtr2 == strongRef,"strong pointer not set properly");
      test(track == TestStrongRefBase::smDestructorCalled,"strong ref destroyed at wrong time");
      track = TestStrongRefBase::smDestructorCalled;
      strongRef2 = new TestStrongRefBase;
      strongPtr2 = strongRef2;
      test(track == TestStrongRefBase::smDestructorCalled,"strong ref destroyed at wrong time");
      // ptr=ref, ptr2=ref2...make sure we can assign to our selves...
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr2 = strongRef2;
      test(track == TestStrongRefBase::smDestructorCalled,"strong ref destroyed at wrong time");
      strongPtr2 = strongPtr2;
      test(track == TestStrongRefBase::smDestructorCalled,"strong ref destroyed at wrong time");
      // make sure we properly delete
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr = strongPtr2;
      test(track + 1 == TestStrongRefBase::smDestructorCalled,"strong ref not destroyed");
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr = NULL;
      test(track == TestStrongRefBase::smDestructorCalled,"strong ref destroyed at wrong time");
      track = TestStrongRefBase::smDestructorCalled;
      strongPtr2 = NULL;
      test(track + 1 == TestStrongRefBase::smDestructorCalled,"strong ref not destroyed");
   }
};