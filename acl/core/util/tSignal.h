//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2011 DuJardin Consulting, LLC.
//-----------------------------------------------------------------------------

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include "core/util/delegate.h"

/// Signals (Multi-cast Delegates)
/// 
/// Signals are used throughout this engine to allow subscribers to listen
/// for generated events for various things.  
/// 
/// Signals are called according to their order parameter (lower
/// numbers first).
///
/// Signal functions can return bool or void.  If bool then returning false
/// from a signal function will cause entries in the ordered signal list after 
/// that one to not be called.
///
/// This allows handlers of a given event to say to the signal generator, "I handled this message, and 
/// it is no longer appropriate for other listeners to handle it"

class SignalBase
{
public:
   SignalBase()
   {
      mList.next = mList.prev = &mList;
      mList.order = 0.5f;
   }

   ~SignalBase();

   bool isEmpty()
   {
      return mList.next == &mList;
   }
protected:
   struct DelegateLink
   {
      DelegateLink *next,*prev;
      float order;

      void insert(DelegateLink* node, float order);
      void unlink();
   };

   DelegateLink mList;
};

template<typename Signature> class SignalBaseT : public SignalBase
{
   typedef Delegate<Signature> DelegateSig;
public:
   SignalBaseT() {}
   SignalBaseT( const SignalBaseT &base )
      : SignalBase()
   {	
      mList.next = mList.prev = &mList;

      for ( DelegateLink *ptr = base.mList.next; ptr != &base.mList; ptr = ptr->next )
      {
         DelegateLinkImpl *del = static_cast<DelegateLinkImpl*>( ptr );
         notify( del->mDelegate, del->order );
      }
   }

   void notify(DelegateSig dlg, float order = 0.5f)
   {
      mList.insert(new DelegateLinkImpl(dlg), order);
   }

   void remove(DelegateSig dlg)
   {
      for(DelegateLink *ptr = mList.next;ptr != &mList;ptr = ptr->next)
      {
         if(DelegateLinkImpl *del = static_cast<DelegateLinkImpl *>(ptr))
         {
            if(del->mDelegate == dlg)
            {
               del->unlink();
               delete del;
               return;
            }
         }
      }
   } 

   template <class T,class U>
   void notify(T obj,U func, float order = 0.5f)
   {
      DelegateSig dlg(obj, func);
      notify(dlg, order);
   }

   template <class T>
   void notify(T func, float order = 0.5f)
   {
      DelegateSig dlg(func);
      notify(dlg, order);
   }

   template <class T,class U>
   void remove(T obj,U func)
   {
      DelegateSig compDelegate(obj, func);
      remove(compDelegate);
   }

   template <class T>
   void remove(T func)
   {
      DelegateSig compDelegate(func);
      remove(compDelegate);
   } 

protected:
   struct DelegateLinkImpl : public SignalBase::DelegateLink
   {
      DelegateSig mDelegate;
      DelegateLinkImpl(DelegateSig dlg) : mDelegate(dlg) {}
   };

   DelegateSig & getDelegate(SignalBase::DelegateLink * link)
   {
      return ((DelegateLinkImpl*)link)->mDelegate;
   }
};

//-----------------------------------------------------------------------------

template<typename Signature> class Signal;

// Short-circuit signal implementations

template<> 
class Signal<bool()> : public SignalBaseT<bool()>
{
public:
   bool trigger()
   {
      SignalBase::DelegateLink *ptr = mList.next;
      while( ptr != &mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         if (!getDelegate(ptr)())
            return false;
         ptr = next;
      }
      return true;
   }
};

template<class A> 
class Signal<bool(A)> : public SignalBaseT<bool(A)>
{   
public:
   bool trigger(A a)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         if (!this->getDelegate(ptr)(a))
            return false;
         ptr = next;
      }
      return true;
   }
};

template<class A, class B>
class Signal<bool(A,B)> : public SignalBaseT<bool(A,B)>
{
public:
   bool trigger(A a, B b)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         if (!this->getDelegate(ptr)(a,b))
            return false;
         ptr = next;
      }
      return true;
   }
};

template<class A, class B, class C> 
class Signal<bool(A,B,C)> : public SignalBaseT<bool(A,B,C)>
{
public:
   bool trigger(A a, B b, C c)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         if (!this->getDelegate(ptr)(a,b,c))
            return false;
         ptr = next;
      }
      return true;
   }
};

template<class A, class B, class C, class D> 
class Signal<bool(A,B,C,D)> : public SignalBaseT<bool(A,B,C,D)>
{
public:
   bool trigger(A a, B b, C c, D d)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         if (!this->getDelegate(ptr)(a,b,c,d))
            return false;
         ptr = next;
      }
      return true;
   }
};

template<class A, class B, class C, class D, class E> 
class Signal<bool(A,B,C,D,E)> : public SignalBaseT<bool(A,B,C,D,E)>
{
public:
   bool trigger(A a, B b, C c, D d, E e)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         if (!this->getDelegate(ptr)(a,b,c,d,e))
            return false;
         ptr = next;
      }
      return true;
   }
};

// Non short-circuit signal implementations

template<> 
class Signal<void()> : public SignalBaseT<void()>
{
public:
   void trigger()
   {
      SignalBase::DelegateLink *ptr = mList.next;
      while( ptr != &mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         getDelegate(ptr)();
         ptr = next;
      }
   }
};

template<class A>
class Signal<void(A)> : public SignalBaseT<void(A)>
{
public:
   void trigger(A a)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         this->getDelegate(ptr)(a);
         ptr = next;
      }
   }
};

template<class A, class B> 
class Signal<void(A,B)> : public SignalBaseT<void(A,B)>
{
public:
   void trigger(A a, B b)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         this->getDelegate(ptr)(a,b);
         ptr = next;
      }
   }
};

template<class A, class B, class C> 
class Signal<void(A,B,C)> : public SignalBaseT<void(A,B,C)>
{
public:
   void trigger(A a, B b, C c)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         this->getDelegate(ptr)(a,b,c);
         ptr = next;
      }
   }
};

template<class A, class B, class C, class D>
class Signal<void(A,B,C,D)> : public SignalBaseT<void(A,B,C,D)>
{
public:
   void trigger(A a, B b, C c, D d)
   {
      SignalBase::DelegateLink *ptr = this->mList.next;
      while( ptr != &this->mList )
      {
         SignalBase::DelegateLink* next = ptr->next;
         this->getDelegate(ptr)(a,b,c,d);
         ptr = next;
      }
   }
};

#endif // _SIGNAL_H_