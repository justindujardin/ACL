//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_CORE_UTIL_TYPEREBIND_H_
#define ACL_CORE_UTIL_TYPEREBIND_H_

#include "core/types.h"
#include "core/assert.h"
#include "core/util/scopedPtr.h"
#include "core/util/delegate.h"
#include "core/util/tReflection.h"
#include "core/containers/tMap.h"

namespace ACLib
{
   //--------------------------------------------------------------------------
   /// Inherit from this if you are specifying your own creation behavior.
   template<typename CreatedType>
   class Behavior
   {
   public:
      virtual ~Behavior() {}
      virtual CreatedType* create() const = 0;
      virtual Behavior* clone() const = 0;
   };

   //--------------------------------------------------------------------------
   /// This behavior will always return a new instance of CreatedType.
   template<typename CreatedType>
   class AlwaysCreateBehavior : public Behavior<CreatedType>
   {
      /// Class contraints
      AssertStaticNamespace(IsAbstract<CreatedType>::False, 
         CreatedType_must_be_concrete);
   public:
      /// Returns a new instance of CreatedType.
      CreatedType* create() const;

      virtual Behavior<CreatedType>* clone() const;
   };

   //--------------------------------------------------------------------------
   /// This behavior will call its given delegate and return the result of that.
   /// The delegate will take no parameters and will return a CreatedType*
   template<typename CreatedType>
   class DelegateCreateBehavior : public Behavior<CreatedType>
   {
   public:
      DelegateCreateBehavior();
      DelegateCreateBehavior(const Delegate<CreatedType*()>& d);
      void assignDelegate(const Delegate<CreatedType*()>& d);
      CreatedType* create() const;

      virtual Behavior<CreatedType>* clone() const;

   private:
      Delegate<CreatedType*()> mDelegate;
   };

   //--------------------------------------------------------------------------
   /// This behavior will only instantiate a single instace of CreatedType and 
   /// always return that instance.  The SingletonBehavior owns its created 
   /// instance and you should not delete it yourself.
   template<typename CreatedType>
   class SingletonBehavior : public Behavior<CreatedType>
   {
      /// Class contraints
      AssertStaticNamespace(IsAbstract<CreatedType>::False, 
         CreatedType_must_be_concrete);
   public:
      SingletonBehavior();
      ~SingletonBehavior();
      CreatedType* create() const;

      /// The given delegate will called when the object is first instantiated.
      /// The only parameter to the delegate will be the just created object.
      void assignDelegate(const Delegate<void(CreatedType*)>& d);

      /// The clone will create a new instance of CreatedType when create is 
      /// first called on it.
      virtual Behavior<CreatedType>* clone() const;   
   private:
      // mutable to support lazy init in create()
      mutable ScopedPtr<CreatedType> mInstance;
      Delegate<void(CreatedType*)> mDelegate;
   };

   //--------------------------------------------------------------------------
   /// AbstractCreator allows the system to refer to Creators without needing
   /// to know what type will actually be instantiated.
   template<typename BaseType>
   class AbstractCreator
   {
   public:
      virtual ~AbstractCreator() {}
      virtual BaseType* create() const = 0;
      virtual AbstractCreator* clone() const = 0;
   };

   /// Creator serves as a bridge between BaseType and DerivedType.  It
   /// enforces the requirement that DerivedType* be convertible to BaseType*.
   template<typename BaseType, typename DerivedType>
   class Creator : public AbstractCreator<BaseType>
   {
      /// Class contraints
      AssertStaticNamespace((IsConvertible<DerivedType*, BaseType*>::True),
         DerivedTypePtr_must_be_convertible_to_BaseTypePtr);
   public:
      /// Defaults to the RequiredBehavior for RebindConstraint<BaseType, DerivedType>,
      /// which is AlwaysCreateBehavior by default.
      Creator();

      /// Immediate specification of behavior.
      /// @param b If NULL, this function will create a new instance of 
      /// BehaviorType<DerivedType>
      template<template<class> class BehaviorType>
      Creator(BehaviorType<DerivedType>* b);

      /// Calls create on its behavior
      BaseType* create() const;

      /// This will bind a new behavior type to the Creator.  The Creator owns
      /// its behavior, so you should not save the return value of this function.
      template<template<class> class BehaviorType>
      BehaviorType<DerivedType>& withBehavior();

