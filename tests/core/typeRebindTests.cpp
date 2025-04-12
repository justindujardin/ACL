//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#include "core/util/typeRebind.h"
#include <gtest/gtest.h>

using namespace ACLib;

class TypeA {
public:
  virtual ~TypeA() {}
  virtual void thing() = 0;
};

class TypeB : public TypeA {
public:
  TypeB() {
    // dostuff
  }

  virtual void thing() {}
};

class TypeC : public TypeA {
public:
  TypeC() {}
  virtual void thing() {}
};

U32 createThingCt;
TypeA *createThing() {
  createThingCt++;
  return new TypeB;
}

TEST(TypeRebind, Creation) {
  createThingCt = 0;
  TypeRebind r;
  r.bind<TypeA>().to<TypeB>();
  ScopedPtr<TypeA> a(r.create<TypeA>());
  //"Expected that a TypeB would be returned"
  EXPECT_TRUE(dynamic_cast<TypeB *>(a.get()) != NULL);

  r.bind<TypeA>().to<TypeA, DelegateCreateBehavior>().assignDelegate(
      Delegate<TypeA *()>(&createThing));
  a.reset(r.create<TypeA>());
  //"Expected that a TypeB would be returned"
  EXPECT_TRUE(dynamic_cast<TypeB *>(a.get()) != NULL);
  //"Expected that createThing would be called"
  EXPECT_EQ(createThingCt, 1);
}

TEST(TypeRebind, Copy) {
  TypeRebind r0;
  r0.bind<TypeA>().to<TypeB>();
  TypeRebind r1(r0);
  r0.bind<TypeA>().to<TypeC>();

  ScopedPtr<TypeA> a(r0.create<TypeA>());
  //"Expected that r0 would return a TypeC"
  EXPECT_TRUE(dynamic_cast<TypeC *>(a.get()) != NULL);
  a.reset(r1.create<TypeA>());
  //"Expected that r1 would return a TypeB"
  EXPECT_TRUE(dynamic_cast<TypeB *>(a.get()) != NULL);
}

TEST(TypeRebind, Assign) {
  TypeRebind r0;
  r0.bind<TypeA>().to<TypeB>();
  TypeRebind r1;
  r1.bind<TypeA>().to<TypeC>();

  r0 = r1;

  ScopedPtr<TypeA> a(r0.create<TypeA>());
  // "Expected that r0 would return a TypeC"
  EXPECT_TRUE(dynamic_cast<TypeC *>(a.get()) != NULL);
}

TEST(TypeRebind, SingletonCreate) {
  SingletonBehavior<TypeB> b;
  TypeB *ptr0 = b.create();
  TypeB *ptr1 = b.create();
  //"Expected that with the SingletonBehavior the same pointer would be returned
  // for multiple calls to create"
  EXPECT_EQ(ptr0, ptr1);
}

TEST(TypeRebind, SingletonClone) {
  ScopedPtr<SingletonBehavior<TypeB>> b0(new SingletonBehavior<TypeB>);
  TypeB *ptr0 = b0->create();
  ScopedPtr<Behavior<TypeB>> b1(b0->clone());
  //"Expected that SingletonBehavior::clone would return SingletonBehavior"
  EXPECT_TRUE(dynamic_cast<SingletonBehavior<TypeB> *>(b1.get()) != NULL);
  TypeB *ptr1 = b1->create();
  //"Expected that cloned singleton behavior would create a new instance of
  // TypeB"
  EXPECT_TRUE(ptr0 != ptr1);
}
