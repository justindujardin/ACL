//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef _UTIL_DELEGATE_H_
#define _UTIL_DELEGATE_H_

#include "core/util/FastDelegate.h"

/// Define a Delegate
///
/// This macro allows translation of Delegate definitions into a target
/// delegate implementation.  
///
/// By default ACLib Juggernaut uses a @see FastDelegate implementation
/// 

#define Delegate fastdelegate::FastDelegate
typedef fastdelegate::DelegateMemento DelegateMemento;

template<class T>
class DelegateRemapper : public DelegateMemento
{
public:
   DelegateRemapper() : mOffset(0) {}

   void set(T * t, const DelegateMemento & memento)
   {
      SetMementoFrom(memento);
      if (m_pthis)
         mOffset = ((int)m_pthis) - ((int)t);
   }

   void rethis(T * t)
   {
      if (m_pthis)
         m_pthis = (fastdelegate::detail::GenericClass *)(mOffset + (int)t);
   }

protected:
   int mOffset;
};

#endif