      /// Directly assign a new creator instance instead of assigning a type.
      /// The Creator assumes ownership of the given behavior, you should not
      /// refernce it further after calling this function.  It is an error to
      /// pass in NULL.  U must be convertible to Behavior<T>.
      template<typename BehaviorType>
      void withBehavior(BehaviorType* b);

      virtual AbstractCreator<BaseType>* clone() const;

   private:
      template<typename BehaviorType>
      void checkConstraint_(IntToType<true> hasConstraint);

      template<typename BehaviorType>
      void checkConstraint_(IntToType<false> noConstraint);

      // Does not enforce constraints
      Creator(Behavior<DerivedType>* b);

      ScopedPtr<Behavior<DerivedType> > mBehavior;
   };

   //--------------------------------------------------------------------------
   /// Empty class used for basic typesafety in TypeRebind.
   class AbstractBinding
   {
   public:
      virtual ~AbstractBinding() {}
      virtual AbstractBinding* clone() const = 0;
   };

   /// This class is only aware of the base type (the type being bound) and is
   /// the entry point to the rebinding system.
   template<typename BaseType>
   class Binding : public AbstractBinding
   {
   public:
      /// Will create and return a new instance of Creator<T, U>.  Note that
      /// Binding owns its creator and you should not save this return value.
      template<typename DerivedType> 
      Creator<BaseType, DerivedType>& to();

      /// Shortcut for to<T>.
      Creator<BaseType, BaseType>& toSelf();

      /// Bind a type and immediately specify a behavior.  This is necessary
      /// if you are binding to an abstract type, as any instantiation of the
      /// default AlwaysCreateBehavior will result in a compile error.  For
      /// readability purposes the preferred method is still to<Type>().withBehavior<Behavior>().
      template<typename DerivedType, template<typename> class BehaviorType>
      BehaviorType<DerivedType>& to(BehaviorType<DerivedType>* behavior = NULL);

      /// Creates a DelegateCreateBehavior and immediately binds it to a new Creator.
      /// No further chaining is possible with this path.
      void to(const Delegate<BaseType*()>& d);

      /// Calls create on its creator.
      BaseType* create() const;

      virtual AbstractBinding* clone() const;

   private:
      ScopedPtr<AbstractCreator<BaseType> > mCreator;
   };

   //--------------------------------------------------------------------------
   class TypeRebind
   {
   public:
      TypeRebind();
      ~TypeRebind();

      TypeRebind(const TypeRebind& other);
      TypeRebind& operator=(TypeRebind other); // Uses the copy-and-swap trick.

      /// Returns a new Binding (instantiating it if necessary).  TypeRebind
      /// owns its bindings, and you should not save the returned reference.
      template<typename BaseType>
      Binding<BaseType>& bind();

      /// Traverses the type rebinding system to create an object of type T.
      /// It is an error to call this if you have not previously set up the
      /// necessary binding.
      template<typename BaseType>
      BaseType* create() const;

      /// Returns true if the type has been bound to something, false if the
      /// type has not been bound.
      template<typename BaseType>
      bool hasType() const;

      void swap(TypeRebind& other);

   private:
      template<typename T>
      ACL_NO_INLINE U32* getTypeId_() const
      {
         static U32 id;
         return &id;
      }

      template<typename BaseType>
      Binding<BaseType>* getBinding_() const;

      Map<U32*, AbstractBinding*> mBindings;
   };

   //--------------------------------------------------------------------------
   /// RebindConstraint can be used to place custom constraints on rebinding
   /// types.  By default it allows you to force the use of a certain behavior
   /// but you can also use AssertStaticNamespace to specify other constraints.
   template<typename BaseType, typename DerivedType>
   struct RebindConstraint
   {
      /// Set this to true in your specialization to cause a compile error on
      /// any attempt to set a behavior other than RequiredBehavior.
      static const bool ForceBehavior = false;

      /// If ForceBehavior is true, any attempt to set a behavior other than 
      /// the one specified here will result in a compile error.  If 
      /// ForceBehavior is false, this will merely be the default behavior for
      /// the binding.
      typedef AlwaysCreateBehavior<DerivedType> RequiredBehavior;
   };

   //--------------------------------------------------------------------------
   // AlwaysCreateBehavior Implementation
   //--------------------------------------------------------------------------
   template<typename CreatedType>
   CreatedType* AlwaysCreateBehavior<CreatedType>::create() const
   {
      return new CreatedType();
   }

