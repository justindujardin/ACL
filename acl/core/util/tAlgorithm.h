//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_CORE_UTIL_ALGORITHM_H_
#define ACL_CORE_UTIL_ALGORITHM_H_

namespace ACLib {
template <typename Type> void Swap(Type &a, Type &b) {
  Type temp(a);
  a = b;
  b = temp;
}

template <class Iterator, class Value>
Iterator Find(Iterator first, Iterator last, Value value) {
  while (first != last && *first != value)
    ++first;
  return first;
}

template <class Iterator, class Functor>
Functor ForEach(Iterator first, Iterator last, Functor d) {
  Iterator temp;
  while (first != last) {
    temp = first++;
    d(*temp);
  }
  return d;
}

template <class Iterator, class Functor, class Param1>
Functor ForEach(Iterator first, Iterator last, Functor d, Param1 p1) {
  Iterator temp;
  while (first != last) {
    temp = first++;
    d(*temp, p1);
  }
  return d;
}
} // namespace ACLib

#endif // ACL_CORE_UTIL_ALGORITHM_H_
