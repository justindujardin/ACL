//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#include "unit/test.h"
#include "core/util/typeRebind.h"

using namespace UnitTesting;

class TypeA
{
public:
   virtual ~TypeA() {}
   virtual void thing() = 0;
};

class TypeB : public TypeA
{
public:
   TypeB()
   {
      //dostuff
   }
   
   virtual void thing() {}
};

class TypeC : public TypeA
{
public:
   TypeC() {}
   virtual void thing() {}
};

CreateUnitTest(TestTypeRebind, "Core/Util/TypeRebind")
{
   U32 createThingCt;
   
   TypeA* createThing()
   {
      createThingCt++;
      return new TypeB;
   }
   
   void run()
   {
      createThingCt = 0;
      ACLib::TypeRebind r;
      r.bind<TypeA>().to<TypeB>();
      ACLib::ScopedPtr<TypeA> a(r.create<TypeA>());
      test(dynamic_cast<TypeB*>(a.get()) != NULL, "Expected that a TypeB would be returned");
      
      r.bind<TypeA>().to<TypeA, ACLib::DelegateCreateBehavior>().assignDelegate(Delegate<TypeA*()>(this, &TestTypeRebind::createThing));
      a.reset(r.create<TypeA>());
      test(dynamic_cast<TypeB*>(a.get()) != NULL, "Expected that a TypeB would be returned");
      test(createThingCt == 1, "Expected that createThing would be called");
   }
};

CreateUnitTest(TestTypeRebindCopy, "Core/Util/TypeRebind/Copy")
{
   void run()
   {
      ACLib::TypeRebind r0;
      r0.bind<TypeA>().to<TypeB>();
      ACLib::TypeRebind r1(r0);
      r0.bind<TypeA>().to<TypeC>();
      
      ACLib::ScopedPtr<TypeA> a(r0.create<TypeA>());
      test(dynamic_cast<TypeC*>(a.get()) != NULL, "Expected that r0 would return a TypeC");
      a.reset(r1.create<TypeA>());
      test(dynamic_cast<TypeB*>(a.get()) != NULL, "Expected that r1 would return a TypeB");
   }
};

CreateUnitTest(TypeTypeRebindAssignment, "Core/Util/TypeRebind/Assignment")
{
   void run()
   {
      ACLib::TypeRebind r0;
      r0.bind<TypeA>().to<TypeB>();
      ACLib::TypeRebind r1;
      r1.bind<TypeA>().to<TypeC>();
      
      r0 = r1;
      
      ACLib::ScopedPtr<TypeA> a(r0.create<TypeA>());
      test(dynamic_cast<TypeC*>(a.get()) != NULL, "Expected that r0 would return a TypeC");
   }
};

CreateUnitTest(TestSingletonBehaviorCreate, "Core/Util/TypeRebind/SingletonBehavior/Create")
{
   void run()
   {
      ACLib::SingletonBehavior<TypeB> b;
      TypeB* ptr0 = b.create();
      TypeB* ptr1 = b.create();
      test(ptr0 == ptr1, "Expected that with the SingletonBehavior the same pointer would be returned for multiple calls to create");
   }
};

CreateUnitTest(TestSingletonBehaviorClone, "Core/Util/TypeRebind/SingletonBehavior/Clone")
{
   void run()
   {
      ACLib::ScopedPtr<ACLib::SingletonBehavior<TypeB> > b0(new ACLib::SingletonBehavior<TypeB>);
      TypeB* ptr0 = b0->create();
      ACLib::ScopedPtr<ACLib::Behavior<TypeB> > b1(b0->clone());
      test(dynamic_cast<ACLib::SingletonBehavior<TypeB>*>(b1.get()) != NULL, "Expected that SingletonBehavior::clone would return SingletonBehavior");
      TypeB* ptr1 = b1->create();
      test(ptr0 != ptr1, "Expected that cloned singleton behavior would create a new instance of TypeB");
   }
};
