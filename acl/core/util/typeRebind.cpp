//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "core/util/typeRebind.h"
#include "core/util/tAlgorithm.h"

namespace Torque
{
   TypeRebind::TypeRebind() : mBindings()
   {
   }
   
   TypeRebind::~TypeRebind()
   {
      for(Map<U32*, AbstractBinding*>::Iterator i = mBindings.begin(); i != mBindings.end(); ++i)
         delete (*i).value;
   }
   
   TypeRebind::TypeRebind(const TypeRebind& other)
   {
      // Can't simply assign Map, the bindings need to be duplicated
      for(Map<U32*, AbstractBinding*>::ConstIterator i = other.mBindings.begin(); i != other.mBindings.end(); ++i)
         mBindings[(*i).key] = (*i).value->clone();
   }
   
   TypeRebind& TypeRebind::operator=(TypeRebind other)
   {
      other.swap(*this);
      return *this;
   }
   
   void TypeRebind::swap(TypeRebind& other)
   {
      Swap(mBindings, other.mBindings);
   }
}