   template<typename CreatedType>
   Behavior<CreatedType>* AlwaysCreateBehavior<CreatedType>::clone() const
   {
      return new AlwaysCreateBehavior<CreatedType>();
   }

   //--------------------------------------------------------------------------
   // DelegateCreateBehavior Implementation
   //--------------------------------------------------------------------------
   template<typename CreatedType>
   DelegateCreateBehavior<CreatedType>::DelegateCreateBehavior()
   {
   }

   template<typename CreatedType>
   DelegateCreateBehavior<CreatedType>::DelegateCreateBehavior(const Delegate<CreatedType*()>& d) :
   mDelegate(d)
   {
   }

   template<typename CreatedType>
   void DelegateCreateBehavior<CreatedType>::assignDelegate(const Delegate<CreatedType*()>& d)
   {
      mDelegate = d;
   }

   template<typename CreatedType>
   CreatedType* DelegateCreateBehavior<CreatedType>::create() const
   {
      return mDelegate();
   }
   template<typename CreatedType>
   Behavior<CreatedType>* DelegateCreateBehavior<CreatedType>::clone() const
   {
      return new DelegateCreateBehavior<CreatedType>(mDelegate);
   }

   //--------------------------------------------------------------------------
   // SingletonBehavior Implementation
   //--------------------------------------------------------------------------
   template<typename CreatedType>
   SingletonBehavior<CreatedType>::SingletonBehavior() : mInstance(NULL)
   {
   }

   template<typename CreatedType>
   SingletonBehavior<CreatedType>::~SingletonBehavior()
   {
   }

   template<typename CreatedType>
   void SingletonBehavior<CreatedType>::assignDelegate(const Delegate<void(CreatedType*)>& d)
   {
      mDelegate = d;
   }

   template<typename CreatedType>
   CreatedType* SingletonBehavior<CreatedType>::create() const
   {
      if(mInstance.isNull())
      {
         mInstance.reset(new CreatedType());
         if(mDelegate)
            mDelegate(mInstance.get());
      }

      return mInstance.get();
   }

   template<typename CreatedType>
   Behavior<CreatedType>* SingletonBehavior<CreatedType>::clone() const
   {
      return new SingletonBehavior<CreatedType>();
   }

   //--------------------------------------------------------------------------
   // Creator Implementation
   //--------------------------------------------------------------------------
   template<typename BaseType, typename DerivedType>
   Creator<BaseType, DerivedType>::Creator() : 
   mBehavior(new typename RebindConstraint<BaseType, DerivedType>::RequiredBehavior())
   {
   }

   template<typename BaseType, typename DerivedType> template<template<class> class BehaviorType>
   Creator<BaseType, DerivedType>::Creator(BehaviorType<DerivedType>* b) : mBehavior(b)
   {
      checkConstraint_<BehaviorType<DerivedType> >
         (IntToType<RebindConstraint<BaseType, DerivedType>::ForceBehavior>());
   }

   template<typename BaseType, typename DerivedType>
   Creator<BaseType, DerivedType>::Creator(Behavior<DerivedType>* b) : mBehavior(b)
   {
      // Private constructor, does not enforce constraints.  Used by clone().
   }

   template<typename BaseType, typename DerivedType>
   BaseType* Creator<BaseType, DerivedType>::create() const
   {
      return mBehavior->create();
   }

   template<typename BaseType, typename DerivedType>
   AbstractCreator<BaseType>* Creator<BaseType, DerivedType>::clone() const
   {
      return new Creator<BaseType, DerivedType>(mBehavior->clone());
   }

   template<typename BaseType, typename DerivedType> template<typename BehaviorType>
   void Creator<BaseType, DerivedType>::checkConstraint_(IntToType<true> hasConstraint)
   {
      AssertStatic((IsSame<BehaviorType, 
         typename RebindConstraint<BaseType, DerivedType>::RequiredBehavior>::True),
         rebind_constraint_failed_incorrect_behavior);
   }

   template<typename BaseType, typename DerivedType> template<typename BehaviorType>
   void Creator<BaseType, DerivedType>::checkConstraint_(IntToType<false> noConstraint)
   {
   }

   template<typename BaseType, typename DerivedType> template<template<class> class BehaviorType>
   BehaviorType<DerivedType>& Creator<BaseType, DerivedType>::withBehavior()
   {
      checkConstraint_<BehaviorType<DerivedType> >(
         IntToType<RebindConstraint<BaseType, DerivedType>::ForceBehavior>());

      BehaviorType<DerivedType>* b = new BehaviorType<DerivedType>();
      mBehavior.reset(b);
      return *b;
   }

   template<typename BaseType, typename DerivedType> template<typename BehaviorType>
   void Creator<BaseType, DerivedType>::withBehavior(BehaviorType* b)
   {
      checkConstraint_<BehaviorType>(
         IntToType<RebindConstraint<BaseType, DerivedType>::ForceBehavior>());

      AssertFatal(b, "Cannot assign NULL behavior");
      if(!b)
         return;
      mBehavior.reset(b);
   }

   //--------------------------------------------------------------------------
   // Binding Implementation
   //--------------------------------------------------------------------------
   template<typename BaseType>
   AbstractBinding* Binding<BaseType>::clone() const
   {
      Binding<BaseType>* ret = new Binding<BaseType>();
      ret->mCreator.reset(mCreator->clone());
      return ret;
   }

   template<typename BaseType> template<typename DerivedType>
   Creator<BaseType, DerivedType>& Binding<BaseType>::to()
   {
      // bind<TypeA>().to<TypeB>() is only valid if TypeB inherits from TypeA
      // or if TypeB* is otherwise convertible to TypeA*.
      AssertStatic((IsConvertible<DerivedType*, BaseType*>::True),
         Must_be_able_to_convert_DerivedTypePtr_to_BaseTypePtr);

      Creator<BaseType, DerivedType>* c = new Creator<BaseType, DerivedType>();
      mCreator.reset(c);
      return *c;
   }

   template<typename BaseType>
   Creator<BaseType, BaseType>& Binding<BaseType>::toSelf()
   {
      return to<BaseType>();
   }

   template<typename BaseType>
   void Binding<BaseType>::to(const Delegate<BaseType*()>& d)
   { 
      // Has no failure conditions, as it circumvents most of the standard
      // rebinding system.
      DelegateCreateBehavior<BaseType>* b = new DelegateCreateBehavior<BaseType>(d);
      to<BaseType, DelegateCreateBehavior>(b);
   };

   template<typename BaseType> template<typename DerivedType, template<typename> class BehaviorType>
   BehaviorType<DerivedType>& Binding<BaseType>::to(BehaviorType<DerivedType>* b /* = NULL*/)
   {
      // Does not require that DerivedType be concrete.

      // bind<TypeA>().to<TypeB>() is only valid if TypeB inherits from TypeA
      // or if TypeB* is otherwise convertible to TypeA*.
      AssertStatic((IsConvertible<DerivedType*, BaseType*>::True),
         Must_be_able_to_convert_DerivedTypePtr_to_BaseTypePtr);
      if(!b)
         b = new BehaviorType<DerivedType>();
      Creator<BaseType, DerivedType>* c = new Creator<BaseType, DerivedType>(b);
      mCreator.reset(c);
      return *b;
   }

   template<typename BaseType>
   BaseType* Binding<BaseType>::create() const
   {
      return mCreator->create();
   }

   //--------------------------------------------------------------------------
   // TypeRebind Implementation
   //--------------------------------------------------------------------------

   template<typename BaseType>
   bool TypeRebind::hasType() const
   {
      return mBindings.contains(getTypeId_<BaseType>());
   }

   template<typename BaseType>
   Binding<BaseType>& TypeRebind::bind()
   {
      Binding<BaseType>* ret = getBinding_<BaseType>();
      if(!ret)
      {
         ret = new Binding<BaseType>();
         mBindings[getTypeId_<BaseType>()] = ret;
      }
      return *ret;
   }

   template<typename BaseType>
   BaseType* TypeRebind::create() const
   {
      Binding<BaseType>* binding = getBinding_<BaseType>();
      if(!binding)
      {
         AssertFatal(false, "Attempting to create an unbound type");
         return NULL;
      }
      return binding->create();
   }

   template<typename BaseType>
   Binding<BaseType>* TypeRebind::getBinding_() const
   {
      U32* id = getTypeId_<BaseType>();
      AbstractBinding* ret = NULL;
      mBindings.tryGetValue(id, ret);
      return static_cast<Binding<BaseType>*>(ret);
   }
}

#endif
